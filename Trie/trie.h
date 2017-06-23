#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

#include <iostream>
#include <string>
#include <map>
class TrieNode {
public:
    std::string val = "";
    TrieNode *parent = nullptr;
    std::map<unsigned char, TrieNode*> children;
    bool isExist = false;

    TrieNode() {}
    ~TrieNode(){}
};


class Trie {
public:
    TrieNode *root;

    Trie() {
        root = new TrieNode();
    }

    void _add(std::string in, TrieNode *_root) {
        TrieNode *_node = _root->children[in[0]];
        if(_node == nullptr) {
            _root->children[in[0]] = new TrieNode();

            _root->children[in[0]]->val = in;
            _root->children[in[0]]->parent = _root;
            _root->children[in[0]]->isExist = true;
            //std::cout << in;
        } else {
            if(_node->val.size() == 1) {
                std::string _s = in.substr(1, in.size()-1);
                if(_s.size() > 0) {
                    _add(in.substr(1, in.size()-1), _node);
                } else {
                    _node->isExist = true;
                }
            } else {
                int i = 0;
                bool test = false;
                while(_node->val[i] == in[i]) {

                    if(_node->val.size() == i+1 && in.size() > i+1) {
                        _add(in.substr(i+1, in.size()-i), _node);
                        test = true;
                        break;
                    } else if(_node->val.size() > i+1 && in.size() == i+1) {
                        _node->children[_node->val[i+1]] = new TrieNode();
                        _node->children[_node->val[i+1]]->val = _node->val.substr(i+1, _node->val.size()-i);
                        _node->children[_node->val[i+1]]->isExist = true;
                        _node->children[_node->val[i+1]]->parent = _node;
                        _node->val = _node->val.substr(0, i+1);
                        _node->isExist = true;
                        test = true;
                        break;
                    } else if(in.size() == i+1 && _node->val.size() == i+1) {
                        test = true;
                    }
                    i++;
                }
                if(!test) {
                    _node->children[in[i]] = new TrieNode();
                    _node->children[in[i]]->val = in.substr(i, in.size()-i);
                    _node->children[in[i]]->isExist = true;
                    _node->children[in[i]]->parent = _node;
                    _node->children[_node->val[i]] = new TrieNode();
                    _node->children[_node->val[i]]->val = _node->val.substr(i, _node->val.size()-i);
                    _node->children[_node->val[i]]->isExist = true;
                    _node->children[_node->val[i]]->parent = _node;
                    _node->val = _node->val.substr(0, i);
                    _node->isExist = false;
                }
            }
        }
    }
    void add(std::string in) {
        _add(in, root);
    }

    bool _exist(std::string in, TrieNode *_root) {
        TrieNode *_node = _root->children[in[0]];
        if(_node == nullptr) {
            return false;
        } else {
            if(_node->val.size() == 1) {
                if(in == _node->val && _node->isExist) {
                    return true;
                } else if(in != _node->val) {
                    return _exist(in.substr(1, in.size()-1), _node);
                } else {
                    return false;
                }
            } else {
                if(in == _node->val && _node->isExist) {
                    return true;
                } else {
                    int i = 0;
                    while(_node->val[i] == in[i]) {

                        if(_node->val.size() == i+1 && in.size() > i+1) {
                            return _exist(in.substr(i+1, in.size()-i), _node);
                        } else if(_node->val.size() > i+1 && in.size() == i+1) {
                            return false;
                        }
                        i++;
                    }
                }
            }
        }
    }

    bool exist(std::string in) {
        return _exist(in, root);
    }

    TrieNode* _del(std::string in, TrieNode *_root) {
        TrieNode *_node = _root->children[in[0]];
        if(_node == nullptr) {
            return nullptr;
        } else {
            if(_node->val.size() == 1) {
                if(in == _node->val && _node->isExist) {
                    return _node;
                } else if(in != _node->val) {
                    return _del(in.substr(1, in.size()-1), _node);
                } else {
                    return nullptr;
                }
            } else {
                if(in == _node->val && _node->isExist) {
                    return _node;
                } else {
                    int i = 0;
                    while(_node->val[i] == in[i]) {

                        if(_node->val.size() == i+1 && in.size() > i+1) {
                            return _del(in.substr(i+1, in.size()-i), _node);
                        } else if(_node->val.size() > i+1 && in.size() == i+1) {
                            return nullptr;
                        }
                        i++;
                    }
                }
            }
        }
    }
    void del(std::string in) {
        TrieNode *delNode = _del(in, root);
       // std::cout << "!!!!!" <<delNode->val;
        if(delNode == nullptr) {
            return;
        } else {
            if(delNode->children.size() == 0) {
                delNode->parent->children[delNode->val[0]] = nullptr;
                delete delNode;
            } else if(delNode->children.size() == 1) {
                auto it = delNode->children.begin();
                delNode->val += (*it).second->val;
                (*it).second->val = delNode->val;
                (*it).second->parent = delNode->parent;
                delNode->parent->children[delNode->val[0]] = (*it).second;
                delete delNode;
            } else {
                delNode->isExist = false;
            }
        }
    }

    unsigned int _size(TrieNode *root) {
        unsigned int sz = 0;
        if(root == nullptr) {
            return 0;
        }
        if(root->children.size() == 0) {
            return (root->isExist) ? 1 : 0;
        }
        for(auto it = root->children.begin(); it != root->children.end(); ++it) {
            sz += _size((*it).second);
        }
        sz += (root->isExist) ? 1 : 0;
        return sz;
    }
    unsigned int size() {
        return _size(root);
    }

    void _clear(TrieNode *_root) {
        if(_root == nullptr) {
            return;
        }
        if(_root->children.size() == 0) {
            delete _root;
            _root = nullptr;
            return;
        }
        for(auto it = _root->children.begin(); it != _root->children.end(); ++it) {
            _clear((*it).second);
        }
        delete _root;
        _root = nullptr;
        return;
    }
    void clear() {
        _clear(root);
        root = new TrieNode();
    }

    void _print(TrieNode *root, int l, int rot) {
        bool if_print = false;
        if(root == nullptr) {
            return;
        }
        if(root->children.size() == 0) {
            if(l != 0) {
                for(int j = 0; j < l-1; ++j) {
                    std::cout << "     ";
                }
                if(rot == 0) {
                    std::cout << "\xc0\xc4\xc4\xc4\xc4";
                } else {
                    std::cout << "\xda\xc4\xc4\xc4\xc4";
                }
            }
            std::cout << root->val << "(" << root->isExist << ")" << std::endl;
        }
        int k = 0;
        //std::cout << root->children.size();
        for (auto it = root->children.begin(); it != root->children.end(); ++it) {
            if(k < root->children.size()/2){
                _print((*it).second, l+1, 1);
                std::cout << std::endl;
            }
            if(k >= root->children.size()/2 && !if_print) {
                if(l != 0) {
                    for(int j = 0; j < l-1; ++j) {
                        std::cout << "     ";
                    }
                    if(rot == 0) {
                        std::cout << "\xc0\xc4\xc4\xc4\xc4";
                    } else {
                        std::cout << "\xda\xc4\xc4\xc4\xc4";
                    }
                }
                std::cout << root->val << "(" << root->isExist << ")" << std::endl;
                if_print = true;
            }
            if(k >= root->children.size()/2) {
                _print((*it).second, l+1, 0);
                std::cout << std::endl;
            }
            k++;
        }
    }

    void print() {
        _print(root, 0, 0);
    }

    ~Trie() {
        clear();
        delete root;
    }

};


#endif // TRIE_H_INCLUDED
