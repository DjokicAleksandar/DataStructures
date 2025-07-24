#pragma once
#include <iostream>
#include <math.h>

template <class T>
class Array
{
protected:
	T* data;
	int base;
	int length;

public:
	Array(int length) {
		this->length = length;
		data = new T[length];
		base = 0;
	};
	Array(int base, int length) {
		this->length = length;
		data = new T[length];
		this->base = base;
	};
	~Array() { delete[] data; };
	void Print() {
		for (int i = 0; i < length; i++) {
			std::cout << data[i] << " ";
		}
	}

	void operator=(Array<T>& arr) {
		if (&arr != this) {
			if (length != arr.GetLength()) {
				if (data) 
					delete[] data;
				length = arr.GetLength();
				data = new T[length];
			}

			for (int i = 0; i < length; i++)
				data[i] = arr.data[i];

			base = arr.GetBase();
		}
	}
	T& operator[](int index) {
		int offset = index - base;
		if (offset >= length)
			return -1;
		return data[offset];
	}

	T* GetData() { return data; };
	T& GetAt(int index) { 
		int offset = index - base;
		if (offset >= length)
			return -1;

		return data[index]; 
	};
	int GetBase() { return base; };
	int GetLength() { return length; };
	void SetBase(int newBase) { base = newBase; };
	void SetLength(int newLength) {
		if (length != newLength) {
			T* newData = new T[newLength];
			int min = length;

			if (min > newLength) 
				min = newLength;

			for (int i = 0; i < min; i++)
				newData[i] = data[i];

			delete[] data;
			data = newData;
		}
	}
	void SetAt(T el, int index) {
		int offset = index - base;
		if (offset > length)
			return;

		data[offset] = el;
	}
};

class Matrix 
{
	// napravi funkciju offset koja za npr. gornju trougaonu (cuva samo nenulte ele) 
	// vraca index elementa zapamcenog u nizu, npr. element mat[2, 2]
	// bi u jednodimenzionalnom nizu bio na poziciji niz[7] i tako za svaku specificnu matricu

	virtual double Get(int i, int j) = 0;
	virtual void Put(int i, int j, double el) = 0;
	virtual void Transpose() = 0;
	virtual void Multiply(Matrix& m) = 0;
	virtual void Add(Matrix& m) = 0;
	virtual void Print() = 0;
};

class DenseMatrix : public Matrix
{
protected:
	int rows;
	int cols;
	double* data;

	// indeksi u data su (i * cols + j)
	// u transponovanoj su (j * rows + i)

public:
	DenseMatrix(int rows, int cols) {
		this->rows = rows;
		this->cols = cols;
		data = new double[rows * cols];
	}

	int GetOffset(int i, int j) {
		int factor1 = 1;
		int factor2 = cols;
		int result = i * factor2 + j * factor1;
		return result;
	}
	double Get(int i, int j) {
		return data[GetOffset(i, j)];
	}
	void Put(int i, int j, double el) {
		if (i < 0 || i >= rows || j < 0 || j >= cols) 
			return;

		data[GetOffset(i, j)] = el;
	}
	void Transpose() {
		double* pom = new double[rows * cols];


		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				pom[j * rows + i] = data[i * cols + j];
			}
		}

		delete[] data;
		data = pom;

		// zamena mesta redovima i kolonama
		int k = rows;
		rows = cols;
		cols = k;
	}
	void Print() {
		for (int i = 0; i < rows * cols; i++) {
			if (i % cols == 0 && i != 0)
				std::cout << endl;

			std::cout << data[i] << " ";
		}
		std::cout << endl;
	}
	void Multiply(Matrix& m) {};
	void Add(Matrix& m) {};
};

template <class T>
class MDArray 
{
protected:
	T* data;
	int* dim; // niz koji drzi velicine svake dimenzije
	int* factor;
	int dimensions; // broj dimenzija
	int GetOffset(int* indices, int n) {
		// indices je niz indeksa, matrica[2][4][3]

		if (n != dimensions)
			return -1;

		int offset = 0;

		for (int i = 0; i < dimensions; i++) {
			if (indices[i] < 0 || indices[i] >= dim[i])
				return -1;

			offset += factor[i] * indices[i];
		}

		return offset;
	};

public:
	MDArray<T>(int* sizesOfDimensions, int n) {
		dimensions = n;
		dim = new int[n];
		factor = new int[n];
		int product = 1;

		for (int i = n - 1; i >= 0; i--) {
			dim[i] = sizesOfDimensions[i];
			factor[i] = product;
			product *= dim[i];
		}

		data = new T[product];
	};
	T& GetAt(int* indices, int n) { return data[GetOffset(indices, n)]; };
	void SetAt(T obj, int* indices, int n) { data[GetOffset(indices, n)] = obj; };
};

