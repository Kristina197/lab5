#include "trl_in_stop.h"
#include "storage.h"
#include <iostream>

using namespace std;

// Обработка команды получения троллейбусов для остановки
void executeTrolleysInStop(const string& stop) {
    // Получаем список троллейбусов
    auto trolleys = getTrolleysForStop(stop);
    
    // Выводим результат
    if (trolleys.empty()) {
        cout << "No trolleys pass through stop " << stop << endl;
    } else {
        cout << "Trolleys passing through " << stop << ":" << endl;
        for (const auto& trolley : trolleys) {
            cout << "- " << trolley << endl;
        }
    }
}
