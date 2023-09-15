#include <iostream>
#include <cstddef>
#include <ostream>
#include <cstring>
#include <chrono>

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
        array_[current_size_] = 0;
        return value;
    }

    void Clear() {
        while (current_size_ > 0) {
            current_size_--;
            array_[current_size_] = 0;
        }
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

void Clean(char* buffer) {
    for (int i = 0; i < 16; ++i) {
        buffer[i] = '\0';
    }
}


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

struct Node {
    Node() {
        this->link = nullptr;
        this->parent = nullptr;
        this->good_link = nullptr;
    }

    Node(Node* parent, String* letter, bool is_terminal) {
        this->parent = parent;
        this->letter = letter;
        this->is_terminal = is_terminal;
        this->link = nullptr;
        this->good_link = nullptr;
    }

    ~Node() {
        delete letter;
        delete link;
    }

    SimpleHashMap children;
    SimpleHashMap go;
    String* letter;
    Node* parent;
    Node* link;
    bool is_terminal;
    Vector<int> pattern_id;
    Node* good_link;
};

template<typename K, typename V>
struct Entry {
    K key;
    V value;

    Entry() = default;

    Entry(K key, V value){
        this->key = key;
        this->value = value;
    }

    bool operator==(const Entry& other) const {
        return key==other.key;
    }
};



class Trie {
  public:
    Trie() {
        root_ = new Node(nullptr, new String("$"), false);
    }

    void Add(Vector<String*>& word) {
        Node* temporary = root_;
        for(String* letter : word) {
            Node* child = temporary->children.Get(letter);
            if (child == nullptr) {
                child = new Node(temporary, letter, false);
                temporary->children.Put(letter, child);
            }
            temporary = child;
        }
        temporary->is_terminal = true;
        temporary->pattern_id.PushBack(counter++);
        size_patterns.PushBack(word.Size());
    }

    Node* GetLink(Node* node) {
        if (node->link == nullptr) {
            if (node == root_ || node->parent == root_) {
                node->link = root_;
            } else {
                node->link = Go(GetLink(node->parent), node->letter);
            }
        }
        return node->link;
    }

    Node* Go(Node* node, String* letter){
        if (node->go.Get(letter) == nullptr) {
            if (node->children.Get(letter) != nullptr) {
                node->go.Put(letter, node->children.Get(letter));
            } else if (node == root_){
                node->go.Put(letter, root_);
            } else {
                node->go.Put(letter, Go(GetLink(node), letter));
            }
        }
        return node->go.Get(letter);
    }

    Node* GetGoodLink(Node* node) {
        if (node->good_link ==nullptr) {
            Node* closest_suffix = GetLink(node);

            if(closest_suffix == root_) {
                node->good_link = root_;
            } else if (closest_suffix->is_terminal) {
                node->good_link = closest_suffix;
            } else {
                node->good_link = GetGoodLink(closest_suffix);
            }
        }
        return node->good_link;
    }

    void Find(Vector<Entry<String*, Entry<int,int>>>& text){
        int size = text.Size();
        Node* current = root_;

        for (int i = 0; i < size; ++i) {
            current = Go(current, text[i].key);
            for (Node* temporary = current; temporary != root_; temporary = GetGoodLink(temporary)) {
                if(temporary->is_terminal) {
                    for (int id : temporary->pattern_id) {
                        std::cout << text[i-size_patterns[id-1]+1].value.key << ", " << text[i-size_patterns[id-1]+1].value.value << ", " << id << "\n";
                    }
                }
            }
        }
    }

  private:
    Node* root_;
    int counter = 1;
    Vector<int> size_patterns;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie();
    auto start = std::chrono::system_clock::now();
    Trie* trie = new Trie();
    char* temporary = new char[16];
    char c = getchar();
    int index = 0;
    Vector<String*> words;
    Clean(temporary);

    while (c > 0) {
        if (c == '\n') {
            if (index > 0) {
                String* pattern = new String(temporary);
                words.PushBack(pattern);
            }

            if(words.Size() > 0) {
                trie->Add(words);
                words.Clear();
            }

            Clean(temporary);
            index = 0;
        } else if (c == ' ' || c == '\t') {
            if (index > 0) {
                String* pattern = new String(temporary);
                words.PushBack(pattern);
                Clean(temporary);
                index = 0;
            }
        } else {
            temporary[index++] = tolower(c);
        }

        c = getchar();

        if(c == '\n' && words.Size()  == 0 && index == 0) {
            break;
        }

    }

    int line = 1;
    int index_word = 1;
    Vector<char> buffer;
    Vector<Entry<String*, Entry<int, int>>> text;
    c = getchar();

    while (c > 0) {
        if (c == '\n') {
            if(buffer.Size() > 0) {
                String* word = new String(buffer);
                text.PushBack({word, {line, index_word}});
            }

            line++;
            index_word= 1;
        } else if(c == ' ' || c == '\t') {
            if(buffer.Size() > 0) {
                String* word = new String(buffer);
                text.PushBack({word, {line, index_word}});
                index_word++;
            }
        } else {
            buffer.PushBack(tolower(c));
        }
        c = getchar();
    }

    if(buffer.Size() > 0) {
        String* word = new String(buffer);
        text.PushBack({word, {line, index_word}});
    }

    trie->Find(text);
    auto end = std::chrono::system_clock::now();
    auto answer = std::chrono::duration_cast<std::chrono::milliseconds> (end - start);
    std::cout << answer.count() / (double) 1000;
}
