#include "graph.cpp"

bool relation(Graph g,int u,int v){
    if(g.adj[u].count(v) != 0){
        return true;
    }
    return false;
}

void shortest_walk(Graph g,vector<int>&stations, vector<int>& path)
{
    for(int i=0;i<stations.size()-1;i++){
        if(relation(g,stations[i],stations[i+1])){
            path.push_back(stations[i]);
            if(i+1 == stations.size()-1){
                path.push_back(stations[i+1]);
            }
        }
        else{
            unordered_map<int,int> parent;
            bfs(g,stations[i+1],parent);
            int a=stations[i+1];
            int p=stations[i];
            while(p!=stations[i+1]){
                path.push_back(p);
                p = parent[p];
            }
            if(i+1 == stations.size()-1){
                path.push_back(stations[i+1]);
            }

        }

    }
    return;
}
