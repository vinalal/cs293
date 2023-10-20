#include "shortest_walk.cpp"
#include <iostream>
#include<map>
using namespace std;

struct user{
    int arrival;
    int source;
    int destination;
};

void shortest_run(vector<user>& users, Graph& g,vector<vector<int> >& paths)
{
    
    vector<vector<int>> record_paths; // this has all the shortest walks of each user 
    for(int i=0;i<users.size();i++){
        vector<int> paths_fetched;
        vector<int> s_d;
        s_d.push_back(users[i].source);
        s_d.push_back(users[i].destination);
        shortest_walk(g,s_d,paths_fetched);
        record_paths.push_back(paths_fetched);
        s_d.clear();
        paths_fetched.clear();

    }
    
    
    unordered_map<int,vector<int>> paths_got;
    map<int,int,greater<int>> current_pos; // this will contain the current pos of each user
    unordered_map<int,int> step;
    int time = 1;
    int flag = 1;
    while(flag == 1){
        int count = 0;
        int a = -1;
        // this loop basically updates my current_pos map and checks if we have reached the destination of any of the users
        for(int i=0;i<users.size();i++){
            if(users[i].arrival >= time+1){
                break;
            }
            if(current_pos.count(i) != 0){
                if(current_pos[i] != users[i].destination){
                    if(current_pos[i]!=a){
                        a = current_pos[i];
                        current_pos[i]=record_paths[i][step[i]];
                        step[i]++;
                        
                        
                    }
                }
            }
            else{
                
                a = current_pos[i];
                current_pos[i]=record_paths[i][step[i]];
                step[i]++;
            }
            
            if(current_pos[i] == users[i].destination){
                count++;
            }
        }
        
        if(count == users.size()){
            flag = 0;
        }
        for(auto i = current_pos.begin();i!=current_pos.end();i++){
            if(!(paths_got[i->first].empty())){
                if(paths_got[i->first].back() != users[i->first].destination){
                    cout <<"helo"<< paths_got[i->first].back()<<endl;
                    paths_got[i->first].push_back(i->second);
                }
            }
            else{
                paths_got[i->first].push_back(i->second);
            }
            
        }

        time++;



    }
    for(auto i=paths_got.begin();i!=paths_got.end();i++){
        paths.push_back(i->second);
    }
    
    return;
}