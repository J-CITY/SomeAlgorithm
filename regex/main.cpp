#include <map>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <string>

enum class Symbols {
  rbar  ,//]
  lbar  ,//[
  rfbar ,//}
  lfbar ,//{
  rpar  ,//)
  lpar  ,//(
  star  ,//*
  plus  ,//+
  qust  ,//?
  rslash,//
  arror ,//^
  dollar,//$
  point , //.
  orop,//|
  id
};
const int keystar = 1;
const int keyplus = 2;
const int keyquest = 3;

class State {
public:
    Symbols symbol;
    //int key = 0;
    int low = -1;
    int hight = -1;
    unsigned char val;
    std::vector<State*> children;

    State(Symbols _symbol, unsigned char _val) : val(_val) {
        symbol = _symbol;
    }
    virtual void print() = 0;
};

class StateOper : public State {
public:

    StateOper(Symbols _symbol, unsigned char _val) : State(_symbol, _val) {}
    void print() {
        std::cout << val;
    }
};

class StateIdent: public State {
public:
    std::vector<unsigned char> ids;
    StateIdent(Symbols _symbol, unsigned char _val) : State(_symbol, _val) {}
    void print() {
        std::cout << val << " ("<< low << "-" << hight <<")";
        for(int i = 0; i < ids.size(); ++i) {
            std::cout << ids[i] << " ";
        }
    }
};


class Regexp {
public:
    int pos = 0;
    std::string regexStr;
    std::string inputStr;
    State *root = nullptr;
    State *term();
    State *expression();
    bool test(State *root);


};

State *Regexp::term() {
    State *fres = nullptr;
    if(pos >= regexStr.size()) {
        return fres;
    }
    if(regexStr[pos] == '[') {
        fres = new StateIdent(Symbols::id, 'i');
        ++pos;
        while(regexStr[pos] != ']') {
            if(pos >= regexStr.size()) {
                exit(-1);
            }
            if(regexStr[pos] == '\\') {
                ++pos;
                ((StateIdent*)fres)->ids.push_back(regexStr[pos]);
                continue;
            }
            if(regexStr[pos] == '-') {
                pos++;
                if(((StateIdent*)fres)->ids.size() > 0) {
                    for(int i = (int)(((StateIdent*)fres)->ids[((StateIdent*)fres)->ids.size()-1]+1); i <= (int)regexStr[pos]; ++i) {
                        ((StateIdent*)fres)->ids.push_back((unsigned char)i);
                    }
                }
                pos++;
                continue;
            }
            ((StateIdent*)fres)->ids.push_back(regexStr[pos]);
            pos++;
        }
        pos++;

    } else if(regexStr[pos] == '.') {
        fres = new StateOper(Symbols::point, '.');
        pos++;
    } else if (regexStr[pos] == '(') {
        pos++;
        fres = expression();
        if(regexStr[pos] != ')'){
            //std::cout << regexStr[pos] << " " << pos;
            fres->print();
            exit(-5);
        }
        pos++;
    } else if(regexStr[pos] != '*' &&
              regexStr[pos] != '+' &&
              regexStr[pos] != '?'){
        if(regexStr[pos] == '\\') {
            ++pos;
        }
        fres = new StateIdent(Symbols::id, 'i');
        ((StateIdent*)fres)->ids.push_back(regexStr[pos]);
        pos++;
    }
    if(pos >= regexStr.size()) {
        return fres;
    }

    if(regexStr[pos] == '*') {
        ++pos;
        if(fres != nullptr) {
            fres->low = 0;
            fres->hight = -2;
        }
        else
            exit(-3);
    } else if(regexStr[pos] == '+') {
        ++pos;
        if(fres != nullptr) {
            fres->low = 1;
            fres->hight = -2;
        }
        else
            exit(-4);
    } else if(regexStr[pos] == '?') {
        ++pos;
        if(fres != nullptr) {
            fres->low = 0;
            fres->hight = 1;
        }
        else
            exit(-5);
    } else if (regexStr[pos] == '{') {
        pos++;
        if(regexStr[pos] == ',') {
            fres->low = -2;
            pos++;
        } else {
            std::string name = "";
            while ( regexStr[pos] >= '0' && regexStr[pos] <= '9') {
                name += regexStr[pos];
                ++pos;
            }
            std::string::size_type sz;
            fres->low = std::stoi(name);
        }
        if(regexStr[pos] == ',') {
            if(fres->low == -2) {
                exit(-6);
            }
            pos++;
            if(regexStr[pos] == '}') {
                fres->hight = -2;
                pos++;
                return fres;
            } else {
                std::string name = "";
                while ( regexStr[pos] >= '0' && regexStr[pos] <= '9') {
                    name += regexStr[pos];
                    ++pos;
                }
                std::string::size_type sz;
                fres->hight = std::stoi(name);
            }
        } else {
            if(regexStr[pos] == '}') {
                pos++;
                return fres;
            }
            std::string name = "";
            while ( regexStr[pos] >= '0' && regexStr[pos] <= '9') {
                name += regexStr[pos];
                ++pos;
            }
            std::string::size_type sz;
            fres->hight = std::stoi(name);
        }
        if(regexStr[pos] != '}') {
            exit(-6);
        }
        pos++;

    }


    return fres;
}

