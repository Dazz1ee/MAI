#include <cstdlib>
#include <iostream>
template<typename T>
struct NodeList{
    T data;
    NodeList* left;
    NodeList* right;
};

template<typename T>
class LinkedList {
  public:
    LinkedList() {
        first_ = nullptr;
        last_ = nullptr;
        size_ = 0;
    }
    ~ LinkedList() {
        while (size_ > 0) {
            NodeList<T>* node = first_;
            first_ = first_->right;
            size_--;
            delete node;
        }
    }
    int Size() {
        return size_;
    }

    void AddFirst(T value) {
        NodeList<T>* node = (NodeList<T>*) malloc(sizeof(NodeList<T>));
        if (!node) {
            std::cout << "ERROR: no memory\n";
            exit(1);
        }

        node->data = value;
        node->right = first_;
        if (last_ == nullptr) {
           last_ = node;
        }
        if(first_ != nullptr) {
            first_->left = node;
        }
        first_ = node;
        size_++;
    }

    void AddLast(T value) {
        NodeList<T>* node = new NodeList<T>;
        node->data = value;
        node->left = last_;

        if (first_ == nullptr) {
            first_ = node;
        }
        if(last_ != nullptr) {
            last_->right = node;
        }

        last_ = node;
        size_++;
    }

    void PopFirst() {
        if (size_ == 0) {
            return;
        }

        NodeList<T>* node = first_;

        if (size_ == 1) {
            first_ = nullptr;
            last_ = nullptr;
        } else {
            first_ = first_->right;
            first_->left = nullptr;
        }

        size_--;
        node = nullptr;
    }


    void PopLast() {
        if (size_ == 0) {
            return;
        }

        NodeList<T>* node = first_;

        if (size_ == 1) {
            first_ = nullptr;
            last_ = nullptr;
        } else {
            last_ = last_->left;
            last_->right = nullptr;
        }

        size_--;
        node = nullptr;
    }

    T PeekFirst() {
        return first_->data;
    }


    T PeekLast() {
        return last_->data;
    }

  private:
    NodeList<T>* first_;
    NodeList<T>* last_;
    int size_;
};