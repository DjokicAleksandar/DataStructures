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

// rasute tablice su one koje koriste unutrasnje ulancavanje ili otvoreno adresiranje

// objekti
template <class T, class R>
class HashObject
{
protected:
	T key;
	R* record;

public:
	HashObject() { key = (T)0; record = nullptr; };
	HashObject(T key) { this->key = key; record = nullptr; };
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

	void DeleteRecord() { 
		std::cout << "record: " << record;

		if (record != nullptr) 
			delete record; 

		record = nullptr; 
	};
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

// tablice ------------------------------
template <class T, class R>
class HashTable
{
protected:
	unsigned int length; // velicina tablice
	unsigned int count; // broj elemenata

	// hash funkcija
	unsigned int h(HashObject<T, R>& const obj) {
		int res = f(obj.GetKey()) % length;
		return res;
	}

	// primarne transformacije
	virtual unsigned int f(int i) {
		int res = abs(i);
		return res;
	}
	virtual unsigned int f(double d) {
		if (d == 0)
			return 0;
		else {
			int exponent;
			double mantisa = frexp(d, &exponent); // odvaja mantisu i eksponent (vraca izmedju -1 i -0.5 ILI 0.5 i 1)       1.
			return (unsigned int)(2 * fabs(mantisa) - 1) * ~0U; // ~0U je negacija sa 0 unsigned i vraca broj od 0 do MAX   3.
								//fabs(mantisa) daje broj izmedju 0.5 i 1, a 2*(od 0.5 do 1) - 1 daje vrednost od 0 do 1    2.
		}
	}
	virtual unsigned int f(char* s) {
		unsigned int res = 0;
		unsigned int a = 7;
		
		for (int i = 0; s[i] != 0; i++)
			res = res << a ^ s[i]; // prvo se pomnozi sa 2^7 pa se radi xor (to je ^ znak)

		return res;
	}

	// sekundarna transformacija
	virtual unsigned int g(unsigned int i) {
		return (i + 1) % length;
	}

public:
	unsigned int GetLength() { return length; };
	virtual double GetLoadFactor() { return (double)count / (double)length; };
};

// ULANCAVANJE-----------------
// spoljno ulancavanje
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
	
	void Insert(HashObject<T, R>& const obj) {
		arr[this->h(obj)].AddToHead(obj);
		this->count++;
	};

	void Withdraw(HashObject<T, R> obj) {
		arr[this->h[obj]].Delete(obj);
		this->count--;
	};

	void Withdraw(T key) {
		HashObject<T, R> obj = Find(key);
		Withdraw(obj);
	}

	HashObject<T, R> Find(T key) {
		HashObject<T, R> obj;
		unsigned int i = this->f(key) % this->length;
		obj = arr[i].GetHead();
		
		while (!(obj.IsEqualKey(key)))
			obj = arr[i].GetNext(obj);

		return obj;
	}
};

// unutrasnje ulancavanje
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

	// umetanje u tablicu sa unu. ulancavanjem sinonima
	void Insert(ChainedScatterObject<T, R> obj) {
		if (this->count == this->GetLength())
			return;

		int probe = this->h(obj);

		if (arr[probe].status == 2) {
			while (arr[probe].next != -1)
				probe = arr[probe].next;

			int tail = probe;
			probe = this->g(probe);
			while (arr[probe].status == 2 && probe != tail)
				probe = this->g(probe);

			if (probe == tail)
				return; // losa sek. transformacija
			arr[tail].next = probe;
		}

		arr[probe] = obj;
		arr[probe].status = 2; // zauzet
		arr[probe].next = -1;
		this->count++;
	}
	void Insert2(ChainedScatterObject<T, R> obj) {
		if (this->count == this->GetLength())
			return;

		int probe = this->h(obj);

		if (arr[probe].status == 2) { // mesto je zauzeto
			while (arr[probe].next != -1)
				probe = arr[probe].next;

			int tail = probe;

			probe = this->g(probe);

			while (arr[probe].status == 2 && probe != tail) {
				probe = this->g(probe);
			}

			if (probe == tail)
				return; // neuspela tranformacija

			arr[tail].next = probe;
		}

		arr[probe] = obj;
		arr[probe].status = 2;
		arr[probe].next = -1;
		this->count++;
	}

	ChainedScatterObject<T, R> Find(T key) {
		int probe = f(key) % this->length;

		while (probe != -1) {
			if (!arr[probe].IsEqualKey(key))
				probe = arr[probe].next;
			else
				return arr[probe];
		}

		return;
	}

	void Withdraw(T key) {
		if (this->count == 0) return;
		int probe = f(key) % this->length; // isto kao h(key) ali hash funnkcija ne prima key vec samo obj
		int prev = -1;

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
				int nextEl = arr[probe].next;
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
// ----------------------------

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
		return 3 * i * i % this->length;
	}

	unsigned int FindUnoccupied(ScatterObject<T, R> obj) {
		unsigned int hash = h(obj);
		unsigned int probe = hash;

		if (arr[probe].status < 2) 
			return probe; // ako je slobodan ili obrisan

		int i = 0;
		while (i < this->length) {
			probe = hash + HashCodeSec2(i);

			if (arr[probe].status < 2)
				return probe;

			i++;
		}

		return 0;
	};

	long FindMatch(T key) {
		unsigned int probe = f(key) % this->length;
		for (unsigned int i = 0; i < this->length; i++) {
			int sin = probe + HashCodeSec2(i);
			if (arr[sin].status == 0) return -1;
			if (arr[sin].IsEqualKey(key)) return probe;
		}

		return -1;
	}

	bool Signup(T key, R* record) {
		int hash = hash(T);
		int probe = hash;
		int i = 1;

		ScatterObject<T, R> obj(key, record);

		while (arr[probe].status == 2 && i < this->length) {
			probe = (probe + 1) % this->length;
			i++;
		}

		if (arr[probe] == obj)
			return false;
		else {
			arr[probe] = obj;
			arr[probe].status = 2;
			this->count++;
			return true;
		}
	}
};
// -------------------------------------

