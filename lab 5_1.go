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
	RACKS     = 10   // Количество стеллажей в зоне
	SECTIONS  = 7    // Количество секций в стеллаже
	SHELVES   = 4    // Количество полок в секции
	MAX_ITEMS = 10   // Максимальное количество товара в одной ячейке
)

// ProductInfo хранит информацию о товаре
type ProductInfo struct {
	name     string // Название товара
	quantity int    // Количество единиц товара
}

// CellAddress представляет адрес ячейки на складе
type CellAddress struct {
	zone    rune // Зона (A-Z)
	rack    int  // Номер стеллажа (1-10)
	section int  // Номер секции (1-7)
	shelf   int  // Номер полки (1-4)
}

// warehouse - основное хранилище данных о ячейках и товарах
var warehouse = make(map[CellAddress]ProductInfo)

// parseAddress преобразует строку адреса в структуру CellAddress
func parseAddress(addressStr string) (CellAddress, bool) {
	var addr CellAddress
	parts := strings.Split(addressStr, "-")
	if len(parts) != 4 { // Проверяем правильность формата
		return addr, false
	}

	// Проверяем зону на коректность ввода
	if len(parts[0]) != 1 || parts[0][0] < 'A' || parts[0][0] > 'Z' {
		return addr, false
	}
	addr.zone = rune(parts[0][0])

	// Проверяем номер стеллажа на коректность ввода
	rack, err := strconv.Atoi(parts[1])
	if err != nil {
		return addr, false
	}
	addr.rack = rack

	// Проверяем номер секции на коректность
	section, err := strconv.Atoi(parts[2])
	if err != nil {
		return addr, false
	}
	addr.section = section

	// Проверяем номер полки на коректность
	shelf, err := strconv.Atoi(parts[3])
	if err != nil {
		return addr, false
	}
	addr.shelf = shelf

	return addr, true
}

// isValidAddress проверяет, что адрес соответствует допустимым диапазонам
func isValidAddress(addr CellAddress) bool {
	return addr.zone == 'A' && // Только зона A
		addr.rack >= 1 && addr.rack <= RACKS &&
		addr.section >= 1 && addr.section <= SECTIONS &&
		addr.shelf >= 1 && addr.shelf <= SHELVES
}

// addItems добавляет товар в указанную ячейку склада
func addItems(productName string, quantity int, cellAddress string) {
	// Парсим адрес ячейки
	addr, ok := parseAddress(cellAddress)
	if !ok {
		fmt.Println("Ошибка: неверный формат адреса ячейки")
		return
	}

	// Проверяем валидность адреса
	if !isValidAddress(addr) {
		fmt.Println("Ошибка: неверный адрес ячейки")
		return
	}

	// Проверяем положительное количество
	if quantity <= 0 {
		fmt.Println("Ошибка: количество должно быть положительным")
		return
	}

	// Получаем текущее содержимое ячейки
	current, exists := warehouse[addr]

	if exists {
		// Проверяем совпадение названия товара
		if current.name != productName {
			fmt.Printf("Ошибка: в ячейке уже находится другой товар: %s\n", current.name)
			return
		}

		// Проверяем не превысим ли максимальную вместимость
		if current.quantity+quantity > MAX_ITEMS {
			fmt.Printf("Ошибка: превышена вместимость ячейки (макс. %d)\n", MAX_ITEMS)
			fmt.Printf("Текущее количество: %d\n", current.quantity)
			return
		}

		// Увеличиваем количество товара
		current.quantity += quantity
	} else {
		// Создаем новую запись о товаре
		current = ProductInfo{name: productName, quantity: quantity}
	}

	// Обновляем данные в хранилище
	warehouse[addr] = current
	fmt.Printf("Добавлено %d единиц товара '%s' в ячейку %s\n", quantity, productName, cellAddress)
}

