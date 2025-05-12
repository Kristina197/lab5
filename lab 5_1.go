package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

// Константы склада
const (
	ZONES     = 1    // Количество зон (только 'A')
	RACKS     = 10   // Стеллажей в зоне
	SECTIONS  = 7    // Секций в стеллаже
	SHELVES   = 4    // Полок в секции
	MAX_ITEMS = 10   // Максимум товара в ячейке
)

// Структура для хранения информации о товаре
type ProductInfo struct {
	name     string
	quantity int
}

// Структура адреса ячейки
type CellAddress struct {
	zone    rune
	rack    int
	section int
	shelf   int
}

var warehouse = make(map[CellAddress]ProductInfo)

// Функция для разбора адреса ячейки из строки
func parseAddress(addressStr string) (CellAddress, bool) {
	var addr CellAddress
	parts := strings.Split(addressStr, "-")
	if len(parts) != 4 {
		return addr, false
	}

	addr.zone = rune(parts[0][0])
	rack, err := strconv.Atoi(parts[1])
	if err != nil {
		return addr, false
	}
	addr.rack = rack

	section, err := strconv.Atoi(parts[2])
	if err != nil {
		return addr, false
	}
	addr.section = section

	shelf, err := strconv.Atoi(parts[3])
	if err != nil {
		return addr, false
	}
	addr.shelf = shelf

	return addr, true
}

// Проверка корректности адреса
func isValidAddress(addr CellAddress) bool {
	return addr.zone == 'A' &&
		addr.rack >= 1 && addr.rack <= RACKS &&
		addr.section >= 1 && addr.section <= SECTIONS &&
		addr.shelf >= 1 && addr.shelf <= SHELVES
}

// Добавление товара
func addItems(productName string, quantity int, cellAddress string) {
	addr, ok := parseAddress(cellAddress)
	if !ok {
		fmt.Println("Ошибка: неверный формат адреса ячейки")
		return
	}

	if !isValidAddress(addr) {
		fmt.Println("Ошибка: неверный адрес ячейки")
		return
	}

	if quantity <= 0 {
		fmt.Println("Ошибка: количество должно быть положительным")
		return
	}

	current, exists := warehouse[addr]
	if exists {
		if current.name != productName {
			fmt.Printf("Ошибка: в ячейке уже находится другой товар: %s\n", current.name)
			return
		}
		if current.quantity+quantity > MAX_ITEMS {
			fmt.Printf("Ошибка: превышена вместимость ячейки (макс. %d)\n", MAX_ITEMS)
			fmt.Printf("Текущее количество: %d\n", current.quantity)
			return
		}
		current.quantity += quantity
	} else {
		current = ProductInfo{name: productName, quantity: quantity}
	}

	warehouse[addr] = current
	fmt.Printf("Добавлено %d единиц товара '%s' в ячейку %s\n", quantity, productName, cellAddress)
}

// Удаление товара
func removeItems(productName string, quantity int, cellAddress string) {
	addr, ok := parseAddress(cellAddress)
	if !ok {
		fmt.Println("Ошибка: неверный формат адреса ячейки")
		return
	}

	if !isValidAddress(addr) {
		fmt.Println("Ошибка: неверный адрес ячейки")
		return
	}

	if quantity <= 0 {
		fmt.Println("Ошибка: количество должно быть положительным")
		return
	}

	current, exists := warehouse[addr]
	if !exists {
		fmt.Println("Ошибка: ячейка пуста")
		return
	}

	if current.name != productName {
		fmt.Printf("Ошибка: в ячейке находится другой товар: %s\n", current.name)
		return
	}

	if current.quantity < quantity {
		fmt.Printf("Ошибка: недостаточно товара в ячейке\n")
		fmt.Printf("Доступно: %d, запрошено: %d\n", current.quantity, quantity)
		return
	}

	current.quantity -= quantity
	if current.quantity == 0 {
		delete(warehouse, addr)
	} else {
		warehouse[addr] = current
	}
	fmt.Printf("Удалено %d единиц товара '%s' из ячейки %s\n", quantity, productName, cellAddress)
}

