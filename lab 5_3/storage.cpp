#include "storage.h"
#include <algorithm>

using namespace std;

// Внутренние структуры для хранения данных
namespace {
    // Основное хранилище маршрутов: троллейбус - остановки
    map<string, vector<string>> trolleyRoutes;
    
    // Хранилище для быстрого поиска: остановка - троллейбусы
    map<string, set<string>> stopToTrolleys;
}

// Инициализация хранилища (очистка всех данных)
void initializeStorage() {
    trolleyRoutes.clear();
    stopToTrolleys.clear();
}

// Добавление маршрута троллейбуса
void addTrolleyRoute(const string& trolleyName, const vector<string>& stops) {
    // Проверяем, существует ли уже такой троллейбус
    auto it = trolleyRoutes.find(trolleyName);
    if (it != trolleyRoutes.end()) {
        // Удаляем старые связи остановок с этим троллейбусом
        for (const auto& stop : it->second) {
            stopToTrolleys[stop].erase(trolleyName);
            // Если для остановки больше нет троллейбусов, удаляем её
            if (stopToTrolleys[stop].empty()) {
                stopToTrolleys.erase(stop);
            }
        }
    }
    
    // Добавляем новый маршрут
    trolleyRoutes[trolleyName] = stops;
    
    // Обновляем информацию по остановкам
    for (const auto& stop : stops) {
        stopToTrolleys[stop].insert(trolleyName);
    }
}

// Получение троллейбусов для остановки
set<string> getTrolleysForStop(const string& stop) {
    auto it = stopToTrolleys.find(stop);
    // Если остановка найдена, возвращаем список троллейбусов, если нет то пустое множество
    return it != stopToTrolleys.end() ? it->second : set<string>{};
}

// Получение информации об остановках троллейбуса
map<string, set<string>> getStopsForTrolley(const string& trolleyName) {
    map<string, set<string>> result;
    
    // Ищем троллейбус в системе
    auto it = trolleyRoutes.find(trolleyName);
    if (it != trolleyRoutes.end()) {
        // Для каждой остановки этого троллейбуса
        for (const auto& stop : it->second) {
            set<string> trolleysAtStop;
            // Находим все троллейбусы на этой остановке
            for (const auto& trolley : stopToTrolleys[stop]) {
                // Исключаем текущий троллейбус из результата
                if (trolley != trolleyName) {
                    trolleysAtStop.insert(trolley);
                }
            }
            // Добавляем в результат только остановки с пересадками
            if (!trolleysAtStop.empty()) {
                result[stop] = trolleysAtStop;
            }
        }
    }
    
    return result;
}

// Получение информации о всех троллейбусах
map<string, vector<string>> getAllTrolleys() {
    return trolleyRoutes;
}
