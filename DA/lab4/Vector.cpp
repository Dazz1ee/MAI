#include <cstddef>
template<typename K, typename V>
struct Entry {
    K key;
    V value;

    Entry() {

    }

    Entry(K key, V value){
        this->key = key;
        this->value = value;
    }

    bool operator==(const Entry& other) const {
        return key==other.key;
    }
};


template <typename T>
class Vector {
  public:
    explicit Vector(int size) {
        size_ = size;
        current_size_ = 0;
        array_ = new T[size_];
    }

    Vector() {
        size_ = 5;
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
        if (current_size_== size_) {
            int new_size = size_ * 2;
            T *new_array = new T[new_size];

            for (int i = 0; i < size_; ++i) {
                new_array[i] = array_[i];
            }

            delete[] array_;
            array_ = new_array;
            size_ = new_size;
        }

        array_[current_size_] = value;
        current_size_++;
    }
    T Pop() {
        current_size_--;
        T value = array_[current_size_];
        array_[current_size_] = NULL;
        return value;
    }

    T& operator[](int index) {
        return array_[index];
    }

    friend T *begin(const Vector& container) { return container.array_; }
    friend T *end(const Vector& container) {
        return container.array_ + container.current_size_;
    }

    T* Find(T word) {
        for (T temporary : this) {
            if (temporary == word) {
                return word;
            }
        }
        return nullptr;
    }

  private:
    int size_;
    int current_size_;
    T *array_;
};
