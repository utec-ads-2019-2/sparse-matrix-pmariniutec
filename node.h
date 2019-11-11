template <typename T>
class Matrix;

template <typename T>
class Node {
  public:
	explicit Node() = default;

	Node(unsigned int row, unsigned int col, T val) : row(row), column(col), value(val) {}

	void deleteSelf() {
	  if(next)
		next->deleteSelf();
	  delete this;
	}

	friend class Matrix<T>;

  protected:
	Node<T> *next = nullptr;
	Node<T> *down = nullptr;
	unsigned int row = 0;
	unsigned int column = 0;
	T value;
};
