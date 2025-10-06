#pragma once
#include <iostream>
#include "Heap.h"

// cesto se koriste dummy cvorovi (koji stoje pre head elementa) da bi se izbegli specificni slucajevi
// i onda funkcija vrati dummy.next, sto je pocetak liste

using namespace std;

template <class T>
class Node
{
public:
	T info;
	Node<T>* next;
	Node() { next = nullptr; };
	Node(T info) { this->info = info; this->next = nullptr; };
	Node(T info, Node<T>* next) { this->info = info, this->next = next; };
	~Node() {};
	T Print() { return info; };
	bool IsEqual(T el) { return el == info; };
};

template <class T>
class List
{
protected:
	Node<T>* head;
	Node<T>* tail;

public:
	List() { head = tail = nullptr; };
	~List() {};
	bool IsEmpty() { return head == nullptr; };
	void AddToHead(T info) {
		Node<T>* novi = new Node<T>(info, this->head);
		this->head = novi;

		if (tail == nullptr) //ako je lista bila prazna, pa smo dodali 1. element
			tail = this->head;
	};
	void AddToTail(T info) {
		if (!IsEmpty()) {
			Node<T>* novi = new Node<T>(info);
			tail->next = novi;
			tail = tail->next;
		}
		else {
			head = tail = new Node<T>(info);
		}
	};
	void DeleteFromHead() {
		if (IsEmpty())
			return;

		T el = head->info;

		//smestamo prvi element u promenljivu da bi mogao da se obrise
		Node<T>* pom = head;

		if (head == tail)
			head = tail = nullptr;
		else
			head = head->next;

		delete pom;
	};
	void DeleteFromTail() {
		if (IsEmpty())
			return;

		Node<T>* forDelete = tail;

		if (head == tail)
		{
			head = tail == nullptr;
		}
		else {
			Node<T>* pom;

			for (pom = head; pom->next != tail; pom = pom->next);

			tail = pom;
			tail->next = nullptr;
		}

		delete forDelete;
	};
	Node<T>* GetHead() { return this->head; };
	Node<T>* GetNext(T info) {
		Node<T>* curr = head;

		while (curr->info != info && curr != nullptr)
			curr = curr->next;

		if (curr != nullptr && curr->next != nullptr)
			return curr->next;
		else
			return NULL;
	};
	Node<T>* GetNext(Node<T>* ptr) {
		Node<T>* curr = head;

		while (curr != ptr && curr != nullptr)
			curr = curr->next;

		if (curr != nullptr && curr->next != nullptr)
			return curr->next;
		else
			return nullptr;
	}
	void Print() {
		for (Node<T>* pom = head; pom != nullptr; pom = pom->next) {
			cout << pom->Print() << " ";
		}
	};

