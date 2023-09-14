template <typename T>
class Vector {
  public:
    Vector() {
      _data = NULL;
      _size = 0;
    }

    ~Vector() {
      if (_data != NULL) {
        free(_data);
      }
    }

    void push_back(T value) {
      _size++;
      _data = (T*) realloc(_data, _size * sizeof(T));
      _data[_size - 1] = value;
    }

    void remove(int index) {
      if (index < 0 || index >= _size) {
        return;
      }
      for (int i = index; i < _size - 1; i++) {
        _data[i] = _data[i + 1];
      }
      _size--;
      _data = (T*) realloc(_data, _size * sizeof(T));
    }

    T& operator[](int index) {
      return _data[index];
    }

    int size() {
      return _size;
    }

    T* begin() {
      return &_data[0];
    }

    T* end() {
      return &_data[_size];
    }

  private:
    T* _data;
    int _size;
};
