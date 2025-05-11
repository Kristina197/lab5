#include "stops_in_trl.h"
#include "storage.h"
#include <iostream>

using namespace std;

// Обработка команды получения остановок троллейбуса
void executeStopsInTrolley(const string& trolleyName) {
    // Получаем информацию об остановках
    auto stopsInfo = getStopsForTrolley(trolleyName);
    
    // Выводим результат
    if (stopsInfo.empty()) {
        cout << "No information available for trolley " << trolleyName << endl;
    } else {
        cout << "Stops for trolley " << trolleyName << " with connecting trolleys:" << endl;
        // Для каждой остановки выводим возможные пересадки
        for (const auto& [stop, trolleys] : stopsInfo) {
            cout << "- " << stop << " (connecting trolleys: ";
            for (const auto& trolley : trolleys) {
                cout << trolley << " ";
            }
            cout << ")" << endl;
        }
    }
}
