#pragma once
#include <iostream>
#include <bitset>
#include "List.h"

// HASH FUNKCIJE 
//
// metod deljenja: 
// h(k) = k * mod M (M je neparan broj ili stepen broja 2: M = 2^x ili prost broj)
// h(k) = k * mod 2^x: zadrzava samo x donjih bitova sto nije dobro jer se javljaju kolizije
// mana: za uzastupne vrednosti k dobijamo uzastopne vrednosti h(k), a cilj je da fja. rasipa vrednosti svuda
//
// metod sredine kvadrata:
// h(k) = (k * k) >> (w - p)

void HowLong(unsigned long long k) {
	unsigned long long kSquare = k * k;
	std::bitset<64> a(kSquare); // broj k * k ali zapisan binarno
	std::cout << a;
}

unsigned int Division(unsigned int k, int length) {// metod deljenja
	return k % length;
};

unsigned int Square(unsigned int k, int length) { // metod kvadrata
	unsigned int kSquare = k * k;
	std::bitset<64> a(kSquare);
	std::cout << a << std::endl;
	HowLong(k);
	return kSquare >> (32 - 6); // jer je tablica 2^6 (primer)
}

unsigned int Fibonacci(unsigned int k, int length) { // fibonacijev metod
	unsigned const int a = 2654435769; // najoptimalniji broj
	unsigned const ak = a * k;
	return ak >> (32 - 6);
}

// to su funkncije koje prevode unsigned int brojeve u opseg adresa tablice (deljenje, sredina kvadrata, fibonacci)
// kljucevi mogu biti celobrojni poz. brojevi, realni br. i karakteri


// objekti
template <class T, class R>
class HashObject
{
protected:
	T key;
	R* record;

public:
	HashObject() { key = (T)0; record = NULL; };
	HashObject(T key) { this->key = key; record = NULL; };
	HashObject(T key, R* record) { this->key = key; this->record = record; };
	~HashObject() { DeleteRecord(); };
	HashObject& operator=(HashObject& const obj) {
		key = obj.key;
		record = obj.record;
		return *this;
	}
	bool operator==(HashObject& const obj) {
		return record == obj.record;
	}

	void DeleteRecord() { if (record) delete record; record = NULL; };
	T GetKey() { return key; };
	R* GetRecord() { return record; };
	bool IsEqualKey(T key) { return this->key == key; };
	void Print() { std::cout << key << " | " << record; };
};

template <class T, class R>
class ScatterObject : public HashObject<T, R>
{
public:
	int status; // 0-slobodan, 1-obrisan, 2-zauzet

	ScatterObject() : HashObject<T, R>() { status = 0; };
	ScatterObject(T key) : HashObject<T, R>(key) { status = 0; };
	ScatterObject(T key, R* record) : HashObject<T, R>(key, record) { status = 0; };
};

template <class T, class R>
class ChainedScatterObject : public ScatterObject<T, R>
{
public:
	long next;

	ChainedScatterObject() : ScatterObject<T, R>() { next = -1; };
	ChainedScatterObject(T key) : ScatterObject<T, R>(key) { next = -1; };
	ChainedScatterObject(T key, R* record) : ScatterObject<T, R>(key, record) { next = -1; };
	ChainedScatterObject(T key, R* record, unsigned int n) : ScatterObject<T, R>(key, record) { next = n; };
};

// tablice
template <class T, class R>
class HashTable
{
protected:
	unsigned int length; // velicina tablice
	unsigned int count; // broj elemenata

	unsigned int h(HashObject<T, R> obj) {
		return HashCode(obj.GetKey()) % length;
	}

	// primarne transformacije
	virtual unsigned int HashCode(int i) {
		return abs(i);
	}

	virtual unsigned int HashCode(double d) {
		if (d == 0)
			return 0;
		else {
			int exponent;
			double mantisa = frexp(d, &exponent); // odvaja mantisu i eksponent (vraca izmedju -1 i -0.5 ILI 0.5 i 1)       1.
			return (unsigned int)(2 * fabs(mantisa) - 1) * ~0U; // ~0U je negacija sa 0 unsigned i vraca broj od 0 do MAX   3.
								//fabs(mantisa) daje broj izmedju 0.5 i 1, a 2*(od 0.5 do 1) - 1 daje vrednost od 0 do 1    2.
		}
	}

	virtual unsigned int HashCode(char* s) {
		unsigned int res = 0;
		unsigned int a = 7;
		
		for (int i = 0; s[i] != 0; i++)
			res = res << a ^ s[i]; // prvo se pomnozi sa 2^7 pa se radi xor (to je ^ znak)

		return res;
	}

	// sekundarna transformacija
	virtual unsigned int HashCodeSec(unsigned int i) {
		return (i + 1) % length;
	}

public:
	unsigned int GetLength() { return length; };
	virtual double GetLoadFactor() { return (double)count / (double)length; };

};

