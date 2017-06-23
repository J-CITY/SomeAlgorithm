#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <fstream>
#include <stdlib.h>

using namespace std;

const int INF = 1000000000;
ofstream fout;

class Edge {
public:
	int nextVertex;
	int invEdge;
	int flow;
	int maxFlow;
	int cost;
	int num;

	bool ifBlock = false;
	int numBackEdge = -1;

	Edge(int nextVertex, int invEdge, int maxFlow, int cost, int num):
                                                            nextVertex(nextVertex),
                                                            invEdge(invEdge),
                                                            flow(0),
                                                            maxFlow(maxFlow),
                                                            cost(cost),
                                                            num(num) {}
};

class Graph {
public:
    //double avTime = 0;
    int k;
	int n;
	vector<vector<Edge> > vertices;
	vector<int> potentials;
	vector<int> distances;
	vector<bool> flags;
	vector<int> resultWay;

	//vector<vector<int>> ways;
    vector<int> ways;


	Graph(int n, int k) :
                        n(n),
                        vertices(n),
                        potentials(n),
                        distances(n),
                        flags(n),
                        resultWay(n),

                        //ways(k),
                        //costs(k),
                        k(k) {}

	void AddEdge(int _a, int _b, int _flow, int _cost, int _num) {//add edge and invers edge
		//int _va = vertices[_a].size();
		//int _vb = vertices[_b].size();
		vertices[_a].insert(vertices[_a].end(), Edge(_b, vertices[_b].size(), _flow, _cost, _num));
		//vertices[_a][vertices[_a].size()-1].numBackEdge = _vb+1;
		vertices[_b].insert(vertices[_b].end(), Edge(_a, vertices[_a].size() - 1, 0, -_cost, _num));
		//vertices[_b][vertices[_b].size()-1].numBackEdge = _va+1;

		vertices[_b].insert(vertices[_b].end(), Edge(_a, vertices[_a].size(), _flow, _cost, _num));
		//vertices[_b][vertices[_b].size()-1].numBackEdge = _va;
		vertices[_a].insert(vertices[_a].end(), Edge(_b, vertices[_b].size() - 1, 0, -_cost, _num));
		//vertices[_a][vertices[_a].size()-1].numBackEdge = _vb;
	}


	void print() {
        for(int i = 0; i < vertices.size(); ++i) {
            cout << i << " :\n";
            for(int j = 0; j < vertices[i].size(); ++j) {
                cout << i << " " << vertices[i][j].nextVertex << " " << vertices[i][j].invEdge << " "
                << vertices[i][j].numBackEdge << " b " << vertices[i][j].ifBlock << " f " << vertices[i][j].flow << endl;
            }
        }
	}

	void dfs(int v) {
	    if(v == n-1) {
            return;
	    }
        for(int i = 0; i < vertices[v].size(); ++i) {
            if(vertices[v][i].flow > 0 && vertices[v][i].num > 0) {
                vertices[v][i].flow--;
                ways.push_back(vertices[v][i].num);
                //cout << vertices[v][i].num << " ";
                dfs(vertices[v][i].nextVertex);
                break;
            }
        }
	}

