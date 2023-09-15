#include "String.cpp"
class Node;
class HashNode {
  public:
    HashNode(String* key, Node* value) {
        key_ = key;
        value_ = value;
        next_ = nullptr;
    }

    String* GetKey() {
        return key_;
    }

    Node* GetValue() {
        return value_;
    }

    void SetValue(Node* value) {
        value_ = value;
    }

    HashNode* GetNext() const {
        return next_;
    }

    void SetNext(HashNode* next) {
        next_ = next;
    }

  private:
    String* key_;
    Node* value_;
    HashNode* next_;
};

class SimpleHashMap {
  public:
    SimpleHashMap() {
        capacity_ = 100;
        table_ = new HashNode * [capacity_];

        for (int i = 0; i < capacity_; ++i) {
            table_[i] = nullptr;
        }
    }

    ~SimpleHashMap() {
        for (size_t i = 0; i < capacity_; ++i) {
            HashNode* current = table_[i];

            while (current != nullptr) {
                HashNode* child = current;
                current = current->GetNext();
                delete child;
            }

            table_[i] = nullptr;
        }
    }

    Node* Get(String* key) {
        unsigned int hashValue = HashCode(key);
        HashNode* current = table_[hashValue];

        while (current !=nullptr) {
            if (*current->GetKey() == *key) {
                return current->GetValue();
            }

            current = current->GetNext();
        }

        return nullptr;
    }

    void Put(String* key, Node* value) {
        unsigned int hashValue = HashCode(key);
        HashNode* child = nullptr;
        HashNode* current = table_[hashValue];

        while (current != nullptr) {
            child = current;
            current = current->GetNext();
        }

        if (current == nullptr) {
            current = new HashNode(key, value);

            if (child == nullptr) {
                table_[hashValue] = current;

            } else {
                child->SetNext(current);
            }

        }
    }

  private:
    HashNode** table_;
    int capacity_;

    unsigned int HashCode(String* word) {
        unsigned int hash_code = 0;
        for (int i = 0; i < word->Size(); ++i) {
            hash_code += word->Get(i) + hash_code*31;
        }
        return hash_code % capacity_;
    };
};