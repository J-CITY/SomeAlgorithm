#include <iostream>
#include "trie.h"

using namespace std;

int main()
{
    Trie trie;
    //trie.add("abcde");
    //trie.add("ac");
    //trie.add("ab");
    //trie.add("c");
    //trie.add("a");

    trie.add("acde");
    trie.add("acde");
    trie.add("a");
    trie.add("acde");
    trie.add("acdeg");
    trie.add("azkl");
    trie.add("aqqq");
    trie.add("acdeg");
    trie.add("ab");
    trie.add("bc");
    trie.add("bbb");

    trie.print();

    //cout << trie.exist("bbb") << endl;
    //cout << trie.exist("bbbb") << endl;
    //cout << trie.exist("a") << endl;
    //cout << trie.exist("b") << endl;
    //cout << trie.exist("acde") << endl;
    //cout << trie.exist("acdeg") << endl;
    //cout << trie.exist("acdegf") << endl;
    //cout << trie.exist("azkl") << endl;
    cout << "size " << trie.size() << endl;
    trie.clear();
    cout << "size " << trie.size() << endl;

    //trie.del("a");
    //trie.print();
    //trie.del("acde");
    //trie.del("bc");
    //trie.print();
    //trie.add("a");
    //trie.add("acde");
    //trie.add("bc");
    //trie.print();


    return 0;
}
