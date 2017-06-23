#include <iostream>
#include <vector>
#include <map>
#include <climits>
#include <cmath>
#include <iomanip>

#define PI 3.14159265

using namespace std;

class Graph {
public:
    long long int cityCount = 0;

    vector<vector<long long int>> g;

    vector<long long int> way;
    vector<vector<long long int>> dp;
    long long int n2 = 0;
    long long int ans = 0;

    void readGraph() {
        unsigned int n;
        FILE *inFile;
        //inFile = fopen("input0.txt", "rt");//180
        //inFile = fopen("input1.txt", "rt");//30
        //inFile = fopen("input5.txt", "rt");//253
        //inFile = fopen("input6.txt", "rt");//25
        //inFile = fopen("input7.txt", "rt");//62
        //inFile = fopen("input8.txt", "rt");//8
        //inFile = fopen("input9.txt", "rt");//10
        //inFile = fopen("input10.txt", "rt");//10
        fscanf(inFile, "%d", &n);

        cityCount = n;
        g.resize(n);
        for(auto i = 0; i < n; ++i) {
            g[i].resize(n);
        }

        int a;
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; ++j) {
                fscanf(inFile, "%d", &a);
                g[i][j] = (a == -1 ? INT_MAX : a);
            }
        }
        fclose(inFile);
        dp.resize(n);
        for(int i = 0; i < dp.size(); ++i) {
            dp[i].resize(1<<n);
        }
    }

    int _find(long int mask, long int i) {
        if(dp[i][mask] != INT_MAX) {
            return dp[i][mask];
        }
        for(long long int j = 0; j < cityCount; ++j) {
            if(g[i][j] != INT_MAX && (mask & (1 << (j)))) {
                dp[i][mask] = min(dp[i][mask], _find(mask - (1<<j), j) + g[i][j]);
            }
        }
        return dp[i][mask];
    }

    void findPath() {

        n2 = 1 << cityCount;
        for(long long int i = 0; i < cityCount; ++i) {
            for(long long int j = 0; j < n2; ++j) {
                dp[i][j] = INT_MAX;
            }
        }
        dp[0][0] = 0;
        ans = _find(n2-1, 0);
        findWay();
        cout << ans << endl;
        for(int i = 0; i < way.size(); ++i) {
            cout << way[i]+1 << " ";
        }
        cout << endl;
    }

    void findWay() {
        long long int i = 0;
        long long int mask = (1<<cityCount)-1;
        way.push_back(0);
        while(mask != 0) {
            for(int j = 0; j < cityCount; ++j) {
                if(g[i][j] != INT_MAX && (mask&1<<j) && dp[i][mask] == dp[j][mask- (1<<j)] + g[i][j]) {
                    way.push_back(j);
                    i = j;
                    mask -= (1<<j);
                    continue;
                }
            }
        }
    }
};

int main() {
    Graph g;
    g.readGraph();
    g.findPath();
    //cout << "Hello world!" << endl;
    return 0;
}
