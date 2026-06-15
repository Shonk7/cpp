#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <queue>
#include <set>
#include <cassert>
#include <map>
#include <list>
#include <unordered_map>

using namespace std;

class OrderBook {
public:
    enum Side {
        Buy = 0,
        Sell = 1
    };

    struct AddOrderRequest {
        long long orderId;
        int price;
        long long amount;
        Side side;
    };

    struct Order {
        long long orderId;
        long long amount;
    };


    // need price level to decide which list we should access when deleting
    // need side to work out which map (bid or asks) we need to look at to find the price level
    struct Handle {
        Side side;
        int price;
        list<Order>::iterator it;
    };

    // add and match order, keep any remaining qty in backlog.
    void add_order(const AddOrderRequest& req) {
        long long remaining = req.amount;
        if (req.side == Buy) {
            // match which asks
            while (remaining > 0 && !asks.empty() && asks.begin()->first <= req.price) {
                auto& [askPrice, askList] = *asks.begin();
                while (remaining > 0 && !askList.empty()) {
                    // pop front of list defs need & cause we dont want copy we want to directly modify
                    Order& front = askList.front();
                    
                    // process matching orders
                    long long ordersFilled = min(remaining, front.amount);
                    remaining -= ordersFilled;
                    front.amount -= ordersFilled;
                    
                    // filled all orders for this one -> go to next
                    if (front.amount <= 0) {
                        // clear hashmap
                        index.erase(front.orderId);
                        askList.pop_front();
                    }
                }
                // remember that asks is a map so if you remove every elemetn in askList and make the list empty
                // the map is just poitning to add empty list need to clean this up by removing it from the map 
                if (askList.empty()) {
                    asks.erase(asks.begin());
                }

            }
            // if there is remaining orders it can be added to the order list for this level and that index (iderator) can 
            // be added to the hashamp for all orders for easy lookup (deletion
            if (remaining > 0) {
                bids[req.price].push_back({req.orderId, remaining});
                // get iterator to the end
                auto it = prev(bids[req.price].end());
                index[req.orderId] = {req.side, req.price, it};
            }
        } else {
            while (remaining > 0 && !bids.empty() && bids.begin()->first >= req.price) {
                auto& [bidPrice, bidList] = *bids.begin();
                while (remaining > 0 && !bidList.empty()) {
                    // pop front of list defs need & cause we dont want copy we want to directly modify
                    Order& front = bidList.front();
                    
                    // process matching orders
                    long long ordersFilled = min(remaining, front.amount);
                    remaining -= ordersFilled;
                    front.amount -= ordersFilled;
                    
                    // filled all orders for this one -> go to next
                    if (front.amount <= 0) {
                        // clear hashmap
                        index.erase(front.orderId);
                        bidList.pop_front();
                    }
                }
                if (bidList.empty()) {
                    bids.erase(bids.begin());
                }
            }
            if (remaining > 0) {
                asks[req.price].push_back({req.orderId, remaining});
                // get iterator to the end
                auto it = prev(asks[req.price].end());
                index[req.orderId] = {req.side, req.price, it};
            }
        }
    }

    // cancel by id, intended target O(1) average with hash lookup.
    bool cancel_order(long long orderId) {
        // find order
        auto found = index.find(orderId);
        if (found == index.end()) return false;

        Handle& han = found->second;
        if (han.side == Buy) {
            bids[han.price].erase(han.it);
            // if bids is now empty
            if (bids[han.price].empty()) {
                bids.erase(han.price);
            }
        } else {
            asks[han.price].erase(han.it);
            // if bids is now empty
            if (asks[han.price].empty()) {
                asks.erase(han.price);
            }
        }
        // delete from index hashmap
        index.erase(orderId);
        return true;
    }

    // highest buy price currently resting.
    int best_bid()  {
        if (bids.empty()) return -1;
        return bids.begin()->first;
    }

    // lowest sell price currently resting.
    int best_ask()  {
        if (asks.empty()) return -1;
        return asks.begin()->first;
    }

private:
    // bids/asks is a sorted hashmap of dlls for each order level
    map<int, list<Order>, greater<int>> bids;
    map<int, list<Order>> asks;
    // so we can jump straight to order from the orderID
    unordered_map<long long, Handle> index;
};