// removeItems удаляет товар из указанной ячейки склада
func removeItems(productName string, quantity int, cellAddress string) {
	
	addr, ok := parseAddress(cellAddress)
	if !ok {
		fmt.Println("Ошибка: неверный формат адреса ячейки")
		return
	}

	// Проверяем правильность адреса
	if !isValidAddress(addr) {
		fmt.Println("Ошибка: неверный адрес ячейки")
		return
	}

	// Проверяем положительное количество
	if quantity <= 0 {
		fmt.Println("Ошибка: количество должно быть положительным")
		return
	}

	// Проверяем существует ли ячейка
	current, exists := warehouse[addr]
	if !exists {
		fmt.Println("Ошибка: ячейка пуста")
		return
	}

	// Проверяем совпадение названия товара
	if current.name != productName {
		fmt.Printf("Ошибка: в ячейке находится другой товар: %s\n", current.name)
		return
	}

	// Проверяем достаточно ли товара для удаления
	if current.quantity < quantity {
		fmt.Printf("Ошибка: недостаточно товара в ячейке\n")
		fmt.Printf("Доступно: %d, запрошено: %d\n", current.quantity, quantity)
		return
	}

	// Уменьшаем количество товара
	current.quantity -= quantity

	// Если товар закончился, удаляем запись о ячейке
	if current.quantity == 0 {
		delete(warehouse, addr)
	} else {
		warehouse[addr] = current
	}
	fmt.Printf("Удалено %d единиц товара '%s' из ячейки %s\n", quantity, productName, cellAddress)
}

// showInfo выводит полную информацию о состоянии склада
func showInfo() {
	// Рассчитываем общее количество ячеек
	totalCells := ZONES * RACKS * SECTIONS * SHELVES
	usedCells := 0
	totalItems := 0

	// Срезы для хранения занятых и свободных ячеек
	var occupiedCells []struct {
		addr    CellAddress
		product ProductInfo
	}
	var emptyCells []CellAddress

	// Сканируем все возможные ячейки
	for r := 1; r <= RACKS; r++ {
		for s := 1; s <= SECTIONS; s++ {
			for sh := 1; sh <= SHELVES; sh++ {
				addr := CellAddress{'A', r, s, sh}
				product, exists := warehouse[addr]
				
				if exists {
					usedCells++
					totalItems += product.quantity
					// Добавляем в список занятых
					occupiedCells = append(occupiedCells, struct {
						addr    CellAddress
						product ProductInfo
					}{addr, product})
				} else {
					// Добавляем в список пустых
					emptyCells = append(emptyCells, addr)
				}
			}
		}
	}

	// Выводим общую информацию
	fmt.Println("\nОБЩАЯ ИНФОРМАЦИЯ")
	fmt.Printf("Загруженность склада: %.2f%%\n", float64(usedCells)*100.0/float64(totalCells))
	fmt.Printf("Занято ячеек: %d из %d\n", usedCells, totalCells)
	fmt.Printf("Всего товара: %d единиц\n", totalItems)

	// Выводим информацию по зонам
	fmt.Println("\nЗАГРУЖЕННОСТЬ ЗОН")
	fmt.Printf("Зона A: %.2f%%\n", float64(usedCells)*100.0/float64(totalCells))

	// Выводим список занятых ячеек
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

	// Выводим список пустых ячеек
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
	// Выводим приветственное сообщение и инструкции
	fmt.Println("СИСТЕМА УЧЕТА ТОВАРОВ")
	fmt.Println("Доступные команды:")
	fmt.Println("ADD <название товара> <количество> <адрес ячейки (A-1-1-1)>")
	fmt.Println("REMOVE <название товара> <количество> <адрес ячейки (A-1-1-1)>")
	fmt.Println("INFO - информация о складе")
	fmt.Println("EXIT - выход")

	// Создаем сканнер для чтения ввода пользователя
	scanner := bufio.NewScanner(os.Stdin)

	// Основной цикл программы
	for {
		fmt.Print("\n> ") // Приглашение к вводу
		if !scanner.Scan() {
			break // Выход при ошибке чтения
		}

		line := scanner.Text()    // Читаем введенную строку
		fields := strings.Fields(line) // Разбиваем на отдельные слова
		if len(fields) == 0 {
			continue // Пустая строка - пропускаем
		}

		command := fields[0] // Первое слово - команда
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
