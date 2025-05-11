#include "trls.h"
#include "storage.h"
#include <iostream>

using namespace std;

// Обработка команды вывода всех троллейбусов
void executeListAllTrolleys() {
    // Получаем информацию о всех троллейбусах
    auto allTrolleys = getAllTrolleys();
    
    // Выводим результат
    if (allTrolleys.empty()) {
        cout << "No trolleys registered in the system." << endl;
    } else {
        cout << "All trolleys and their routes:" << endl;
        // Для каждого троллейбуса выводим его остановки
        for (const auto& [trolley, stops] : allTrolleys) {
            cout << "- " << trolley << ": ";
            for (const auto& stop : stops) {
                cout << stop << " ";
            }
            cout << endl;
        }
    }
}