class Solution {
public:
    int getNumberOfBacklogOrders(vector<vector<int>>& orders) {
        // heaps
        // price amount
        priority_queue<pair<int, int>> buys;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> sells;
        for (auto& order : orders) {
            // check if other way compiles later
            auto price = order[0];
            auto amount = order[1];
            auto type = order[2];
            // buy
            if (type == 0) {
                // fill all orders we can
                while (amount > 0 && !sells.empty() && sells.top().first <= price) {
                    auto [sellPrice, sellAmount] = sells.top();
                    sells.pop();
                    // want to fill out the sell order by the buy order
                    // need to work out min
                    // orders filled
                    int mini = min(amount, sellAmount);
                    amount -= mini;
                    sellAmount -= mini;

                    // push back into heaps
                    if (sellAmount > 0) {
                        sells.push({sellPrice, sellAmount});
                    }
                }
                if (amount > 0) {
                    buys.push({price, amount});
                }
            } 
            // sell
            else {
                while (amount > 0 && !buys.empty() && buys.top().first >= price) {
                    auto [buyPrice, buyAmount] = buys.top();
                    buys.pop();

                    int mini = min(amount, buyAmount);
                    amount -= mini;
                    buyAmount -= mini;

                    if (buyAmount > 0) {
                        buys.push({buyPrice, buyAmount});
                    }
                }
                if (amount > 0) {
                    sells.push({price, amount});
                }
            }
        }
        long long count = 0;
        int MOD = 1e9 + 7;
        while (!buys.empty()) {
            count = (count + buys.top().second) % MOD;
            buys.pop();
        }
        while (!sells.empty()) {
            count = (count + sells.top().second) % MOD;
            sells.pop();
        }


        return count;
    }
};

void test_orderbook_core() {
    {
        OrderBook ob;
        assert(ob.best_bid() == -1);
        assert(ob.best_ask() == -1);
        assert(ob.cancel_order(42) == false);
    }

    {
        OrderBook ob;
        ob.add_order({1, 100, 10, OrderBook::Buy});
        ob.add_order({2, 99, 8, OrderBook::Buy});
        assert(ob.best_bid() == 100);
        assert(ob.best_ask() == -1);

        assert(ob.cancel_order(1) == true);
        assert(ob.best_bid() == 99);
        assert(ob.cancel_order(1) == false);
    }

    {
        OrderBook ob;
        ob.add_order({10, 103, 6, OrderBook::Sell});
        ob.add_order({11, 105, 5, OrderBook::Sell});
        assert(ob.best_ask() == 103);
        assert(ob.best_bid() == -1);

        // Buy crosses best ask and partially fills id 10 (6 -> 2).
        ob.add_order({12, 104, 4, OrderBook::Buy});
        assert(ob.best_bid() == -1);
        assert(ob.best_ask() == 103);

        // id 10 should still be resting with remaining qty, so cancel succeeds.
        assert(ob.cancel_order(10) == true);
        assert(ob.best_ask() == 105);
        assert(ob.cancel_order(10) == false);
    }

    {
        OrderBook ob;
        // FIFO at same price: id 20 should fill before id 21.
        ob.add_order({20, 100, 3, OrderBook::Sell});
        ob.add_order({21, 100, 3, OrderBook::Sell});
        ob.add_order({22, 100, 4, OrderBook::Buy});

        // id 20 fully matched (cancel fails), id 21 partially matched (cancel succeeds).
        assert(ob.cancel_order(20) == false);
        assert(ob.cancel_order(21) == true);
        assert(ob.best_ask() == -1);
        assert(ob.best_bid() == -1);
    }
}

void test_leetcode_1801() {
    Solution s;

    // Example 1
    vector<vector<int>> orders1 = {
        {10, 5, 0},
        {15, 2, 1},
        {25, 1, 1},
        {30, 4, 0}
    };
    assert(s.getNumberOfBacklogOrders(orders1) == 6);

    // Example 2
    vector<vector<int>> orders2 = {
        {7, 1000000000, 1},
        {15, 3, 0},
        {5, 999999995, 0},
        {5, 1, 1}
    };
    assert(s.getNumberOfBacklogOrders(orders2) == 999999984);

    // No matches, all remain in backlog.
    vector<vector<int>> orders3 = {
        {10, 2, 0},
        {11, 3, 0},
        {15, 4, 1}
    };
    assert(s.getNumberOfBacklogOrders(orders3) == 9);
}

int main() {
    test_leetcode_1801();
    test_orderbook_core();
    cout << "All tests passed\n";
    return 0;
}