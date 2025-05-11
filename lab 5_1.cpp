#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <sstream>

using namespace std;

// Константы склада
const int ZONES = 1;          // Количество зон (только 'A')
const int RACKS = 10;         // Стеллажей в зоне
const int SECTIONS = 7;       // Секций в стеллаже
const int SHELVES = 4;        // Полок в секции
const int MAX_ITEMS = 10;     // Максимум товара в ячейке

// Структура для хранения информации о товаре
struct ProductInfo {
    string name;
    int quantity;
};

// Структура адреса ячейки
struct CellAddress {
    char zone;
    int rack;
    int section;
    int shelf;

    bool operator<(const CellAddress& other) const {
        if (zone != other.zone) return zone < other.zone;
        if (rack != other.rack) return rack < other.rack;
        if (section != other.section) return section < other.section;
        return shelf < other.shelf;
    }
};

map<CellAddress, ProductInfo> warehouse;

// Функция для разбора адреса ячейки из строки
bool parseAddress(const string& addressStr, CellAddress& addr) {
    char sep;
    istringstream addrStream(addressStr);
    if (!(addrStream >> addr.zone >> sep >> addr.rack >> sep >> addr.section >> sep >> addr.shelf) || sep != '-') {
        return false;
    }
    return true;
}

// Проверка корректности адреса
bool isValidAddress(const CellAddress& addr) {
    return (addr.zone == 'A') && 
           (addr.rack >= 1 && addr.rack <= RACKS) &&
           (addr.section >= 1 && addr.section <= SECTIONS) &&
           (addr.shelf >= 1 && addr.shelf <= SHELVES);
}

// Добавление товара
void addItems(const string& productName, int quantity, const string& cellAddress) {
    CellAddress addr;
    if (!parseAddress(cellAddress, addr)) {
        cout << "Ошибка: неверный формат адреса ячейки. Используйте формат: A-1-1-1\n";
        return;
    }

    if (!isValidAddress(addr)) {
        cout << "Ошибка: неверный адрес ячейки\n";
        return;
    }

    if (quantity <= 0) {
        cout << "Ошибка: количество должно быть положительным\n";
        return;
    }

    auto& current = warehouse[addr];
    
    if (current.quantity + quantity > MAX_ITEMS) {
        cout << "Ошибка: превышена вместимость ячейки (макс. " << MAX_ITEMS << ")\n";
        cout << "Текущее количество: " << current.quantity << "\n";
    } else {
        if (current.quantity == 0) {
            current.name = productName;
            current.quantity = quantity;
        } else if (current.name != productName) {
            cout << "Ошибка: в ячейке уже находится другой товар: " << current.name << "\n";
            return;
        } else {
            current.quantity += quantity;
        }
        
        cout << "Добавлено " << quantity << " единиц товара '" << productName 
             << "' в ячейку " << cellAddress << "\n";
    }
}

// Удаление товара
void removeItems(const string& productName, int quantity, const string& cellAddress) {
    CellAddress addr;
    if (!parseAddress(cellAddress, addr)) {
        cout << "Ошибка: неверный формат адреса ячейки. Используйте формат: A-1-1-1\n";
        return;
    }

    if (!isValidAddress(addr)) {
        cout << "Ошибка: неверный адрес ячейки\n";
        return;
    }

    if (quantity <= 0) {
        cout << "Ошибка: количество должно быть положительным\n";
        return;
    }

    auto it = warehouse.find(addr);
    if (it == warehouse.end()) {
        cout << "Ошибка: ячейка пуста\n";
        return;
    }

    if (it->second.name != productName) {
        cout << "Ошибка: в ячейке находится другой товар: " << it->second.name << "\n";
        return;
    }

    if (it->second.quantity < quantity) {
        cout << "Ошибка: недостаточно товара в ячейке\n";
        cout << "Доступно: " << it->second.quantity << ", запрошено: " << quantity << "\n";
    } else {
        it->second.quantity -= quantity;
        cout << "Удалено " << quantity << " единиц товара '" << productName 
             << "' из ячейки " << cellAddress << "\n";
        
        if (it->second.quantity == 0) {
            warehouse.erase(it);
        }
    }
}