State *Regexp::expression() {
    State *tnres = term();
    if(regexStr[pos] == ')') {
        return tnres;
    }
    State *tnresbuf = tnres;
    while(regexStr[pos] != '|' && pos < (regexStr.size()) && regexStr[pos] != ')') {
        //std::cout << regexStr[pos] << "  " << pos << "\n";
        tnresbuf->children.push_back(term());
        tnresbuf = tnresbuf->children[tnresbuf->children.size()-1];
    }
    if(pos >= regexStr.size()) {
        return tnres;
    }

    State *tn = nullptr;
    State *oper = nullptr;
    while (regexStr[pos] == '|') {
        //std::cout << "#$%^&";
        oper = new StateOper(Symbols::orop, '|');
        ++pos;
        oper->children.push_back(tnres);
        tn = term();
        oper->children.push_back(tn);
        tnres = oper;
        oper = nullptr;
        tn = nullptr;
    }

    return tnres;
}

void printTree(State *root, int l) {
    bool if_print = false;
    if(root == nullptr) {
        return;
    }
    if(root->children.size() == 0) {
        for(int j = 0; j < l; ++j) {
            std::cout << "     ";
        }
        root->print();
        std::cout << std::endl;
    }

    for (unsigned int k = 0; k < root->children.size(); ++ k) {
        if(k < root->children.size()/2 && root->children[k] != nullptr){
            printTree(root->children[k], l+1);
            std::cout << std::endl;
        }
        if(k >= root->children.size()/2 && !if_print) {
            for(int j = 0; j < l; ++j) {
                std::cout << "     ";
            }
            root->print();
            std::cout << std::endl;
            if_print = true;
        }
        if(k >= root->children.size()/2  && root->children[k] != nullptr) {
            printTree(root->children[k], l+1);
            std::cout << std::endl;
        }
    }
}

bool Regexp::test(State *root) {
    bool testb = false;
    if(pos >= inputStr.size()) {
        return true;
    }
    if(root == nullptr) {
        return false;
    }
    if(root->val == '|') {
        bool a = test(root->children[0]);
        bool b = test(root->children[1]);
        if(a || b) {
            if(root->children.size()>2) {
               return test(root->children[2]);
            }
        } else {
            return false;
        }
    }
    if(root->val == 'i') {
        if(root->low == -1 && root->hight == -1) {
            //std::cout <<inputStr[pos] << " !";
            bool _b = false;
            for(int i = 0; i < ((StateIdent*)root)->ids.size(); ++i) {
                if(inputStr[pos] == ((StateIdent*)root)->ids[i]) {
                    pos++;
                    _b = true;
                    break;
                }
            }
            if(!_b) {
                return false;
            } else {
                if(root->children.size() == 0)
                    return true;
                //for(int j = 0; j < root->children.size(); ++j) {

                        testb = test(root->children[root->children.size()-1]);
                        if(testb == true) {
                            return true;
                        }
                //}
                return false;
            }
        } else {
            int _count = 0;
            bool _b = false;
            for(int i = 0; i < ((StateIdent*)root)->ids.size(); ++i) {
                if(inputStr[pos] == ((StateIdent*)root)->ids[i]) {
                    pos++;
                    _b = true;
                    _count++;
                    break;
                }
            }
            while(_b) {
                if(pos >= inputStr.size() && root->children.size() == 0) {
                    return true;
                }
                if(pos >= inputStr.size() && root->children.size() != 0) {
                    return false;
                }
                _b = false;
                for(int i = 0; i < ((StateIdent*)root)->ids.size(); ++i) {
                    if(inputStr[pos] == ((StateIdent*)root)->ids[i]) {
                        pos++;
                        _b = true;
                        _count++;
                        break;
                    }
                }
            }
            //std::cout << _count << " " <<inputStr[pos];
            if((root->low != -1 && root->hight == -1) && _count != root->low) {//=n
                return false;
            }
            if((root->low != -2 && root->hight == -2) && _count < root->low) {//=n
                return false;
            }
            if((root->low == -2 && root->hight != -2) && _count > root->hight) {//=n
                return false;
            }
            if((root->low != -2 && root->hight != -2 && root->hight != -1) && ((_count < root->low) ||
               (_count > root->hight))) {//=n
                return false;
            }
            if(root->children.size() == 0)
                return true;
            //for(int j = 0; j < root->children.size(); ++j) {

            testb = test(root->children[root->children.size()-1]);
            if(testb == true) {
                return true;
            }
            //}
            return false;
        }

    }
}

int main(int argc, char* argv[])
{
    Regexp re;
    //re.regexStr = "([_A-Za-z])([_A-Za-z0-9])*";
    //re.inputStr = "_1abc";

    //re.regexStr = "(a*|b)q?123";
    //re.inputStr = "aaaaaaaaaaaq123";

    //re.regexStr = "(a*|b)..";
    //re.inputStr = "aaaab";

    //re.regexStr = "\\**";
    //re.inputStr = "********";

    re.regexStr = "ab((abc)|d)qz";
    re.inputStr = "ababcqz";


    re.root = re.expression();
    re.pos = 0;
    printTree(re.root, 0);

    std::cout << re.test(re.root);

    return 0;
}