// za retko posednutu matricu
template <class T>
class MatNode
{
public:
	T info;
	int row, col;
	MatNode<T>* right;
	MatNode<T>* down;
	MatNode() { info = 0, row = 0, col = 0, right = nullptr, down = nullptr; };
	MatNode(int i, int j, T info) { row = i, col = j, this->info = info; };
	MatNode(MatNode<T>* right, MatNode<T>* down) { this->right = right, this->down = down; };
	MatNode(int i, int j, T info, MatNode<T>* right, MatNode<T>* down) {
		row = i;
		col = j;
		this->info = info;
		this->right = right;
		this->down = down;
	}
	T Print() { return info; };
	bool IsEqual(T el) { return info == el; };
};

template <class T>
class SparseMat
{
private:
	int rows, cols;
	MatNode<T>** rowHeads;
	MatNode<T>** colHeads;

public:
	SparseMat(int r, int c) {
		rows = r;
		cols = c;

		rowHeads = new MatNode<T>*[rows];
		colHeads = new MatNode<T>*[cols];

		for (int i = 0; i < rows; i++) rowHeads[i] = nullptr;
		for (int i = 0; i < cols; i++) colHeads[i] = nullptr;
	};
	void Set(T value, int i, int j) {
		if (i < 0 || i >= rows || j < 0 || j >= cols)
			return;

		MatNode<T>* prevRow = nullptr;
		MatNode<T>* currRow = rowHeads[i];

		while (currRow != nullptr && currRow->col < j)
		{
			prevRow = currRow;
			currRow = currRow->right;
		}

		if (currRow && currRow->col == j)
		{
			if (value == 0)
			{
				//brisanje
				if (prevRow != nullptr)
					prevRow->right = currRow->right;
				else
					rowHeads[i] = currRow->right;

				//ukloni iz kolone
				MatNode<T>* prevCol = nullptr;
				MatNode<T>* currCol = colHeads[j];

				while (currCol != nullptr && currCol != currRow)
				{
					prevCol = currCol;
					currCol = currCol->down;
				}

				if (currCol)
				{
					if (prevCol != nullptr)
						prevCol->down = currCol->down;
					else
						colHeads[j] = currCol->down;
					delete currRow;
				}
			}
			else
				currRow->info = value;

			return;
		}

		if (value == 0)
			return;

		MatNode<T>* newNode = new MatNode<T>(i, j, value);

		//uvezi u red
		if (prevRow)
		{
			newNode->right = prevRow->right;
			prevRow->right = newNode;
		}
		else
		{
			newNode->right = rowHeads[i];
			rowHeads[i] = newNode;
		}

		//uvezi u kolonu
		MatNode<T>* prevCol = nullptr;
		MatNode<T>* currCol = colHeads[j];

		while (currCol && currCol->row < i) {
			prevCol = currCol;
			currCol = currCol->down;
		}

		if (prevCol) {
			newNode->down = prevCol->down;
			prevCol->down = newNode;
		}
		else {
			newNode->down = colHeads[j];
			colHeads[j] = newNode;
		}
	}
	T Get(int i, int j) {

	}
	void Print() {
		for (int i = 0; i < rows; i++)
		{
			MatNode<T>* ptr = rowHeads[i];

			for (int j = 0; j < cols; j++)
			{
				if (ptr != nullptr && ptr->col == j)
				{
					std::cout << " " << ptr->Print() << " ";
					ptr = ptr->right;
				}
				else
					std::cout << " 0 ";
			}

			std::cout << "\n";
		}
	}
};

// SORT ALGORITMI
void SelectionSort(int* data, int dim) {
	// trazi se indeks najmanjeg el. u celom nizu i on menja
	// mesta sa prvim elementom u nizu

	int i, j, min;

	for (int i = 0; i < dim - 1; i++) {
		min = i;

		for (int j = i + 1; j < dim; j++) {
			if (data[j] < data[min])
				min = j;
		}

		int pom = data[i];
		data[i] = data[min];
		data[min] = pom;
	}
}

void BubbleSort(int* data, int dim) {
	// vazno je da petlje idu u suprotnom smeru
	int i, j;

	for (int i = 0; i < dim - 1; i++) {
		for (int j = dim - 1; j > i; j--) {
			if (data[j] < data[j - 1]) {
				int pom = data[j];
				data[j] = data[j - 1];
				data[j - 1] = pom;
			}
		}
	}
}