template <class T, class R>
class ChainedHashTable : public HashTable<T, R>
{
protected:
	List<HashObject<T, R>>* arr;

public:
	ChainedHashTable(unsigned int length) {
		this->length = length;
		this->count = 0;
		arr = new List<HashObject<T, R>>[length];
	}
	~ChainedHashTable() {
		HashObject<T, R> obj;
		for (unsigned int i = 0; i < this->length; i++) {
			try {
				obj = arr[i].GetHead();
				while (true) {
					obj.DeleteRecord();
					obj = arr[i].GetNext(obj);
				}
			}
			catch(exception e) {}
		}

		delete arr;
	}

	void Insert(HashObject<T, R> obj) {
		arr[h(obj)].AddToHead(obj);
		this->count++;
	};

	void Withdraw(HashObject<T, R> obj) {
		arr[h[obj]].Delete(obj);
		this->count--;
	};

	void Withdraw(T key) {
		HashObject<T, R> obj = Find(key);
		Withdraw(obj);
	}

	HashObject<T, R> Find(T key) {
		HashObject<T, R> obj;
		unsigned int i = HashCode(key) % length;
		obj = arr[i].GetHead();
		
		while (!(obj.IsEqualKey(key))
			obj = arr[i].GetNext(obj);

		return obj;
	}
};

// ulancavanje
template <class T, class R>
class ChainedScatterTable : public HashTable<T, R>
{
protected:
	ChainedScatterObject<T, R>* arr;

public:
	ChainedScatterTable(unsigned int length) {
		this->length = length;
		this->count = 0;
		arr = new ChainedScatterObject<T, R>[length];
	}
	~ChainedScatterTable() { delete[] arr; };

	// umetanje u tablicu sa ulancavanjem sinonima
	void Insert(ChainedScatterObject<T, R> obj) {
		if (this->count == GetLength())
			return;

		long probe = h(obj);

		if (!arr[probe].free) {
			while (arr[probe].next != -1)
				probe = arr[probe].next;

			long tail = probe;
			probe = HashCodeSec(probe);
			while (!arr[probe].free && probe != tail)
				probe = HashCodeSec(probe);

			if (probe == tail)
				return; // losa sek. transformacija
			arr[tail].next = probe;
		}

		arr[probe] = obj;
		arr[probe].status = 2; // zauzet
		arr[probe].next = -1;
		this->count++;
	}

	ChainedScatterObject<T, R> Find(T key) {
		long probe = HashCode(key) % length;

		while (probe != -1) {
			if (!arr[probe].IsEqualKey(key))
				probe = arr[probe].next;
			else
				return arr[probe];
		}

		return new SBPException("Greska");
	}

	void Withdraw(T key) {
		if (this->count == 0) return;
		long probe = HashCode(key) % length;
		long prev = -1;

		while (probe != -1 && !arr[probe].IsEqualKey(key)) {
			prev = probe;
			probe = arr[probe].next;
		}

		if (probe == 1)
			return;

		if (prev == -1) { // brise se sinonim
			arr[prev].next = arr[probe].next;
			arr[probe].DeleteRecord();
			arr[probe].status = 1;
		}
		else {
			if (arr[probe].next == -1) { // prvi i jedini u listi sinonima
				arr[probe].DeleteRecord();
				arr[probe].status = 1;
			}
			else { // prvi ali ima sinonime
				long nextEl = arr[probe].next;
				arr[probe].DeleteRecord();
				arr[probe] = arr[nextEl];
				arr[probe].next = arr[nextEl].next;
				arr[nextEl] = ChainedScatterObject<T, R>();
				arr[nextEl].status = 1;
			}
		}

		this->count--;
	}
};

// otvoreno adresiranje
template <class T, class R>
class OpenScatterTable : public HashTable<T, R>
{
protected:
	ScatterObject<T, R>* arr;

public:
	OpenScatterTable(unsigned int length) {
		this->length = length;
		this->count = 0;
		arr = new ScatterObject<T, R>[length];
	};
	
	~OpenScatterTable() { delete[] arr; };
	
	unsigned int HashCodeSec2(unsigned int i) {
		return 3 * i * i % length;
	}

	unsigned int FindUnoccupied(ScatterObject<T, R> obj) {
		unsigned int hash = h(obj);
		unsigned int probe = hash;

		if (arr[probe].status < 2) return probe; // ako je slobodan ili obrisan

		int i = 1;
		do {
			probe = hash + HashCodeSec2(i);
			if (arr[probe].status < 2) return probe;
			i++;
		} while (probe != hash && i < length);

		return 0;
	};

	long FindMatch(T key) {
		unsigned int probe = HashCode(key) % length;
		for (unsigned int i = 0; i < length; i++) {
			int sin = probe + HashCodeSec2(i);
			if (arr[sin].status == 0) return -1;
			if (arr[sin].IsEqualKey(key)) return probe;
		}

		return -1;
	}
};