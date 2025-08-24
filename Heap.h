#pragma once

// heap je binary tree kod kojeg svaki cvor mora biti manji ili jednak od svojih direktnih naslednika

// BINARNI MIN HEAP
template <class T>
class Heap
{
protected:
	T* arr; // krece se sa pozicije 1, 0 se ne koristi
	int length;
	int numberOfElements;

public:
	Heap(int length) {
		this->length = length;
		numberOfElements = 0;
		arr = new T[length + 1];
	};
	~Heap() { delete[] arr; };
	bool IsEmpty() { return numberOfElements == 0; };
	void Insert(T el) {
		if (numberOfElements == length - 1)
			return;

		numberOfElements++;
		int i = numberOfElements;

		while (i > 1 && arr[i / 2] > el) { // arr[i/2] je roditelj elementa arr[i]
			arr[i] = arr[i / 2];
			i /= 2;
		}

		arr[i] = el;
	};
	T DeleteRoot() {
		if (numberOfElements == 0)
			return;

		T result = arr[1];
		T last = arr[numberOfElements];
		int i = 1;
		
		numberOfElements--;

		while (2 * i < numberOfElements + 1) {
			int child = 2 * i; 
			
			if (child + 1 < numberOfElements + 1 && arr[child + 1] < arr[child])
				child += 1;

			if (last < arr[child])
				break;

			arr[i] = arr[child];
			i = child;
		}

		arr[i] = last;
		return result;
	};

	void SwapNodes(int i, int j) {
		std::swap(arr[i], arr[j]);
	}
	void BubbleUp(int i) {
		while (i > 1) {
			if (arr[i] > arr[i / 2]) {
				arr[i] = arr[i / 2];
				i /= 2;
			}
			else
				break;
		}
	}
	void BubbleDown(int i) {
		while (2 * i < numberOfElements + 1) {
			int child = 2 * i;
			int smallest = i;

			if (child < numberOfElements + 1 && arr[child] < arr[smallest])
				smallest = child;

			if (child + 1 < numberOfElements + 1 && arr[child + 1] < arr[smallest])
				smallest = child + 1;

			if (smallest != i) {
				SwapNodes(i, smallest);
				i = smallest;
			}
			else
				break;

		}
	}
	void Update(T oldValue, T newValue) {
		int index;
		
		for (int i = 0; i < numberOfElements; i++)
			if (arr[i] == T)
				index = i;

		if (oldValue < newValue)
			BubbleDown(index);
		else
			BubbleUp(index);
	}

	void HeapSort(T* a, int n) {
		Heap<T> heap(n + 1);

		for (int i = 0; i < n; i++)
			heap.Insert(a[i]);

		int i = 0;
		while (!heap.IsEmpty())
			a[i++] = heap.DeleteRoot();
	};
};

