﻿#include <iostream>
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
        cout << "---- Aktualne akcje na rynku ----" << endl;
        for (auto& pair : stocks) {
            cout << pair.first << " (" << pair.second.name << "): $" << pair.second.price
                << " | Dostępne: " << pair.second.quantity << endl;
        }
        cout << "---------------------------------" << endl;
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
        cout << "Saldo: $" << balance << endl;
    }

    void showPortfolio() {
        cout << "---- Twój portfel ----" << endl;
        if (portfolio.empty()) {
            cout << "Brak akcji." << endl;
        }
        else {
            for (auto& pair : portfolio) {
                cout << pair.first << ": " << pair.second << " akcji" << endl;
            }
        }
        cout << "----------------------" << endl;
    }

    bool buyStock(Stock* stock, int quantity) {
        double cost = stock->price * quantity;
        if (stock->quantity < quantity) {
            cout << "Nie ma wystarczającej ilości akcji na rynku." << endl;
            return false;
        }
        if (cost > balance) {
            cout << "Niewystarczające środki." << endl;
            return false;
        }
        balance -= cost;
        stock->quantity -= quantity;
        portfolio[stock->name] += quantity;
        cout << "Zakupiłeś " << quantity << " akcji " << stock->name << "." << endl;
        return true;
    }

    bool sellStock(Stock* stock, int quantity) {
        if (portfolio[stock->name] < quantity) {
            cout << "Nie posiadasz wystarczającej liczby akcji." << endl;
            return false;
        }
        double revenue = stock->price * quantity;
        balance += revenue;
        stock->quantity += quantity;
        portfolio[stock->name] -= quantity;
        if (portfolio[stock->name] == 0) {
            portfolio.erase(stock->name);
        }
        cout << "Sprzedałeś " << quantity << " akcji " << stock->name << "." << endl;
        return true;
    }
};

int main() {
    setlocale(LC_CTYPE, "Polish");
    Market market;
    Investor investor(10000.0);

    int option;

    do {
        cout << "\n--- Symulacja giełdy ---" << endl;
        cout << "1. Pokaż rynek\n2. Kup akcje\n3. Sprzedaj akcje\n4. Pokaż portfel\n5. Pokaż saldo\n6. Aktualizuj ceny\n7. Wyjdź\n";
        cout << "Wybierz opcję: ";
        cin >> option;

        switch (option) {
        case 1:
            market.showMarket();
            break;
        case 2: {
            string symbol;
            int quantity;
            cout << "Podaj symbol akcji do kupienia: ";
            cin >> symbol;
            Stock* stock = market.getStock(symbol);
            if (stock) {
                cout << "Podaj ilość akcji: ";
                cin >> quantity;
                investor.buyStock(stock, quantity);
            }
            else {
                cout << "Nie znaleziono akcji." << endl;
            }
            break;
        }
        case 3: {
            string symbol;
            int quantity;
            cout << "Podaj symbol akcji do sprzedaży: ";
            cin >> symbol;
            Stock* stock = market.getStock(symbol);
            if (stock) {
                cout << "Podaj ilość akcji: ";
                cin >> quantity;
                investor.sellStock(stock, quantity);
            }
            else {
                cout << "Nie znaleziono akcji." << endl;
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
            cout << "Ceny zostały zaktualizowane!" << endl;
            break;
        case 7:
            cout << "Koniec symulacji." << endl;
            break;
        default:
            cout << "Nieprawidłowa opcja." << endl;
        }
    } while (option != 7);

    return 0;
}