// ZADACI SA ISPITA
class HashTableExample1 
{
//Implementirati na programskom jeziku C++ heš funkciju
//unsigned int h(ScatterObject o), sekundarnu funkciju sa
//kvadratnim trazenjem unsigned int g(int i) i funkciju za
//smestanje objekta void insert(ScatterObject o) u hes
//tablicu sa otvorenim adresiranjem velicine m. Smatrati da
//su kljucevi celobrojni.


protected:
	int length = 10;
	int count;
	ScatterObject<int, int>* table[10];

	HashTableExample1() {
		this->length = 10;
		this->count = 0;
		
		for (int i = 0; i < 10; i++)
			table[i] = nullptr;
	}

	unsigned int h(int key) {
		return key % length;
	}

	unsigned int g(int i) {
		return i * i;
	}

	void Insert(ScatterObject<int, int> o) {
		int hash = h(o.GetKey());
		int i = 0;

		while (table[(hash + g(i)) % 10] != nullptr && i < 10)
			i++;

		if (i == 10) // tablica je puna
			return;

		table[(hash + g(i)) % 10] = new ScatterObject<int, int>(o); // ili new ScatterObject<int, int>{o} ???
	}
};

class HashTableSparseMatElement
{
public:
	int i, j;
	double value;
	int status; // 0 - slobodan, 1 - obrisan, 2 - zauzet

	HashTableSparseMatElement() { status = 0; };
};
class HashTableSparseMat
// Retko posednuta matrica zapamcena je rasutoj tablici(hes tablica sa
// otvorenim adresiranjem). Napisati funkcije za : inicijalizaciju – void
// init(int m, int n), primarnu transformaciju – int h(int i, int j),
// sekundarnu transformaciju – int g(...), umetanje elementa – void
// insert(int i, int j, double val), citanje elementa – double get(int i, int j)
// i mnozenje dve retkoposednute matrice – mul(). Smatrati da je tablica
// dimenzija m x n, i da ima manje od 20 posto nenultih elemenata tipa
// double. Samo nenulti elementi se pamte u tablici

