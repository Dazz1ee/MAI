#include <iostream>
//#include "Vector.cpp"
#include "Node.cpp"



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