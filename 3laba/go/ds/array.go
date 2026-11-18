package ds

import (
	"bufio"
	"strings"
)

// Array — динамический массив строк (как Array в C++).
// Поля с маленькой буквы закрыты (private), методы с большой — открыты (public).
type Array struct {
	data []string // выделенная память, len(data) — ёмкость
	size int      // сколько элементов записано
}

func NewArray() *Array {
	return &Array{data: make([]string, 10)}
}

// grow увеличивает ёмкость в 2 раза, когда массив заполнен.
func (a *Array) grow() {
	newCapacity := len(a.data) * 2
	if newCapacity == 0 {
		newCapacity = 10
	}
	newData := make([]string, newCapacity)
	copy(newData, a.data[:a.size])
	a.data = newData
}

func (a *Array) PushBack(value string) {
	if a.size == len(a.data) {
		a.grow()
	}
	a.data[a.size] = value
	a.size++
}

// Insert вставляет по индексу; index == Size() — вставка в конец.
func (a *Array) Insert(index int, value string) bool {
	if index < 0 || index > a.size {
		return false
	}
	if a.size == len(a.data) {
		a.grow()
	}
	for i := a.size; i > index; i-- {
		a.data[i] = a.data[i-1]
	}
	a.data[index] = value
	a.size++
	return true
}

func (a *Array) Get(index int) (string, bool) {
	if index < 0 || index >= a.size {
		return "", false
	}
	return a.data[index], true
}

func (a *Array) Set(index int, value string) bool {
	if index < 0 || index >= a.size {
		return false
	}
	a.data[index] = value
	return true
}

func (a *Array) Delete(index int) bool {
	if index < 0 || index >= a.size {
		return false
	}
	for i := index; i < a.size-1; i++ {
		a.data[i] = a.data[i+1]
	}
	a.size--
	a.data[a.size] = ""
	return true
}

func (a *Array) Size() int {
	return a.size
}

// String — все элементы через пробел.
func (a *Array) String() string {
	return strings.Join(a.data[:a.size], " ")
}

// ---------- Сериализация ----------
// Load* читает во временный объект и только при успехе заменяет текущий,
// поэтому при ошибке массив не меняется.

func (a *Array) SaveText(fileName string) error {
	return saveFile(fileName, func(w *bufio.Writer) {
		for i := 0; i < a.size; i++ {
			writeLine(w, a.data[i])
		}
	})
}

func (a *Array) LoadText(fileName string) error {
	return loadFile(fileName, func(r *bufio.Reader) error {
		tmp := NewArray()
		for _, word := range readWords(r) {
			tmp.PushBack(word)
		}
		*a = *tmp
		return nil
	})
}

func (a *Array) SaveBinary(fileName string) error {
	return saveFile(fileName, func(w *bufio.Writer) {
		writeInt(w, a.size)
		for i := 0; i < a.size; i++ {
			writeString(w, a.data[i])
		}
	})
}

func (a *Array) LoadBinary(fileName string) error {
	return loadFile(fileName, func(r *bufio.Reader) error {
		count, err := readCount(r)
		if err != nil {
			return err
		}
		tmp := NewArray()
		for i := 0; i < count; i++ {
			value, err := readString(r)
			if err != nil {
				return err
			}
			tmp.PushBack(value)
		}
		*a = *tmp
		return nil
	})
}
