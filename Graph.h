#pragma once
#include <iostream>
#include "Queue.h"
#include "Heap.h"

// orijentisan graf - bitno je koji je 1. a koji 2. element
//
// grana e orijentisanog grafa IZVIRE iz cvora v ako se cvor v javlja kao prvi cvor
// koji definise dati poteg. A ako se nalazi kao drugi cvor datog potega, onda UVIRE
//
// izlazni stepen nekog cvora je broj potega koji izvire iz njega, 
// a ulazni stepen je broj potega koji uvire u njega.
// stepen cvora je zbir ta dva
//
// put P u usmerenom cvoru je neprazan niz cvorova takvih da izmedju svaga dva postoji poteg
// duzina puta P je k-1, odnosno broj potega koji treba preci da bi se stiglo od 1. do poslednjeg
// put je PROST ako ne prelazi dva puta preko istog cvora
//
// CIKLUS je put kod koga se poklapaju prvi i poslednji cvor
// ciklus je PROST ako zadovoljava uslov jednostavnog puta
// ciklus duzine 1 je PETLJA
// usmereni graf koji ne sadrzi cikluse naziva se usmereni aciklicni graf (primer toga je stablo)
//
// JAKO POVEZANI graf je orijentisani graf kod koga postoji put izmedju bilo koja dva cvora
// SLABO POVEZANI nema to svojstvo, ali ga mora imati ako se sve orijentisane grane zamene neorijentisanim
// ukoliko nije ni jedno ni drugo onda je NEPOVEZAN
// 
// graf se moze predstaviti staticki (matrica susedstva) i dinamicki (lista suseda)
// ------------------------------------------------------------------------------
//
// MATRICA SUSEDSTVA ima samo 0 i 1, 1 se upisuje ako postji poteg izmedju dva cvora
// npr. ako postoji poteg od cvora 1 ka 4, na poziciji (1, 4) odnosno indeksima (0, 3) se upisuje 1
// ako po glavnoj dijagonali te matrice su sve 0, znaci da ne postoje petlje u tom grafu.
// pored te matrice ce uglavnom morati da se cuva i vektor koji ce cuvati informacije o cvora,
// posto matrica cuva samo da li postoji veza ili ne
// 
// -------------------------------------------------
//
// LISTA SUSEDSTVA se koristi kada nije visok stepen povezanosti cvorova
// cvorovi u sebi sadrze kljuc i pokazivac na listu suseda, npr. |1| -> |3| -> |4|
//																  |
//                                                               |2| -> |1| -> |3| -> |4| ..
// a svi cvorovi su smesteni u jednu lancanu listu
// 
// ------------------------------------------------
// 
// ALGORITAM OBILASKA GRAFA: 1. svim cvorovima se postavi status neobradjen
//				   2. uzeti startni cvor i smestiti ga u RED (za obilazak po sirini) ili STEK (za obilazak po dubini) i promeniti na smesten
//				   3. dok struktura (red ili stek) nije prazna:
//						a) uzeti cvor iz strukture, obraditi ga i postaviti na obradjen
//						b) sve njegove susede koji imaju neobradjen, smestiti u strukturu i promeniti na smesten
// 
// -------------------------------------------------
//
// TOPOLOSKO UREDJENJE: to je proces linearizacije grafa. On se moze linearizovati ako cvorovi zadovoljavaju:
// 1. nerefleksivno (nije u relaciji sa samim sobom); 2. antisimetricno (ako je A u rel. sa B, onda B nije u rel. sa A)
// 3. tranzitivno (ako je A u rel. sa B, a B u rel. sa C, onda je i A u rel. sa C)
// topoloskim uredjenjem cvorova dobija se vektor cvorova koji sadrzi uredjenost definisanu granama grafa
//
// ALGORITAM TOPOLOSKOG UREDJENJA
// 1. sve cvorove koji imaju ulazni stepen 0 smestiti u red
// 2. dok red nije prazan:
//				a) uzeti cvor sa pocetka reda i obraditi ga
//				b) svim njegovim susedima smanjiti ulazni stepen za 1 (ako postane 0, smestiti ih u red)
// 
// -------------------------------------------------
//
// NAJKRACI PUT: postoje algoritmi koji nalaze najkr. put od jednog do svih ostalih cvorova i koji nalaze od svih do svih
//
// DIJKSTRIN ALGORITAM: 1. svim cvorovima postavi status obredjen = false i rastojanje = beskonacno
//						2. izabrati startni cvor S i njemu postaviti obradjen = true i rastojanje = 0
//						3. svim susedima (vi) cvora S postaviti vi.rastojanje = C(S, vi), gde je to C tezina grane od C do vi
//						4. dok nisu svi obradjeni (odnosno u vecini slucajeva dok trazeni cvor ne postane radni cvor r)
//							a) za radni cvor r izabrati (od svih cvorova u grafu) cvor sa najmanjim rastojanjem
//							ciji je status obradjen = false i postaviti ga na true
//							b) za sve susede vi cvora r ciji je status obradjen = false proveriti da li je vrednost
//							vi.rastojanje > r.rastojanje + C(r, vi), ako jeste postaviti
//							vi.rastojanje = r.rastojanje + C(r, vi) 
//
// za korak 4.a: sve cvorove smestati u minHeap i on sam pokazuje najmanji
//
// ---------------------------------------------------
//
// FLOJDOV ALGORITAM: nalazi najkraci put na osnovu matrice tezine grana
// 
// flojdov algoritam zatim racuna matrice D1, D2... Dv koriscenjem formule:
// D(i+1) = min(Di(v, v(i+1)) + Di(v(i+1), w), Di(v, w))
// kojom se za svaki par cvorova (v, w) proverava da li je rastojanje D(v, w), koje predstavlja najkraci put od v do w
// koji ne prolazi kroz v(i+1) duze od zbira najkracih rastojanja od v do v(i+1) i od v(i+1) do w.
// Ako jeste, novo najkrace rastojanje predstavlja sumu najkracih puteva preko cvora v(i+1)
// 
// FlojdovAlgoritam(matrix w) - tezinska matrica
// {
//		for (int i = 1; i <= vertexNo; i++)
//			for (int j = 1; j <= vertexNo; j++)
//				for (int k = 1; j <= vertexNo; k++)
//					if (w[j][k] > (w[j][i] + w[i][k]))
//						w[j][k] = w[j][i] + w[i][k];
// }
// petlja j i k sluze za prolazak kroz citavu matricu a patlja i kaze preko kog elementa se trazi kraci put
// ako je mali graf, koristi se ovaj algoritam
//

