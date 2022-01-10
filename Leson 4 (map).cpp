#include <string>
#include <vector>
#include <iostream>
#include <map>

using namespace std;
/// Git test
map<string, int> CreateCountedMap(const vector<string>& word) {
    map<string, int> countedMap;
    for (string finder : word) {
        if (countedMap.count(finder) == 0) {
            countedMap[finder] = 1;
        }
        else {
            ++countedMap[finder];
        }
    }
    return countedMap;
}

string FindWidespreadBird(const vector<string>& types) {
    map<string, int> listOfBird = CreateCountedMap(types);
    int currentMax = 0;
    string widespreadBird;
   
   // int value;
    for (auto finder : listOfBird) {
        if (finder.second > currentMax) {
            widespreadBird = finder.first;
            currentMax = finder.second;
        }
    }
   
    return widespreadBird;
    // напишите здесь ваше решение
}

int main() {
    vector<string> bird_types1 = { "zyablik"s, "sinica"s, "vorobey"s, "zyablik"s, "sinica"s, "sinica"s };
    if (FindWidespreadBird(bird_types1) == "sinica"s) {
        cout << "Correct"s << endl;
    }
    else {
        cout << "Not correct"s << endl;
    }

    vector<string> bird_types2 = { "ruh"s, "sirin"s, "blue bird of fortune"s, "finist"s, "fenix"s };
    if (FindWidespreadBird(bird_types2) == "blue bird of fortune"s) {
        cout << "Correct"s << endl;
    }
    else {
        cout << "Not correct"s << endl;
    }
}