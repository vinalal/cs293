#include<unordered_map>
#include<vector>
#include<iostream>
#include<utility>
#include<queue>
#include <set>
#include <list>


using namespace std;

// Implement a node struct if you feel 




// Implement the graph class
class Graph
{
  public:
        unordered_map<int,set<int>> adj;
        Graph(int no_of_vertices, vector<pair<int,int>> edges){
          for(auto i:edges){
            adj[i.first].insert(i.second);
            adj[i.second].insert(i.first);
          }
          return;
        };
};


void bfs(Graph g, int source,unordered_map<int, int>& parent)
{
  queue<int> q;
  set<int> visited;
  q.push(source);
  visited.insert(source);
  parent[source] = -1;
  while(!q.empty()){
    int v = q.front();
    q.pop();
    for(auto i:g.adj[v]){
      if(visited.count(i) == 0){
        q.push(i);
        visited.insert(i);
        parent[i]=v;
      }
    }

  }
  return;
};