{
public:
	int m, n;
	int capacity;
	HashTableSparseMatElement* table;

	void Init(int m, int n) {
		this->m = m;
		this->n = n;
		this->capacity = (m * n) / 5;
		table = new HashTableSparseMatElement[capacity];
	};

	int h(int i, int j) {
		return abs(i * 31 + j * 17) % capacity; // 31 i 17 jer su prosti brojevi, cesto se koriste
	};

	int g(int i) {
		return (17 * i * i) % capacity;
	};

	void Insert(int i, int j, double value) {
		int hash = h(i, j);
		int count = 0;

		while (table[hash].status == 2 && !(table[hash].i == i && table[hash].j == j) && count < capacity) {
			hash = (h(i, j) + g(++count)) % capacity;
		}

		table[hash].i = i;
		table[hash].j = j;
		table[hash].status = 2;
		table[hash].value = value;
	}

	double Get(int i, int j) {
		int hash = h(i, j);
		int count = 0;

		while (table[hash].status != 0) {
			if (table[hash].status == 2 && table[hash].i == i && table[hash].j == j)
				return table[hash].value;

			hash = (h(i, j) + g(++count)) % capacity;
		}

		return -1;
	}

	HashTableSparseMat Multiply(HashTableSparseMat& A, HashTableSparseMat& B) {
		HashTableSparseMat C;
		C.Init(A.m, A.n);

		for (int i = 0; i < A.capacity; i++) {
			if (A.table[i].status != 2)
				continue;

			int a_i = A.table[i].i;
			int a_j = A.table[i].j;
			double a_value = A.table[i].value;

			for (int j = 0; j < B.capacity; j++) {
				if (B.table[j].status != 2)
					continue;

				if (B.table[j].i == a_j) {
					int b_j = B.table[j].j;
					double b_value = B.table[j].value;

					double current = C.Get(a_i, b_j);
					C.Insert(a_i, b_j, current + a_value * b_value);
				}
			}
		}
	}
};

class HashTablePriorityQueueElement {
public:
	int priority;
	double value;

	HashTablePriorityQueueElement() { priority = 0; value = 0; };
	HashTablePriorityQueueElement(int priority, double value) { 
		this->priority = priority; 
		this->value = value; 
	};
};
class HashTablePriorityQueue {

// Red sa prioritetom implementiran je u obliku hash tablice sa
// unutrasnjim ulancanjem (sa zasebnim prostorom za
// smestanje sininima). Prioriteti su definisani celim brojevima
// u opsegu 0 - 299. Manja vrednost oznacava viši prioritet.
// Maksimalni broj elemenata u redu je 2000. Napisati funkciju
// za dodavanje i citanje iz reda.

public:
	List<HashTablePriorityQueueElement>* table = new List<HashTablePriorityQueueElement>[300];
	int size;

	HashTablePriorityQueue() {};

	void Enqueue(double value, int priority) {
		HashTablePriorityQueueElement obj(priority, value);

		if (table[priority].GetHead() == nullptr) {
			table[priority].AddToHead(obj);
		}
		else {
			table[priority].AddToTail(obj);
		}

		size++;
	}

	double Dequeue() { // skida element najviseg prioriteta
		for (int i = 0; i < 300; i++) {
			if (table[i].GetHead() != nullptr) {
				double result = table[i].GetHead()->info.value;
				table[i].DeleteFromHead();
			}
		}
	}
};

class HashTableInnerChainedElement
{
public:
	int key;
	double value;
	int next; // -1 po default
	bool free;

	HashTableInnerChainedElement() {
		key = value = 0;
		next = -1; 
		free = true; 
	};
	HashTableInnerChainedElement(int key, int value) {
		this->key = key;
		this->value = value;
		next = -1; 
		free = true; 
	};
	HashTableInnerChainedElement(int key, double value, int next, bool free) {
		this->key = key;
		this->value = value;
		this->next = next;
		this->free = free;
	};
};
class HashTableInnerChained
{
// Napisati funkciju Data* Get(int key), koja u hash tablici
// sa unutrasnjim ulancavanjem vraca elemenat sa
// zadatim kljucem i istovremeno premesta ovaj element
// na prvo mesto u listi sinonima, zadrzavajuci redosled
// ostalih sinonima u listi.


public:
	HashTableInnerChainedElement* table;
	int capacity;

	HashTableInnerChained() { capacity = 0; table = nullptr; };
	HashTableInnerChained(int capacity) {
		this->capacity = capacity;
		table = new HashTableInnerChainedElement[capacity];
	};

	int hash(int key) {
		return key % capacity;
	}

	void Insert(int key, double value) {
		int hashValue = hash(key);
		
		if (table[hashValue].free) {
			table[hashValue] = { key, value, -1, false };
		}
		else {
			int current = hashValue;
			while (table[current].next != -1) {
				current = table[current].next;
			}

			int newIndex = 0;
			while (newIndex < capacity && !table[newIndex].free) {
				newIndex++;
			}

			newIndex = newIndex < capacity ? newIndex : -1;

			if (newIndex == 1)
				return;

			table[newIndex] = { key, value, -1, false };
			table[current].next = newIndex;
		}
	}

	HashTableInnerChainedElement* Get(int key) {
		int hashValue = hash(key);

		if (table[hashValue].free)
			return nullptr;

		if (table[hashValue].key == key)
			return &table[hashValue];

		int prev = hashValue;
		int curr = table[hashValue].next;

		while (curr != -1) {
			if (table[curr].key == key) {
				// izbaci curr
				table[prev].next = table[curr].next;

				table[curr].next = table[hashValue].next;
				table[hashValue].next = curr;

				this->capacity -= 1;
				return &table[curr];
			}

			prev = curr;
			curr = table[curr].next;
		}

		
		return nullptr;
	}
};

