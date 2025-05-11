#include <iostream>   
#include <vector>     
#include <algorithm>  
#include <iomanip>    // Для форматирования вывода (setw, setfill)
#include <sstream>    // Для работы со строками (ostringstream)

using namespace std;  

// Структура, описывающая окно приема
struct Window {
    int total = 0;      // Общее время обработки в этом окне
    vector<string> tickets;  // Список талонов в этом окне
    int num;          // Номер окна
};

// Структура, описывающая посетителя
struct Visitor {
    int time;           // Продолжительность визита 
    string ticket;          // Номер талона
};

// Функция распределения очереди по окнам
vector<Window> distribute(const vector<Visitor>& queue, int num_windows) {
    vector<Window> windows(num_windows);// Создаем вектор окон с заданным количеством
    
    // Инициализируем номера окон
    for (int i = 0; i < num_windows; ++i) {
        windows[i].num = i + 1;
    }

    // Распределяем посетителей по окнам
    for (const auto& visitor : queue) {
        // Находим окно с минимальным текущим временем обработки
        auto window = min_element(windows.begin(), windows.end(),
            [](const Window& a, const Window& b) {
                return a.total < b.total;
            });
        
        // Добавляем время посетителя к выбранному окну
        window->total += visitor.time;
        // Добавляем талон посетителя в это окно
        window->tickets.push_back(visitor.ticket);
    }

    return windows;
}

int main() {
    // Запрос количества окон
    int num_windows;
    cout << ">>> Введите кол-во окон\n<<< ";
    cin >> num_windows;

    // Очередь посетителей
    vector<Visitor> queue;
    // Счетчик для генерации номеров талонов
    int ticket_num = 1;

    // Основной цикл обработки команд
    while (true) {
        string command;
        cout << "<<< ";
        cin >> command;

        // Обработка команды ENQUEUE
        if (command == "ENQUEUE") {
            int enqueue;
            cin >> enqueue;
            
            // Генерация номера талона
            ostringstream num;
            num << "T" << setw(3) << setfill('0') << ticket_num++;
            string ticket = num.str();
            
            // Добавление посетителя в очередь
            queue.push_back({enqueue, ticket});
            cout << ">>> " << ticket << '\n';
        }
        // Обработка команды DISTRIBUTE
        else if (command == "DISTRIBUTE") {
            // Распределение очереди по окнам
            auto windows = distribute(queue, num_windows);
            
            // Сортировка окон по номеру
            sort(windows.begin(), windows.end(),
                [](const Window& a, const Window& b) {
                    return a.num < b.num;
                });

            // Вывод результатов распределения
            for (const auto& window : windows) {
                cout << ">>> Окно " << window.num << " (" << window.total << " минут): ";
                
                // Вывод списка талонов через запятую
                bool first = true;
                for (const auto& ticket : window.tickets) {
                    if (!first) cout << ", ";
                    cout << ticket;
                    first = false;
                }
                cout << '\n';
            }
            break;  // Завершение работы после распределения
        }
        // Обработка неизвестных команд
        else {
            cout << ">>> Неизвестная команда\n";
        }
    }

    return 0;
}