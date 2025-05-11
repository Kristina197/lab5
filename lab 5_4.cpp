#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>

using namespace std;

int total_students = 0;         // Общее количество студентов
set<int> suspected;             // Множество номеров студентов в списке на отчисление
set<int> immortal_students;              // Множество номеров неприкасаемых студентов

// Обработка команды NEW_STUDENTS - добавление новых студентов
void new_students(int number) {
    // Если число отрицательное - это удаление студентов
    if (number < 0) {
        cerr << "GoodBye " << -number << " clever students!" << endl;
        return;
    }
    
    // Увеличиваем общее количество студентов
    total_students += number;
    cout << "Welcome " << number << " clever students!" << endl;
}

// Обработка команды SUSPICIOUS - добавление студента в список на отчисление
void suspicious(int number_student) {
    // Проверка корректности номера студента
    if (number_student <= 0 || number_student > total_students) {
        cerr << "Incorrect" << endl;
        return;
    }
    
    // Если студент неприкасаемый - игнорируем команду
    if (immortal_students.find(number_student) != immortal_students.end()) {
        return;
    }
    
    // Добавляем студента в список на отчисление
    suspected.insert(number_student);
    cout << "The suspected student " << number_student << endl;
}

// Обработка команды IMMORTAL - добавление студента в список неприкасаемых
void immortal(int number_student) {
    // Проверка корректности номера студента
    if (number_student <= 0 || number_student > total_students) {
        cerr << "Incorrect" << endl;
        return;
    }
    
    // Удаляем студента из списка на отчисление (если он там был)
    suspected.erase(number_student);
    // Добавляем в список неприкасаемых
    immortal_students.insert(number_student);
    cout << "Student " << number_student << " is immortal!" << endl;
}

// Обработка команды TOP-LIST - вывод списка студентов на отчисление
void top_list() {
    cout << "List of students for expulsion:";
    bool first = true;  // Флаг для обработки первой записи (чтобы не ставить запятую перед ней)
    
    // Перебираем всех студентов в списке на отчисление
    for (int num : suspected) {
        if (first) {
            cout << " Student " << num;
            first = false;
        } else {
            cout << ", Student " << num;
        }
    }
    cout << endl;
}

// Обработка команды SCOUNT - вывод количества студентов в списке на отчисление
void scount() {
    cout << "List of students for expulsion consists of " << suspected.size() << " students" << endl;
}

int main() {
    int n;  // Количество команд
    cin >> n;
    
    // Обработка всех команд
    for (int i = 0; i < n; ++i) {
        string command;
        cin >> command;
        
        if (command == "NEW_STUDENTS") {
            int number;
            cin >> number;
            new_students(number);
        }
        else if (command == "SUSPICIOUS") {
            int num;
            cin >> num;
            suspicious(num);
        }
        else if (command == "IMMORTAL") {
            int num;
            cin >> num;
            immortal(num);
        }
        else if (command == "TOP-LIST") {
            top_list();
        }
        else if (command == "SCOUNT") {
            scount();
        }
    }
    
    return 0;
}