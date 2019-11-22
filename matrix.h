#include <stdexcept>
#include <iostream>

#include "node.h"

template <typename T>
class Matrix {
  public:

	Matrix() = default;
	Matrix(unsigned int rows, unsigned int columns);

	Matrix(const Matrix<T> &other);
	Matrix<T>& operator=(const Matrix<T> &);

	void set(unsigned int row, unsigned int column, T val);
	T operator()(unsigned int row, unsigned int column) const;

	const Matrix<T> operator*(T scalar) const;
	const Matrix<T> operator*(Matrix<T> other) const;
	const Matrix<T> operator+(Matrix<T> other) const;
	const Matrix<T> operator-(Matrix<T> other) const;
	const Matrix<T> transpose() const;
	void print() const;

	~Matrix();

  private:
	Node<T> *m_Root = nullptr;
	unsigned int m_Rows, m_Columns;
	void checkBounds(unsigned int, unsigned int) const;
};

template <typename T>
void Matrix<T>::checkBounds(unsigned int row, unsigned int column) const {
  if (row >= m_Rows || column >= m_Columns) {
	throw std::out_of_range("Row/Column out of range");
  }
}

template <typename T>
Matrix<T>::Matrix(unsigned int rows, unsigned int columns) {
  m_Rows = rows;
  m_Columns = columns;
  m_Root = new Node<T>{};

  auto tmp = m_Root;
  for (int i = 0; i <= m_Rows; ++i) {
	tmp->down = new Node<T>();
	tmp = tmp->down;
  }

  tmp = m_Root;
  for (int i = 0; i <= m_Columns; ++i) {
	tmp->next = new Node<T>();
	tmp = tmp->next;
  }
}

template <typename T>
Matrix<T>::~Matrix() {
  for (auto tmp = m_Root; tmp; tmp = tmp) {
	auto tmp2 = tmp->down;
	tmp->deleteSelf();
	tmp = tmp2;
  }
}
template <typename T>
Matrix<T>::Matrix(const Matrix<T> &other) {
  m_Rows = other.m_Rows;
  m_Columns = other.m_Columns;

  m_Root = new Node<T>();
  auto tmp = m_Root;

  for (int i = 0; i<=m_Rows; ++i) {
	tmp->down = new Node<T>();
	tmp = tmp->down;
  }

  tmp = m_Root;
  for (int i = 0; i<=m_Columns; ++i) {
	tmp->next = new Node<T>();
	tmp = tmp->next;
  }

  for (int i = 0; i<m_Rows; ++i) {
	for (int j = 0; j<m_Columns; ++j) {
	  this->set(i, j, other(i, j));
	}
  }
}

template <typename T>
void Matrix<T>::set(unsigned int row, unsigned int column, T val) {
  checkBounds(row, column);
  // Los nodos se crean aún si no se insertan (i.e. 0), solo deberías crear en ciertos casos
  auto newNode = new Node<T>(row, column, val);

  auto tmp = &m_Root;
  for (int i = 0; i<=row; ++i) {
	tmp = &((*tmp)->down);
  }

  tmp = &((*tmp)->next);
  while (*tmp && (*tmp)->column < column) {
	tmp = &((*tmp)->next);
  }

  if (*tmp && (*tmp)->column == column) {
	if (val) {
	  (*tmp)->value = val;
	  return;
	}
	auto nextNode = (*tmp)->next;
	*tmp = nextNode;
  } else if (val != 0) {
	newNode->next = *tmp;
	*tmp = newNode;
  } else {
	return;
  }

  tmp = &m_Root;
  for (int i = 0; i<=column; ++i) {
	tmp = &((*tmp)->next);
  }

  tmp = &((*tmp)->down);
  while (*tmp && (*tmp)->row < row) {
	tmp = &((*tmp)->down);
  }

  if (*tmp && (*tmp)->row == row) {
	auto downNode = (*tmp)->down;
	delete *tmp;
	*tmp = downNode;
	return;
  }

  newNode->down = *tmp;
  *tmp = newNode;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T> &other) {
  m_Rows = other.m_Rows;
  m_Columns = other.m_Columns;

  m_Root = new Node<T>();
  auto tmp = m_Root;

  for (int i = 0; i <= m_Rows; ++i) {
	tmp->down = new Node<T>();
	tmp = tmp->down;
  }

  tmp = m_Root;
  for (int i = 0; i <= m_Columns; ++i) {
	tmp->next = new Node<T>();
	tmp = tmp->next;
  }

  for (int i = 0; i < m_Rows; ++i) {
	for (int j = 0; j < m_Columns; ++j) {
	  this->set(i, j, other(i, j));
	}
  }

  return *this;
}

