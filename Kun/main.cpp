#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;
/*
vector< vector<int> > g;
vector<int> matching;
vector<bool> used;

bool dfs(int v) {
    if(used[v])
        return false;
    used[v] = true;
    for(int i = 0; i < g[v].size(); ++i) {
        int to = g[v][i];
        if(matching[to] == -1 || dfs(matching[to])) {
            matching[to] = v;
            return true;
        }
    }
    return false;
}

int main()
{
    unsigned int _size=0;
    //FILE *inFile;
    //inFile = fopen("input.txt", "rt");
    //fscanf(inFile, "%d", &_size);
    cin >> _size;

    //vector< vector<int> > g(_size, vector<int>(_size));
    g.resize(_size);
    //for(int i = 0; i < _size; ++i) {
    //    g[i].resize(_size);
    //}
    //vector<int> matching(_size);
    matching.resize(_size);
    fill(matching.begin(), matching.end(), -1);
    used.resize(_size);
    //vector<false> used(_size);

    int a, b;
    int i = 0;
    //while(!feof(inFile)) {
    while(cin >> a >> b) {
        //fscanf(inFile, "%d", &a);
        //fscanf(inFile, "%d", &b);
        //cin >> a >> b;
        //cout << a << " " <<b;
        g[a-1].push_back(b-1);
        g[b-1].push_back(a-1);
        ++i;
    }
    //fclose(inFile);

    for(int i = 0; i < _size; ++i) {
        fill(used.begin(), used.end(), false);
        dfs(i);
    }
    int k = 0;
    for(int i = 0; i < _size; ++i) {
        //cout << matching[i] << " ";
        if(matching[i] != -1 && matching[i] != 0) {
            k++;
        }

    }
    cout  << k << endl;
    int j = 0;
    for(int i = 0; i < _size; ++i) {
        if(matching[i] != -1 && matching[i] != 0) {
            j++;
            //if(j == 1)
                cout << i+1 << " " << matching[i]+1 << "\n";
                matching[matching[i]] = -1;
        }
        //if(j == 2) {
        //    cout << "\n";
        //    j = 0;
        //}

    }

    //cout << "Hello world!" << endl;
    return 0;
}*/


#include <stdio.h>
#include <string.h>




const int MAXN = 4; // максимально возможное число вершин во входном графе


int n;
vector<int> g[MAXN];
int match[MAXN], p[MAXN], base[MAXN], q[MAXN];
bool used[MAXN], blossom[MAXN];





int lca (int a, int b) {
	bool used[MAXN] = { 0 };
	// поднимаемся от вершины a до корня, помечая все чётные вершины
	for (;;) {
		a = base[a];
		used[a] = true;
		if (match[a] == -1)  break; // дошли до корня
		a = p[match[a]];
	}
	// поднимаемся от вершины b, пока не найдём помеченную вершину
	for (;;) {
		b = base[b];
		if (used[b])  return b;
		b = p[match[b]];
	}
}
void mark_path (int v, int b, int children) {
	while (base[v] != b) {
		blossom[base[v]] = blossom[base[match[v]]] = true;
		p[v] = children;
		children = match[v];
		v = p[match[v]];
	}
}
int find_path (int root) {
	memset (used, 0, sizeof used);
	memset (p, -1, sizeof p);
	for (int i=0; i<n; ++i)
		base[i] = i;

	used[root] = true;
	int qh=0, qt=0;
	q[qt++] = root;
	while (qh < qt) {
		int v = q[qh++];
		for (size_t i=0; i<g[v].size(); ++i) {
			int to = g[v][i];
			if (base[v] == base[to] || match[v] == to) {

                continue;
			}
			if (to == root || match[to] != -1 && p[match[to]] != -1) {
            //    cout << "v: " << v << " " << to << endl;
				int curbase = lca (v, to);
				memset (blossom, 0, sizeof blossom);
				mark_path (v, curbase, to);
				mark_path (to, curbase, v);
				for (int i=0; i<n; ++i)
					if (blossom[base[i]]) {
						base[i] = curbase;
						if (!used[i]) {
							used[i] = true;
							q[qt++] = i;
						}
					}
			}
			else if (p[to] == -1) {
			cout << "v: " << v << " " << match[to] << endl;
				p[to] = v;
				if (match[to] == -1)
					return to;
				to = match[to];
				used[to] = true;
				q[qt++] = to;
			}
		}
	}
	return -1;
}


int main() {
    int _size;
    FILE *inFile;
    inFile = fopen("input.txt", "rt");
    fscanf(inFile, "%d", &n);

    int a, b;
    while(!feof(inFile)) {
    //while(cin >> a >> b) {
        fscanf(inFile, "%d", &a);
        fscanf(inFile, "%d", &b);
        //cout << a << " " << b << endl;
        g[a-1].push_back(b-1);
        g[b-1].push_back(a-1);
        //g.g[a-1][b-1] = 1;
        //g.g[b-1][a-1] = 1;
    }


	memset (match, -1, sizeof match);
	for (int i=0; i<n; ++i)
		if (match[i] == -1) {
			int v = find_path (i);
			cout << v << "!!!\n";
			while (v != -1) {
				int pv = p[v],  ppv = match[pv];
				match[v] = pv,  match[pv] = v;
				v = ppv;
			}
		}

    int k = 0;
    for(int i = 0; i < n; ++i) {
        cout << match[i] << " ";
        if(match[i] != -1) {
            k++;
        }

    }
    cout  << k << endl;

    for(int i = 0; i < n; ++i) {
        if(match[i] != -1) {
            cout << i+1 << " " << match[i] + 1 << "\n";
            match[match[i]] = -1;
        }

    }
}











