#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;

int main(){
    unordered_map<int,vector<int>> m;
    m[0].push_back(0);
    m[0].push_back(0);
    cout << m[0].size();

}