	// zadaci sa ispita
	void DeleteEl(T info) {
			if (IsEmpty())
				return;

			if (head == tail && head->isEqual(info)) {
				delete head;
				head = tail = nullptr;
			}
			else if (head->info == info)
			{
				Node<T>* pom = head;
				head = head->next;
				delete pom;
			}
			else
			{
				Node<T>* pom;
				Node<T>* prev = head;

				for (pom = head; pom != nullptr; pom = pom->next)
				{
					if (pom->isEqual(info))
					{
						prev->next = pom->next;

						delete pom;
						return;
					}
					prev = pom;
				}
			}
		}
	void Update(int value, int add) {
		if (IsEmpty())
			return;

		Node<T>* prev = nullptr;
		Node<T>* curr = head;

		while (curr != nullptr && !curr->IsEqual(value)) {
			prev = curr;
			curr = curr->next;
		}

		if (curr == nullptr)
			return;

		curr->info += add;
		int ref = curr->info;

		bool needsMove = false;

		if (curr->next->info < ref)
			needsMove = true;

		if (!needsMove)
			return;

		if (curr == head) {
			head = curr->next;
		} 
		else
		{
			prev->next = curr->next;
		}

		if (curr == tail)
			tail = prev;

		Node<T>* previous = nullptr;
		Node<T>* position = head;
			
		while (position != nullptr && position->info < ref) {
			previous = position;
			position = position->next;
		}

		if (previous == nullptr) {
			curr->next = head;
			head = curr;
		}
		else
		{
			curr->next = position;
			previous->next = curr;
		}
	};
	void Transpose2(int key) {
		Node<T>* prev3 = nullptr;
		Node<T>* prev2 = nullptr;
		Node<T>* prev1 = nullptr;
		Node<T>* curr = head;

		while (curr != nullptr) {
			if (curr->info == key) {
				if (curr == head)
					break;
				else if (prev2 == nullptr) { // element koji treba da se prebaci je 2. u listi
					head->next = curr->next;
					curr->next = head;
					head = curr;
				}
				else if (prev3 == nullptr) { // element koji treba da se prebaci je 3. u listi
					prev1->next = curr->next;
					curr->next = head;
					head = curr;
				}
				else {
					prev1->next = curr->next; 
					curr->next = prev3->next;
					prev3->next = curr;
				}
			}

			prev3 = prev2;
			prev2 = prev1;
			prev1 = curr;
			curr = curr->next;
		}


	}
	void TransposeFromTo(int from, int to) {
		// pomeri element sa mesta from na mesto to
		Node<T>* dummy = new Node<T>(0);
		dummy->next = head;

		Node<T>* prevFrom = dummy;
		for (int i = 0; i < from; i++) {
			if (prevFrom->next == nullptr) 
				return;

			prevFrom = prevFrom->next;
		}

		Node<T>* prevTo = dummy;
		for (int i = 0; i < to; i++) {
			if (prevTo->next == nullptr)
				return;

			prevTo = prevTo->next;
		}

		Node<T>* elementToMove = prevFrom->next;
		if (elementToMove == nullptr)
			return;

		prevFrom->next = elementToMove->next;

		elementToMove->next = prevTo->next;
		prevTo->next = elementToMove;
	}
	Node<T>* Merge(Node<T>* h1, Node<T>* h2) {
		// dve uredjene liste spaja u jednu tako da nova lista ostane uredjena
	}
	void FillBlanks() {
		/*koja iz
		dinamicki implementirane lancane liste, ciji su info delovi
		celi brojevi, uredjene u neopadajuci redosled, brise sve duple
		cvorove a dodaje cvorove za sve one vrednosti koje nedostaju
		izmedju vrednosti dva uzastopna cvora liste. Na primer,
		ukoliko su info delovi dva uzastopna cvora vrednosti 1 i 4,
		izmedju njih je potrebno dodati i cvorove sa vrednostima 2 i 3.*/

		Node<T>* pom = head;

		while (pom->next != nullptr) 
		{
			if (pom->next->info == pom->info + 1)
			{
				pom = pom->next;
				continue;
			}
			else if (pom->next->info == pom->info) //treba obrisati pom->next (npr. 3, 4, 4, 6, 7. dodje do 2. elementa i preskoci na 4.
													//i zbog toga ne doda broj 5 izmedju 4 i 6
			{
				pom->next = pom->next->next;
				continue;
			}
			else if (pom->next == nullptr)
				return;
			else //znaci da pom->next->info nije za 1 veci od pom->info
			{
				Node<T>* newNode = new Node<T>(pom->info + 1);
				newNode->next = pom->next;
				pom->next = newNode;
			}

			pom = pom->next;
		}
	}
	void MovePartToEnd(int val1, int val2) {
		//znamo da val2 nije ispred val1
		//val2 i val1 sigurno postoje u listi
		//treba uzeti deo lista od val1 do val2 i smestiti ga na kraj liste
		//lista nema tail pokazivac

		Node<T>* p1 = head;
		Node<T>* p1prev = nullptr;
		Node<T>* p2 = nullptr;

		while (p1 != nullptr && !p1->IsEqual(val1))
		{
			p1prev = p1;
			p1 = p1->next;
		}

		p2 = p1;

		while (p2 != nullptr && !p2->IsEqual(val2))
			p2 = p2->next;

		if (p2->next == nullptr) //vec je na kraju
			return;

		Node<T>* p2after = p2->next;

		if (p1prev != nullptr)
			p1prev->next = p2after;
		else
			head = p2after;

		p2->next = nullptr;

		Node<T>* end = head;

		while (end->next != nullptr)
			end = end->next;

		end->next = p1;
	}
	void Swap(T val1, T val2) {
		// vrednosti val1 i val2 su jedinstvene i postoje u listi
		// cvorovi sa vrednostima val1 i val2 nisu susedni
		// cvor sa vr. val2 nije ispred cvora sa vr. val1

		Node<T>* ptr1 = head;
		Node<T>* ptr1Prev = nullptr;
		Node<T>* ptr2 = nullptr;
		Node<T>* ptr2Prev = nullptr;

		Node<T>* pom = nullptr;

		while (ptr1 != nullptr && ptr1->info != val1) {
			ptr1Prev = ptr1;
			ptr1 = ptr1->next;
		}

		ptr2 = ptr1;

		while (ptr2 != nullptr && ptr2->info != val2) {
			ptr2Prev = ptr2;
			ptr2 = ptr2->next;
		}

		if (ptr1 == nullptr || ptr2 == nullptr)
			return; // cvorovi nisu pronadjeni

		pom = ptr2->next;

		if (ptr1 == head) {
			// ptr1Prev == nullptr jer je ptr1 == head
			head = ptr2;
			ptr2Prev->next = ptr1;
			ptr2->next = ptr1->next;
			ptr1->next = pom;
		}
		else {
			ptr1Prev->next = ptr2;
			ptr2Prev->next = ptr1;
			ptr2->next = ptr1->next;
			ptr1->next = pom;
		}
	}
	void GroupDuplicates() {
		Node<T>* dummy = new Node<T>(0);
		dummy->next = head;

		Node<T>* curr = head;
		Node<T>* prevRef = curr;
		Node<T>* ref = curr->next;
		Node<T>* moveTo = nullptr;

		while (curr != nullptr) {
			moveTo = curr;
			prevRef = curr;
			ref = curr->next;

			while (ref != nullptr) {
				if (ref->info == curr->info) {
					Node<T>* pom = ref->next;

					// izbaci ref 
					prevRef->next = ref->next;

					// prebaci ref iza moveTo
					ref->next = moveTo->next;
					moveTo->next = ref;

					ref = pom;
				}
				else {
					prevRef = ref;
					ref = ref->next;
				}
			}

			curr = curr->next;
		}
	};

