#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Graph {
public:
    int gSize;
    vector<int> parent;//для каждой нечётной вершины храним предка
    vector<int> match; //хранит смежные вершины
    vector<int> base;//для псевдоверщин

    vector<bool> flower;//хранит вершины текушего цветка

    vector<vector<int>> g;
    vector<bool> used;
    Graph(int n) {
        gSize = n;
        g.resize(n);
        parent.resize(n);
        used.resize(n);
        match.resize(n);
        base.resize(n);
        flower.resize(n);
    }

    int findMaxParent(int a, int b) {
        vector<bool> used(gSize);
        std::fill(used.begin(), used.end(), 0);
        while(true) {
            a = base[a];
            used[a] = true;
            //getchar();
            if (match[a] == -1) {
                    break;
            }
            a = parent[match[a]];
        }
        while(true) {//ишем уже помеченную вершину
            b = base[b];
            if (used[b]) {
                return b;
            }
            b = parent[match[b]];
        }
    }

    void markFlower(int v, int b, int children) {//заполняем flower
        while(base[v] != b) {
            flower[base[v]] = flower[base[match[v]]] = true;
            parent[v] = children;
            children = match[v];
            v = parent[match[v]];
        }
    }

    int dfs(int root) {
        queue<int> _queue;
        std::fill(used.begin(), used.end(), 0);
        std::fill(parent.begin(), parent.end(), -1);
        for (int i = 0; i < gSize; ++i) {
            base[i] = i;
        }

        used[root] = true;
        _queue.push(root);
        while(!_queue.empty()) {
            int v = _queue.front();
            _queue.pop();

            for(int i = 0; i < g[v].size(); ++i) {
                int to = g[v][i];
                if(base[v] == base[to] || match[v] == to) {
                    continue;
                }
                if(to == root || match[to] != -1 && parent[match[to]] != -1) {
                    int cur = findMaxParent(v, to);
                    std::fill(flower.begin(), flower.end(), 0);
                    markFlower(v, cur, to);
                    markFlower(to, cur, v);
                    for(int j = 0; j < gSize; ++j)
                        if(flower[base[j]]) {
                            base[j] = cur;
                            if(!used[j]) {
                                used[j] = true;
                                _queue.push(j);
                            }
                        }
                } else if(parent[to] == -1) {
                    parent[to] = v;
                    if(match[to] == -1)
                        return to;
                    to = match[to];
                    used[to] = true;
                    _queue.push(to);
                }
            }
        }
        return -1;
    }
};


int main() {
    int _size;
    //FILE *inFile;
    //inFile = fopen("input0.txt", "rt");
    //inFile = fopen("input1.txt", "rt");
    //inFile = fopen("input2.txt", "rt");
    //inFile = fopen("input3.txt", "rt");
    //inFile = fopen("input4.txt", "rt");
    //inFile = fopen("input5.txt", "rt");
    //fscanf(inFile, "%d", &_size);
    cin >> _size;
    //cout << _size << endl;
    Graph g(_size);

    int a, b;
    //while(!feof(inFile)) {
    while(cin >> a >> b) {
        //fscanf(inFile, "%d", &a);
        //fscanf(inFile, "%d", &b);
        //cout << a << " " << b << endl;
        g.g[a-1].push_back(b-1);
        g.g[b-1].push_back(a-1);
    }
    /*
    for(int i = 0; i < _size; ++i) {
        for(int j = 0; j < _size; ++j)
            cout << g.g[i][j] << " ";
        cout << "\n";
    }
    */
    std::fill(g.match.begin(), g.match.end(), -1);
    for(int i = 0; i < _size; ++i) {
		if(g.match[i] == -1) {
			int v = g.dfs(i);
			while(v != -1) {
				int pv = g.parent[v];
				int ppv = g.match[pv];
				g.match[v] = pv;
				g.match[pv] = v;
				v = ppv;
			}
		}
    }

    int k = 0;
    for(int i = 0; i < g.match.size(); ++i) {
        //cout << g.match[i] << " ";
        if(g.match[i] != -1) {
            k++;
        }

    }
    cout  << k << endl;
    for(int i = 0; i < _size; ++i) {
        if(g.match[i] != -1) {
            cout << i+1 << " " << g.match[i] + 1 << "\n";
            g.match[g.match[i]] = -1;
        }

    }
    return 0;
}