// Получение информации о складе
func showInfo() {
	totalCells := ZONES * RACKS * SECTIONS * SHELVES
	usedCells := 0
	totalItems := 0
	var occupiedCells []struct {
		addr    CellAddress
		product ProductInfo
	}
	var emptyCells []CellAddress

	// Сбор статистики
	for r := 1; r <= RACKS; r++ {
		for s := 1; s <= SECTIONS; s++ {
			for sh := 1; sh <= SHELVES; sh++ {
				addr := CellAddress{'A', r, s, sh}
				product, exists := warehouse[addr]
				if exists {
					usedCells++
					totalItems += product.quantity
					occupiedCells = append(occupiedCells, struct {
						addr    CellAddress
						product ProductInfo
					}{addr, product})
				} else {
					emptyCells = append(emptyCells, addr)
				}
			}
		}
	}

	// Вывод информации
	fmt.Println("\nОБЩАЯ ИНФОРМАЦИЯ")
	fmt.Printf("Загруженность склада: %.2f%%\n", float64(usedCells)*100.0/float64(totalCells))
	fmt.Printf("Занято ячеек: %d из %d\n", usedCells, totalCells)
	fmt.Printf("Всего товара: %d единиц\n", totalItems)

	fmt.Println("\nЗАГРУЖЕННОСТЬ ЗОН")
	fmt.Printf("Зона A: %.2f%%\n", float64(usedCells)*100.0/float64(totalCells))

	fmt.Println("\nЗАНЯТЫЕ ЯЧЕЙКИ")
	if len(occupiedCells) == 0 {
		fmt.Println("Нет занятых ячеек")
	} else {
		for _, item := range occupiedCells {
			fmt.Printf("%c-%d-%d-%d: %s, %d ед.\n",
				item.addr.zone, item.addr.rack, item.addr.section, item.addr.shelf,
				item.product.name, item.product.quantity)
		}
	}

	fmt.Println("\nПУСТЫЕ ЯЧЕЙКИ")
	if len(emptyCells) == 0 {
		fmt.Println("Нет пустых ячеек")
	} else {
		for _, addr := range emptyCells {
			fmt.Printf("%c-%d-%d-%d\n", addr.zone, addr.rack, addr.section, addr.shelf)
		}
	}
}

func main() {
	fmt.Println("СИСТЕМА УЧЕТА ТОВАРОВ")
	fmt.Println("Доступные команды:")
	fmt.Println("ADD <название товара> <количество> <адрес ячейки (A-1-1-1)>")
	fmt.Println("REMOVE <название товара> <количество> <адрес ячейки (A-1-1-1)>")
	fmt.Println("INFO - информация о складе")
	fmt.Println("EXIT - выход")

	scanner := bufio.NewScanner(os.Stdin)
	for {
		fmt.Print("\n> ")
		if !scanner.Scan() {
			break
		}
		line := scanner.Text()
		fields := strings.Fields(line)
		if len(fields) == 0 {
			continue
		}

		command := fields[0]
		switch command {
		case "ADD":
			if len(fields) != 4 {
				fmt.Println("Ошибка: неверный формат команды")
				continue
			}
			quantity, err := strconv.Atoi(fields[2])
			if err != nil {
				fmt.Println("Ошибка: неверное количество")
				continue
			}
			addItems(fields[1], quantity, fields[3])

		case "REMOVE":
			if len(fields) != 4 {
				fmt.Println("Ошибка: неверный формат команды")
				continue
			}
			quantity, err := strconv.Atoi(fields[2])
			if err != nil {
				fmt.Println("Ошибка: неверное количество")
				continue
			}
			removeItems(fields[1], quantity, fields[3])

		case "INFO":
			showInfo()

		case "EXIT":
			return

		default:
			if command != "" {
				fmt.Println("Неизвестная команда")
			}
		}
	}
}