	Node<T>* MergeTwoLists(Node<T>* h1, Node<T>* h2) {
		Node<T> dummy(0);
		Node<T>* tail = &dummy;

		while (h1 != nullptr && h2 != nullptr) {
			if (h1->info < h2->info) {
				tail->next = h1;
				h1 = h1->next;
			}
			else {
				tail->next = h2;
				h2 = h2->next;
			}

			tail = tail->next;
		}

		tail->next = h1 != nullptr ? h1 : h2;

		return dummy.next;
	}
	Node<T>* MergeNLists(Node<T>* h, int n) {
		Node<T>* merged = h[0];

		for (int i = 1; i < n; i++) {
			merged = MergeTwoLists(merged, h[i]);
		}

		return merged;
	}

	class NodeForDerivate
	{
	public:
		int coef;
		int exp;
		NodeForDerivate* next;

		NodeForDerivate() { next = nullptr; };
		NodeForDerivate(int coef, int exp) { this->coef = coef; this->exp = exp; };
	};
	NodeForDerivate* MakeDerivate(NodeForDerivate* poly, int ext) {
		// funkcija koja od
		// polinoma koji je prosledjen kao parametar funkcije (poly je pokazivac
		// na prvi elemenat liste), formira novi polinom koji predstavlja ext - i
		// izvod presledjenog polinoma. Polinom je zapamcen kao jednostruko
		// ulancana lista elemenata koji predstavljaju nenulte clanove polinoma.
		// Lista je uredjena u opadajuci redosled po eksponentima.

		for (int i = 0; i < ext; i++) {
			NodeForDerivate* dummy = new NodeForDerivate(0, 0);
			dummy->next = poly; // dummy->next = head, on je pre head cvora
			
			NodeForDerivate* prev = dummy;
			NodeForDerivate* curr = poly;

			while (curr != nullptr) {
				if (curr->exp == 0) {
					prev->next = curr->next;
					delete curr;
					curr = prev->next;
				}
				else {
					curr->coef *= curr->exp;
					curr->exp -= 1;

					prev = curr;
					curr = curr->next;
				}
			}

			poly = dummy->next;
			delete dummy;
		}

		return poly;
	}

