template<typename T>

class Vector {
  public:
    explicit Vector(int size) {
        size_ = size;
        current_size_ = 0;
        array_ = new T[size_];
    }

    Vector() {
        size_ = 50;
        current_size_ = 0;
        array_ = new T[size_];
    }
    ~ Vector() {
        delete[] array_;
    }
    int Size() {
        return current_size_;
    }

    void PushBack(T value) {
        if (current_size_==size_) {
            int newSize = size_*1.25;
            T *new_array = new T[newSize];

            for (int i = 0; i < size_; ++i) {
                new_array[i] = array_[i];
            }

            delete[] array_;
            array_ = new_array;
            size_ = newSize;
        }

        array_[current_size_] = value;
        current_size_++;
    }
    T Pop() {
        current_size_--;
        return array_[current_size_];
    }

    T& operator[](int index) {
        return array_[index];
    }

    friend T *begin(const Vector& container) { return container.array_; }
    friend T *end(const Vector& container) {
        return container.array_ + container.current_size_;
    }

  private:
    int size_;
    int current_size_;
    T *array_;
};