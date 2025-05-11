#include <iostream>
#include <sstream>
#include <vector>
#include "create_trl.h"
#include "trl_in_stop.h"
#include "stops_in_trl.h"
#include "trls.h"
#include "storage.h"

using namespace std;

// Типы поддерживаемых команд
enum class CommandType {
    CREATE_TRL,    // Создание троллейбуса
    TRL_IN_STOP,   // Троллейбусы на остановке
    STOPS_IN_TRL,  // Остановки троллейбуса
    TRLS,          // Все троллейбусы
    UNKNOWN        // Неизвестная команда
};

// Определение типа команды по строке
CommandType parseCommandType(const string& commandStr) {
    if (commandStr == "CREATE_TRL") return CommandType::CREATE_TRL;
    if (commandStr == "TRL_IN_STOP") return CommandType::TRL_IN_STOP;
    if (commandStr == "STOPS_IN_TRL") return CommandType::STOPS_IN_TRL;
    if (commandStr == "TRLS") return CommandType::TRLS;
    return CommandType::UNKNOWN;
}

// Обработка введенной команды
void processCommand(const string& commandLine) {
    istringstream iss(commandLine);
    string commandStr;
    iss >> commandStr;  // Извлекаем первое слово (команду)
    
    // Определяем тип команды
    CommandType command = parseCommandType(commandStr);
    
    // Выполняем соответствующую команду
    switch (command) {
        case CommandType::CREATE_TRL: {
            string trolleyName;
            iss >> trolleyName;  // Извлекаем название троллейбуса
            
            vector<string> stops;
            string stop;
            // Извлекаем все остальные слова как остановки
            while (iss >> stop) {
                stops.push_back(stop);
            }
            
            executeCreateTrolley(trolleyName, stops);
            break;
        }
        
        case CommandType::TRL_IN_STOP: {
            string stop;
            iss >> stop;  // Извлекаем название остановки
            executeTrolleysInStop(stop);
            break;
        }
        
        case CommandType::STOPS_IN_TRL: {
            string trolleyName;
            iss >> trolleyName;  // Извлекаем название троллейбуса
            executeStopsInTrolley(trolleyName);
            break;
        }
        
        case CommandType::TRLS: {
            executeListAllTrolleys();
            break;
        }
        
        case CommandType::UNKNOWN:
        default:
            cout << "Unknown command: " << commandStr << endl;
            break;
    }
}

int main() {
    // Инициализация хранилища
    initializeStorage();
    
    // Приветственное сообщение
    cout << "Trolley route management system" << endl;
    cout << "Available commands:" << endl;
    cout << "CREATE_TRL trl stop1 ... stopN" << endl;
    cout << "TRL_IN_STOP stop" << endl;
    cout << "STOPS_IN_TRL trl" << endl;
    cout << "TRLS" << endl;
    cout << "Enter 'exit' to quit" << endl << endl;
    
    // Основной цикл обработки команд
    string commandLine;
    while (true) {
        cout << "> ";
        getline(cin, commandLine);  // Чтение всей строки команды
        
        // Выход из программы
        if (commandLine == "exit") {
            break;
        }
        
        // Обработка непустой команды
        if (!commandLine.empty()) {
            processCommand(commandLine);
        }
    }
    
    return 0;
}