	// Jun 2 2024.
	void GroupSegments(int k) {
		// u ostatku lancane liste nalazi sve segmente koji su identicni segmenut duzine k
		// koji pocinje od head, i brise ih

		Node<T>* ptr = head;
		Node<T>* prev = nullptr;
		int i = 1;

		while (i < k) {
			ptr = ptr->next;
			i++;
		}
		// sad ptr ukazuje na poslednji iz segmenta k

		ptr = ptr->next;
		Node<T>* start = ptr;

		while (ptr != nullptr) {
			Node<T>* end = nullptr;
			Node<T>* curr = head;
			int counter = 0;

			
			if (ptr->info == head->info) {
				while (ptr != nullptr && curr != nullptr && ptr->info == curr->info && counter < k) {
					counter++;
					end = ptr;
					ptr = ptr->next;
					curr = curr->next;
				}


				if (counter == k) {
					start->next = end->next;
				}
			}

			start = ptr;
			ptr = ptr->next;
		}
	}

	// Januar 2025.
	void SortWithPivot(int pivot) { 
		// treba sve elemente manje od pivot pomeriti na pocetak liste a sve vece na kraj liste

		Node<T>* curr = nullptr;
		Node<T>* lessHead = nullptr;
		Node<T>* lessTail = nullptr;
		Node<T>* aboveHead = nullptr;
		Node<T>* aboveTail = nullptr;

		curr = head;

		while (curr != nullptr) {
			Node<T>* next = curr->next; // zapamti sledeci cvor
			curr->next = nullptr;

			if (curr->info < pivot) {
				if (lessHead == nullptr) {
					lessHead = curr;
					lessTail = curr;
				}
				else {
					lessTail->next = curr;
					lessTail = curr;
				}
			} 
			else {
				if (aboveHead == nullptr) {
					aboveHead = curr;
					aboveTail = curr;
				}
				else {
					aboveTail->next = curr;
					aboveTail = curr;
				}
			}

			curr = next;
		}

		if (lessHead == nullptr)
			this->head = aboveHead;
		else {
			this->head = lessHead;
			lessTail->next = aboveHead;
		}
	}

	// Jun 2024. // uradi opet
	void GroupElements() { 
		// grupise sve elemente koji imaju istu vrednost

		Node<T>* curr = head;

		while (curr != nullptr) {
			Node<T>* prev = curr;
			Node<T>* runner = curr->next;

			while (move != nullptr) {

				if (runner->info == curr->info) {
					prev->next = runner->next;

					runner->next = curr->next;
					curr->next = runner;

					curr = runner;

					runner = prev->next;
				}
				else {
					prev = runner;
					runner = runner->next;
				}
			}

			curr = curr->next;
		}
	}