template <typename T>
T Matrix<T>::operator()(unsigned int row, unsigned int column) const {
  checkBounds(row, column);
  auto tmp = m_Root;

  for (int i = 0; i <= row; ++i) {
	tmp = tmp->down;
  }

  tmp = tmp->next;
  while (tmp && tmp->column < column) {
	tmp = tmp->next;
  }

  if (!tmp || tmp->column > column)
	return 0;

  return tmp->value;
}

// Utilizar las operaciones de la matriz afectará el performance de las siguientes operaciones

template <typename T>
const Matrix<T> Matrix<T>::operator*(T scalar) const {
  Matrix<T> answer(m_Rows, m_Columns);

  for (auto t = m_Root->down; t; t = t->down) {
	auto tmp = t->next;
	while (tmp) {
	  answer.set(tmp->row, tmp->column, tmp->value * scalar);
	  tmp = tmp->next;
	}
  }
  return answer;
}

template <typename T>
const Matrix<T> Matrix<T>::operator*(Matrix<T> other) const {
  Matrix<T> answer(m_Rows, other.m_Columns);

  for (int i = 0; i < m_Rows; ++i) {
	for (int j = 0; j < other.m_Columns; ++j) {
	  T tmpValue = 0;
	  for (int k = 0; k < m_Columns; ++k) {
		T val1 = (*this)(i, k);
		T val2 = other(k, j);
		tmpValue += val1 * val2;
	  }

	  if (tmpValue)
		answer.set(i, j, tmpValue);
	}
  }

  return answer;
}

template <typename T>
const Matrix<T> Matrix<T>::operator+(Matrix<T> other) const {
  Matrix<T> answer(m_Rows, m_Columns);

  for (int i = 0; i<m_Rows; ++i) {
	for (int j = 0; j<m_Columns; ++j) {
	  T val1 = (*this)(i, j);
	  T val2 = other(i, j);
	  if (val1 + val2)
		answer.set(i, j, val1 + val2);
	}
  }

  return answer;
}

template <typename T>
const Matrix<T> Matrix<T>::operator-(Matrix<T> other) const {
  Matrix<T> answer(m_Rows, m_Columns);

  for (int i = 0; i<m_Rows; ++i) {
	for (int j = 0; j<m_Columns; ++j) {
	  T val1 = (*this)(i, j);
	  T val2 = other(i, j);
	  if (val1 - val2)
		answer.set(i, j, val1 - val2);
	}
  }

  return answer;
}


template <typename T>
const Matrix<T> Matrix<T>::transpose() const {
  Matrix<T> answer(m_Rows, m_Columns);

  for (int i = 0; i < m_Rows; ++i) {
	for (int j = 0; j < m_Columns; ++j) {
	  T tmp = (*this)(i, j);
	  if (tmp)
		answer.set(j, i, tmp);
	}
  }

  return answer;
}

template <typename T>
void Matrix<T>::print() const {
  for (int i = 0; i<m_Rows; ++i) {
	for (int j = 0; j<m_Rows; ++j) {
	  std::cout << (*this)(i, j) << " ";
	}
	std::cout << '\n';
  }
}
