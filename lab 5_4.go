package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

var total_students int = 0         // Общее количество студентов
var suspected = make(map[int]bool) // Множество номеров студентов в списке на отчисление
var immortal_students = make(map[int]bool) // Множество номеров неприкасаемых студентов

// Обработка команды NEW_STUDENTS - добавление новых студентов
func new_students(number int) {
	// Если число отрицательное - это удаление студентов
	if number < 0 {
		fmt.Fprintln(os.Stderr, "GoodBye", -number, "clever students!")
		return
	}

	// Увеличиваем общее количество студентов
	total_students += number
	fmt.Println("Welcome", number, "clever students!")
}

// Обработка команды SUSPICIOUS - добавление студента в список на отчисление
func suspicious(number_student int) {
	// Проверка корректности номера студента
	if number_student <= 0 || number_student > total_students {
		fmt.Fprintln(os.Stderr, "Incorrect")
		return
	}

	// Если студент неприкасаемый - игнорируем команду
	if _, exists := immortal_students[number_student]; exists {
		return
	}

	// Добавляем студента в список на отчисление
	suspected[number_student] = true
	fmt.Println("The suspected student", number_student)
}

// Обработка команды IMMORTAL - добавление студента в список неприкасаемых
func immortal(number_student int) {
	// Проверка корректности номера студента
	if number_student <= 0 || number_student > total_students {
		fmt.Fprintln(os.Stderr, "Incorrect")
		return
	}

	// Удаляем студента из списка на отчисление (если он там был)
	delete(suspected, number_student)
	// Добавляем в список неприкасаемых
	immortal_students[number_student] = true
	fmt.Println("Student", number_student, "is immortal!")
}

// Обработка команды TOP-LIST - вывод списка студентов на отчисление
func top_list() {
	fmt.Print("List of students for expulsion:")
	first := true // Флаг для обработки первой записи (чтобы не ставить запятую перед ней)

	// Перебираем всех студентов в списке на отчисление
	for num := range suspected {
		if first {
			fmt.Print(" Student ", num)
			first = false
		} else {
			fmt.Print(", Student ", num)
		}
	}
	fmt.Println()
}

// Обработка команды SCOUNT - вывод количества студентов в списке на отчисление
func scount() {
	fmt.Println("List of students for expulsion consists of", len(suspected), "students")
}

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	scanner.Scan()
	n, _ := strconv.Atoi(scanner.Text())

	for i := 0; i < n; i++ {
		scanner.Scan()
		command := strings.Fields(scanner.Text())

		switch command[0] {
		case "NEW_STUDENTS":
			number, _ := strconv.Atoi(command[1])
			new_students(number)
		case "SUSPICIOUS":
			num, _ := strconv.Atoi(command[1])
			suspicious(num)
		case "IMMORTAL":
			num, _ := strconv.Atoi(command[1])
			immortal(num)
		case "TOP-LIST":
			top_list()
		case "SCOUNT":
			scount()
		}
	}
}
