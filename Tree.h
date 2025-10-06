#pragma once
#include <iostream>
#include "Stack.h"
#include "Queue.h"

// mogu biti predstavljena dinamicki i staticki
// kod dinamicke reprezentacije BINARNOG stabla, svaki cvor se sastoji iz info, left* i right*
// gde left i right pokazuju na cvorove u sledecem nivou ako ih ima
//
// kod staticke reprezentacije BINARNOG stabla, vrednosti se upisuju u niz tako da je
// roditeljski cvor smesten na indeksu i, njegov levi potomak na 2i + 1, a desni na 2i + 2
// ako znamo indeks potomka, do roditelja se dolazi tako sto se od indeksa oduzme 1, pa se celobrojno podeli sa 2
//
// obilazak moze biti DepthFirst(preorder, inorder, postorder) ili breadthFirst
//
// velicina steka kod DepthFirst treba biti jednaka velicini stabla (visini),
// a velicina reda kod BreadthFirst treba biti jednaka sirini najsireg sloja
//
// ako imamo binarno stablo (gde su levi elemtni manji a desni veci) INORDER search ce dati rastuci uredjeni niz
//
// BRISANJE (3 slucaja): 1. ako se brise cvor koji je list - samo se uklanja
//						 2. ako se brise cvor koji ima 1 potomka - cvor se uklanja a na njegovo mesto dolazi potomak
//						 3. ako se brise cvor sa 2 potomka - koristi se deleteByCopying ili deleteByMerge

template <class T>
class BNode
{
public:
	T key;
	BNode<T>* left, * right;

	BNode() { left = right = nullptr; };
	BNode(T key) { this->key = key; left = right = nullptr; };
	BNode(T key, BNode<T>* left, BNode<T>* right) {
		this->key = key;
		this->left = left;
		this->right = right;
	}

	bool IsLessThan(T key) {
		if (this->key < key) return true;
		else return false;
	}
	bool IsGreaterThan(T key) {
		if (this->key > key) return true;
		else return false;
	}
	bool IsEqual(T key) {
		return this->key == key;
	}
	void SetKey(T key) { this->key = key; };
	T GetKey() { return key; };
	void Print() { std::cout << key << " "; };
};

template <class T>
class BTree
{
protected:
	BNode<T>* root;
	int numberOfElements;

public:
	BTree() { root = nullptr; numberOfElements = 0; };
	~BTree() { DeleteTree(root); };
	void DeleteTree(BNode<T>* p) {
		// postorder pristup 
		if (p != nullptr) {
			DeleteTree(p->left);
			DeleteTree(p->right);
			delete p;
		}
	}
	bool IsEmpty() { return root == nullptr; };
	void Insert(T el) {
		// umetanje u UREDJENO stablo
		BNode<T>* p = root;
		BNode<T>* prev = nullptr;

		while (p != nullptr) {
			prev = p;
			if (p->IsLessThan(el))
				p = p->right;
			else
				p = p->left;
		}

		if (root == nullptr) // stablo je prazno
			root = new BNode<T>(el);
		else if (prev->IsLessThan(el))
			prev->right = new BNode<T>(el);
		else
			prev->left = new BNode<T>(el);

		numberOfElements++;
	};

