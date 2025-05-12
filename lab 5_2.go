package main

import (
	"fmt"
	"sort"
	"strings"
)

// Структура, описывающая окно приема
type Window struct {
	total   int       // Общее время обработки в этом окне
	tickets []string  // Список талонов в этом окне
	num     int       // Номер окна
}

// Структура, описывающая посетителя
type Visitor struct {
	time   int    // Продолжительность визита
	ticket string // Номер талона
}

// Функция распределения очереди по окнам
func distribute(queue []Visitor, numWindows int) []Window {
	windows := make([]Window, numWindows) // Создаем слайс окон с заданным количеством

	// Инициализируем номера окон
	for i := 0; i < numWindows; i++ {
		windows[i].num = i + 1
	}

	// Распределяем посетителей по окнам
	for _, visitor := range queue {
		// Находим окно с минимальным текущим временем обработки
		minIndex := 0
		for i := 1; i < len(windows); i++ {
			if windows[i].total < windows[minIndex].total {
				minIndex = i
			}
		}

		// Добавляем время посетителя к выбранному окну
		windows[minIndex].total += visitor.time
		// Добавляем талон посетителя в это окно
		windows[minIndex].tickets = append(windows[minIndex].tickets, visitor.ticket)
	}

	return windows
}

func main() {
	// Запрос количества окон
	var numWindows int
	fmt.Print(">>> Введите кол-во окон\n<<< ")
	fmt.Scan(&numWindows)

	// Очередь посетителей
	var queue []Visitor
	// Счетчик для генерации номеров талонов
	ticketNum := 1

	// Основной цикл обработки команд
	for {
		var command string
		fmt.Print("<<< ")
		fmt.Scan(&command)

		// Обработка команды ENQUEUE
		if command == "ENQUEUE" {
			var duration int
			fmt.Scan(&duration)

			// Генерация номера талона
			ticket := "T" + fmt.Sprintf("%03d", ticketNum)
			ticketNum++

			// Добавление посетителя в очередь
			queue = append(queue, Visitor{duration, ticket})
			fmt.Println(">>>", ticket)
		} else if command == "DISTRIBUTE" {
			// Обработка команды DISTRIBUTE
			windows := distribute(queue, numWindows)

			// Сортировка окон по номеру
			sort.Slice(windows, func(i, j int) bool {
				return windows[i].num < windows[j].num
			})

			// Вывод результатов распределения
			for _, window := range windows {
				fmt.Printf(">>> Окно %d (%d минут): %s\n", 
					window.num, window.total, strings.Join(window.tickets, ", "))
			}
			break // Завершение работы после распределения
		} else {
			// Обработка неизвестных команд
			fmt.Println(">>> Неизвестная команда")
		}
	}
}
