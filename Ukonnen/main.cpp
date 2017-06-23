#include <vector>
#include <algorithm>
#include <string>
#include <limits>
#include <iostream>
#include <fstream>

using namespace std;

const int INF = 250001;


struct Link {
   int start, end;
   int to = -1;
   Link() {}
   Link(int _s, int _e, int _t) : start(_s), end(_e), to(_t){}
};

struct Vertex {
   vector<Link> links;
   int suffix = -1;

   Vertex() {
      links.assign(26, Link());
   }
};

class Tree {
public:
    vector<Vertex> tree;
    int root, dummy;
    string text;

    unsigned char getCh(int i) {
        return (i<0) ? (-i-1+97) : text[i];
    }

    int newVertex() {
        tree.push_back(Vertex());
        return tree.size() - 1;
    }

    void addLink(int _from, int _to, int _start, int _end) {
        tree[_from].links[getCh(_start)-97] = Link(_start, _end, _to);
    }

    pair<int, int> mov(int _v, int _start, int _end) {
        if(_end <= _start) {
            return make_pair(_v, _start);
        } else {
            Link _link = tree[_v].links[getCh(_start)-97];
            while(_end - _start >= _link.end - _link.start) {
                _start += _link.end - _link.start;
                _v = _link.to;
                if(_end > _start)
                    _link = tree[_v].links[getCh(_start)-97];
            }
            return make_pair(_v, _start);
        }
    }

    pair<bool, int> checkTransition(int _v, int _start, int _end, unsigned char _ch) {
        if(_end <= _start) {
            return make_pair(tree[_v].links[_ch-97].to != -1, _v);
        } else {
            Link _link = tree[_v].links[getCh(_start)-97];
            if(_ch == getCh(_link.start + _end - _start))
                return make_pair(true, _v);

            int middle = newVertex();
            addLink(_v, middle, _link.start, _link.start + _end - _start);
            addLink(middle, _link.to, _link.start + _end - _start, _link.end);
            return make_pair(false, middle);
        }
    }

    pair<int, int> update(int _v, int _start, int _end) {
        pair<bool, int> splitRes;
        int _root = root;

        splitRes = checkTransition(_v, _start, _end, getCh(_end));
        while(!splitRes.first) {
            addLink(splitRes.second, newVertex(), _end, INF);
            //add suffix link from the prev vertex
            if(_root != root)
                tree[_root].suffix = splitRes.second;///
            _root = splitRes.second;
            //go to the next vertex
            pair<int, int> newNode = mov(tree[_v].suffix, _start, _end);///
            _v = newNode.first;
            _start = newNode.second;
            splitRes = checkTransition(_v, _start, _end, getCh(_end));
        }
        if(_root != root)
            tree[_root].suffix = splitRes.second;///
        return make_pair(_v, _start);
    }

    void createTree() {
        dummy = newVertex();
        root = newVertex();

        tree[root].suffix = dummy;///
        for(int i = 0; i < 26; i++)
            addLink(dummy, root, -i-1, -i);

        pair<int, int> activePoint = make_pair(root, 0);
        for(unsigned int i = 0; i < text.length(); i++) {
            activePoint = update(activePoint.first, activePoint.second, i);
            activePoint = mov(activePoint.first, activePoint.second, i+1);
        }
    }

    bool checkWord(string word) {
        int _v = root, _start = 0, _end = 0;
        for(unsigned int i = 0; i < word.length(); i++) {
            unsigned char cur = word[i];
            if(_end == _start) {
                if(tree[_v].links[cur-97].to == -1) {
                    return false;
                }
                _start = tree[_v].links[cur-97].start;
                _end = _start + 1;
            } else {
                if(cur != getCh(_end)) {
                    return false;
                }
                _end++;
            }
            if(_end == tree[_v].links[getCh(_start)-97].end) {
                _v = tree[_v].links[getCh(_start)-97].to;
                _start = 0;
                _end = 0;
            }
        }
        return true;
    }
};



int main() {
    Tree tree;

    ifstream fs("input.txt");
	if(!fs)
        return -1;
    getline(fs, tree.text);
    //cout << tree.text.size() << endl;
    //for(unsigned int i = 0; i < tree.text.size(); ++i) {
    //    tree.text[i]-=97;
    //}
    tree.createTree();
    //cout << "!!!!!!" << endl;

    string num;
    getline(fs, num);
    int sz = stoi(num);

    string word;
    ofstream fsout("output.txt");
	for(int r = 0; r < sz; r++) {
        getline(fs, word);
	    //cout << word << " " << tree.checkWord(word) << endl;
	    fsout << tree.checkWord(word) << " ";
	}
	fsout.close();
	fs.close();
    return 0;
}
