#include <ostream>
#include <cstring>
#include "Vector.cpp"

class String {
  public:

    String(char* data) {
        size_t size = strlen(data)+1;

        this->data_ = new char[size];
        this->size_ = size;

        if (this->data_ != nullptr) {
            strncpy(this->data_, data, size);
        }
        this->data_[size-1] = 0;
    }

    String(Vector<char>& buffer) {
        size_ = buffer.Size() + 1;
        data_ = new char [size_];
        data_[size_ - 1] = '\0';
        for (int index = size_ - 2; index >= 0; --index) {
            data_[index] = buffer.Pop();
        }
    }

    ~String() {
        if (this->data_ != nullptr) {
            delete[] data_;
        }
    }

    bool operator==(const String& string2) {
        return this->compare(string2);
    }

    bool operator!=(const String& string2) {
        return !this->compare(string2);
    }

    int Size() const {
        return size_;
    }

    char Get(int position){
        return data_[position];
    }

  private:
    char* data_;
    int size_;
    bool compare(const String& string) {
        return strcmp(this->data_, string.data_) == 0;
    }


};