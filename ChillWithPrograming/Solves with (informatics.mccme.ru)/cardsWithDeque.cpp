#include <iostream>
#include <fstream>
#include <deque>
#include <string>

using namespace std;

int main() {
    ifstream fin;
    ofstream fout;
    fin.open("input.txt");
    fout.open("output.txt");

    string command;
    deque<string> cards;

    while (fin >> command) {
        switch (command[0]) {
            case '^':
                cards.pop_back();
                break;
            
            case '/':
                cards.pop_front();
                break;

            case '+':
                command.erase(0, 1);
                cards.push_back(command);
                break;

            case '#':
                command.erase(0, 1);
                cards.push_front(command);
                break;
        }
    }
    
    for (auto it = cards.end() - 1; it >= cards.begin(); it--) {
        fout << *it << " ";
    }
    return 0;
}
