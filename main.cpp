//
//  main.cpp
//  A Star Algo
//
//  Created by Александр Малышев on 16.05.15.
//  Copyright (c) 2015 SecurityQQ. All rights reserved.
//

#include <iostream>
#include <vector>
#include <queue>
#include <set>

struct Point;
class Field;

typedef unsigned long long ull;
typedef long long ll;

class Field {
public:
    
    Field(ull fieldSize);
    ~Field();
    void changeField(std::vector<ull> vecFormatedByRLE, ull rowNum);
    void printField();
    std::vector<std::vector<unsigned int>> field;
    bool isOnField(Point p);
    
private:
    
};

Field:: Field(ull fieldSize) {
    field.assign(fieldSize, std::vector<unsigned int>(fieldSize, 0));
}

Field:: ~Field() {
    
}

void Field:: changeField(std::vector<ull> vecFormatedByRLE, ull rowNum) {
    
    field[rowNum].assign(field.size(), 0);
    
    for (ull i = 0; i < vecFormatedByRLE.size(); i += 2) {
        if (i + 1 >= vecFormatedByRLE.size()) {
            //replacing to the end of vector:
            for (ull j = vecFormatedByRLE[i]; j < field.size(); ++j) {
                field[rowNum][j] = 1;
            }
            
        } else {
            
            for (ull j = vecFormatedByRLE[i]; j < vecFormatedByRLE[i + 1]; ++j) {
                field[rowNum][j] = 1;
            }
        }
    }
}

void Field:: printField() {
    for (auto i = field.begin(); i != field.end(); ++i) {
        for (auto j = i->begin(); j != i->end(); ++j) {
            if (*j != 0) {
                std::cout<<*j;
            } else {
                std::cout<<" ";
            }
        }
        std::cout<<std::endl;
    }
}

struct Point {
    ll x;
    ll y;
    ull minRoad;
    ull prior;
    
    ull eur(const Point point, const Point goal) { //euristic function
        return llabs(point.x - goal.x) + llabs(point.y - goal.y);
    }
    void setPriority(const Point goal) {
        prior = minRoad + eur(*this, goal);
    }

    Point(ll x, ll y) : x(x), y(y), minRoad(0) {}
    Point(ll x, ll y, ull minRoad, ull prior) : x(x), y(y), minRoad(minRoad), prior(prior) {}
    Point(ll x, ll y, Point prevPoint, Point goal) : x(x), y(y), minRoad(prevPoint.minRoad + 1) { setPriority(goal); }
    bool operator == (const Point p2) const {
        return (x == p2.x) && (y == p2.y);
    }
    bool operator != (const Point p2) const {
        return !((x == p2.x) && (y == p2.y));
    }
    
    void print() const{
        std::cout<<"point: x:"<<x <<" y:"<<y<<" minRoad:"<< minRoad << " prior:" <<prior <<std::endl;
    }
};

struct compPointsPrior{
    bool operator () (const Point &p1, const Point &p2) const{
        return p1.prior > p2.prior;
    }
};
struct compPoints{
    bool operator () (const Point &p1, const Point &p2) const{
        return p1.x == p2.x ? p1.y > p2.y : p1.x > p2.x;
    }
};

bool Field:: isOnField(Point p) {
    if (p.x < 0 || p.y < 0 || p.x >= field.size() || p.y >= field.size()) {
        return false;
    }
    if (field[p.x][p.y] == 1) {
        return false;
    }
    return true;
}

void addAdjPointsToQueue(Point point, Point goal, Field &field, std::priority_queue<Point, std::vector<Point>, compPointsPrior>& queue, std::set<Point, compPoints>& checked) {
    Point pT(point.x, point.y + 1, point, goal);
    Point pD(point.x, point.y - 1, point, goal);
    Point pR(point.x + 1, point.y, point, goal);
    Point pL(point.x - 1, point.y, point, goal);
    
    if (field.isOnField(pT)) {

        if (*checked.find(pT) != pT) {
            std::cout<<"pushing: "; pT.print();
            queue.push(pT);
        }
    }
    if (field.isOnField(pD)) {
        
        if (*checked.find(pD) != pD) {
            std::cout<<"pushing: "; pD.print();
            queue.push(pD);
        }
    }
    if (field.isOnField(pR)) {
        if (*checked.find(pR) != pR) {
            std::cout<<"pushing: "; pR.print();
            queue.push(pR);
        }
    }
    if (field.isOnField(pL)) {
        if (*checked.find(pL) != pL) {
            std::cout<<"pushing: "; pL.print();
            queue.push(pL);
        }
    }
}




ull findMinRoad(Field& field, Point start, Point goal) {
    std::set<Point, compPoints> checked;
    std::priority_queue<Point, std::vector<Point>, compPointsPrior> opened;
    start.minRoad = 0;
    start.setPriority(goal);
    
    opened.push(start);

    while (!opened.empty()) {
        
        Point x = opened.top();
        opened.pop();
        x.print();
        if (x == goal) {
            return x.minRoad;
        }
        checked.insert(x);
        addAdjPointsToQueue(x, goal, field, opened, checked);
        
    }
    
    return -1;
}

void debug() {
    Field myTestField(7);
    std::vector<ull> RLE0;
    RLE0.push_back(1);
    RLE0.push_back(4);
    
    
    std::vector<ull> RLE1;
    RLE1.push_back(3);
    RLE1.push_back(4);
    
    std::vector<ull> RLE2;
    RLE2.push_back(1);
    RLE2.push_back(2);
    RLE2.push_back(3);
    RLE2.push_back(4);
    
    std::vector<ull> RLE3;
    RLE3.push_back(1);
    RLE3.push_back(4);
    
    
    myTestField.changeField(RLE0, 1);
    myTestField.changeField(RLE1, 2);
    myTestField.changeField(RLE2, 3);
    myTestField.changeField(RLE3, 4);
    myTestField.printField();
    
    
    std::cout<<findMinRoad(myTestField, Point(5, 6), Point(3, 2));

}

int main(int argc, const char * argv[]) {
    debug();
    return 0;
}
