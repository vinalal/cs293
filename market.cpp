#include "market.h"
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<queue>
#include<map>
#include<unordered_map>

market::market(int argc, char** argv)
{
	
}

struct order{
    std::string broker;
    std::vector<std::string> companies;
    std::map<std::string,int> quantities;
    int quantity;
    int price;
    int start_time;
    int end_time;
    int offer;
};

void print(order* t){
    std::cout<<t->start_time<<" "<<t->broker<<" "<<t->offer<<" "<<t->companies[0]<<" "<<t->price<<" "<<t->quantity<<" "<<t->end_time-t->start_time<<std::endl;
}

struct structure{
    std::map<std::string,int> quants;

    bool operator==(const structure& other) const {
        return quants==other.quants;
    }
    bool operator<(const structure& other) const {
        // Customize the comparison logic based on your requirements
        int thisTotal = 0;
        int otherTotal = 0;

        for (const auto& entry : quants) {
            thisTotal += entry.second;
        }

        for (const auto& entry : other.quants) {
            otherTotal += entry.second;
        }

        return thisTotal < otherTotal;
    }
};

struct max_compare{
    bool operator()(const std::pair<int, order*>& lhs, const std::pair<int, order*>& rhs) {
        if(lhs.first == rhs.first){
            if(lhs.second->start_time == rhs.second->start_time){
                return lhs.second->broker > rhs.second->broker;
            }
            return lhs.second->start_time > rhs.second->start_time;
        }
        return lhs.first < rhs.first; // Comparing based on the first parameter (integer)
    }
};

struct min_compare{
    bool operator()(const std::pair<int, order*>& lhs, const std::pair<int, order*>& rhs) {
        if(lhs.first == rhs.first){
            if(lhs.second->start_time == rhs.second->start_time){
                return lhs.second->broker > rhs.second->broker;
            }
            return lhs.second->start_time > rhs.second->start_time;
        }
        return lhs.first > rhs.first; // Comparing based on the first parameter (integer)
    }
};