	// Jun 2 2022.
	List<T>* SplitOrMove() {
		List<T>* newList = new List<T>();

		Node<T>* ptr = head;
		Node<T>* prev = nullptr;

		while (ptr != nullptr) {
			Node<T>* curr = ptr->next;
			Node<T>* nextPtr = ptr->next;
			Node<T>* nextPrevPtr = ptr;
			
			// da li ima duplikata? ako nema onda ce nextPtr postati nullptr
			// ako ima, onda koristimo nextPtrPrev i nextPtr da prelancamo nextPtr u novu listu
			while (nextPtr != nullptr) {
				if (ptr->info == nextPtr->info) 
					break;

				nextPrevPtr = nextPtr;
				nextPtr = nextPtr->next;
			}

			// granicni slucaj u oba slucajeva je ako prebacujemo element sa head pozicije

			if (nextPtr == nullptr) { // znaci nema duplikata, treba da prevezemo ptr u novu listu na head
				if (ptr == head) {
					head = ptr->next;

					ptr->next = newList.head;
					newList.head = ptr;
				}
				else {
					prev->next = ptr->next;

					ptr->next = newList.head;
					newList.head = ptr;
				}
			} 
			else { // postoji duplikat, pa prelancavamo nextPtr u listu
				nextPrevPtr->next = nextPtr->next;

				nextPtr->next = newList.head;
				newList.head = nextPtr;
			}

			prev = ptr;
			ptr = curr;
		}

		return &newList;
	}

	// Jun 2 2023.
	bool MoveSectionToHead(int start, int end) {
		Node<int>* startNode = nullptr;
		Node<int>* startPrev = nullptr;
		Node<int>* endNode = nullptr;
		Node<int>* curr = head;
		Node<int>* prev = nullptr;

		while (curr != nullptr) {
			if (curr->info == start) {
				startNode = curr;
				startPrev = prev;
			}

			if (curr->info == end) {
				endNode = curr;
			}

			prev = curr;
			curr = curr->next;
		}

		if (startNode == nullptr || endNode == nullptr)
			return false;

		if (startPrev == nullptr) // vec je taj segment pocinjao od head
			return true;

		if (startNode == endNode) { // ako je samo jedan cvor u pitanju
			startPrev->next = endNode->next;
			startNode->next = head;
			head = startNode;
		}
		else { // klasicna situacija
			startPrev->next = endNode->next;
			endNode->next = head;
			head = startNode;
		}

		return true;
	}

	// Decembar 2023.
	bool ReverseOrder(int first, int last) {
		Node<int>* firstNode = nullptr;
		Node<int>* lastNode = nullptr;

		Node<int>* prev = nullptr;
		Node<int>* prev2 = nullptr;
		Node<int>* curr = head;
		Node<int>* next = nullptr;

		bool first = false;
		bool last = false;

		while (curr != nullptr) {
			if (curr->info == first) {
				firstNode = curr;
				first = true;
				prev2 = prev;
			}

			if (curr->info == last) {
				lastNode = curr;
				if (!first)
					last = true;
			}

			prev = curr;
			curr = curr->next;
		}

		if (firstNode == nullptr || lastNode == nullptr)
			return false;

		if (last) // znaci last je pre first
			return false;

		prev = prev2;
		curr = firstNode;
		next = curr->next;
		// prev je element pre firstNode
		// curr je firstNode
		// next je sledeci posle firstNode

		while (curr != lastNode) {
			next = curr->next;
			curr->next = prev;
			prev = curr;
			curr = next;
		}
	}
};

template <class T>
class StaticNode
{
public:
	T info;
	int next;

	StaticNode() { next = -1; };
	StaticNode(T info, int next) { this->info = info; this->next = next; };
	T Print() { return info; };
	bool IsEqual(T el) { return this->info == el; };
};

template <class T>
class StaticList
{
public:
	StaticNode<T>* arr;
	int size;
	int currNumOfElements;
	int head;
	int lrmp;

	// lrmp je lista raspolozivog mem. prostora. On pokazuje koji je sledeci element koji je slobodan
	StaticList() { arr = nullptr; size = 0; currNumOfElements = 0; head = -1; lrmp = -1; };
	StaticList(int size) { 
		arr = new T[size]; 
		this->size = size; 
		currNumOfElements = 0;
		head = -1; 
		lrmp = 0;
		
		for (int i = 0; i < size; i++) {
			arr[i].next = i + 1;
		}

		arr[size - 1].next = -1;
	};

	int GetFirstFreeNode() {
		if (lrmp == -1)
			return -1;

		int p = lrmp;
		lrmp = arr[p].next;

		return p;
	}