void InsertionSort(int* data, int dim) {
	// polazeci od i-tog el. pomeraju se svi el. levo od njega koji su veci,
	// sve do el. manjeg od i-tog, za po jedno mesto udesno, da bi se napravilo mesto za i-ti el.

	int i, j, tmp;

	for (i = 1; i < dim; i++) {
		tmp = data[i];

		for (j = i; j > 0 && tmp < data[j - 1]; j--)
			data[j] = data[j - 1];
		
		data[j] = tmp;
	}
}

void MergeSort(int* data, int dim) {
	// ima funkciju merge i funkciju sort
}
// -------------------

// za ispit
void SetZeroElements(int* data, int* index, int m, int n) {
	// Data su dva niza: data duzine m i index duzine n. Niz index
	// sadrzi indekse elemenata niza data koji su razliciti od nule
	// sortirane u rastucem redosledu. Napisati funkciju koja postavlja
	// svaki element jednak nuli(sa indeksom k) na osnovu dva
	// najbliza nenulta elementa(sa indeksima p i q, gde je p < q) po
	// sledecoj formuli data[k] = ((q - k) * data[p] + (k - p) * data[q]) / (q - p).

	for (int i = 0; i < n - 1; i++) {
		int p = index[i];
		int q = index[i + 1];

		for (int k = p + 1; k < q; k++) {
			data[k] = ((q - k) * data[p] + (k - p) * data[q] / (q - p));
		}
	}
}

double* GetDerivate(int m, double x, int* pol, int n) {
	// polinom je zadat poljem svojih koef.
	// 3x^2 + 2x + 3 = [3, 2, 3]
	// vraca m-ti izvod datog polinoma u tacki x
	// int* pol i int n nisu parametri na ispitu ali posto nije fja clanica, moraju u ovom slucaju
	
	double a = 0;
	double* res = &a;

	if (m > n) { // ako se trazi izvod veceg stepena od polinoma
		a = 0;
		return res;
	}

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			pol[j] = pol[j + 1] * (j + 1);
		}

		pol[n - i] = 0;
	}

	

	for (int i = 0; i < n; i++) {
		a += pol[i] * pow(x, i);
	}

	return res;
}

double* MulMat(int tip, int M, double* mat1, double* mat2) {
	// Napisati funkciju double* MulMat(int tip, int M,
	// double* mat1, double* mat2), koja mnozi dve
	// trougaone matrice dimenzija MxM, zapamcene kao
	// vektori(i to samo nenulti elementi). Ukoliko je tip = 1,
	// matrice su donje trougaone, a ako je tip = 2, matrice
	// su gornje trougaone.

	int* offset = new int[M];
	double* newMat = new double[M];

	if (tip == 1) {
		for (int i = 0; i < M; i++) {
			offset[i] = i * (i + 1) / 2;
		}

		for (int i = 0; i < M; i++) {
			for (int j = 0; j < M; j++) {
				newMat[offset[i] + j] = 0;

				for (int k = j; k <= i; k++)
					newMat[offset[i] + j] = mat1[offset[i] + k] * mat2[offset[k] + j];
			}
		}
	}
	else {
		for (int i = 0; i < M; i++) {
			offset[i] = M * i - (i * (i - 1) / 2);
		}

		for (int i = 0; i < M; i++) {
			for (int j = 0; j < M; j++) {
				newMat[offset[i] + j] = 0;

				for (int k = j; k <= i; k++)
					newMat[offset[i] + (j - i)] = mat1[offset[i] + (k - i)] * mat2[offset[k] + (j - k)];
			}
		}
	}

	return newMat;
}

class FiveDimArray
{
protected:
	// int dimension = 5;
	int* data;
	int* dimensions;
	int* factor;
	int GetOffset(int* indices) {
		int offset = 0;

		for (int i = 0; i < 5; i++) {
			if (indices[i] < 0 || indices[i] > dimensions[i])
				return -1;

			offset = factor[i] * indices[i];
		}

		return offset;
	}

public:
	FiveDimArray(int* sizesOfDimensions) {
		factor = new int[5];
		dimensions = new int[5];
		int product = 1;

		for (int i = 5 - 1; i >= 0; i--) {
			dimensions[i] = sizesOfDimensions[i];
			factor[i] = product;
			product *= dimensions[i];
		}
	}

	void Add(int* indices, int value) {
		for (int i = 0; i < 5; i++)
			if (indices[i] > dimensions[i])
				return;

		int position = GetOffset(indices);

		data[position] = value;
	}
};

