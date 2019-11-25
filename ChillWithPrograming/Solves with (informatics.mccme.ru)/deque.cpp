#include <iostream>
#include <deque>

using namespace std;

int main() {
    int n;
    cin >> n;

    //key
    int k;
    cin >> k;

    //init deque
    deque<int> persons;
    for (int i = 1; i <= n; i++) {
        persons.push_back(i);
    }
    
    while (!persons.empty()) {
        int m = 1;
        while (m++ < k) {
            int temp = persons.front();
            persons.pop_front();
            persons.push_back(temp);
        }
        
        cout << persons.front() << " ";
        persons.pop_front();
    }
    return 0;
}