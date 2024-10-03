#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include <ctime>
#include <locale.h>

using namespace std;

class Stock {
public:
    string name;
    double price;
    int quantity;

    Stock() : name("Unknown"), price(0.0), quantity(0) {}

    Stock(string name, double price, int quantity) : name(name), price(price), quantity(quantity) {}
};


class Market {
private:
    map<string, Stock> stocks;

public:
    Market() {
        stocks["AAPL"] = Stock("Apple", 150.0, 1000);
        stocks["GOOG"] = Stock("Google", 2800.0, 500);
        stocks["AMZN"] = Stock("Amazon", 3500.0, 300);
    }

    void showMarket() {
        cout << "---- Current Stocks on the Market ----" << endl;
        for (auto& pair : stocks) {
            cout << pair.first << " (" << pair.second.name << "): $" << pair.second.price
                << " | Available: " << pair.second.quantity << endl;
        }
        cout << "---------------------------------------" << endl;
    }

    void updatePrices() {
        srand(time(0));
        for (auto& pair : stocks) {
            double change = ((rand() % 2001) - 1000) / 100.0;
            pair.second.price += change;
            if (pair.second.price < 1) pair.second.price = 1;
        }
    }

    Stock* getStock(string symbol) {
        if (stocks.find(symbol) != stocks.end()) {
            return &stocks[symbol];
        }
        return nullptr;
    }
};

class Investor {
private:
    double balance;
    map<string, int> portfolio;

public:
    Investor(double initialBalance) : balance(initialBalance) {}

    void showBalance() {
        cout << "Balance: $" << balance << endl;
    }

    void showPortfolio() {
        cout << "---- Your Portfolio ----" << endl;
        if (portfolio.empty()) {
            cout << "No stocks." << endl;
        }
        else {
            for (auto& pair : portfolio) {
                cout << pair.first << ": " << pair.second << " shares" << endl;
            }
        }
        cout << "-----------------------" << endl;
    }

    bool buyStock(Stock* stock, int quantity) {
        double cost = stock->price * quantity;
        if (stock->quantity < quantity) {
            cout << "Not enough shares available on the market." << endl;
            return false;
        }
        if (cost > balance) {
            cout << "Insufficient funds." << endl;
            return false;
        }
        balance -= cost;
        stock->quantity -= quantity;
        portfolio[stock->name] += quantity;
        cout << "You bought " << quantity << " shares of " << stock->name << "." << endl;
        return true;
    }

    bool sellStock(Stock* stock, int quantity) {
        if (portfolio[stock->name] < quantity) {
            cout << "You do not own enough shares." << endl;
            return false;
        }
        double revenue = stock->price * quantity;
        balance += revenue;
        stock->quantity += quantity;
        portfolio[stock->name] -= quantity;
        if (portfolio[stock->name] == 0) {
            portfolio.erase(stock->name);
        }
        cout << "You sold " << quantity << " shares of " << stock->name << "." << endl;
        return true;
    }
};

int main() {
    setlocale(LC_CTYPE, "Polish");
    Market market;
    Investor investor(10000.0);

    int option;

    do {
        cout << "\n--- Stock Market Simulation ---" << endl;
        cout << "1. Show Market\n2. Buy Stock\n3. Sell Stock\n4. Show Portfolio\n5. Show Balance\n6. Update Prices\n7. Exit\n";
        cout << "Choose an option: ";
        cin >> option;

        switch (option) {
        case 1:
            market.showMarket();
            break;
        case 2: {
            string symbol;
            int quantity;
            cout << "Enter the stock symbol to buy: ";
            cin >> symbol;
            Stock* stock = market.getStock(symbol);
            if (stock) {
                cout << "Enter the number of shares: ";
                cin >> quantity;
                investor.buyStock(stock, quantity);
            }
            else {
                cout << "Stock not found." << endl;
            }
            break;
        }
        case 3: {
            string symbol;
            int quantity;
            cout << "Enter the stock symbol to sell: ";
            cin >> symbol;
            Stock* stock = market.getStock(symbol);
            if (stock) {
                cout << "Enter the number of shares: ";
                cin >> quantity;
                investor.sellStock(stock, quantity);
            }
            else {
                cout << "Stock not found." << endl;
            }
            break;
        }
        case 4:
            investor.showPortfolio();
            break;
        case 5:
            investor.showBalance();
            break;
        case 6:
            market.updatePrices();
            cout << "Prices have been updated!" << endl;
            break;
        case 7:
            cout << "End of simulation." << endl;
            break;
        default:
            cout << "Invalid option." << endl;
        }
    } while (option != 7);

    return 0;
}
