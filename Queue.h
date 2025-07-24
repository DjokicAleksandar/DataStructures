#pragma once
#include <iostream>

// red
template <class T>
class Queue
{
public:
	virtual T GetHead() { return; };
	virtual void Enqueue(T object) { return; };
	virtual void Dequeue() { return; };
	virtual void IsEmpty() { return true; };
	virtual int NumberOfElements() { return 0; };
};

template <class T>
class QueueArray : public Queue<T>
{
protected:
	T* arr;
	int size;
	int head;
	int tail;
	int numberOfElements;

public:
	QueueArray(int size) {
		this->size = size;
		head = tail = -1;
		numberOfElements = 0;
		arr = new T[size];
	}
	bool IsEmpty() { return numberOfElements == 0; };
	int NumberOfElements() { return numberOfElements; };
	~QueueArray() { delete[] arr; };
	
	T GetHead() {
		if (numberOfElements == 0)
			return;

		return arr[head];
	}
	void Enqueue(T object) {
		if (numberOfElements == size)
			return;

		if (++tail == size)
			tail = 0; //ako je tail preso granice reda, vraca se na pocetak

		arr[tail] = object;

		if (numberOfElements == 0)
			head = tail;

		numberOfElements++;
	}
	T Dequeue() {
		if (numberOfElements == 0)
			return;

		T result = arr[head];

		if (++head == size)
			head = 0;

		numberOfElements--;

		if (numberOfElements == 0)
			head = tail = -1;

		return result;
	}

};

// dupli red
template <class T>
class Deque
{
public:
	virtual T GetHead() { return; };
	virtual T GetTail() { return; };
	virtual void EnqueueHead(T object) { return; };
	virtual void EnqueueTail(T object) { return; };
	virtual T DequeueHead() { return; };
	virtual T DequeueTail() { return; };
	virtual bool IsEmpty() { return true; };
	virtual int NumberOfElements() { return 0; };
};

//template <class T>
//class DequeArray : public QueueArray<T>, public Deque<T>
//{
//public:
//	DequeArray(int size) : QueueArray<T>(size) {};
//
//	//head
//	T GetHead() {
//		if (numberOfElements == 0)
//			return;
//
//		return arr[head];
//	}
//
//	void EnqueueHead(T object) {
//		if (numberOfElements == size)
//			return;
//		if (numberOfElements == 0)
//			head = tail = 0;
//		if (head == 0)     //prvo proveri da li je head == 0, ako jeste prebacimo head na kraj 
//			head = size - 1;
//		else if (head != 0) //head nije bio nula, samo ga smanjimo za jedno mesto
//			head--;
//
//		arr[head] = object;
//		numberOfElements++;
//	};
//
//	T DequeueHead() {
//		return Dequeue();
//	};
//
//	//tail
//	T GetTail() {
//		if (numberOfElements == 0)
//			return;
//
//		return arr[tail];
//	}
//
//	void EnqueueTail(T object) {
//		Enqueue(object);
//	}
//
//	T DequeueTail() {
//		if (numberOfElements == 0)
//			return;
//
//		if (tail == 0)
//			tail = size - 1;
//		else
//			tail--;
//
//		numberOfElements--;
//
//		return arr[tail];
//	}
//};

// zadaci sa ispita
class PriorityQueueNode
{
public:
	int priority; // 0 - 3
	int* data;
	PriorityQueueNode* next;

	PriorityQueueNode() { next = nullptr; data = nullptr; priority = 0; };
	PriorityQueueNode(int* data) { this->data = data; this->next = nullptr; priority = 0; };
};
class PriorityQueue
{
private:
	PriorityQueueNode* head;
	PriorityQueueNode* tail[4];
	
public:
	PriorityQueue() { 
		head = nullptr; 
		for (int i = 0; i < 4; i++)
			tail[i] = nullptr;
	}
	void Add(int prioritet, int* podaci) {
		if (head == nullptr) {
			PriorityQueueNode* newNode = new PriorityQueueNode(podaci);
			head = newNode;
			tail[prioritet] = newNode;
			return;
		}

		if (tail[prioritet] != nullptr) {
			PriorityQueueNode* newNode = new PriorityQueueNode(podaci);

			newNode->next = tail[prioritet]->next;
			tail[prioritet]->next = newNode;

			tail[prioritet] = newNode;
		} 
		else {
			// ne postoji nista na tail[prioritet], znaci treba ubaciti element iza tail[prioritet - 1]
			for (int i = prioritet - 1; i >= 0; i--) {
				if (tail[i] != nullptr) {
					PriorityQueueNode* newNode = new PriorityQueueNode(podaci);

					newNode->next = tail[i]->next;
					tail[i]->next = newNode;

					tail[i] = newNode; 

					break;
				}

				if (i == 1) { 
					// ako je stiglo i do 1 i nije naislo na break, znaci da je do prioriteta 0
					// i na 0 prioritetu nema nicega, znaci dodajemo 1. element i head postavljamo na njega
					PriorityQueueNode* newNode = new PriorityQueueNode(podaci);

					tail[prioritet] = newNode;
					head = newNode;
				}
			}
		}
	}
	int* Get() {
		if (head == nullptr)
			return nullptr;

		PriorityQueueNode* temp = head;
		int* ret = temp->data;

		if (tail[temp->priority] == temp) // da li je temp bio poslednji sa tim prioritetom
			tail[temp->priority] = nullptr;

		head = head->next;
		return ret;
	}
};