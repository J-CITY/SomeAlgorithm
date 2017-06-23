#include <iostream>
#include <vector>
#include <map>
#include <climits>
#include <cmath>
#include <iomanip>
#include <SFML/Graphics.hpp>

#define PI 3.14159265

using namespace std;

struct Val {
    int i, j, val;
    Val(int _i, int _j, int _val) : i(_i), j(_j), val(_val) {}
};


class Graph {
public:
    int cityCount = 0;
    vector<vector<int> > graph;
    vector<vector<int> > _graph;
    vector<Val> vals;
    vector<int> di;
    vector<int> dj;

    vector<pair<int, int>> path;

    Graph() {}

    void readGraph() {
        unsigned int n;
        FILE *inFile;
        //inFile = fopen("input0.txt", "rt");//180
        //inFile = fopen("input1.txt", "rt");//30

        //inFile = fopen("input2.txt", "rt");//2090
        //inFile = fopen("input3.txt", "rt");//871
        //inFile = fopen("input4.txt", "rt");//1055
        inFile = fopen("input5.txt", "rt");//254
        //inFile = fopen("input6.txt", "rt");//25
        //inFile = fopen("input7.txt", "rt");//65
        //inFile = fopen("input8.txt", "rt");//8
        //inFile = fopen("input9.txt", "rt");//10
        //inFile = fopen("input10.txt", "rt");//10
        fscanf(inFile, "%d", &n);

        cityCount = n;
        di.resize(n);
        dj.resize(n);
        graph.resize(n+1);
        for(auto i = 0; i < n+1; ++i) {
            graph[i].resize(n+1);
        }
        _graph.resize(n+1);
        for(auto i = 0; i < n+1; ++i) {
            _graph[i].resize(n+1);
        }

        int a;
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; ++j) {
                fscanf(inFile, "%d", &a);
                graph[i+1][j+1] = (a == -1 ? INT_MAX : a);
                _graph[i+1][j+1] = (a == -1 ? INT_MAX : a);
            }
        }
        for(int i = 0; i < n+1; ++i) {
            graph[0][i] = i;
        }
        for(int i = 0; i < n+1; ++i) {
            graph[i][0] = i;
        }
        for(int i = 0; i < n+1; ++i) {
            _graph[0][i] = i;
        }
        for(int i = 0; i < n+1; ++i) {
            _graph[i][0] = i;
        }
        fclose(inFile);
    }

    bool getDI() {
        for(auto i = 1; i < graph.size(); ++i) {
            int val = INT_MAX;
            for(auto j = 1; j < graph[i].size(); ++j) {
                if(graph[i][j] < val) {
                    val = graph[i][j];
                }
            }
            di[i-1] = val;
            if(val == INT_MAX)
                return false;
        }
        return true;
    }
    void subDI() {
        for(auto i = 1; i < graph.size(); ++i) {
            for(auto j = 1; j < graph[i].size(); ++j) {
                if(graph[i][j] < INT_MAX) {
                    graph[i][j] -= di[i-1];
                }
            }
        }
    }

    bool getDJ() {
        for(auto j = 1; j < graph[0].size(); ++j) {
            int val = INT_MAX;
            for(auto i = 1; i < graph.size(); ++i) {
                if(graph[i][j] < val) {
                    val = graph[i][j];
                }
            }
            dj[j-1] = val;
            if(val == INT_MAX)
                return false;
        }
        return true;
    }

    void subDJ() {
        for(auto j = 1; j < graph[0].size(); ++j) {
            for(auto i = 1; i < graph.size(); ++i) {
                if(graph[i][j] < INT_MAX) {
                    graph[i][j] -= dj[j-1];
                }
            }
        }
    }

    int getValIJ(int _i, int _j) {
        int minI = INT_MAX;
        int minJ = INT_MAX;
        for(auto i = 1; i < graph.size(); ++i) {
            if(graph[_i][i] < minI && i != _j) {
                minI = graph[_i][i];
            }
        }
        for(auto i = 1; i < graph.size(); ++i) {
            if(graph[i][_j] < minJ && i != _i) {
                minJ = graph[i][_j];
            }
        }
        if(minI == INT_MAX)
            minI = 0;
        if(minJ == INT_MAX)
            minJ = 0;

        return minI + minJ;
    }

    void getVal() {
        //cout << graph.size() <<"\n";
        for(auto i = 1; i < graph.size(); ++i) {
            for(auto j = 1; j < graph[i].size(); ++j) {
                if(graph[i][j] == 0) {
                    //    cout << i << " " << j << "\n";
                    int val = getValIJ(i, j);
                    vals.push_back(Val(i, j, val));
                    //graph[i][j] = val;
                }
            }
        }
    }

    Val getMaxVal() {
        int _i, val = 0;
        for(auto i = 0; i < vals.size(); ++i) {
            if(vals[i].val > val) {
                //bool b = true;
                //for(int j = 0; j < badedge.size(); j++) {
                //    if(vals[i].i == badedge[j].first && vals[i].j == badedge[j].second) {
                //        b = false;
                //    }
                //}
                //if(b) {
                    _i = i;
                    val = vals[i].val;
                //}
            }
        }
        return vals[_i];
    }

    void removeFromGraph(int _i, int _j) {
        graph.erase(graph.begin() + _i);
        for(auto i = 0; i < graph.size(); ++i) {
            graph[i].erase(graph[i].begin()+_j);
        }
    }