// MinHeap
template <class T, class W>
class LinkedNode;
template <class T, class W>
class MinHeapForGraph
{
private:
	LinkedNode<T, W>** arr;
	int length;
	int numberOfElements;

public:
	MinHeapForGraph(int length) {
		this->length = length;
		numberOfElements = 0;
		arr = new LinkedNode<T, W>*[length + 1];

		for (int i = 0; i < length + 1; i++)
			arr[i] = nullptr;
	};
	~MinHeapForGraph() { delete[] arr; };
	bool IsEmpty() { return numberOfElements == 0; };
	void Insert(LinkedNode<T, W>* el) {
		if (numberOfElements == length - 1)
			return;

		numberOfElements++;
		int i = numberOfElements;

		while (i > 1 && arr[i / 2]->distance > el->distance) { // arr[i/2] je roditelj elementa arr[i]
			arr[i] = arr[i / 2];
			i /= 2;
		}

		arr[i] = el;
	};
	LinkedNode<T, W>* DeleteRoot() {
		if (numberOfElements == 0)
			return nullptr;

		LinkedNode<T, W>* result = arr[1];
		LinkedNode<T, W>* last = arr[numberOfElements];
		int i = 1;

		numberOfElements--;

		while (2 * i < numberOfElements + 1) {
			int child = 2 * i;

			if (child + 1 < numberOfElements + 1 && arr[child + 1]->distance < arr[child]->distance)
				child += 1;

			if (last->distance < arr[child]->distance)
				break;

			arr[i] = arr[child];
			i = child;
		}

		arr[i] = last;
		return result;
	};

