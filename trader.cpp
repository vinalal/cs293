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

struct quant_order{
    struct trade_order3* order;
    int quant;
};

void subset_arbit(std::vector<trade_order*> cancel_check,int &max_profit,std::vector<trade_order*>& arbitrage,int index,std::vector<trade_order*> v){
    if(index == cancel_check.size()){
        int profit = 0;
        bool flag2 = true;
        std::map<std::string,int> net_quantity;
        for(int l=0;l<v.size();l++){
            for(int c=0;c<v[l]->companies.size();c++){
                if(v[l]->offer == 0){
                    net_quantity[v[l]->companies[c]] += v[l]->quantities[v[l]->companies[c]];
                }
                else{
                    net_quantity[v[l]->companies[c]] -= v[l]->quantities[v[l]->companies[c]];
                }
                
            }
            if(v[l]->offer == 0){
                profit += v[l]->price;
            }
            else{
                profit -= v[l]->price;
            }
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
        return;
    }
    
    if(cancel_check[index]->price != 0){
        std::vector<trade_order*> x = v;
        x.push_back(cancel_check[index]);
        subset_arbit(cancel_check,max_profit,arbitrage,index+1,x);
        subset_arbit(cancel_check,max_profit,arbitrage,index+1,v);
    }
    else{
        subset_arbit(cancel_check,max_profit,arbitrage,index+1,v);
    }
     // so x contains the tradeorder at index position but v does not
    
}

void subset_arbit3(std::vector<trade_order3*> cancel_check,int &max_profit,std::vector<quant_order*>& arbitrage,int index,std::vector<quant_order*> v){
    if(index == cancel_check.size()){
        int profit = 0;
        bool flag2 = true;
        std::map<std::string,int> net_quantity;
        for(int l=0;l<v.size();l++){
            if(v[l]->order->offer == 0){   // buy
                for(int c=0;c<v[l]->order->companies.size();c++){
                    net_quantity[v[l]->order->companies[c]] += (v[l]->quant)*(v[l]->order->quantities[v[l]->order->companies[c]]);
                }
                profit += v[l]->quant*v[l]->order->price;
            }
            else{           // sell
                for(int c=0;c<v[l]->order->companies.size();c++){
                    net_quantity[v[l]->order->companies[c]] -= v[l]->quant*v[l]->order->quantities[v[l]->order->companies[c]];
                }
                profit -= v[l]->quant*v[l]->order->price;
            }
            
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

        return;
    }

    
    for(int i=1;i<=cancel_check[index]->quantity;i++){
        std::vector<quant_order*> x = v;
        struct quant_order* p = new quant_order();
        p->order = cancel_check[index];
        p->quant = i;
        x.push_back(p);
        subset_arbit3(cancel_check,max_profit,arbitrage,index+1,x);
    }
    subset_arbit3(cancel_check,max_profit,arbitrage,index+1,v);

}




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
        
        std::vector<trade_order*> cancel_check;
        int a;
        int total_profit = 0;

        for(int i=0;i<trade_offers.size();i++){
            trade_offer = trade_offers[i];
            a = 0;
            word = "";
            struct trade_order* t = new trade_order();
            for(auto y : trade_offer){
                if(y == ' '){
                    if(a%2 == 0){
                        t->companies.push_back(word);                 // companies has the price at its last position
                    }
                    else{
                        if(word != "s#" && word != "b#"){
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
            struct trade_order* r = new trade_order();
            struct trade_order* w = new trade_order();
            for(int zo = 0;zo < cancel_check.size();zo++){     // iteration of cancel_check
                // if(z.second == false)continue;
                struct trade_order* z = cancel_check[zo];
                std::map<std::string,bool> check;
                int count1 = 0;
                int count2 = 0;
                bool flag = true;
                for(int j=0;j<z->companies.size();j++){
                    check[z->companies[j]] = true;
                    count1++;
                }
                for(int j=0;j<t->companies.size();j++){
                    count2++;
                    if(check[t->companies[j]] == false){
                        flag = false;
                        break;
                    }
                    if(t->quantities[t->companies[j]] != z->quantities[t->companies[j]]){
                        flag = false;
                        break;
                    }
                }                   // structure check
                if(flag == false || count1 != count2){
                    z++;
                    continue;
                }      // if it goes further then their structures are same
                flag1 = true;
                if(t->offer == 0 && z->offer == 0){
                    if(t->price > z->price){        // remove cancel_check[i] and add t
                        replace = true;
                        r = z;
                    }
                    else{
                        cancelled = true;
                        break;
                    }
                }
                if(t->offer == 1 && z->offer == 1){
                    if(t->price < z->price){       // remove cancel_check[i] and add t
                        replace = true;
                        r = z;
                    }
                    else{
                        cancelled = true;
                        break;
                    }
                }
                if(t->offer != z->offer){
                    if(t->price == z->price){      // don't remove cancel_check[i] now and wait for the iteration to be over
                        w = z;
                        buy_sell = true;
                    }
                    else{     // dont add t now to the subsets wait for the search to be over and then add if t doesn't gets cancelled
                        buy_sell = false;
                    }
                }
            
            }
            
            if(!flag1){     // check arbitrage with all the order above it
                std::vector<trade_order*> v;
                v.push_back(t);
                int max_profit = 0;
                std::vector<trade_order*> arbitrage;
                subset_arbit(cancel_check,max_profit,arbitrage,0,v);
                if(arbitrage.empty()){
                    cancel_check.push_back(t);
                    std::cout<<"No Trade"<<std::endl;
                }
                else{
                    //first we need to print the new order
                    char offer_f;
                    if(t->offer == 0){
                        offer_f = 's';
                    }
                    else{
                        offer_f = 'b';
                    }
                    for(int j=0;j<t->companies.size();j++){
                        std::cout<<t->companies[j]<<" "<<t->quantities[t->companies[j]] << " ";
                    }
                    std::cout << t->price << " " << offer_f << "#" << std::endl;


                    for(int i = arbitrage.size()-1;i >= 1;i--){
                        if(arbitrage[i]->offer == 0){
                            offer_f = 's';
                        }
                        else{
                            offer_f = 'b';
                        }
                        for(int j = 0;j < arbitrage[i]->companies.size();j++){
                            std::cout<<arbitrage[i]->companies[j]<<" "<<arbitrage[i]->quantities[arbitrage[i]->companies[j]] << " ";
                        }
                        std::cout << arbitrage[i]->price <<" "<<offer_f<<"#"<<std::endl;
                    }

                    total_profit += max_profit;

                    for(int i=0;i<arbitrage.size();i++){
                        arbitrage[i]->price = 0;
                    }
                }

            }
            else if(cancelled){
                std::cout<<"No Trade"<<std::endl;
            }
            else if(!replace && !cancelled && buy_sell){  // if a buy_sell cancelation is possible and t doesn't get cancelled in full iteration.
                // just remove r
                w->price = 0;
                std::cout<<"No Trade"<<std::endl;
            }
            else if(!cancelled && replace && !buy_sell){    // only inner cancelation and add t
                r->price = 0;
                std::vector<trade_order*> v;
                v.push_back(t);
                int max_profit = 0;
                std::vector<trade_order*> arbitrage;
                subset_arbit(cancel_check,max_profit,arbitrage,0,v);
                if(arbitrage.empty()){
                    cancel_check.push_back(t);
                    std::cout<<"No Trade"<<std::endl;
                }
                else{
                    char offer_f;
                    if(t->offer == 0){
                        offer_f = 's';
                    }
                    else{
                        offer_f = 'b';
                    }
                    for(int j=0;j<t->companies.size();j++){
                        std::cout<<t->companies[j]<<" "<<t->quantities[t->companies[j]] << " ";
                    }
                    std::cout << t->price << " " << offer_f << "#" << std::endl;

                    for(int i = arbitrage.size()-1;i >= 1;i--){
                        if(arbitrage[i]->offer == 0){
                            offer_f = 's';
                        }
                        else{
                            offer_f = 'b';
                        }
                        for(int j = 0;j < arbitrage[i]->companies.size();j++){
                            std::cout<<arbitrage[i]->companies[j]<<" "<<arbitrage[i]->quantities[arbitrage[i]->companies[j]] << " ";
                        }
                        std::cout << arbitrage[i]->price <<" "<<offer_f<<"#"<<std::endl;
                    }

                    total_profit += max_profit;

                    for(int i=0;i<arbitrage.size();i++){
                        arbitrage[i]->price = 0;
                    }
                }
            }
            if(!cancelled && replace && buy_sell){     // both cross cancelation and inner cancelation
                w->price = 0;
                r->price = 0;
                
                std::cout<<"No Trade"<<std::endl;
            }
        }

        std::cout << total_profit << std::endl;
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

        //std::vector<std::pair<std::map<quant_order*,bool>,std::vector<quant_order*>>> subsets;
        //std::vector<std::pair<std::map<quant_order*,bool>,std::vector<quant_order*>>> copy;
        std::vector<trade_order3*> cancel_check;
        int total_profit = 0;
        int a;

        for(int i=0;i<trade_offers.size();i++){
            trade_offer = trade_offers[i];
            a = 0;
            word = "";
            struct trade_order3* t = new trade_order3();
            
            for(auto y : trade_offer){
                if(y == ' '){
                    if(a%2 == 0){
                        if(word != "s#" && word != "b#"){
                            t->companies.push_back(word);                 // companies has the price at its last position
                        }
                        else if(word == "s#"){
                            t->offer = 1;
                        }
                        else{
                            t->offer = 0;
                        }
                    }
                    else{
                        t->quantities[t->companies.back()] = std::stoi(word);        // has quantity at its last position
                    }
                    word = "";
                    a++;
                }
                else{
                    word = word + y;
                }
            }                                             // till now i have the vector of all the trades as strings.
            t->price = std::stoi(t->companies.back());
            t->quantity = t->quantities[t->companies.back()];
            t->quantities.erase(t->companies.back());
            t->companies.pop_back();                      // till now i got t

            bool buy_sell = false;
            bool update = false;
            struct trade_order3* r = new trade_order3();
            struct trade_order3* w = new trade_order3();
            for(int zo = 0; zo < cancel_check.size();zo++){     // iteration of cancel_check
                struct trade_order3* z = cancel_check[zo];
                if(z->quantity != 0){
                    std::map<std::string,bool> check;
                    int count1 = 0;
                    int count2 = 0;
                    bool flag = true;
                    for(int j=0;j<z->companies.size();j++){
                        check[z->companies[j]] = true;
                        count1++;
                    }
                    for(int j=0;j<t->companies.size();j++){
                        count2++;
                        if(check[t->companies[j]] == false){
                            flag = false;
                            break;
                        }
                        if(t->quantities[t->companies[j]] != z->quantities[t->companies[j]]){
                            flag = false;
                            break;
                        }
                    }                   // structure check
                    if(flag == false || count1 != count2){
                        z++;
                        continue;
                    }      // if it goes further then their structures are same
                    if(t->offer == z->offer){
                        if(t->price == z->price){
                            r = z;
                            update = true;
                        }
                    }
                    if(t->offer != z->offer){
                        if(t->price == z->price){      // don't remove cancel_check[i] now and wait for the iteration to be over
                            w = z;
                            buy_sell = true;
                        }
                    }
                }
            }
            if(update && !buy_sell){             // update the quantity in the cancel check and subsets
                r->quantity += t->quantity;
                std::vector<quant_order*> arbitrage;
                int max_profit = 0;
                std::vector<quant_order*> v{};
                subset_arbit3(cancel_check,max_profit,arbitrage,0,v);
                if(arbitrage.empty()){
                    std::cout<<"No Trade"<<std::endl;
                }
                else{  // we have found an arbitrage
                    for(int i=arbitrage.size()-1;i>=0;i--){
                        for(int j=0;j<arbitrage[i]->order->companies.size();j++){
                            std::cout<<arbitrage[i]->order->companies[j]<<" "<<arbitrage[i]->order->quantities[arbitrage[i]->order->companies[j]]<<" ";
                        }
                        std::cout<<arbitrage[i]->order->price<<" "<<arbitrage[i]->quant<<" ";
                        if(arbitrage[i]->order->offer == 0){
                            std::cout<<"s#"<<std::endl;
                        }
                        else{
                            std::cout<<"b#"<<std::endl;
                        }
                        
                        arbitrage[i]->order->quantity -= arbitrage[i]->quant;
                        
                    }
                    total_profit += max_profit;
                }
            }
            else if(buy_sell && !update){           // buy sell can cancel each other
                if(w->quantity >= t->quantity){
                    w->quantity -= t->quantity;
                    std::cout << "No Trade" << std::endl;
                }
                else{
                    t->quantity -= w->quantity;
                    w->quantity = 0;
                    int max_profit = 0;
                    std::vector<quant_order*> arbitrage;

                    for(int j = 1; j <= t->quantity;j++){
                        std::vector<quant_order*> v;
                        struct quant_order* p = new quant_order();
                        p->order = t;
                        p->quant = j;
                        v.push_back(p);
                        subset_arbit3(cancel_check,max_profit,arbitrage,0,v);

                    }

                    if(arbitrage.empty()){
                        cancel_check.push_back(t);
                        std::cout<<"No Trade"<<std::endl;
                    }
                    else{      // we have found an arbitrage
                        for(int j=0;j<arbitrage[0]->order->companies.size();j++){
                            std::cout<<arbitrage[0]->order->companies[j] << " " << arbitrage[0]->order->quantities[arbitrage[0]->order->companies[j]]<<" ";
                        }
                        std::cout<<arbitrage[0]->order->price<<" "<<arbitrage[0]->quant<<" ";
                        if(arbitrage[0]->order->offer == 0){
                            std::cout<<"s#"<<std::endl;
                        }
                        else{
                            std::cout<<"b#"<<std::endl;
                        }

                        if(arbitrage[0]->order->quantity != arbitrage[0]->quant){
                            t->quantity -= arbitrage[0]->quant;
                            cancel_check.push_back(t);
                        }

                        for(int i=arbitrage.size()-1;i>=1;i--){
                            for(int j=0;j<arbitrage[i]->order->companies.size();j++){
                                std::cout<<arbitrage[i]->order->companies[j]<<" "<<arbitrage[i]->order->quantities[arbitrage[i]->order->companies[j]]<<" ";
                            }
                            std::cout<<arbitrage[i]->order->price<<" "<<arbitrage[i]->quant<<" ";
                            if(arbitrage[i]->order->offer == 0){
                                std::cout<<"s#"<<std::endl;
                            }
                            else{
                                std::cout<<"b#"<<std::endl;
                            }
                            
                            arbitrage[i]->order->quantity -= arbitrage[i]->quant;
                        }
                        total_profit += max_profit;
                    }

                }
            }
            else if(!update && !buy_sell){         // add in cancel check and subsets
                int max_profit = 0;
                std::vector<quant_order*> arbitrage;

                for(int j = 1; j <= t->quantity;j++){
                    std::vector<quant_order*> v;
                    struct quant_order* p = new quant_order();
                    p->order = t;
                    p->quant = j;
                    v.push_back(p);
                    subset_arbit3(cancel_check,max_profit,arbitrage,0,v);

                }

                if(arbitrage.empty()){
                    cancel_check.push_back(t);
                    std::cout<<"No Trade"<<std::endl;
                }
                else{      // we have found an arbitrage
                    for(int j=0;j<arbitrage[0]->order->companies.size();j++){
                        std::cout<<arbitrage[0]->order->companies[j] << " " << arbitrage[0]->order->quantities[arbitrage[0]->order->companies[j]]<<" ";
                    }
                    std::cout<<arbitrage[0]->order->price<<" "<<arbitrage[0]->quant<<" ";
                    if(arbitrage[0]->order->offer == 0){
                        std::cout<<"s#"<<std::endl;
                    }
                    else{
                        std::cout<<"b#"<<std::endl;
                    }

                    if(arbitrage[0]->order->quantity != arbitrage[0]->quant){
                        t->quantity -= arbitrage[0]->quant;
                        cancel_check.push_back(t);
                    }

                    for(int i=arbitrage.size()-1;i>=1;i--){
                        for(int j=0;j<arbitrage[i]->order->companies.size();j++){
                            std::cout<<arbitrage[i]->order->companies[j]<<" "<<arbitrage[i]->order->quantities[arbitrage[i]->order->companies[j]]<<" ";
                        }
                        std::cout<<arbitrage[i]->order->price<<" "<<arbitrage[i]->quant<<" ";
                        if(arbitrage[i]->order->offer == 0){
                            std::cout<<"s#"<<std::endl;
                        }
                        else{
                            std::cout<<"b#"<<std::endl;
                        }
                        
                        arbitrage[i]->order->quantity -= arbitrage[i]->quant;
                    }
                    total_profit += max_profit;
                }
            }

        }

        std::cout<< total_profit << std::endl;
    }
    
    return 0;
}