	void Insert(T el) {
		int p = GetFirstFreeNode();

		if (p == -1)
			return;

		arr[p].info = el;
		arr[p].next = -1;

		if (head == -1)
			head = p;
		else { // uveze poslednji sa dodatim
			int q = head;

			while (arr[q].next != -1)
				q = arr[q].next;

			arr[q].next = p;
		}
	}
};

class PlayerAsNode
{
public:
	int id;
	char* name;
	char* lastName;
	int numOfWins;
	PlayerAsNode* next;
	PlayerAsNode() { this->next = nullptr; };
	PlayerAsNode(int id, const char* name, const char* lastName, int numOfWins) {
		this->id = id;
		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
		this->lastName = new char[strlen(lastName) + 1];
		strcpy(this->lastName, lastName);
		this->numOfWins = numOfWins;
	}
	int GetWins() { return numOfWins; };
	char* GetName() { return name; };
};
class PlayerAsLL
{
public:
	PlayerAsNode* head;

	void UpdatePlayer(char* name, int numberOfWins) {
		// Aplikacija vodi evidenciju o broju pobeda aktivnih tenisera u
		// toku karijere. Informacije o teniserima su zapamcene u
		// lancanoj listi uredjenoj po broju pobeda. Za svakog tenisera se
		// pamti jedinstveni broj(int), ime i prezime i broj pobeda. Nakon
		// svakog teniskog turnira se azuriraju bodovi svih ucesnika
		// turnira. Napisati funkciju void UpdatePlayer(char* name, int
		// noWin) koja azurira(povecava) broj pobeda zadatog tenisera,
		// cije je ime name, za zadatu vrednost noWin, ali tako da
		// koriscena struktura ostane uredjena.

		PlayerAsNode* p = head;
		PlayerAsNode* prev = nullptr;

		while (strcmp(p->name, name) != 0) {
			prev = p;
			p = p->next;
		}

		p->numOfWins += numberOfWins;

		prev->next = p->next; // izbaci cvor iz liste
		p->next = nullptr;

		if (p->numOfWins < head->numOfWins) {
			p->next = head;
			head = p;
		}
		else {
			PlayerAsNode* curr = head;

			while (curr->next != nullptr && curr->next->numOfWins < p->numOfWins) {
				curr = curr->next;
			}

			p->next = curr->next;
			curr->next = p;
		}

	}
};

// Jun 2025
class JunNode
{
public:
	int indeks;
	int bodovi;
	JunNode* next;

	JunNode() : next(nullptr) {};
	JunNode(int bodovi) : next(nullptr), bodovi(bodovi) {};
};
class JunList
{
public:
	JunNode* head;
	JunNode* tail;

	JunList() : head(nullptr), tail(nullptr) {};

	void AddToHead(int bodovi) {
		JunNode* cvor = new JunNode(bodovi);

		if (head != nullptr)
			cvor->next = head;
		
		head = cvor;
	}
	JunNode* RemoveFromHead() {
		if (head == nullptr)
			return nullptr;

		JunNode* temp = head;
		head = temp->next;
		return temp;
	}
	void Calculate(JunList* lista) {
		// lista this je uredjena, a lista lista nije
		// treba da prebaci elemente iz lista u this tako da ona ostane uredjena po broju bodova

		if (lista->head == nullptr)
			return;

		JunNode* probe = lista->RemoveFromHead();

		while (probe != nullptr) {
			JunNode* temp = probe->next;

			JunNode* curr = head;
			JunNode* prev = nullptr;

			while (curr != nullptr && probe->bodovi >= curr->bodovi) {
				prev = curr;
				curr = prev->next;
			}

			if (prev == nullptr) { // treba da se ubaci ispred head
				probe->next = head;
				head = probe;
			}
			else {
				prev->next = probe;
				probe->next = curr;
			}

			probe = temp;
		}
	}
	void Print() {
		JunNode* curr = head;

		while (curr != nullptr) {
			std::cout << curr->bodovi << " -> ";
			curr = curr->next;
		}
	}
};