	void Swap(int i, int j) {
		std::swap(arr[i], arr[j]);
	}
	void BubbleUp(int i) {
		while (i > 1) {
			int parent = 2 * i;

			if (arr[i]->distance < arr[parent]->distance) {
				Swap(i, parent);
				i = parent;
			}
			else
				break;
		}
	};
	void BubbleDown(int i) {
		while (2 * i < numberOfElements + 1) {
			int child = 2 * i;
			int smallest = i;

			if (arr[child]->distance < arr[smallest]->distance) smallest = child;

			if (arr[child + 1]->distance < arr[smallest]->distance) smallest = child + 1;

			if (smallest != i) {
				Swap(i, smallest);
				i = smallest;
			}
			else
				break;
		}
	};
	void Update(LinkedNode<T, W>* oldValue, double newValue) {
		int index;

		for (int i = 0; i < numberOfElements; i++)
			if (arr[i]->distance == oldValue->distance)
				index = i;

		if (oldValue->key < newValue)
			BubbleDown[index];
		else
			BubbleUp[index];
	}

};
// ---------

template <class T, class W>
class Edge;

template <class T, class W>
class LinkedNode
{
public:
	T key;
	Edge<T, W>* adj; // pokazivac na prvi u listi suseda
	LinkedNode<T, W>* next; // pokazivac na sledeci u listi svih cvorova
	int status; // 1 - neobradjen, 2 - smesten u red/stek, 3 - obradjen
	int degree; // ulazni stepen

	// za Dijsktra algoritam
	LinkedNode<T, W>* prev;
	double distance;
	// ---------------------

	LinkedNode() { adj = nullptr; next = nullptr; status = 0; degree = 0; prev = nullptr; };
	LinkedNode(T key) { this->key = key; adj = nullptr; next = nullptr; status = 0; degree = 0; };
	LinkedNode(T key, Edge<T, W>* adj, LinkedNode<T, W>* next, int status) {
		this->key = key;
		this->adj = adj;
		this->next = next;
		this->status = status;
		prev = nullptr;
		distance = 0;
		degree = 0;
	};
	void Visit() { std::cout << key << std::endl; };
};

template <class T, class W>
class Edge
{
public:
	LinkedNode<T, W>* dest;
	Edge<T, W>* link;
	double weight;

	Edge() { dest = nullptr; link = nullptr; };
	Edge(LinkedNode<T, W>* dest, Edge<T, W>* link) {
		this->dest = dest;
		this->link = link;
	}
	Edge(LinkedNode<T, W>* dest, Edge<T, W>* link, double weight) {
		this->dest = dest;
		this->link = link;
		this->weight = weight;
	}
};