// jun 2 2022.

class HashStudentElement 
{
public:
	char* ime;
	char* prezime;
	double prosek;
	int jmbg;
	int status;
	int next;

	HashStudentElement() 
		: ime(nullptr), prezime(nullptr), status(0), jmbg(0), prosek(0) {};
	HashStudentElement(const char* ime, const char* prezime, double prosek, int jmbg)
		: prosek(prosek), jmbg(jmbg) 
	{
		this->ime = new char[strlen(ime) + 1];
		strcpy(this->ime, ime);

		this->prezime = new char[strlen(prezime) + 1];
		strcpy(this->prezime, prezime);
	}
};
class StaticListForSynonyms
{
public:
	int size;
	int lrmp;
	int head;
	int* next;
	HashStudentElement* info;

	StaticListForSynonyms(int size) : size(size), lrmp(0), head(0) {
		next = new int[size];
		info = new HashStudentElement[size];

		lrmp = head = 0;

		for (int i = 0; i < size - 1; i++)
			next[i] = i + 1;

		next[size - 1] = -1; // pokazuje nazad na lrmp
	}

	void AddToLrmp(HashStudentElement obj) {
		int pom = lrmp;
		info[pom] = obj;
		lrmp = next[pom];

		// postavlja prethodni element na pom
		for (int i = 0; i < size - 1; i++) {
			if (next[i] == -1)
				next[i] = pom;
		}

		info[pom].status = 2; // zauzet
		next[pom] = -1;
	}

	void Delete(int jmbg) {
		for (int i = 0; i < size - 1; i++) {
			if (info[i].jmbg == jmbg) {
				next[i] == -1;
				info[i].status = 1;
			}
		}
	}

	void print() {
		int p = head;
		while (p != -1) {
			std::cout << info[p].jmbg << " ";
			p = next[p];
		}
		std::cout << "\n";
	}

};
class HashStudent
{
public:
	int length;
	int count;
	HashStudentElement* arr;
	StaticListForSynonyms* sinonimi;

	HashStudent(int length) { 
		this->count = 0; 
		this->length = length;  
		arr = new HashStudentElement[length];
		sinonimi = new StaticListForSynonyms(length);
	};

	int h(HashStudentElement obj) {
		double A = (sqrt(5) - 1) / 2;
		double frac = fmod(obj.jmbg * A, 1.0);
		return (int)(length * frac);
	}
	void Insert(HashStudentElement obj) {
		int probe = h(obj);

		if (arr[probe].status == 2) {
			// proveri da li postoji isti
			for (int i = 0; i < sinonimi->size; i++) {
				if (sinonimi->info[i].jmbg == obj.jmbg) {
					return;
				}
			}

			sinonimi->AddToLrmp(obj);
		}
		else {
			arr[probe] = obj;
			arr[probe].status = 2;
		}
	}
	bool Exsists(int jmbg) {

	}
};

// jun 2 2023.

class HashOpenAdrStudentEl
{
public:
	int indeks;
	long jmbg;
	char* ime;
	char* prezime;
	int upis;
	float prosek;
	int status;

	HashOpenAdrStudentEl()
		: indeks(0), jmbg(0), ime(0), prezime(0), upis(0), prosek(0), status(0) {};

	HashOpenAdrStudentEl(int indeks, long jmbg, const char* ime, const char* prezime, int upis, float prosek)
		: indeks(indeks), jmbg(jmbg), upis(upis), prosek(prosek), status(0)
	{
		this->ime = new char[strlen(ime) + 1];
		strcpy(this->ime, ime);

		this->prezime = new char[strlen(prezime) + 1];
		strcpy(this->prezime, prezime);
	}
};
class HashOpenAdrStudent
{
public:
	int size = 12503;
	int count;
	HashOpenAdrStudentEl* arr;

	int h(HashOpenAdrStudentEl obj) {
		return obj.jmbg % size;
	}

	int secH(int i) {
		return 3 * i * i;
	}

	void Insert(HashOpenAdrStudentEl obj) {
		int hash = h(obj);

		if (arr[hash].status != 2) {
			arr[hash] = obj;
			arr[hash].status = 2;
			return;
		}

		int probe = hash;

		for (int i = 0; i < size; i++) {

		}
	}
};