	pair<double, double> Jhonson(int source, int sink) {
	    fout.open("brides.out");
	    //fout << "              \n";

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
		double resultFlow = 0, resultCost = 0;
        int _k = 0;
		while(true) {
			fill(flags.begin(), flags.end(), 0);
			fill(distances.begin(), distances.end(), INF);
			distances[source] = 0;

			// цикл алгоритма Дейкстры
			while(true) {
				int i = -1;
				for(int j = 0; j < n; ++j) {
					if(!flags[j] && distances[j] < INF && (i < 0 || distances[i] > distances[j])) {
						i = j;
					}
				}
				if(i < 0) {
                    break;
				}
				flags[i] = 1;

				for(int j = 0; j < vertices[i].size(); ++j) {
					if(vertices[i][j].maxFlow > vertices[i][j].flow) {
						int a = vertices[i][j].nextVertex;
						if(!flags[a] && distances[a] > distances[i] + vertices[i][j].cost + potentials[i] - potentials[a] && !vertices[i][j].ifBlock) {
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
				//potentials[i] += distances[i];
			}
			int augFlow = INF, augCost = 0;
			for(int i = sink; i != source; ) {
				int a = vertices[i][resultWay[i]].nextVertex;
				int b = vertices[i][resultWay[i]].invEdge;
				augFlow = min(augFlow, vertices[a][b].maxFlow - vertices[a][b].flow);
                augCost += vertices[a][b].cost;
                i = a;
			}
            //cout << "CITY:" << 5 << endl;
            //ways.clear();
			for(int i = sink; i != source; ) {
				int a = vertices[i][resultWay[i]].nextVertex;
				int b = vertices[i][resultWay[i]].invEdge;
				//cout << "CITY:" << a+1 << " No WAY: " << vertices[i][resultWay[i]].num << " \n";
				//cout << _k << " "  << endl;
				//ways[_k].push_back(vertices[i][resultWay[i]].num);
				//ways.push_back(vertices[i][resultWay[i]].num);
				vertices[a][b].flow += augFlow;
				/*if(vertices[a][b].flow == 0) {
                    vertices[a][b].ifBlock = false;
                    vertices[vertices[a][b].nextVertex][vertices[a][b].numBackEdge].ifBlock = false;
                } else {
                    vertices[a][b].ifBlock = true;
                    vertices[vertices[a][b].nextVertex][vertices[a][b].numBackEdge].ifBlock = true;
                }*/
				vertices[i][resultWay[i]].flow -= augFlow;

				//vertices[vertices[a][b].nextVertex][vertices[a][b].numBackEdge].flow += augFlow;
				//vertices[vertices[i][resultWay[i]].nextVertex][vertices[i][resultWay[i]].numBackEdge].flow -= augFlow;

				/*if(vertices[i][resultWay[i]].flow == 0) {
                    vertices[i][resultWay[i]].ifBlock = false;
                    vertices[vertices[i][resultWay[i]].nextVertex][vertices[i][resultWay[i]].numBackEdge].ifBlock = false;
				} else {
                    vertices[i][resultWay[i]].ifBlock = true;
                    vertices[vertices[i][resultWay[i]].nextVertex][vertices[i][resultWay[i]].numBackEdge].ifBlock = true;
				}*/




				i = a;
			}
			/*cout << ways.size() << " ";
			fout << ways.size() << " ";
			for(int j = ways.size()-1; j >= 0; j--) {
                fout << ways[j] << " ";
                cout << ways[j] << " ";
            }
            cout << endl;
            fout << endl;*/

			//costs[_k] = augFlow*augCost;
            //cout << "FLOW: " << augFlow << " COST: " << augFlow*augCost << endl;
            //cout << "\n";
            resultFlow += augFlow;
			resultCost += augFlow * augCost;
			if(_k < k)
                _k++;
			if(k == _k) {
                //double avTime;
                //for(int l = 0; l < costs.size(); ++l) {
                    //avTime += costs[l];
                    //cout << costs[l] << endl;
                //}
                //cout << "K: " << k << endl;
                //avTime /= k;
                //cout << avTime;
                //fout.close();
                //cout << resultCost/k;

                fout.setf(std::ios_base::fixed,std::ios_base::floatfield);
                fout.precision(5);
                //cout << fixed << setprecision(5) << p.second << endl;
                fout << fixed << resultCost/k << "\n";
                for(int ii = 0; ii < k; ++ii) {
                    dfs(0);
                    //cout << ways.size() << "\n";
                    fout << ways.size() << " ";
                    for(int jj = 0; jj < ways.size(); ++jj) {
                        //cout << ways[jj] << " ";
                        fout << ways[jj] << " ";
                    }
                    ways.clear();
                    //cout << "\n";
                    fout << "\n";
                }
                fout.close();
                return make_pair(resultFlow, resultCost/k);
			}
		}
        if(_k < k) {
            k = -1;
            cout << -1 << "\n";
            fout << -1;
        }
        //for(int l = 0; l < costs.size(); ++l) {
        //    avTime += costs[l];
        //}
        //avTime /= k;
        fout.close();
		return make_pair(resultFlow, resultCost);
	}
};

int main() {
    unsigned int n, m, k;
    FILE *inFile;
    inFile = fopen("brides.in", "rt");
    fscanf(inFile, "%d", &n);
    fscanf(inFile, "%d", &m);
    fscanf(inFile, "%d", &k);
    Graph g(n, k);
    //g.k = k;
    int a, b, c;
    for(int i = 0; i < m; i++) {
        fscanf(inFile, "%d", &a);
        fscanf(inFile, "%d", &b);
        fscanf(inFile, "%d", &c);
        g.AddEdge(a-1, b-1, 1, c, i+1);
    }
    fclose(inFile);

    //g.print();

    pair<double, double> p = g.Jhonson(0, n-1);
    cout << "PAIR: " << p.first << " " << p.second << endl;

    //g.print();
    //fout.open("brides.out");
    //fout.seekp(ios::beg);
    /*if(g.k == -1) {
        cout << -1 << "\n";
        fout << -1;
    } else {
        fout.setf(std::ios_base::fixed,std::ios_base::floatfield);
        fout.precision(5);
        //cout << fixed << setprecision(5) << p.second << endl;
        fout << fixed << p.second << "\n";
        for(int i = g.k-1; i >=0; i--) {
            fout << g.ways[i].size() << " ";
            //cout << g.ways[i].size() << " ";
            for(int j = g.ways[i].size()-1; j >= 0; j--) {
                fout << g.ways[i][j] << " ";
                //cout << g.ways[i][j] << " ";
            }
            fout << "\n";
            //cout << endl;
        }
    }
    fout.close();*/
    return 0;
}
