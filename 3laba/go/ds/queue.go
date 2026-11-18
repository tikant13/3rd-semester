package ds

import (
	"bufio"
	"strings"
)

// Queue — очередь на односвязном списке (как Queue в C++).
type Queue struct {
	head *listNode // начало: отсюда удаляем
	tail *listNode // конец: сюда добавляем
}

func NewQueue() *Queue {
	return &Queue{}
}

func (q *Queue) Push(value string) {
	node := &listNode{value: value}
	if q.tail == nil {
		q.head = node
	} else {
		q.tail.next = node
	}
	q.tail = node
}

// Pop удаляет первый элемент; false — очередь пуста.
func (q *Queue) Pop() (string, bool) {
	if q.head == nil {
		return "", false
	}
	value := q.head.value
	q.head = q.head.next
	if q.head == nil {
		q.tail = nil
	}
	return value, true
}

func (q *Queue) Peek() (string, bool) {
	if q.head == nil {
		return "", false
	}
	return q.head.value, true
}

func (q *Queue) Size() int {
	count := 0
	for cur := q.head; cur != nil; cur = cur.next {
		count++
	}
	return count
}

// String — элементы от начала к концу.
func (q *Queue) String() string {
	var parts []string
	for cur := q.head; cur != nil; cur = cur.next {
		parts = append(parts, cur.value)
	}
	return strings.Join(parts, " ")
}

// ---------- Сериализация (от начала к концу) ----------

func (q *Queue) SaveText(fileName string) error {
	return saveFile(fileName, func(w *bufio.Writer) {
		for cur := q.head; cur != nil; cur = cur.next {
			writeLine(w, cur.value)
		}
	})
}

func (q *Queue) LoadText(fileName string) error {
	return loadFile(fileName, func(r *bufio.Reader) error {
		tmp := NewQueue()
		for _, word := range readWords(r) {
			tmp.Push(word)
		}
		*q = *tmp
		return nil
	})
}

func (q *Queue) SaveBinary(fileName string) error {
	return saveFile(fileName, func(w *bufio.Writer) {
		writeInt(w, q.Size())
		for cur := q.head; cur != nil; cur = cur.next {
			writeString(w, cur.value)
		}
	})
}

func (q *Queue) LoadBinary(fileName string) error {
	return loadFile(fileName, func(r *bufio.Reader) error {
		count, err := readCount(r)
		if err != nil {
			return err
		}
		tmp := NewQueue()
		for i := 0; i < count; i++ {
			value, err := readString(r)
			if err != nil {
				return err
			}
			tmp.Push(value)
		}
		*q = *tmp
		return nil
	})
}