	bool IsInTree(T el) { return Search(el) != nullptr; };
	BNode<T>* Search(T el) { return Search(root, el); };
	BNode<T>* Search(BNode<T>* p, T el) {
		// p je root po default ali moze da bude bilo koji cvor
		while (p != nullptr) {
			if (p->IsEqual(el))
				return p;
			else if (p->IsLessThan(el))
				p = p->right;
			else
				p = p->left;
		}

		return nullptr;
	}
	void Balance(int data[], int first, int last) {
		// 1. sve cvorove iskopirati u polje i obrisati stablo
		// 2. urediti polje u rastuci redosled
		// 3. formirati novo stablo ciji koren postaje sredisnji element uredjenog polja
		// 4. levo podstablo kreira se od leve polovine uredjenog polja
		// 5. desno podstablo kreira se od desne polovine

		if (first <= last) {
			int middle = (first + last) / 2;
			Insert(data[middle]);
			Balance(data, first, middle - 1);
			Balance(data, middle + 1, last);
		}
	}
	void DeleteByCopying(T el) {
		// trazi element koji je >= od svih u levom podstablu i < od svih u desnom podstablu
		// kada bude postavljen na mesto obrisanog elementa
		// kandidati za zamenu sa obrisanim elementom su: krajnji desni cvor levog podstabla ili krajnji levi desnog podstabla
		
		BNode<T>* node, * p = root, * prev = nullptr;
		
		// trazenje zeljenog cvora
		while (p != nullptr && !p->IsEqual(el)) { 
			prev = p;
			if (p->IsLessThan(el))
				p = p->right;
			else
				p = p->left;
		}
		node = p;
		// ------------------

		if (p != nullptr && p->IsEqual(el)) {
			// ako nema jednog potomka
			if (node->right == nullptr) 
				node = node->left;
			else if (node->left == nullptr)
				node = node->right;
			// ----------------

			// kad ima oba potomka
			else {
				BNode<T>* tmp = node->left; // pocetak levog podstabla
				BNode<T>* previous = node;

				while (tmp->right != nullptr) { // trazenje krajnjeg desnog u levom podstablu
					previous = tmp;
					tmp = tmp->right;
				}

				node->SetKey(tmp->GetKey());

				if (previous == node)           // znaci nije se izvrsila gornja while petlja
					previous->left = tmp->left;
				else
					previous->right = tmp->left;

				delete tmp;
				numberOfElements--;
				
				return;
			}

			if (p == root)
				root = node;
			else if (prev->left == p)
				prev->left = node;
			else
				prev->right = node;

			delete p;
			numberOfElements--;
		}
		else if (root != nullptr)
			return; // element ne postoji
		else
			return; // drvo je prazno
	}
	void DeleteByMerging(T el) {
		// nadju se krajnji desni u levom podstablu (A) i krajnji levi u desnom podstablu (B)
		// i onda se celo to desno podstablo zakaci na desnu granu cvora A
		// i onda cvor koji zelimo da brisemo ima samo jednog potomka, pa je brisanje jednostavno

		BNode<T>* tmp, * node, * p = root, * prev = nullptr;

		while (p != nullptr && !p->IsEqual(el)) {
			prev = p;
			if (p->IsLessThan(el))
				p = p->right;
			else
				p = p->left;
		}
		node = p;

		if (p != nullptr && p->IsEqual(el)) {
			if (node->right == nullptr)
				node = node->left;
			else if (node->left == nullptr)
				node = node->right;
			else {
				tmp = node->left;

				while (tmp->right != nullptr) // nalazenje krajnjeg desnog u levom podstablu
					tmp = tmp->right;

				tmp->right = node->right; // zakaci levo podstablo za cvor A
				node = node->left;
			}

			if (p == root)
				root = node;
			else if (prev->left == p)
				prev->left = node;
			else
				prev->right = node;

			delete p;
			numberOfElements--;
		}
		else if (root != nullptr)
			return; // element ne postoji
		else
			return; // drvo je prazno
	}
	BNode<T>* GetRoot() { return root; };

