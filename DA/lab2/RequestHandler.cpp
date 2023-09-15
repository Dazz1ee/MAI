#import "AVL.cpp"
#import <iostream>

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