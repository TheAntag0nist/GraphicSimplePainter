#pragma once

template <class T>
class Matrix {
private:
	T** matrix;

	int lin;
	int col;
public:
	Matrix(int lin, int col) {
		// save data
		this->lin = lin;
		this->col = col;
		
		if (lin > 0 && col > 0) {
			// allocate mem for lines
			matrix = new T*[lin];

			// allocate mem for columns
			for (int i = 0; i < lin; ++i)
				matrix[i] = new T[col];
		}
	}

	int Matrix<T>::operator=(const Matrix<T>& secondM) {
		if ((this->col != secondM.getCol()) && (this->lin != secondM.getLin()))
			return NULL;

		this->~Matrix();

		for (int i = 0; i < this->lin; ++i)
			for (int j = 0; j < this->col; ++j)
				*this[i][j] = secondM[i][j];

		return true;
	}

	Matrix<T>& Matrix<T>::operator*(const Matrix<T>& secondM) {
		if (this->col != secondM.getLines())
			return NULL;

		// create tmp for calculate
		Matrix<T> tmp(this->lin, secondM.getCol());

		for (int i = 0; i < this->lin; ++i)
			for (int k = 0; k < secondM.getCol(); ++k)
				for (int n = 0; k < secondM.getLines(); ++n)
					tmp[i][k] += (this->[i][n]) * secondM[n][k];

		return tmp;
	}

	T** operator[][](int i, int k) {
		return matrix[i][k];
	}

	int getLines() {
		return lin;
	}
	int getCol() {
		return col;
	}

	~Matrix() {
		// free mem
		for (int i = 0; i < lin; ++i)
			delete[] matrix[i];

		delete[] matrix;
	}
};