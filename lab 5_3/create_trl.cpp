#include "create_trl.h"
#include "storage.h"
#include <iostream>

using namespace std;

// Обработка команды создания троллейбуса
void executeCreateTrolley(const string& trolleyName, const vector<string>& stops) {
    // Проверка на пустой список остановок
    if (stops.empty()) {
        cout << "Error: No stops provided for trolley " << trolleyName << endl;
        return;
    }
    
    // Добавление маршрута в систему
    addTrolleyRoute(trolleyName, stops);
    cout << "Trolley " << trolleyName << " route created with " << stops.size() << " stops." << endl;
}