template <class T, class W>
class GraphAsList
{
protected:
	LinkedNode<T, W>* start;
	int numberOfNodes;

public:
	GraphAsList() { start = nullptr; numberOfNodes = 0; };
	~GraphAsList() {};
	LinkedNode<T, W>* FindNode(T pod) {
		LinkedNode<T, W>* ptr = start;

		while (ptr != nullptr && ptr->key != pod)
			ptr = ptr->next;

		return ptr;
	};
	Edge<T, W>* FindEdge(T a, T b) {
		// trazi poteg koji ide od cvora sa vrednoscu a do cvora sa vrednosu b
		LinkedNode<T, W>* nodeA = FindNode(a);
		LinkedNode<T, W>* nodeB = FindNode(b);

		if (nodeA == nullptr || nodeB == nullptr)
			return nullptr;

		Edge<T, W>* ptr = nodeA->adj;
		
		while (ptr != nullptr && ptr->dest != nodeB)
			ptr = ptr->link;

		return ptr;
	};
	bool InsertNode(T pod) {
		LinkedNode<T, W>* ptr = start;

		while (ptr != nullptr) {
			if (ptr->key == pod)
				return false;

			ptr = ptr->next;
		}

		LinkedNode<T, W>* newNode = new LinkedNode<T, W>(pod, nullptr, start, 0);

		if (newNode == nullptr)
			return false;

		start = newNode;
		numberOfNodes++;
		return true;
	};
	bool DeleteNode(T pod) {
		LinkedNode<T, W>* ptr;

		if (start == nullptr)
			return false;

		// ako treba da se brise startni cvor
		if (start->key == pod) {
			Edge<T, W>* pom = start->adj;

			// brise sve edgeve (celu listu suseda)
			while (pom != nullptr) {
				Edge<T, W>* nextPom = pom->link;
				delete pom;
				pom = nextPom;
			}

			DeleteEdgeToNode(start); // brise sve potege koji za odrediste imaju cvor koji se brise

			ptr = start;
			start = start->next;
			delete ptr;
			numberOfNodes--;
			return true;
		} 
		else {
			ptr = start->next;
			LinkedNode<T, W>* parent = start;

			while (ptr != nullptr && ptr->key != pod) {
				parent = ptr;
				ptr = ptr->next;
			}

			if (ptr == nullptr)
				return false;

			parent->next = ptr->next;

			Edge<T, W>* firstInList = ptr->adj;

			while (firstInList != nullptr) {
				Edge<T, W>* nextInList = firstInList->link;
				delete firstInList;
				firstInList = nextInList;
			}

			DeleteEdgeToNode(ptr);
			delete ptr;
			numberOfNodes--;
			return true;
		}
	};
	bool InsertEdge(T a, T b, double weight) {
		LinkedNode<T, W>* nodeA = FindNode(a);
		LinkedNode<T, W>* nodeB = FindNode(b);

		if (nodeA == nullptr || nodeB == nullptr)
			return false;

		// dodaje se na pocetak liste suseda za cvor A
		// znaci umetne se na trenutno mesto nodeA->adj
		Edge<T, W>* ptr = new Edge<T, W>(nodeB, nodeA->adj, weight);
		
		if (ptr == nullptr)
			return false;

		nodeA->adj = ptr;
		return true;
	};
	bool DeleteEdge(T a, T b) {
		Edge<T, W>* ptr = FindEdge(a, b);
		
		if (ptr == nullptr)
			return false;

		LinkedNode<T, W>* nodeA = FindNode(a);

		// da li je prvi u listi suseda
		if (ptr == nodeA->adj) {
			nodeA->adj = ptr->link;
			delete ptr;	
			return true;
		}

		Edge<T, W>* pom = nodeA->adj;

		while (pom->link != ptr) {
			pom = pom->link;
		}

		pom->link = ptr->link;
		delete ptr;
		return true;
	};
	int BreadthTrav(T a) {
		// obilazak po sirini
		// krece se od cvora a

		int returnValue = 0; // broj obradjenih cvorova
		LinkedNode<T, W>* ptr = start;
		QueueArray<LinkedNode<T, W>*> queue(numberOfNodes);

		// svaki cvor postavlja na neobradjen
		while (ptr != nullptr) {
			ptr->status = 1;
			ptr = ptr->next;
		}

		ptr = FindNode(a);

		if (ptr == nullptr)
			return 0;

		queue.Enqueue(ptr);
		ptr->status = 2;

		while (!queue.IsEmpty()) {
			ptr = queue.Dequeue(); // skida sa pocetka reda
			ptr->status = 3;
			ptr->Visit();
			returnValue++;

			Edge<T, W>* poteg = ptr->adj;

			while (poteg != nullptr) {
				// ako je neobradjen
				if (poteg->dest->status == 1) {
					poteg->dest->status = 2;
					queue.Enqueue(poteg->dest);
				}

				poteg = poteg->link;
			}
		}

		return returnValue;
	};
	int DepthTrav(T a) {
		// isto kao po sirini samo koristi stack
	};

	int DepthTravRecursive(T a) {
		LinkedNode<T, W>* pFirst = nullptr;
		LinkedNode<T, W>* ptr = start;

		while (ptr != nullptr) {
			ptr->status = 0;

			if (ptr->key == a)
				pFirst = ptr;

			ptr = ptr->next;
		}

		if (pFirst == nullptr)
			return 0;

		return DepthTravRecursive(pFirst);
	};
	int DepthTravRecursive(LinkedNode<T, W>* ptr) {
		int returnValue = 0;

		if (ptr->status != 1) {
			ptr->status = 1;
			ptr->Visit();
			returnValue++;
			Edge<T, W>* poteg = ptr->adj;

			while (poteg != nullptr) {
				returnValue += DepthTravRecursive(poteg->dest);
				poteg = poteg->link;
			}
		}

		return returnValue;
	}