// Получение информации о складе
void showInfo() {
    const int totalCells = ZONES * RACKS * SECTIONS * SHELVES;
    int usedCells = 0;
    int totalItems = 0;
    vector<pair<CellAddress, ProductInfo>> occupiedCells;
    vector<CellAddress> emptyCells;

    // Сбор статистики
    for (int r = 1; r <= RACKS; r++) {
        for (int s = 1; s <= SECTIONS; s++) {
            for (int sh = 1; sh <= SHELVES; sh++) {
                CellAddress addr{'A', r, s, sh};
                auto it = warehouse.find(addr);
                
                if (it != warehouse.end()) {
                    usedCells++;
                    totalItems += it->second.quantity;
                    occupiedCells.emplace_back(*it);
                } else {
                    emptyCells.push_back(addr);
                }
            }
        }
    }

    // Вывод информации
    cout << "\nОБЩАЯ ИНФОРМАЦИЯ\n";
    cout << "Загруженность склада: " 
         << fixed << setprecision(2) 
         << (usedCells * 100.0 / totalCells) << "%\n";
    cout << "Занято ячеек: " << usedCells << " из " << totalCells << "\n";
    cout << "Всего товара: " << totalItems << " единиц\n";

    cout << "\nЗАГРУЖЕННОСТЬ ЗОН\n";
    cout << "Зона A: " 
         << fixed << setprecision(2) 
         << (usedCells * 100.0 / totalCells) << "%\n";

    cout << "\nЗАНЯТЫЕ ЯЧЕЙКИ\n";
    if (occupiedCells.empty()) {
        cout << "Нет занятых ячеек\n";
    } else {
        for (const auto& [addr, product] : occupiedCells) {
            cout << addr.zone << "-" << addr.rack << "-" 
                 << addr.section << "-" << addr.shelf 
                 << ": " << product.name << ", " << product.quantity << " ед.\n";
        }
    }

    cout << "\nПУСТЫЕ ЯЧЕЙКИ\n";
    if (emptyCells.empty()) {
        cout << "Нет пустых ячеек\n";
    } else {
        for (const auto& addr : emptyCells) {
            cout << addr.zone << "-" << addr.rack << "-" 
                 << addr.section << "-" << addr.shelf << "\n";
        }
    }
}

int main() {
    cout << "СИСТЕМА УЧЕТА ТОВАРОВ\n";
    cout << "Доступные команды:\n"
         << "ADD <название товара> <количество> <адрес ячейки (A-1-1-1)>\n"
         << "REMOVE <название товара> <количество> <адрес ячейки (A-1-1-1)>\n"
         << "INFO - информация о складе\n"
         << "EXIT - выход\n";

    string line;
    while (true) {
        cout << "\n> ";
        getline(cin, line);
        istringstream iss(line);
        string command;
        iss >> command;

        if (command == "ADD") {
            string productName, cellAddress;
            int quantity;
            
            if (!(iss >> productName >> quantity >> cellAddress)) {
                cout << "Ошибка: неверный формат команды. Пример: ADD Апельсины 8 A-1-1-1\n";
                continue;
            }
            addItems(productName, quantity, cellAddress);
        }
        else if (command == "REMOVE") {
            string productName, cellAddress;
            int quantity;
            
            if (!(iss >> productName >> quantity >> cellAddress)) {
                cout << "Ошибка: неверный формат команды. Пример: REMOVE Апельсины 8 A-1-1-1\n";
                continue;
            }
            removeItems(productName, quantity, cellAddress);
        }
        else if (command == "INFO") {
            showInfo();
        }
        else if (command == "EXIT") {
            break;
        }
        else if (!command.empty()) {
            cout << "Неизвестная команда\n";
        }
    }

    return 0;
}