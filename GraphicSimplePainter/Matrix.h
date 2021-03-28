#pragma once

template <class T>
class Matrix {
private:
	int lin;
	int col;
public:
	T** matrix;

	Matrix() {
		this->Matrix(1, 1);
	}

	// constructor
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

			// create ONE matrix
			for (int i = 0; i < lin; ++i)
				for (int j = 0; j < col; ++j) {
					matrix[i][j] = (T) 0.0;

					if (j == i && lin == col)
						matrix[i][j] = (T) 1.0;
				}
		}
	}
	void allocateMem(int lin, int col) {
		// save data
		this->lin = lin;
		this->col = col;

		if (lin > 0 && col > 0) {
			// allocate mem for lines
			matrix = new T * [lin];

			// allocate mem for columns
			for (int i = 0; i < lin; ++i)
				matrix[i] = new T[col];

			// create ONE matrix
			for (int i = 0; i < lin; ++i)
				for (int j = 0; j < col; ++j) {
					matrix[i][j] = (T) 0.0;

					if (j == i && lin == col)
						matrix[i][j] = (T)1.0;
				}
		}
	}

	Matrix(Matrix<T>& secondM) {
		this->allocateMem(secondM.getLines(), secondM.getCol());

		for (int i = 0; i < this->lin; ++i)
			for (int j = 0; j < this->col; ++j)
				*this.matrix[i][j] = secondM.matrix[i][j];
	}

	void zeroM() {
		for (int i = 0; i < this->lin; ++i)
			for (int j = 0; j < this->col; ++j) {
				this->matrix[i][j] = (T)NULL;
			}
	}
	void oneM() {
		for (int i = 0; i < this->lin; ++i)
			for (int j = 0; j < this->col; ++j) {
				this->matrix[i][j] = (T)NULL;

				if (j == i && lin == col)
					matrix[i][j] = (T)1.0;
			}
	}

	// overload = for matrix
	void equal(Matrix<T>& secondM) {
		this->~Matrix();
		this->allocateMem(secondM.getLines(), secondM.getCol());

		for (int i = 0; i < this->lin; ++i)
			for (int j = 0; j < this->col; ++j)
				this->matrix[i][j] = secondM.matrix[i][j];
	}

	// matrix on matrix
	void multi(Matrix<T>& secondM, Matrix<T>& res) {
		// create tmp for calculate
		Matrix<T> tmp(this->getLines(), secondM.getCol());

		// start compute
		for (int i = 0; i < this->getLines(); ++i)
			for (int k = 0; k < secondM.getCol(); ++k) {
				tmp.matrix[i][k] =(T) 0.0;

				for (int n = 0; n < secondM.getLines(); ++n)
					tmp.matrix[i][k] += ((*this).matrix[i][n]) * (secondM.matrix[n][k]);
			}

		res.equal(tmp);
	}

	// get lines and cols
	int getLines() {
		return lin;
	}
	int getCol() {
		return col;
	}

	// free mem
	~Matrix() {
		// free mem
		for (int i = 0; i < lin; ++i)
			delete[] matrix[i];

		delete[] matrix;
	}
};