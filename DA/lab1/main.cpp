#include <iostream>

template<typename K, typename V>

struct Entry {
    K key{};
    V value{};
    bool operator<(const Entry& other) const {
        return key < other.key;
    }
    bool operator>(const Entry& other) const {
        return other < *this;
    }
    bool operator==(const Entry& other) const {
        return key==other.key;
    }
    bool operator<=(const Entry& other) const {
        return !(other < *this);
    }
    bool operator>=(const Entry& other) const {
        return !(*this < other);
    }
};

template<typename T>

class Vector {
  public:
    explicit Vector(int size) {
        size_ = size;
        currentSize_ = 0;
        array_ = new T[size_];
    }

    Vector() {
        size_ = 50;
        currentSize_ = 0;
        array_ = new T[size_];
    }
    ~ Vector() {
        delete[] array_;
    }
    int size() {
        return currentSize_;
    }

    void pushBack(T value) {
        if (currentSize_==size_) {
            int newSize = size_*1.25;
            T *new_array = new T[newSize];

            for (int i = 0; i < size_; ++i) {
                new_array[i] = array_[i];
            }

            delete[] array_;
            array_ = new_array;
            size_ = newSize;
        }

        array_[currentSize_] = value;
        currentSize_++;
    }
    T pop() {
        currentSize_--;
        return array_[currentSize_];
    }

    T& operator[](int index) {
        return array_[index];
    }

    friend T *begin(const Vector& container) { return container.array_; }
    friend T *end(const Vector& container) {
        return container.array_ + container.currentSize_;
    }

  private:
    int size_;
    int currentSize_;
    T *array_;
};

void radixSort(Vector<Entry<unsigned long long, unsigned long long>>& keysValues) {
    for (int i = 0; i < 64; ++i) {
        Vector<Vector<Entry<unsigned long long, unsigned long long>>> bits(2);
        for (Entry<unsigned long long, unsigned long long> keyValue : keysValues) {
            bits[(keyValue.key >> i) & 1].
                pushBack(keyValue);
        }
        int index = 0;
        for (Entry<unsigned long long, unsigned long long> keyValue : bits[0]) {
            keysValues[index] = keyValue;
            index++;
        }
        for (Entry<unsigned long long, unsigned long long> keyValue : bits[1]) {
            keysValues[index] = keyValue;
            index++;
        }
    }
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    Vector<Entry<unsigned long long, unsigned long long>> keysValues;
    Entry<unsigned long long, unsigned long long> temporary;

    while (std::cin >> temporary.key >> temporary.value) {
        keysValues.pushBack(temporary);
    }

    radixSort(keysValues);

    for (Entry<unsigned long long, unsigned long long> keyValue : keysValues) {
        std::cout << keyValue.key << "\t" << keyValue.value << "\n";
    }

}