//    map<pair<int, int>> badedge;
    void changeGraph(int x1, int x2) {
        int i = 0;
        while(i < path.size()) {
            if(x2 == path[i].first) {
                //graph[v.i][0], graph[0][v.j]
                //cout << x1 << " " << x2 << " : " << path[i].first << " " << path[i].second << "\n";
                int xrem = 0, yrem = 0;
                for(int j = 0; j < graph.size(); ++j) {
                    if(graph[j][0] == path[i].second) {
                        xrem = j;
                        break;
                    }
                }
                for(int j = 0; j < graph.size(); ++j) {
                    if(graph[0][j] == x1) {
                        yrem = j;
                        break;
                    }
                }
                //badedge.push_back(make_pair(xrem, yrem));
                if(xrem != 0 && yrem != 0) {
                    cout << "@@@@@::: " << xrem << yrem << "\n";
                    graph[xrem][yrem] = INT_MAX;
                }
                x2 =  path[i].second;
                i = 0;
                continue;
            }
            i++;
        }
        i = 0;
        while(i < path.size()) {
            if(x1 == path[i].second) {
                //graph[v.i][0], graph[0][v.j]
                cout << x1 << " " << x2 << " : " << path[i].first << " " << path[i].second << "\n";
                int xrem = 0, yrem = 0;
                for(int j = 0; j < graph.size(); ++j) {
                    if(graph[j][0] == x2) {
                        xrem = j;
                        break;
                    }
                }
                for(int j = 0; j < graph.size(); ++j) {
                    if(graph[0][j] == path[i].first) {
                        yrem = j;
                        break;
                    }
                }
                //badedge.push_back(make_pair(xrem, yrem));
                if(xrem != 0 && yrem != 0) {
                    cout << xrem << " " << yrem << "\n";
                    graph[xrem][yrem] = INT_MAX;
                }
                x1 =  path[i].first;
                i = 0;
                continue;
            }
            i++;
        }
    }

    bool findPath() {
        while(graph.size() > 3) {
            printGraph();
            if(!getDI())
                return false;
            printDI();
            subDI();
            //printGraph();
            if(!getDJ())
                return false;
            printDJ();
            subDJ();
            printGraph();
            getVal();
            printVals();
            Val v = getMaxVal();
            cout << "Add: " << graph[v.i][0] << " " << graph[0][v.j] << "\n";
            changeGraph(graph[v.i][0], graph[0][v.j]);
            path.push_back(make_pair(graph[v.i][0], graph[0][v.j]));
            int _i = -1;
            int _j = -1;
            for(int i = 1; i < graph.size(); ++i) {
                if(graph[0][i] == graph[v.i][0]) {
                    _j = i;
                    break;
                }
            }
            for(int i = 1; i < graph.size(); ++i) {
                if(graph[i][0] == graph[0][v.j]) {
                    _i = i;
                    break;
                }
            }
            if(_i != -1 && _j != -1) {
                graph[_i][_j] = INT_MAX;
            }



            removeFromGraph(v.i, v.j);
            printGraph();
            cout << "_________________\n";
            vals.clear();
            di.resize(di.size()-1);
            dj.resize(dj.size()-1);
            //printPath();
        }
        if(graph[1][0] != graph[0][1] && graph[1][1] != INT_MAX) {
            path.push_back(make_pair(graph[1][0], graph[0][1]));
            path.push_back(make_pair(graph[2][0], graph[0][2]));
        } else {
            path.push_back(make_pair(graph[1][0], graph[0][2]));
            path.push_back(make_pair(graph[2][0], graph[0][1]));
        }
        //path.push_back(make_pair(graph[1][0], graph[0][1]));
        cout << "PATH:\n";
        printPath();
        return false;
    }

    void printGraph() {
        for(auto i = 0; i < graph.size(); ++i) {
            for(auto j = 0; j < graph.size(); ++j) {
                if(graph[i][j] == INT_MAX)
                    cout << setw(6) << "-1 ";
                else
                    cout << setw(6) << graph[i][j] << " ";
            }
            cout << "\n";
        }
    }
    void printDI() {
        cout << "DI: \n";
        for(auto i = 0; i < di.size(); ++i) {
            cout << di[i] << " ";
        }
        cout << "\n";
    }
    void printDJ() {
        cout << "DJ: \n";
        for(auto i = 0; i < dj.size(); ++i) {
            cout << dj[i] << " ";
        }
        cout << "\n";
    }
    void printVals() {
        cout << "Vals: \n";
        for(auto i = 0; i < vals.size(); ++i) {
            cout << graph[vals[i].i][0] << " " << graph[0][vals[i].j] << " " << vals[i].val << "\n";
        }
    }
    void printPath() {
        int price = 0;
        cout << "Path: \n";
        for(auto i = 0; i < path.size(); ++i) {
            cout << path[i].first << " " << path[i].second << "\n";
            price += _graph[path[i].first][path[i].second];
        }
        cout << price << "\n";
    }

    float radius = 10;
    float globalRadius = 200;
    float winW = 500, winH = 500;
    float  x0 = winW / 2, y0 = winH / 2;
    vector<sf::CircleShape> shapes;
    sf::Text text;
    sf::Font font;

    bool showPrice = true;
    bool showAllWay = true;

    void update() {
        float alpha = 0;
        float alphaStep = 360 / cityCount;
        x0 = winW / 2;
        y0 = winH / 2;
        double x, y;
        for(int i = 0; i < shapes.size(); ++i) {
            shapes[i].setRadius(radius);
            shapes[i].setFillColor(sf::Color::Green);
            x = (float) (x0 + globalRadius * sin(alpha * PI / 180));
            y = (float) (y0 + globalRadius * cos(alpha * PI / 180));
            shapes[i].setPosition(x, y);
            alpha += alphaStep;
        }
    }

    void visualGraph() {
        sf::RenderWindow window(sf::VideoMode(winW, winH), "TSP");
        font.loadFromFile("arial.ttf");
        float alpha = 0;
        float alphaStep = 360 / cityCount;
        text.setFont(font);
        text.setCharacterSize(24);


        double x, y;
        for(int i = 0; i < cityCount; ++i) {
            shapes.push_back(sf::CircleShape());
            shapes[shapes.size()-1].setRadius(radius);
            shapes[shapes.size()-1].setFillColor(sf::Color::Green);
            x = (float) (x0 + globalRadius * sin(alpha * PI / 180));
            y = (float) (y0 + globalRadius * cos(alpha * PI / 180));
            shapes[shapes.size()-1].setPosition(x, y);
            alpha += alphaStep;
        }


        while (window.isOpen()) {
            sf::Event event;
            while(window.pollEvent(event)) {
                if(event.type == sf::Event::Closed)
                    window.close();
                if(event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up) {
                        if(radius < 100) {
                            radius += 10;
                            update();
                        }
                    }
                    if (event.key.code == sf::Keyboard::Down) {
                        if(radius > 0) {
                            radius -= 10;
                            update();
                        }
                    }
                    if (event.key.code == sf::Keyboard::F1) {
                        showPrice = !showPrice;
                    }
                    if (event.key.code == sf::Keyboard::F2) {
                        showAllWay = !showAllWay;
                    }
                }
            }
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                float mouseX = sf::Mouse::getPosition(window).x;
                float mouseY = sf::Mouse::getPosition(window).y;
                //cout << mouseX << " " << mouseY << "\n";
                for(int i = 0; i < shapes.size(); ++i) {
                    sf::FloatRect rect(shapes[i].getPosition().x, shapes[i].getPosition().y, radius*2, radius*2);
                    if(rect.contains(mouseX, mouseY)) {
                        shapes[i].setPosition(mouseX-radius, mouseY-radius);
                        break;
                    }
                }
            }
            //shape.setPosition(x, y);
            int movline = 0;
            window.clear();
            for(int i = 1; i < _graph.size(); ++i) {
                for(int j = 1; j < _graph.size(); ++j) {
                    if(_graph[i][j] != INT_MAX) {
                        int mov = 0;
                        float x, y;

                        if(i < j) {
                            movline = 3;
                            mov = 15;
                            x = (shapes[i-1].getPosition().x + shapes[j-1].getPosition().x)/2 + mov;
                            y = (shapes[i-1].getPosition().y + shapes[j-1].getPosition().y)/2 + mov;
                            text.setColor(sf::Color::Yellow);
                            sf::Vertex line[] = {
                                sf::Vertex(sf::Vector2f(shapes[i-1].getPosition().x+radius+movline,
                                            shapes[i-1].getPosition().y+radius+movline), sf::Color::Yellow),
                                sf::Vertex(sf::Vector2f(shapes[j-1].getPosition().x+radius+movline,
                                            shapes[j-1].getPosition().y+radius+movline), sf::Color::Yellow)
                            };
                            if(showAllWay)
                                window.draw(line, 2, sf::Lines);
                        } else {
                            movline = -3;
                            mov = -15;
                            x = (shapes[i-1].getPosition().x + shapes[j-1].getPosition().x)/2 + mov;
                            y = (shapes[i-1].getPosition().y + shapes[j-1].getPosition().y)/2 + mov;
                            text.setColor(sf::Color::Blue);
                            sf::Vertex line[] = {
                                sf::Vertex(sf::Vector2f(shapes[i-1].getPosition().x+radius+movline,
                                            shapes[i-1].getPosition().y+radius+movline), sf::Color::Blue),
                                sf::Vertex(sf::Vector2f(shapes[j-1].getPosition().x+radius+movline,
                                            shapes[j-1].getPosition().y+radius+movline), sf::Color::Blue)
                            };
                            if(showAllWay)
                                window.draw(line, 2, sf::Lines);
                        }

                        //cout << x << " " << y << "\n";
                        //text.setColor(sf::Color::White);
                        if(showPrice) {
                            text.setPosition(x, y);
                            text.setString(to_string(_graph[i][j]));
                        }
                        window.draw(text);
                    }
                }
            }
            for(int i = 0; i < path.size(); ++i) {
                //movline = (path[i].first < path[i].second) ? 2 : -2;
                if(path[i].first < path[i].second) {
                    movline = 3;
                } else {
                    movline = -3;
                }
                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(shapes[path[i].first-1].getPosition().x+radius+movline,
                                            shapes[path[i].first-1].getPosition().y+radius+movline), sf::Color::Red),
                    sf::Vertex(sf::Vector2f(shapes[path[i].second-1].getPosition().x+radius+movline,
                                            shapes[path[i].second-1].getPosition().y+radius+movline), sf::Color::Red)
                };
                window.draw(line, 2, sf::Lines);

            }
            for(int i = 0; i < shapes.size(); ++i) {
                window.draw(shapes[i]);
                text.setColor(sf::Color::White);
                text.setPosition(shapes[i].getPosition().x+radius, shapes[i].getPosition().y+radius);
                text.setString(to_string(i+1));
                window.draw(text);
            }
            for(int i = 0; i < path.size(); ++i) {
                if(path[i].first < path[i].second) {
                    movline = 3;
                } else {
                    movline = -3;
                }
                sf::CircleShape arr(5, 4);
                arr.setFillColor(sf::Color::Red);
                arr.setPosition(shapes[path[i].second-1].getPosition().x+radius+movline,
                                shapes[path[i].second-1].getPosition().y+radius+movline);
                window.draw(arr);

            }
            window.display();
        }

    }

};

Graph g;

int main() {
    g.readGraph();
    g.findPath();
    g.visualGraph();
    return 0;
}