	// DEPTH FIRST SEARCH
	void Preorder() { Preorder(root); };
	void Inorder() { Inorder(root); };
	void Postorder() { Postorder(root); };
	void Preorder(BNode<T>* p) {
		// prvo se obradi 1. cvor (root) pa onda prvo cela njegova leva strana, pa tek desna
		if (p != nullptr){       //				A
			p->Print();          //			 B     C
			Preorder(p->left);   //          F E   D G
			Preorder(p->right);  //
								 //      -> A B F E C D G
		}
	}
	void Inorder(BNode<T>* p) {  //              A
		if (p != nullptr) {      //           B     C
			Inorder(p->left);    //          F E   D G
			p->Print();          //
			Inorder(p->right);   //      -> F B E A D C G
		}
	}
	void Postorder(BNode<T>* p) {//              A
		if (p != nullptr) {      //           B     C
			Postorder(p->left);  //          F E   D G
			Postorder(p->right); //
			p->Print();			 //      -> F E B D G C A
		}
	}
	void IterativePreorder() { // NAJCESCI
		BNode<T>* p = root;
		StackArray<BNode<T>*> stack(numberOfElements);

		if (p != nullptr) {               //               7
			stack.Push(p);                //           3       8
			                              //        2     5      9
			while (!stack.IsEmpty()) {    //       1     4
				p = stack.Pop();          //
				p->Print();               //	-> 7 3 2 1 5 4 8 9
				                          // 
				if (p->right != nullptr)  //	stavlja se prvo desni potomak da bi se ispisao prvo levi
					stack.Push(p->right);

				if (p->left != nullptr)
					stack.Push(p->left);
			}
		}
	}
	void IterativeInorder() {
		BNode<T>* p = root;
		StackArray<BNode<T>* > stack(numberOfElements);

		while (p != nullptr) {
			while (p != nullptr) {
				if (p->right != nullptr)
					stack.Push(p->right);
				
				stack.Push(p);
				p = p->left;
			}
			
			p = stack.Pop();
			
			while (!stack.IsEmpty() && p->right == nullptr) {
				p->Print();
				p = stack.Pop();
			}

			p->Print();

			if (!stack.IsEmpty())
				p = stack.Pop();
			else
				p = nullptr;
		}
	}
	void IterativePostorder() {
		BNode<T>* p = root;
		BNode<T>* last = nullptr;

		StackArray<BNode<T>*> stack(numberOfElements);

		while (p != nullptr) {
			for (; p->left != nullptr; p = p->left)
				stack.Push(p); // sve leve potomke doda na stek

			while (p != nullptr && (p->right == nullptr || p->right == last)) {
				p->Print(); // nema desnog potomka ili je on vec obradjen
				last = p;

				if (stack.IsEmpty())
					return;

				p = stack.Pop();
			}

			stack.Push(p);
			p = p->right;
		}
	};
	//------------
	void BreadthFirst() {
		BNode<T>* p = root;

		QueueArray<BNode<T>* > queue(numberOfElements);

		if (p != nullptr) {
			queue.Enqueue(p);
			                                                     
			while (!queue.IsEmpty()) {       //                 7
				p = queue.Dequeue();         //             3       8
				p->Print();                  //          2    5        9     
				                             //         1    4
				if (p->left != nullptr)      // 
					queue.Enqueue(p->left);  //      -> 7 3 8 2 5 9 1 4
				if (p->right != nullptr)     //
					queue.Enqueue(p->right); //
			}
		}
	};

	// ZA ISPIT
	// stablo algebarskog izraza (prevodjenje iz infix u prefix ili postfix notaciju)
	// postorder obilazak za izracunavanje izraza

	int Height(BNode<T>* p) {
		if (p == nullptr)
			return 0;

			return 1 + max(Height(p->left), Height(p->right));
	}

	int NodeCount(BNode<T>* p) {
		if (p == nullptr)
			return 0;

		return 1 + NodeCount(p->left) + NodeCount(p->right);
	}

	int GetDepth(BNode<T>* p) {
		// vrati na kojoj je dubini cvor p
		return GetDepth2(root, p, 0);
	} 
	int GetDepth2(BNode<T>* p, BNode<T>* q, int lvl) { // koristi se u funkciji GetDepth
		if (p == nullptr)
			return -1;

		if (p == q)
			return lvl;

		return max(GetDepth2(p->left, q, lvl + 1), GetDepth2(p->right, q, lvl + 1));
	} 

	BNode<T>* BiggestSumOfChildren() {
		// vraca cvor ciji je zbir direktih potomaka najveci
		BNode<T>* curr = root;
		int maxSum = 0;

		return BiggestSumOfChildren2(curr, maxSum);
	}
	BNode<T>* BiggestSumOfChildren2(BNode<T>* node, int& maxSum) { // ne radi
		if (node == nullptr)
			return nullptr;

		BNode<T>* leftMax = BiggestSumOfChildren2(node->left, maxSum);
		BNode<T>* rightMax = BiggestSumOfChildren2(node->right, maxSum);

		int leftValue = node->left ? node->left->key : 0;
		int rightValue = node->right ? node->right->key : 0;

		int sum = leftValue + rightValue;

		BNode<T>* maxNode = node;
		maxSum = sum;

		if (leftValue > maxSum) {
			maxSum = leftValue;
			maxNode = leftMax;
		}
		
		if (rightValue > maxSum) {
			maxSum = rightValue;
			maxNode = rightMax;
		}

		return maxNode;
	}

