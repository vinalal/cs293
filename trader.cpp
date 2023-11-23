#include "receiver.h"
#include<vector>
#include<map>

struct trade_order{
    std::vector<std::string> companies;
    std::map<std::string,int> quantities;
    int price;
    bool offer;    // 0 -> buy and 1 -> sell
};

struct trade_order3{
    std::vector<std::string> companies;
    std::map<std::string,int> quantities;
    int price;
    bool offer;
    int quantity;
};

int main(int argc, char* argv[]) {

    Receiver rcv;
    bool foundDollar = false;
    std::string message = "";
    while(!foundDollar){
        message = message + rcv.readIML();
        if(message.find('$') != std::string::npos){
            rcv.terminate();
            foundDollar = true;
        }
    }
    if(argv[1][0] == '1'){                       // part1
        std::stringstream ss(message);
        std::string word;
        std::vector<std::string> trade_offers;
        std::string trade_offer = "";
        while(ss >> word){
            if(word.substr(1,1) == "#"){
                trade_offer = trade_offer + word.substr(0,1);
                trade_offers.push_back(trade_offer);
                trade_offer = "";
                continue;
            }
            trade_offer = trade_offer + word + " ";
        }                                  // till now i have the vector of all the trades as strings.

        std::map<std::string,int> prices;
        std::string company;
        std::string price;
        std::string offer;
        std::map<std::string,std::pair<int,int>> survival_stocks;  // 1st is the buy and 2nd is the sell
        int x = 0;
        for(int i=0;i<trade_offers.size();i++){
            trade_offer = trade_offers[i];
            x = 0;
            word = "";
            for(auto y : trade_offer){
                if(y == ' '){
                    if(x == 0)company = word;
                    else if(x == 1)price = word;
                    else if(x == 2)offer = word;
                    word = "";
                    x++;
                }
                else{
                    word = word + y;
                }
            }                            // i have redeeemed the company name, price, offer of the trading offer till now

            x = std::stoi(price);
            int z;
            if(prices[company] == 0){
                prices[company] = x;
                if(trade_offer.substr(trade_offer.length()-1,1) == "b"){
                    std::cout<<trade_offer.substr(0,trade_offer.length()-1) + "s"<<std::endl;
                }
                if(trade_offer.substr(trade_offer.length()-1,1) == "s"){
                    std::cout<<trade_offer.substr(0,trade_offer.length()-1) + "b"<<std::endl;
                }
            }

            else{
                if(trade_offer.substr(trade_offer.length()-1,1) == "s"){            // if it is a sell trade
                    z = survival_stocks[company].second;
                    if(x >= prices[company]){
                        std::cout<<"No Trade"<<std::endl;
                        if(x < z){
                            if(survival_stocks[company].first == x){
                                survival_stocks[company].first = 0;
                                survival_stocks[company].second = 0;
                            }
                            else{
                                survival_stocks[company].second = x;
                            }
                        }

                        if (z == 0){
                            if(survival_stocks[company].first == x){
                                survival_stocks[company].first = 0;
                            }
                            else{
                                survival_stocks[company].second = x;
                            }
                        }
                    }
                    else{
                        if(x < z || z == 0){
                            if(survival_stocks[company].first == x){
                                std::cout<<"No Trade"<<std::endl;
                                survival_stocks[company].first = 0;
                                survival_stocks[company].second = 0;
                            }
                            else{
                                std::cout<<trade_offer.substr(0,trade_offer.length()-1) + "b"<<std::endl;
                                prices[company] = x;
                                survival_stocks[company].second = 0;
                            }
                        }
                        else{
                            std::cout<<"No Trade"<<std::endl;

                        }

                    }
                }
                if(trade_offer.substr(trade_offer.length()-1,1) == "b"){            // if it is a buy trade
                    z = survival_stocks[company].first;
                    if(x <= prices[company]){
                        std::cout<<"No Trade"<<std::endl;
                        if(x > z){
                            if(survival_stocks[company].second == x){
                                survival_stocks[company].first = 0;
                                survival_stocks[company].second = 0;
                            }
                            else{
                                survival_stocks[company].first = x;
                            }
                        }
                    }
                    else{
                        if(x > z){
                            if(survival_stocks[company].second == x){
                                std::cout<<"No Trade"<<std::endl;
                                survival_stocks[company].first = 0;
                                survival_stocks[company].second = 0;
                            }
                            else{
                                std::cout<<trade_offer.substr(0,trade_offer.length()-1) + "s"<<std::endl;
                                prices[company] = x;
                                survival_stocks[company].first = 0;
                            }
                        }
                        else{
                            std::cout<<"No Trade"<<std::endl;

                        }

                    }
                }
            }
        }                                // part 1 done
    }



    if(argv[1][0] == '2'){                       // part2
        std::stringstream ss(message);
        std::string word;
        std::vector<std::string> trade_offers;
        std::string trade_offer = "";
        while(ss >> word){
            if(word.substr(1,1) == "#"){
                trade_offer = trade_offer + word.substr(0,2) + " ";
                trade_offers.push_back(trade_offer);
                trade_offer = "";
                continue;
            }
            trade_offer = trade_offer + word + " ";
        }                                  // till now i have the vector of all the trades as strings.

        std::vector<std::pair<std::map<trade_order*,bool>,std::vector<trade_order*>>> subsets;
        std::pair<std::map<trade_order*,bool>,std::vector<trade_order*>> x;
        std::map<trade_order*,bool> m;
        std::vector<trade_order*> v;
        x.first = m;
        x.second = v;
        subsets.push_back(x);
        std::map<trade_order*,bool> cancel_check;
        int a;

        for(int i=0;i<trade_offers.size();i++){
            trade_offer = trade_offers[i];
            a = 0;
            word = "";
            struct trade_order* t = (trade_order*)malloc(sizeof(trade_order));
            for(auto y : trade_offer){
                if(y == ' '){
                    if(a%2 == 0){
                        t->companies.push_back(word);                 // companies has the price at its last position
                    }
                    else{
                        if(word != "s#" || word != "b#"){
                            t->quantities[t->companies.back()] = std::stoi(word);                    // nums has b# or s# at its last place
                        }
                        else if(word == "s#"){
                            t->offer = 1;
                        }
                        else{
                            t->offer = 0;
                        }
                    }
                    word = "";
                    a++;
                }
                else{
                    word = word + y;
                }
            }                                             // till now i have the vector of all the trades as strings.
            t->price = std::stoi(t->companies.back());
            t->companies.pop_back();                      // till now i got t

            bool flag1 = false;
            bool cancelled = false;
            bool buy_sell = false;
            bool replace = false;
            struct trade_order* r = (trade_order*)malloc(sizeof(trade_order));
            struct trade_order* w = (trade_order*)malloc(sizeof(trade_order));
            for(auto z = cancel_check.begin();z != cancel_check.end();){     // iteration of cancel_check
                // if(z.second == false)continue;
                std::map<std::string,bool> check;
                int count1 = 0;
                int count2 = 0;
                bool flag = true;
                for(int j=0;j<z->first->companies.size();j++){
                    check[z->first->companies[j]] = true;
                    count1++;
                }
                for(int j=0;j<t->companies.size();j++){
                    count2++;
                    if(check[t->companies[j]] == false){
                        flag = false;
                        break;
                    }
                    if(t->quantities[t->companies[j]] != z->first->quantities[t->companies[j]]){
                        flag = false;
                        break;
                    }
                }                   // structure check
                if(flag == false || count1 != count2){
                    z++;
                    continue;
                }      // if it goes further then their structures are same
                flag1 = true;
                if(t->offer == 0 && z->first->offer == 0){
                    if(t->price > z->first->price){        // remove cancel_check[i] and add t
                        replace = true;
                        r = z->first;
                    }
                    else{
                        cancelled = true;
                        break;
                    }
                }
                if(t->offer == 1 && z->first->offer == 1){
                    if(t->price < z->first->price){       // remove cancel_check[i] and add t
                        replace = true;
                        r = z->first;
                    }
                    else{
                        cancelled = true;
                        break;
                    }
                }
                if(t->offer != z->first->offer){
                    if(t->price == z->first->price){      // don't remove cancel_check[i] now and wait for the iteration to be over
                        w = z->first;
                        buy_sell = true;
                    }
                    else{     // dont add t now to the subsets wait for the search to be over and then add if t doesn't gets cancelled
                        buy_sell = false;
                    }
                }
                z++;
            }
            if(!flag1){     // add t to cancel_check and subsets
                cancel_check[t] = true;
                int max_profit = 0;
                int profit = 0;
                std::vector<trade_order*> arbitrage;
                bool flag2;
                for(int j=0;j<subsets.size();j++){
                    m = subsets[j].first;
                    v = subsets[j].second;
                    flag2 = true;
                    m[t] = true;
                    v.push_back(t);
                    profit = 0;
                    max_profit = 0;
                    std::map<std::string,bool> company;
                    std::map<std::string,int> net_quantity;
                    for(int l=0;l<v.size();l++){
                        for(int c=0;c<v[l]->companies.size();c++){
                            company[v[l]->companies[c]] = true;
                            net_quantity[v[l]->companies[c]] += v[l]->quantities[v[l]->companies[c]];
                        }
                        profit += v[l]->price;
                    }
                    for(auto k : net_quantity){
                        if(k.second != 0){
                            flag2 = false;
                        }
                    }
                    if(flag2 == true){             // an arbitrage is detected
                        if(profit > max_profit){
                            arbitrage = v;
                            max_profit = profit;
                        }
                    }
                    x.first = m;
                    x.second = v;
                    subsets.push_back(x);    // pushing each subset after adding t in it to the subsets vector
                }          // now arbitrage contains best arbitrage and max_profit contains best profit
                if(arbitrage.empty()){
                    std::cout<<"No Trade"<<std::endl;
                }
                else{
                    for(int i = arbitrage.size()-1;i >= 0;i--){
                        for(int j = 0;j < arbitrage[i]->companies.size();j++){
                            std::cout<<arbitrage[i]->companies[j]<<" "<<arbitrage[i]->quantities[arbitrage[i]->companies[j]]<<std::endl;
                        }
                    }
                    std::cout<<max_profit<<std::endl;
                }
            }
            else if(!cancelled && buy_sell){  // if a buy_sell cancelation is possible and t doesn't get cancelled in full iteration.
                // just remove r
                cancel_check.erase(w);
                for(int j=0;j<subsets.size();j++){
                    if(subsets[j].first[w] == true){
                        subsets.erase(subsets.begin()+j);
                    }
                }
            }
            if(replace && !buy_sell){    // only inner cancelation and add t
                cancel_check.erase(r);
                for(int j=0;j<subsets.size();j++){
                    if(subsets[j].first[r] == true){
                        subsets.erase(subsets.begin()+j);
                    }
                }
                int max_profit = 0;
                int profit = 0;
                std::vector<trade_order*> arbitrage;
                bool flag2;
                for(int j=0;j<subsets.size();j++){
                    m = subsets[j].first;
                    v = subsets[j].second;
                    flag2 = true;
                    m[t] = true;
                    v.push_back(t);
                    profit = 0;
                    max_profit = 0;
                    std::map<std::string,bool> company;
                    std::map<std::string,int> net_quantity;
                    for(int l=0;l<v.size();l++){
                        for(int c=0;c<v[l]->companies.size();c++){
                            company[v[l]->companies[c]] = true;
                            net_quantity[v[l]->companies[c]] += v[l]->quantities[v[l]->companies[c]];
                        }
                        profit += v[l]->price;
                    }
                    for(auto k : net_quantity){
                        if(k.second != 0){
                            flag2 = false;
                        }
                    }
                    if(flag2 == true){             // an arbitrage is detected
                        if(profit > max_profit){
                            arbitrage = v;
                            max_profit = profit;
                        }
                    }
                    x.first = m;
                    x.second = v;
                    subsets.push_back(x);    // pushing each subset after adding t in it to the subsets vector
                }          // now arbitrage contains best arbitrage and max_profit contains best profit
            }
            if(replace && buy_sell){     // both cross cancelation and inner cancelation
                cancel_check.erase(r);
                for(int j=0;j<subsets.size();j++){
                    if(subsets[j].first[r] == true){
                        subsets.erase(subsets.begin()+j);
                    }
                }
                cancel_check.erase(w);
                for(int j=0;j<subsets.size();j++){
                    if(subsets[j].first[w] == true){
                        subsets.erase(subsets.begin()+j);
                    }
                }
            }
        }
    }



    if(argv[1][0] == '3'){                       // part3
        std::stringstream ss(message);
        std::string word;
        std::vector<std::string> trade_offers;
        std::string trade_offer = "";
        while(ss >> word){
            if(word.substr(1,1) == "#"){
                trade_offer = trade_offer + word.substr(0,2) + " ";
                trade_offers.push_back(trade_offer);
                trade_offer = "";
                continue;
            }
            trade_offer = trade_offer + word + " ";
        }                                  // till now i have the vector of all the trades as strings.
        std::cout<<trade_offers[0]<<std::endl;
    }
    return 0;
}
