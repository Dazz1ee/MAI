#include "SimpleHashMap.cpp"

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