	// Jun 2 2024.
	BNode<T>* minEvenDiff() {
		BNode<T>* result = nullptr;
		int minDiff = 99999;
		FindMinDiff(root, result, minDiff);
		return result;
	}
	int SumEven(BNode<T>* node) {
		if (node == nullptr)
			return 0;

		int sum = 0;
		
		if (node->key % 2 == 0)
			sum += node->key;

		sum += SumEven(node->left);
		sum += SumEven(node->right);

		return sum;
	}
	void FindMinDiff(BNode<T>* node, BNode<T>*& result, int& minDiff) {
		if (node == nullptr)
			return;

		int leftSum = SumEven(node->left);
		int rightSum = SumEven(node->right);
		int diff = abs(leftSum - rightSum);

		if (diff < minDiff) {
			minDiff = diff;
			result = node;
		}

		FindMinDiff(node->left, result, minDiff);
		FindMinDiff(node->right, result, minDiff);
	}

	void DeleteByCopying2(T el) {
		BNode<T>* curr = root;
		BNode<T>* parent = nullptr;
		BNode<T>* node = nullptr;

		while (curr != nullptr && !curr->IsEqual(el)) {
			parent = curr;

			if (curr->IsLessThan(el))
				curr = curr->right;
			else
				curr = curr->left;
		}

		// curr ce biti trazeni element
		node = curr;

		if (curr != nullptr && curr->IsEqual(el)) {

			if (node->left == nullptr) // nema levog potomka
				node = node->right;
			else if (curr->right == nullptr) // nema desnog potomka
				node = node->left;
			else { // ima oba potomka, treba naci krajnji desni cvor u levom podstablu
				BNode<T>* tmp = node->left;
				BNode<T>* tmpPrevious = node;

				while (tmp->right != nullptr) {
					tmpPrevious = tmp;
					tmp = tmp->right;
				}

				node->SetKey(tmp->GetKey());

				// posto moze da se desi da ne postoji desni potomak u celom levom podstablu
				// mora se proveriti i to
				if (tmpPrevious == node)
					tmpPrevious->left = tmp->left;
				else
					tmpPrevious->right = tmp->left; // situacija u 99% slucajeva

				delete tmp;
				numberOfElements--;

				return;
			}

			if (curr == root)
				root = node;
			else if (parent->left == curr) // curr drzi element koji treba da se brise, a node njegovog potomka levog ili desnog (za 1. i 2. slucaj)
				parent->left = node;
			else
				parent->right = node;

			delete curr;
			numberOfElements--;
		}
	}

	// rekurzivne funkcije 

	BNode<T>* FindMaximal() {
		//Napisati metodu Node* findMaximal()  clanicu klase neuredjenog
		//binarnog stabla koja odredjuje  cvor stabla za koji vazi da je razlika zbira
		//vrednosti njegovog levog podstabla i zbira vrednosti njegovog desnog
		//podstabla maksimalna.Pri implementaciji je dozvoljeno koristiti pomocne
		//funkcije, ali ne dodatne atribute u cvoru grafa, niti dodatne pomocne
		//strukture podataka(npr.niz, lista, ...).Voditi racuna o efikasnosti resenja.

		int max;
		return FindMaximal2(root, &max);
	}
	BNode<T>* FindMaximal2(BNode<T>* p, int* max) {
		if (p == nullptr)
			return nullptr;

		if (p->left == nullptr && p->right == nullptr) {
			*max = 0;
			return p;
		}

	}

	// Jun 2025
	int DeleteSingleLeafParents(BNode<T>* root) {
		// brise cvorove koji imaju samo jednog direktnog potomka koji je list i vraca ukupan broj 
		// tako obrisanih cvorova

		// posto se brise cvor koji ima samo jednog potomka koji je list, treba samo uvezati pretka tog cvora sa listom
		if (root == nullptr)
			return 0;

		int res = 0;

		DeleteSingleLeafParents(root->left);
		DeleteSingleLeafParents(root->right);

		// proveri da pstoji samo jedan left ili right
		if ((root->left == nullptr && root->right != nullptr && IsLeaf(root->right)) 
			|| (root->left != nullptr && root->right == nullptr && IsLeaf(root->left))) 
		{
			BNode<T>* child = root->left != nullptr ? root->left : root->right;

			delete root;
			root = child;

			return res + 1;
		}

		return res;
	}
	bool IsLeaf(BNode<T>* node) {
		if (node->left == nullptr && node->right == nullptr)
			return true;

		return false;
	}

