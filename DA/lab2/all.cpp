#include <cstring>
#include <iostream>
#include <fstream>

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

struct Node {
    unsigned long long data;
    char* key;
    char balance;
    Node* left;
    Node* right;
};

typedef Node* NodePointer;

class Tree {
  public:

    bool Insert(char* key, unsigned long long data){
        bool balance = true;
        bool is_exist = false;
        root_ = Add(root_,key, data, balance, is_exist);
        return is_exist;
    }

    bool Delete(char* key){
        bool balance = true;
        bool is_exist = true;
        root_ = Erase(root_,key, balance, is_exist);
        return is_exist;
    }

    NodePointer Find(char* key){
        return FindHelper(root_, key);
    }

    void Save(std::ofstream& file) {
        SaveHelper(root_, file);
    }

    void Load(std::ifstream& file) {
        DestroyHelper(root_);
        root_ = nullptr;
        char* key = new char[257];
        unsigned long long value;

        while (file.read(key, sizeof(char) * 257)) {
            file.read(reinterpret_cast<char*>(&value), sizeof(unsigned long long));
            Insert(key, value);
            key = new char[257];
        }
    }


  private:
    NodePointer root_ = nullptr;

    void SaveHelper(NodePointer node, std::ofstream& file) {
        if (node == nullptr) {
            return;
        }

        LinkedList<Node*>* queue = new LinkedList<Node*>;
        queue->AddFirst(node);

        while (queue->Size() != 0) {
            int size = queue->Size();
            for (int i = 0; i < size; ++i) {
                NodePointer temporary = queue->PeekFirst();
                if (temporary->left!=nullptr) {
                    queue->AddLast(temporary->left);
                }
                if (temporary->right!=nullptr) {
                    queue->AddLast(temporary->right);
                }

                file.write(temporary->key, sizeof(char)*257);
                file.write(reinterpret_cast<char*>(&temporary->data), sizeof(unsigned long long));
                queue->PopFirst();
            }
        }
    }

    void DestroyHelper(NodePointer node) {
        if (node == nullptr) {
            return;
        }

        DestroyHelper(node->left);
        DestroyHelper(node->right);
        free(node);
        node= nullptr;
    }

    NodePointer FindHelper(NodePointer node, char* key) {
        if (node == nullptr) {
            return nullptr;
        }
        int result_compare = Compare(node->key, key);
        if (result_compare > 0) {
            return FindHelper(node->left, key);
        } else if (result_compare < 0) {
            return FindHelper(node->right, key);
        } else {
            return node;
        }

    }

    NodePointer Erase(NodePointer node, char* key, bool& need_balance, bool& is_exist) {
        if (node == nullptr) {
            is_exist = false;
            return node;
        }

        int result_compare = Compare(node->key, key);

        if (result_compare > 0) {
            node->left = Erase(node->left, key, need_balance, is_exist);
            if (!need_balance || !is_exist) {
                return node;
            }

            if (node->balance == -1) {
                NodePointer balanced_node = Balance(node, need_balance);
                if (balanced_node->balance != 0) {
                    need_balance = false;
                }
                return balanced_node;
            }

            if (node->balance == 0) {
                need_balance = false;
            }

            node->balance -=1;
            return node;
        } else if (result_compare < 0) {
            node->right = Erase(node->right, key, need_balance, is_exist);

            if (!need_balance || !is_exist) {
                return node;
            }

            if (node->balance == 1) {
                NodePointer balanced_node = Balance(node, need_balance);
                if (balanced_node->balance != 0) {
                    need_balance = false;
                }
                return balanced_node;
            }

            if (node->balance == 0) {
                need_balance = false;
            }

            node->balance += 1;
            return node;
        } else {
            is_exist = true;
            NodePointer temporary;

            if (node->right == nullptr || node->left == nullptr){
                NodePointer temporary;
                if (node->right == nullptr && node->left == nullptr) {
                    free(node);
                    node = nullptr;
                    return nullptr;
                } else if (node->left == nullptr) {
                    temporary = node->right;
                } else {
                    temporary = node->left;
                }
                free(node);
                return temporary;
            } else {
                NodePointer temporary = node->right;
                while (temporary->left != nullptr) {
                    temporary = temporary->left;
                }

                char* buffer_key = node->key;
                unsigned long long buffer_data = node->data;
                node->key = temporary->key;
                node->data = temporary->data;
                temporary->data = buffer_data;
                temporary->key = buffer_key;

                node->right = Erase(node->right, buffer_key, need_balance, is_exist);
                if (!need_balance) {
                    return node;
                }
                if (node->balance == 1) {
                    NodePointer balanced_node = Balance(node,need_balance);
                    if (balanced_node->balance != 0) {
                        need_balance = false;
                    }
                    return balanced_node;
                }

                if (node->balance == 0) {
                    need_balance = false;
                }
                node->balance += 1;
                return node;
            }
        }

    }

