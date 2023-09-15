//#include <iostream>
//#include "Trie.cpp"
//
//void Clean(char* buffer) {
//    for (int i = 0; i < 17; ++i) {
//        buffer[i] = '\0';
//    }
//}
//
//int main() {
//    std::cin.tie();
//    int len = 1;
//    char c;
////    Vector<char> word;
//    Trie* trie = new Trie();
//    char* temporary = new char[17];
//    while((c = getchar()) && c != '\n') {
//        Vector<String*> words;
//        Clean(temporary);
//        int index = 1;
//        temporary[0] = tolower(c);
//        while ((c = getchar()) && c != '\n') {
//            if (c == ' ') {
//                String* word = new String(temporary);
//                words.PushBack(word);
//                Clean(temporary);
//                index = 0;
//            } else {
//                temporary[index++] = tolower(c);
//            }
//        }
//        String* word = new String(temporary);
//        words.PushBack(word);
//        trie->Add(words);
////        std::cout << temporary;
//        for (String* s : words) {
//            std::cout << (*s) << " ";
//        }
//        std::cout << "\n";
//    }
//    int line = 1;
//    int index_word = 1;
//    Vector<char> buffer;
//    Vector<Entry<String*, Entry<int, int>>> text;
//    while ((c = getchar()) > 0) {
//        if (c == '\n') {
//            String* word = new String(buffer);
//            text.PushBack({word, {line, index_word}});
//            line++;
//            index_word= 1;
//        } else if(c == ' ') {
//            while (c == ' ') {
//                c = getchar();
//            }
//            std::cin.putback(c);
//            String* word = new String(buffer);
//            text.PushBack({word, {line, index_word}});
//            index_word++;
//        } else {
//            buffer.PushBack(tolower(c));
//        }
//    }
//
//    if(buffer.Size() != 0) {
//        String* word = new String(buffer);
//        text.PushBack({word, {line, index_word}});
//    }
//
//    trie->Find(text);
////    Vector<String*> test;
////    test.PushBack(new String("cat"));
////    test.PushBack(new String("dog"));
////        trie->find(test);
//}