	// Jun 2 2022.
	int BrojCvorova(int keyMin, int valueMin) {
		Node<T>* first = root;

		return BrojCvorova(keyMin, valueMin, false, first);
	}
	int BrojCvorova(int keyMin, int valueMin, bool isValid, Node<T>* root) {
		if (root == nullptr)
			return 0;

		if (isValid && root->info > keyMin) { // ako je vec nekom predaku true, onda ne mora proveravati ovom
			return 1 + BrojCvorova(keyMin, valueMin, true, root->left) + BrojCvorova(keyMin, valueMin, true, root->right);
		}

		// preci mu nemaju cvor veci od valueMin, da li on ima
		if (root->info > valueMin) 
			return BrojCvorova(keyMin, valueMin, true, root->left) + BrojCvorova(keyMin, valueMin, true, root->right);
		else
			return BrojCvorova(keyMin, valueMin, false, root->left) + BrojCvorova(keyMin, valueMin, false, root->right);
	}

	// Jun 2 2023.
	int CountSmaller(int d) {
		// broji koliko ima cvorova u stablu n koji u svojim podstablima imaju bar jedan cvor np tako da vazi n-d<np<n

		BNode<T>* ptr = root;
		return CountSmaller(d, ptr);
	}
	int CountSmaller(int d, BNode<T>* ptr) {
		if (ptr == nullptr)
			return 0;

		bool found = HasValidNode(ptr->left, ptr->key, d) || HasValidNode(ptr->right, ptr->key, d);

		int res = found ? 1 : 0;

		res += CountSmaller(d, ptr->left);
		res += CountSmaller(d, ptr->right);

		return res;
	}
	bool HasValidNode(BNode<T>* node, int parentValue, int d) {
		if (node == nullptr)
			return false;

		if (node->key > parentValue - d && node->key < parentValue)
			return true;

		return HasValidNode(node->left, parentValue, d) || HasValidNode(node->right, parentValue, d);
	}

	// Januar 2024.
	int CountCloseParents(BNode<T>* node, int n) {
		if (node == nullptr)
			return 0;

		// znam
	}

	// Jun 2024.
	void DeleteLeaves() {
		BNode<T>* ptr = root;
		DeleteLeaves(ptr);
	}
	void DeleteLeaves(BNode<T>* ptr) {
		if (ptr == nullptr)
			return;

		DeleteLeaves(ptr->left);
		DeleteLeaves(ptr->right);

		if (ptr->left == nullptr && ptr->right == nullptr) {
			ptr = nullptr;
			delete ptr;
		}
	}

	int MaxLevel() {
		int level = 0;
		int* niz = new int[numberOfElements];
		BNode<T>* ptr = root;

		for (int i = 0; i < numberOfElements; i++)
			niz[i] = 0;

		MaxLevel(ptr, level, niz);

		int max = 0, curr = 0, maxLevel = 0;
		for (int i = 0; i < numberOfElements; i++) {
			curr = niz[i];

			if (curr > max) {
				max = curr;
				maxLevel = i;
			}
		}

		return maxLevel;
	}
	void MaxLevel(BNode<T>* ptr, int level, int* niz) {
		if (ptr == nullptr)
			return;

		if (niz[level] == 0)
			niz[level] = 1;
		else
			niz[level]++;

		MaxLevel(ptr->left, level + 1, niz);
		MaxLevel(ptr->right, level + 1, niz);
	}

	// Oktobar 2, 2022.
	BNode<T>* FindSibling(BNode<T>* p) {
		// vraca pokazivac na jedan, bilo koji, cvor koji je u istom nivou sa cvorom p
		// treba obici stablo samo jedanput
		int currentLevel = 0;
		int targetLevel = -1;

		if (p == nullptr)
			return nullptr;

		BNode<T>* node = root;

		return BNode<T>* FindSibling(p, node, currentLevel, targetLevel);
	}
	BNode<T>* FindSibling(BNode<T>* p, BNode<T>* node, int currentLevel, int targetLevel) {
		if (p == nullptr || node == nullptr)
			return nullptr;

		if (p == node)
			targetLevel = currentLevel;

		if (currentLevel == targetLevel && node != p)
			return node;

		return FindSibling(p, node->left, currentLevel + 1, targetLevel) || FindSibling(p, node->right, currentLevel + 1, targetLevel);
	}
};