	int TopologicalOrderTrav() {
		int returnValue = 0;
		LinkedNode<T, W>* ptr = start;

		while (ptr != nullptr) {
			ptr->numberOfPot = 0;
			ptr = ptr->next;
		}

		ptr = start;

		while (ptr != nullptr) {
			Edge<T, W>* poteg = ptr->adj;

			// ovime odredjujemo koliko potega dolazi do svakog od cvorova, odnosno ulazni stepen svakog cvora
			// ovde degree koristimo kao broj potega koji dolaze do datog cvora
			while (poteg != nullptr) {
				poteg->dest->degree += 1;
				poteg = poteg->link;
			}

			ptr = ptr->next;
		}

		QueueArray<LinkedNode<T, W>* > queue(numberOfNodes);
		ptr = start;

		while (ptr != nullptr) {
			if (ptr->degree == 0)
				queue.Enqueue(ptr);

			ptr = ptr->next;
		}

		while (!queue.IsEmpty()) {
			ptr = queue.Dequeue();
			ptr->Visit();
			returnValue += 1;
			Edge<T, W>* poteg = ptr->adj;

			while (poteg != nullptr) {
				poteg->dest->degree -= 1;

				if (poteg->dest->degree == 0)
					queue.Enqueue(poteg->dest);

				poteg = poteg->link;
			}
		}

		return returnValue;
	};
	void Print() {
		LinkedNode<T, W>* ptr = start;
		
		while (ptr != nullptr) {
			std::cout << ptr->key << " -> ";
			Edge<T, W>* edge = ptr->adj;

			while (edge != nullptr) {
				std::cout << edge->dest->key << " | ";
				edge = edge->link;
			}

			std::cout << "\r\n";
			ptr = ptr->next;
		}
	};
	void PrintDistance() {
		LinkedNode<T, W>* ptr = start;

		while (ptr != nullptr) {
			std::cout << ptr->key << " -> ";
			std::cout << ptr->distance;

			std::cout << "\r\n";
			ptr = ptr->next;
		}
	}

	// ispravi FindPath tako da ne koristi arPath vec da koristi prev pokazivac za pamcenje puta
	int FindPath(T first, T last, T* arPath, int* lPath) {
		// funkcija za nalazenje puta izmedju cvorova koriscenjem rekurzivnog pretrazivanja po dubini
		
		LinkedNode<T, W>* pFirst = nullptr;
		LinkedNode<T, W>* pLast = nullptr;
		LinkedNode<T, W>* ptr = start;

		while (ptr != nullptr) {
			ptr->status = 0;

			if (ptr->key == first)
				pFirst = ptr;

			if (ptr->key == last)
				pLast = ptr;

			ptr = ptr->next;
		}

		if (pFirst == nullptr || pLast == nullptr)
			return 0;

		return FindPath(pFirst, pLast, arPath, lPath);
	}
	int FindPath(LinkedNode<T, W>* pFirst, LinkedNode<T, W>* pLast, T* arPath, int* lPath) {
		// lPath je duzina puta
		// arPath su obidjeni cvorovi
		// returnValue vraca broj svih obidjenih cvorova 

		if (pFirst->key == pLast->key) {
			if (arPath != nullptr)
				arPath[(*lPath)++] = pFirst->key;

			return 1;
		}

		int returnValue = 0;

		if (pFirst->status != 1) {
			pFirst->status = 1;
			returnValue++;
			Edge<T, W>* poteg = pFirst->adj;

			while (lPath != nullptr && *lPath == 0 && poteg != nullptr) {
				returnValue += FindPath(poteg->dest, pLast, arPath, lPath);
				poteg = poteg->link;
			}

			if (lPath != nullptr && *lPath != 0 && arPath != nullptr)
				arPath[(*lPath)++] = pFirst->key;
		}

		return returnValue;
	}

