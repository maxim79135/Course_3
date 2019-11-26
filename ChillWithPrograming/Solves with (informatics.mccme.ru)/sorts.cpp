#include <iostream>
#include <deque>
#include <algorithm>
#include <string>

using namespace std;

struct RailWay {
    int type;
    string number;
};

bool comp(RailWay r1, RailWay r2) {
    return r1.type < r2.type;
}

int main() {
    int n;
    cin >> n;
    
    RailWay railway;
    deque<RailWay> railways;

    for (int i = 0; i < n; i++) {
        cin >> railway.type >> railway.number;
        railways.push_back(railway);
    }
    sort(railways.begin(), railways.end(), comp);

    for (auto it: railways) {
        cout << it.type << " " << it.number << endl;
    }
}