    NodePointer Add(NodePointer node, char* key, unsigned long long data, bool&  need_balance, bool& is_exist) {
        if (node == nullptr) {
            NodePointer new_node = (NodePointer) malloc(sizeof(Node));

            if (!new_node) {
                std::cerr << "ERROR: no memory\n";
            }

            new_node->left = nullptr;
            new_node->right = nullptr;
            new_node->key = key;
            new_node->data = data;
            new_node->balance = 0;
            if(root_ == nullptr) {
                root_ = new_node;
            }
            return new_node;
        }

        int result_compare = Compare(node->key, key);

        if (result_compare == 0) {
            need_balance = false;
            is_exist = true;
            return node;
        }

        if (result_compare > 0) {
            node->left = Add(node->left, key, data, need_balance, is_exist);

            if (!need_balance || is_exist) {
                return node;
            }

            if(node->balance == 1) {
                NodePointer balanced_node = Balance(node, need_balance);
                if (balanced_node->balance == 0) {
                    need_balance = false;
                }
                return balanced_node;
            } else if(node->balance == -1) {
                need_balance = false;
            }

            node->balance += 1;
            return node;
        } else {
            node-> right = Add(node->right, key, data, need_balance, is_exist);
            if(!need_balance || is_exist) {
                return node;
            }

            if (node->balance == -1) {
                NodePointer balanced_node = Balance(node, need_balance);
                if (balanced_node->balance == 0) {
                    need_balance = false;
                }
                return balanced_node;
            }
            if (node->balance == 1) {
                need_balance = false;
            }
            node->balance -=1;
            return node;
        }

    }

    NodePointer Balance(NodePointer node, bool& need_balance) {
        if (node->balance == 1) {
            if (!need_balance) {
                return node;
            }
            if (node->left->balance==-1) {
                if (node->left->right->balance==1) {
                    node->left->right->balance = 0;
                    node->left->balance = 0;
                    node->balance = -1;
                } else if (node->left->right->balance==-1) {
                    node->left->right->balance = 0;
                    node->left->balance = 1;
                    node->balance = 0;
                } else {
                    node->left->right->balance = 0;
                    node->left->balance = 0;
                    node->balance = 0;
                }

                node->left = LeftRotate(node->left);
                return RightRotate(node);
            } else {
                NodePointer current_node = RightRotate(node);

                if (current_node->balance==1) {
                    node->balance = 0;
                    current_node->balance = 0;
                } else {
                    node->balance = 1;
                    current_node->balance = -1;
                }
                return current_node;
            }

        } else {
            if (!need_balance) {
                return node;
            }


            if (node->right->balance==1) {
                if (node->right->left->balance==1) {
                    node->right->left->balance = 0;
                    node->right->balance = -1;
                    node->balance = 0;
                } else if (node->right->left->balance==-1) {
                    node->right->left->balance = 0;
                    node->right->balance = 0;
                    node->balance = 1;
                } else {
                    node->right->left->balance = 0;
                    node->right->balance = 0;
                    node->balance = 0;
                }

                node->right = RightRotate(node->right);
                return LeftRotate(node);
            } else {

                NodePointer current_node = LeftRotate(node);

                if (current_node->balance==-1) {
                    node->balance = 0;
                    current_node->balance = 0;
                } else {
                    node->balance = -1;
                    current_node->balance = 1;
                }
                return current_node;
            }
        }

    }


    NodePointer RightRotate(NodePointer node) {
        NodePointer left_child = node->left;
        node->left = left_child->right;
        left_child->right = node;
        return left_child;
    }

    NodePointer LeftRotate(NodePointer node) {
        NodePointer right_child = node->right;
        node->right = right_child->left;
        right_child->left = node;
        return right_child;
    }

    int Compare(char* parent, char* child) {
        int length_parent = strlen(parent);
        int length_child = strlen(child);

        if (length_child == length_parent) {
            for (int i = 0; i < length_parent; ++i) {
                if(parent[i] > child[i]) {
                    return 1;
                } else if (parent[i] < child[i]) {
                    return -1;
                }
            }
            return 0;
        }
        return length_parent - length_child;
    }


};


class RequestHandler {
  public:
    static void Start() {
        std::cin.tie(0);
        std::ios_base::sync_with_stdio(false);
        Tree tree;
        char* key = new char[257];
        unsigned long long value = 0;
        while (std::cin >> key) {
            if (key[0] == '+') {
                std::cin >> key;
                std::cin >> value;
                ToLower(key);

                if (tree.Insert(key,value)){
                    std::cout << "Exist" << std::endl;
                } else {
                    std::cout << "OK" << std::endl;
                }
            } else if (key[0] == '-') {
                std::cin >> key;
                ToLower(key);

                if(!tree.Delete(key)){
                    std::cout << "NoSuchWord" << std::endl;
                } else {
                    std::cout << "OK" << std::endl;
                }
            } else if (key[0] == '!') {
                std::cin >> key;

                if(strcmp(key, "Save") == 0) {
                    std::cin >> key;
                    std::ofstream file(key, std::ios_base::binary | std::ios_base::trunc);
                    tree.Save(file);
                    std::cout << "OK" << std::endl;

                    file.close();
                } else {
                    std::cin >> key;
                    std::ifstream file(key, std::ios_base::binary);
                    tree.Load(file);
                    std::cout << "OK" << std::endl;
                    file.close();
                }
            } else {
                ToLower(key);
                NodePointer result = tree.Find(key);
                if (result == nullptr) {
                    std::cout << "NoSuchWord" << std::endl;
                } else {
                    std::cout << "OK: ";
                    std::cout << result->data << std::endl;
                }
            }
            key = new char[257];
        }
    }
  private:

    static void ToLower(char* key){
        for (int i = 0; i < strlen(key); ++i) {
            if (key[i] > 64 && key[i] < 91) {
                key[i] += 32;
            }
        }
    }

};

int main() {
    RequestHandler::Start();
}