void market::start()
{
	// Declare an input file stream object
    std::ifstream inputFile;

    // Open the file (replace "filename.txt" with your actual file name)
    inputFile.open("output.txt");

    // Check if the file is successfully opened
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
    }

    // Read and process the contents of the file
    std::string line;
    std::string message = "";
    std::vector<std::string> orders;
    std::string line1;
    int a = 0;
    while (std::getline(inputFile, line)) {
        // Process each line as needed (e.g., print it)
        if(line == "!@")break;
        line1 = line;
        a++;
        if(a!=1){
            orders.push_back(line1+" ");
        }
    }           // till now i have got my whole market orders in orders
    inputFile.close();
    
    std::string word = "";
    a = 0;
    std::vector<order*> trade_orders;
    for(int i=0;i<orders.size();i++){
        word = "";
        a = 0;
        struct order* t = new order;
        for(auto y:orders[i]){   // iterating throught the ith order
            if(y == ' '){
                if(a == 0){
                    t->start_time = std::stoi(word);
                }
                if(a == 1){
                    t->broker = word;
                }
                if(a == 2){
                    if(word == "BUY"){    // buy
                        t->offer = 0;
                    }
                    else{         // sell
                        t->offer = 1;
                    }
                }     // after a = 2, i have linear combination and after that i have price and quantity.
                if(a > 2 || a == -1){
                    if(a == -1){
                        int b = stoi(word);
                        if(b!= -1){
                            t->end_time = t->start_time + std::stoi(word);
                        }
                        else{
                            t->end_time = 10000;
                        }
                    }
                    if(word.substr(0,1) == "$"){
                        t->price = std::stoi(word.substr(1,word.size()-1));
                    }
                    else if(word.substr(0,1) == "#"){
                        t->quantity = std::stoi(word.substr(1,word.size()-1));
                        a = -2;
                    }
                    else if(a%2==1){
                        t->companies.push_back(word);
                    }
                    else if(a%2==0){
                        t->quantities[t->companies.back()] = std::stoi(word);
                    }
                }
                word="";
                a++;
            }
            else{
                word = word + y;
            }
        }
        if(t->quantities[t->companies[0]] == 0){
            t->quantities[t->companies[0]] = 1;
        }
        trade_orders.push_back(t);
    }
    // Now, i will start iterating the orders

    std::map<std::string,std::priority_queue<std::pair<int, order*>, std::vector<std::pair<int, order*>>, max_compare>> buy_queues;   // max heap for buy
    std::pair<int,order*> x;
    std::vector<std::pair<int,order*>> v;
    std::map<std::string,std::priority_queue<std::pair<int, order*>, std::vector<std::pair<int, order*>>, min_compare>> sell_queues;  // min heap for sell
    int total_amount = 0;
    int number_trades = 0;
    int number_of_shares = 0;
    std::map<std::string,std::pair<int,int>> shares;
    std::map<std::string,int> amount;

    for(int i=0;i<trade_orders.size();i++){
        struct order* t = trade_orders[i];

        std::string s = t->companies[0];
        if(t->offer == 0){  // buy order
            // std::cout<<"buy"<<std::endl;
            if(sell_queues[s].empty()){
                buy_queues[s].push(std::make_pair(t->price,t));
                continue;
            }
            else{
                while(!sell_queues[s].empty()){
                    if(t->price < sell_queues[s].top().first){
                        // std::cout<<"hii1"<<std::endl;
                        break;
                    }
                    else if(t->price >= sell_queues[s].top().first){
                        // return;
                        if(t->start_time > sell_queues[s].top().second->end_time){
                            v.push_back(sell_queues[s].top());
                            sell_queues[s].pop();
                            continue;
                        }
                        // if it reaches here then it means we have found the deal for the best trade...now take it and go to next one if quantity still remains
                        x = sell_queues[s].top();
                        if(t->quantity < x.second->quantity){
                            x.second->quantity -= t->quantity;
                            //now print the message
                            total_amount += t->quantity * x.first;
                            number_trades += 1;
                            number_of_shares += t->quantity;
                            shares[t->broker].first += t->quantity;
                            shares[x.second->broker].second += t->quantity;
                            amount[t->broker] -= t->quantity * x.first;
                            amount[x.second->broker] += t->quantity * x.first;
                            std::cout<<t->broker<<" purchased "<<t->quantity<<" share of ";
                            if(t->companies.size() == 1){
                                if(t->quantities[t->companies[0]] == 1){
                                    std::cout << t->companies[0];
                                }
                                else{
                                    std::cout << t->companies[0] << " " << t->quantities[t->companies[0]];
                                }
                            }
                            else{
                                for(int i=0;i<t->companies.size();i++){
                                    std::cout << t->companies[i] << " " << t->quantities[t->companies[i]];
                                }
                            }
                            std :: cout << " from " << x.second->broker << " for $" << x.first << "/share"<<std::endl;
                            t->quantity = 0;
                            break;
                        }
                        else if(t->quantity == x.second->quantity){
                            //print the message
                            total_amount += t->quantity * x.first;
                            number_trades += 1;
                            number_of_shares += t->quantity;
                            shares[t->broker].first += t->quantity;
                            shares[x.second->broker].second += t->quantity;
                            amount[t->broker] -= t->quantity * x.first;
                            amount[x.second->broker] += t->quantity * x.first;
                            std::cout<<t->broker<<" purchased "<<t->quantity<<" share of ";
                            if(t->companies.size() == 1){
                                if(t->quantities[t->companies[0]] == 1){
                                    std::cout << t->companies[0];
                                }
                                else{
                                    std::cout << t->companies[0] << " " << t->quantities[t->companies[0]];
                                }
                            }
                            else{
                                for(int i=0;i<t->companies.size();i++){
                                    std::cout << t->companies[i] << " " << t->quantities[t->companies[i]];
                                }
                            }
                            std :: cout << " from " << x.second->broker << " for $" << x.first << "/share"<<std::endl;
                            sell_queues[s].pop();
                            t->quantity = 0;
                            break;
                        }
                        else if(t->quantity > x.second->quantity){
                            //print the message
                            total_amount += x.second->quantity * x.first;
                            number_trades += 1;
                            number_of_shares += x.second->quantity;
                            shares[t->broker].first += x.second->quantity;
                            shares[x.second->broker].second += x.second->quantity;
                            amount[t->broker] -= x.second->quantity * x.first;
                            amount[x.second->broker] += x.second->quantity * x.first;
                            std::cout<<t->broker<<" purchased "<<x.second->quantity<<" share of ";
                            if(t->companies.size() == 1){
                                if(t->quantities[t->companies[0]] == 1){
                                    std::cout << t->companies[0];
                                }
                                else{
                                    std::cout << t->companies[0] << " " << t->quantities[t->companies[0]];
                                }
                            }
                            else{
                                for(int i=0;i<t->companies.size();i++){
                                    std::cout << t->companies[i] << " " << t->quantities[t->companies[i]];
                                }
                            }
                            std :: cout << " from " << x.second->broker << " for $" << x.first << "/share"<<std::endl;

                            t->quantity -= x.second->quantity;
                            sell_queues[s].pop();
                        }
                    }
                }

                if(t->quantity > 0){
                    buy_queues[s].push(std::make_pair(t->price,t));
                }
                
                for(int i=0;i<v.size();i++){
                    sell_queues[s].push(v.back());
                    v.pop_back();
                }
            }
            
        }
        else{    // sell order
            // std::cout<<"sell"<<std::endl;
            if(buy_queues[s].empty()){
                // std::cout<<"hii"<<std::endl;
                sell_queues[s].push(std::make_pair(t->price,t));
                continue;
            }
            else{
                while(!buy_queues[s].empty()){
                    if(t->price > buy_queues[s].top().first){
                        break;
                    }
                    else if(t->price <= buy_queues[s].top().first){
                        if(t->start_time > buy_queues[s].top().second->end_time){
                            v.push_back(buy_queues[s].top());
                            buy_queues[s].pop();
                            continue;
                        }
                        // if it reaches here then it means we have found the deal for the best trade...now take it and go to next one if quantity still remains
                        x = buy_queues[s].top();
                        if(t->quantity < x.second->quantity){
                            x.second->quantity -= t->quantity;
                            //print messsage
                            total_amount += t->quantity * x.first;
                            number_trades += 1;
                            number_of_shares += t->quantity;
                            shares[t->broker].second += t->quantity;
                            shares[x.second->broker].first += t->quantity;
                            amount[t->broker] += t->quantity * x.first;
                            amount[x.second->broker] -= t->quantity * x.first;
                            std::cout<<x.second->broker<<" purchased "<<t->quantity<<" share of ";
                            if(t->companies.size() == 1){
                                if(t->quantities[t->companies[0]] == 1){
                                    std::cout << t->companies[0];
                                }
                                else{
                                    std::cout << t->companies[0] << " " << t->quantities[t->companies[0]];
                                }
                            }
                            else{
                                for(int i=0;i<t->companies.size();i++){
                                    std::cout << t->companies[i] << " " << t->quantities[t->companies[i]];
                                }
                            }
                            std :: cout << " from " << t->broker << " for $" << x.first << "/share"<<std::endl;
                            t->quantity = 0;
                            break;
                        }
                        else if(t->quantity == x.second->quantity){
                            
                            //print the message
                            total_amount += t->quantity * x.first;
                            number_trades += 1;
                            number_of_shares += t->quantity;
                            shares[t->broker].second += t->quantity;
                            shares[x.second->broker].first += t->quantity;
                            amount[t->broker] += t->quantity * x.first;
                            amount[x.second->broker] -= t->quantity * x.first;
                            std::cout<<x.second->broker<<" purchased "<<t->quantity<<" share of ";
                            if(t->companies.size() == 1){
                                if(t->quantities[t->companies[0]] == 1){
                                    std::cout << t->companies[0];
                                }
                                else{
                                    std::cout << t->companies[0] << " " << t->quantities[t->companies[0]];
                                }
                            }
                            else{
                                for(int i=0;i<t->companies.size();i++){
                                    std::cout << t->companies[i] << " " << t->quantities[t->companies[i]];
                                }
                            }
                            std :: cout << " from " << t->broker << " for $" << x.first << "/share"<<std::endl;
                            buy_queues[s].pop();
                            t->quantity = 0;
                            break;
                        }
                        else if(t->quantity > x.second->quantity){
                            //print the message
                            total_amount += x.second->quantity * x.first;
                            number_trades += 1;
                            number_of_shares += x.second->quantity;
                            shares[t->broker].second += x.second->quantity;
                            shares[x.second->broker].first += x.second->quantity;
                            amount[t->broker] += x.second->quantity * x.first;
                            amount[x.second->broker] -= x.second->quantity * x.first;
                            std::cout<<x.second->broker<<" purchased "<<x.second->quantity<<" share of ";
                            if(t->companies.size() == 1){
                                if(t->quantities[t->companies[0]] == 1){
                                    std::cout << t->companies[0];
                                }
                                else{
                                    std::cout << t->companies[0] << " " << t->quantities[t->companies[0]];
                                }
                            }
                            else{
                                for(int i=0;i<t->companies.size();i++){
                                    std::cout << t->companies[i] << " " << t->quantities[t->companies[i]];
                                }
                            }
                            std :: cout << " from " << t->broker << " for $" << x.first << "/share"<<std::endl;

                            t->quantity -= x.second->quantity;
                            buy_queues[s].pop();
                        }
                    }
                }

                if(t->quantity > 0){
                    sell_queues[s].push(std::make_pair(t->price,t));
                }

                for(int i=0;i<v.size();i++){
                    buy_queues[s].push(v.back());
                    v.pop_back();
                }   
            }
        }
        // return;

    }

    std::cout << "\n";
    std::cout << "---End of Day---"<<std::endl;
    std::cout << "Total Amount of Money Transferred: $"<<total_amount<<std::endl;
    std::cout << "Number of Completed Trades: "<<number_trades<<std::endl;
    std::cout << "Number of Shares Traded: "<<number_of_shares << std::endl;
    for(auto i : amount){
        std::cout << i.first << " bought " << shares[i.first].first << " and sold " << shares[i.first].second << " for a net transfer of $"<< i.second << std::endl;
    }


    return;
}
