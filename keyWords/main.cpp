#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <fstream>

using namespace std;

const int INF = 1000000000;

vector<string> words;
ofstream fout;

class Edge {
public:
	int nextVertex;
	int invEdge;
	int flow;
	int maxFlow;
	int cost;

	Edge(int nextVertex, int invEdge, int maxFlow, int cost):
                                                            nextVertex(nextVertex),
                                                            invEdge(invEdge),
                                                            flow(0),
                                                            maxFlow(maxFlow),
                                                            cost(cost) {}
};

class Graph {
public:
    double avTime = 0;
	int n;
	vector<vector<Edge> > vertices;
	vector<int> potentials;
	vector<int> distances;
	vector<int> flags;
	vector<int> resultWay;

	vector<int> costs;
	vector<pair<int, int>> ways;

	int wordSize = 0;

	Graph(int n) :
                        n(n),
                        vertices(n),
                        potentials(n),
                        distances(n),
                        flags(n),
                        resultWay(n),
                        costs(n) {}



	void AddEdge(int _a, int _b, int _flow, int _cost) {//add edge and invers edge
		vertices[_a].insert(vertices[_a].end(), Edge(_b, vertices[_b].size(), _flow, _cost));
		vertices[_b].insert(vertices[_b].end(), Edge(_a, vertices[_a].size() - 1, 0, -_cost));
	}

	void print() {
        for(int i = 0; i < vertices.size(); ++i) {
            cout << i << " :\n";
            for(int j = 0; j < vertices[i].size(); ++j) {
                if(vertices[i][j].cost > 0)
                cout << i << " " << vertices[i][j].nextVertex << " " << vertices[i][j].maxFlow << " " << vertices[i][j].cost << endl;
            }
        }
	}

	pair<int, int> Jhonson(int source, int sink) {
        fill(potentials.begin(), potentials.end(), INF);
        potentials[source] = 0;

        for(int i = 0; i < n-1; ++i) {
            for(int j = 0; j < n; ++j) {
                if(potentials[j] < INF) {
                    for(int k = 0; k < vertices[j].size(); ++k) {
                        // если ребро - прямое, и позволяет улучшить оценку расстояния до вершины на ее конце
                        if(vertices[j][k].maxFlow > 0 &&
                           potentials[vertices[j][k].nextVertex] > potentials[j] + vertices[j][k].cost) {
                            potentials[vertices[j][k].nextVertex] = potentials[j] + vertices[j][k].cost;
                        }
                    }
                }
            }
        }

        ////TURE

		int resultFlow = 0, resultCost = 0;
        int _k = 0;
		while(true) {
			fill(flags.begin(), flags.end(), 0);
			fill(distances.begin(), distances.end(), INF);
			distances[source] = 0;

			// цикл алгоритма Дейкстры
			while(true) {
				// найти непомеченную вершину с наименьшим расстоянием до истока
				int i = -1;
				for(int j = 0; j < n; ++j) {
					if(!flags[j] && distances[j] < INF && (i < 0 || distances[i] > distances[j])) {
						i = j;
					}
				}
				if(i < 0) {
                    break;
				}
				// помечаем найденную вершину
				flags[i] = 1;

				for(int j = 0; j < vertices[i].size(); ++j) {
					if(vertices[i][j].maxFlow > vertices[i][j].flow) {
						int a = vertices[i][j].nextVertex;
						if(!flags[a] && distances[a] > distances[i] + vertices[i][j].cost + potentials[i] - potentials[a]) {
							distances[a] = distances[i] + vertices[i][j].cost + potentials[i] - potentials[a];
							resultWay[a] = vertices[i][j].invEdge;
						}
					}
				}
			}

			if(!flags[sink]) {
                break;
			}

			for(int i = 0; i < n; ++i) {
				potentials[i] += flags[i] ? distances[i] : distances[sink];
			}
			int augFlow = INF, augCost = 0;
			for(int i = sink; i != source; ) {
				int a = vertices[i][resultWay[i]].nextVertex;
				int b = vertices[i][resultWay[i]].invEdge;
				augFlow = min(augFlow, vertices[a][b].maxFlow - vertices[a][b].flow);
                augCost += vertices[a][b].cost;
                i = a;

			}
            //cout << "\n";
            //cout << "Out\n" << endl;
			for(int i = sink; i != source; ) {
				int a = vertices[i][resultWay[i]].nextVertex;
				int b = vertices[i][resultWay[i]].invEdge;
				vertices[a][b].flow += augFlow;
				vertices[i][resultWay[i]].flow -= augFlow;
				//cout << vertices[i][resultWay[i]].nextVertex << " \n";
				i = a;
			}


            resultFlow += augFlow;
			resultCost += augFlow * augCost;
		}

		/*for(int ii = 0; ii < wordSize; ++ii) {
		    if(vertices[27+ii].size() == 2) {
                int val = vertices[27+ii][0].cost;
                for(int jj = 0; jj < words[ii].size(); ++jj) {
                    if(jj == val-1) {
                        cout << "&";
                    }
                    cout << words[ii][jj];
                }
                cout << "\n";

		    } else {
                cout << words[ii] << "\n";
            }

		}*/
		for(int ii = 0; ii < wordSize; ++ii) {
            dfs(0);
            int wres = ways[1].first-27;
            //cout << "!!!!! " << wres;
            if(ways.size() == 3) {
                words[wres].insert(ways[1].second-1, "&");
                /*for(int jj = 0; jj < words[wres].size(); ++jj) {
                    if(jj == ways[1].second-1) {
                        cout << "&";
                    }
                    cout << words[wres][jj];
                }
                cout << "\n";*/
                //cout << ways[1] << "\n"
            } else {
                //cout << words[wres] << "\n";
            }
            ways.clear();
        }
        fout.open("output.txt");
        for(int ii = 0; ii < wordSize; ++ii) {
            //cout << words[ii] << "\n";
            fout << words[ii] << "\n";
        }
        fout.close();
		return make_pair(resultFlow, resultCost);
	}

	void dfs(int v) {
	    if(v == n-1) {
            return;
	    }
        for(int i = 0; i < vertices[v].size(); ++i) {
            if(vertices[v][i].flow > 0) {
                vertices[v][i].flow--;
                ways.push_back(make_pair(vertices[v][i].nextVertex, vertices[v][i].cost));
                //cout << vertices[v][i].num << " ";
                dfs(vertices[v][i].nextVertex);
                break;
            }
        }
	}

};

int main() {
    unsigned int _size=0;

    ifstream infile;
    infile.open("input.txt");
    infile >> _size;
    //cout << _size << endl;
    words.resize(_size);
    getline(infile, words[0]);
    for(int i = 0; i < _size; ++i) {
        getline(infile, words[i]);
        //cout << words[i] << "\n";
    }
    //cout << "\n";

    Graph g(1 + 26 + _size + 1);//start+ letters + words + end
    g.wordSize = _size;
    for(int i = 0; i < _size; i++) {
        for(int j = 0; j < words[i].size(); j ++) {
            g.AddEdge(1+words[i][j]-97, 27+i, 1, j+1);
        }
    }
    for(int i = 0; i < 26; i++) {
        g.AddEdge(0, 1+i, 1, 1);
    }
    for(int i = 0; i < words.size(); i++) {
        g.AddEdge(27+i, g.n-1, 1, 1);
    }
    //g.print();
    pair<double, double> p = g.Jhonson(0, 1 + 26 + _size);
    //cout << "PAIR: " << p.first << " " << p.second << endl;



    return 0;
}
