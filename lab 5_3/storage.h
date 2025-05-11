#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

// Инициализация хранилища (очистка всех данных)
void initializeStorage();

// Добавление маршрута троллейбуса в систему
// trolleyName - название троллейбуса
// stops - вектор остановок на маршруте
void addTrolleyRoute(const string& trolleyName, const vector<string>& stops);

// Получение всех троллейбусов, проходящих через указанную остановку
// stop - название остановки
// возвращает множество названий троллейбусов
set<string> getTrolleysForStop(const string& stop);

// Получение информации об остановках для конкретного троллейбуса
// trolleyName - название троллейбуса
// возвращает map: ключ - остановка, значение - множество троллейбусов (кроме текущего)
map<string, set<string>> getStopsForTrolley(const string& trolleyName);

// Получение информации о всех троллейбусах в системе
// возвращает map: ключ - название троллейбуса, значение - вектор остановок
map<string, vector<string>> getAllTrolleys();

#endif // STORAGE_H
