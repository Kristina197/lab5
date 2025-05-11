#ifndef CREATE_TRL_H
#define CREATE_TRL_H

#include <string>
#include <vector>

using namespace std;

// Создание нового троллейбуса с указанным маршрутом
// trolleyName - название троллейбуса
// stops - список остановок на маршруте
void executeCreateTrolley(const string& trolleyName, const vector<string>& stops);

#endif // CREATE_TRL_H