	int Dijsktra(T a, int type) {
		// type sluzi da odabere kod da li ce koristiti minheap ili nesto drugo
		// najbolje uvek koristiti minheap

		int returnValue = 0;
		LinkedNode<T, W>* ptr = start;

		// dodeljivanje status = 1 svim cvorovima
		while (ptr != nullptr) {
			ptr->status = 1;
			ptr->prev = nullptr;
			ptr->distance = 999999; // najbolje numeric_limits<double>::max();

			ptr = ptr->next;
		}

		ptr = FindNode(a);

		if (ptr == nullptr)
			return 0;

		MinHeapForGraph<T, W>* priority = new MinHeapForGraph<T, W>*(numberOfNodes);

		ptr->distance = 0;
		priority->Insert(ptr);
		ptr->status = 2;

		while (!priority->IsEmpty()) {
			ptr = priority->DeleteRoot();
			ptr->status = 3;
			returnValue++;

			Edge<T, W>* poteg = ptr->adj;

			while (poteg != nullptr) {
				if (poteg->dest->status == 1) {
					poteg->dest->distance = ptr->distance + poteg->weight;
					poteg->dest->status = 2;
					poteg->dest->prev = ptr;
					priority->Insert(poteg->dest);
				}
				else if (poteg->dest->status == 2) {
					if (poteg->dest->distance > ptr->distance + poteg->weight) {
						double newDistance = ptr->distance + poteg->weight;
						poteg->dest->prev = ptr;
						priority->Update(poteg->dest, newDistance);
						poteg->dest->distance = newDistance;
					}

					poteg = poteg->link;
				}
			}
		}


		return returnValue;
	}

	void Dijkstra2(T a) {
		// sam napisi
		LinkedNode<T, W>* ptr = start;
		LinkedNode<T, W>* curr = nullptr;
		int returnValue = 0;

		while (ptr != nullptr) {
			ptr->distance = 99999;
			ptr->status = 1;
			
			ptr = ptr->next;
		}

		ptr = FindNode(a);
		ptr->distance = 0;

		MinHeapForGraph<T, W>* minHeap = new MinHeapForGraph<T, W>(numberOfNodes);
		minHeap->Insert(ptr);
		ptr->status = 2;

		while (!minHeap->IsEmpty()) {
			curr = minHeap->DeleteRoot();
			returnValue++;
			
			if (curr->status == 3) 
				continue;

			curr->status = 3;

			Edge<T, W>* poteg = curr->adj;

			while (poteg != nullptr) {
				LinkedNode<T, W>* neighbor = poteg->dest;
				double newDistance = curr->distance + poteg->weight;

				if (newDistance < neighbor->distance) {
					neighbor->distance = newDistance;
					neighbor->prev = curr;
					minHeap->Insert(neighbor);
					neighbor->status = 2;
				}

				poteg = poteg->link;
			}
		}
	}

	// ISPIT

	// Jun 2 2024.
	void DepthFirstSearch(LinkedNode<T, W>* node, int* result, int& index) {
		node->status = 2; // posecen

		Edge<T, W>* edge = node->adj;

		while (edge != nullptr) {
			LinkedNode<T, W>* neighbor = edge->dest;

			if (neighbor->status != 2)
				DepthFirstSearch(neighbor, result, index);

			edge = edge->link;
		}

		result[index++] = node->key;
	}
	int* DefineOrder(int* dep, int n) {
		// dobija polje, npr [1, 2, 1, 4 ..], to znaci proces 1 zaviis od 2 i od 4
		// funkcija treba da vrati redosled aktiviranja procesa, tako da se svaki aktivira nakon sto se aktiviraju oni
		// od kojih on zavisi
		GraphAsList graph;
		int* result = new int[n];
		int index = 0;

		for (int i = 0; i < n; i++) {
			graph.InsertNode(dep[i]);
		}

		for (int i = 0; i < n; i += 2) {
			graph.InsertEdge(dep[i], dep[i + 1], 1);
		}

		LinkedNode<T, W>* ptr = graph.start;

		while (ptr != nullptr) {
			if (ptr->status != 2)
				DepthFirstSearch(ptr, result, index);

			ptr = ptr->next;
		}

		return result;
	}



protected:
	void DeleteEdgeToNode(LinkedNode<T, W>* ptr) {};
};