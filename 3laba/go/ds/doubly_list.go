package ds

import (
	"bufio"
	"strings"
)

// doublyNode — узел двусвязного списка (закрытый тип).
type doublyNode struct {
	value string
	next  *doublyNode
	prev  *doublyNode
}

// DoublyList — двусвязный список (как DoublyList в C++).
type DoublyList struct {
	head *doublyNode
	tail *doublyNode
}

func NewDoublyList() *DoublyList {
	return &DoublyList{}
}

func (l *DoublyList) PushFront(value string) {
	node := &doublyNode{value: value, next: l.head}
	if l.head == nil {
		l.tail = node
	} else {
		l.head.prev = node
	}
	l.head = node
}

func (l *DoublyList) PushBack(value string) {
	node := &doublyNode{value: value, prev: l.tail}
	if l.tail == nil {
		l.head = node
	} else {
		l.tail.next = node
	}
	l.tail = node
}

// find возвращает первый узел со значением value или nil.
func (l *DoublyList) find(value string) *doublyNode {
	for cur := l.head; cur != nil; cur = cur.next {
		if cur.value == value {
			return cur
		}
	}
	return nil
}

// remove вырезает узел из списка, поправляя соседей.
func (l *DoublyList) remove(node *doublyNode) {
	if node.prev == nil {
		l.head = node.next
	} else {
		node.prev.next = node.next
	}
	if node.next == nil {
		l.tail = node.prev
	} else {
		node.next.prev = node.prev
	}
}

func (l *DoublyList) PushBefore(target, value string) bool {
	node := l.find(target)
	if node == nil {
		return false
	}
	if node == l.head {
		l.PushFront(value)
		return true
	}
	newNode := &doublyNode{value: value, prev: node.prev, next: node}
	node.prev.next = newNode
	node.prev = newNode
	return true
}

func (l *DoublyList) PushAfter(target, value string) bool {
	node := l.find(target)
	if node == nil {
		return false
	}
	if node == l.tail {
		l.PushBack(value)
		return true
	}
	newNode := &doublyNode{value: value, prev: node, next: node.next}
	node.next.prev = newNode
	node.next = newNode
	return true
}

func (l *DoublyList) Head() (string, bool) {
	if l.head == nil {
		return "", false
	}
	return l.head.value, true
}

func (l *DoublyList) Tail() (string, bool) {
	if l.tail == nil {
		return "", false
	}
	return l.tail.value, true
}

func (l *DoublyList) Get(index int) (string, bool) {
	if index < 0 {
		return "", false
	}
	cur := l.head
	for i := 0; i < index && cur != nil; i++ {
		cur = cur.next
	}
	if cur == nil {
		return "", false
	}
	return cur.value, true
}

func (l *DoublyList) DelFront() bool {
	if l.head == nil {
		return false
	}
	l.remove(l.head)
	return true
}

func (l *DoublyList) DelBack() bool {
	if l.tail == nil {
		return false
	}
	l.remove(l.tail)
	return true
}

func (l *DoublyList) DelBefore(target string) bool {
	node := l.find(target)
	if node == nil || node.prev == nil {
		return false
	}
	l.remove(node.prev)
	return true
}

func (l *DoublyList) DelAfter(target string) bool {
	node := l.find(target)
	if node == nil || node.next == nil {
		return false
	}
	l.remove(node.next)
	return true
}

func (l *DoublyList) DelValue(value string) bool {
	node := l.find(value)
	if node == nil {
		return false
	}
	l.remove(node)
	return true
}

func (l *DoublyList) Find(value string) int {
	index := 0
	for cur := l.head; cur != nil; cur = cur.next {
		if cur.value == value {
			return index
		}
		index++
	}
	return -1
}

func (l *DoublyList) Size() int {
	count := 0
	for cur := l.head; cur != nil; cur = cur.next {
		count++
	}
	return count
}

func (l *DoublyList) String() string {
	var parts []string
	for cur := l.head; cur != nil; cur = cur.next {
		parts = append(parts, cur.value)
	}
	return strings.Join(parts, " ")
}

// Backward — элементы от хвоста к голове (проверка связей prev).
func (l *DoublyList) Backward() string {
	var parts []string
	for cur := l.tail; cur != nil; cur = cur.prev {
		parts = append(parts, cur.value)
	}
	return strings.Join(parts, " ")
}

// ---------- Сериализация (от головы к хвосту) ----------

func (l *DoublyList) SaveText(fileName string) error {
	return saveFile(fileName, func(w *bufio.Writer) {
		for cur := l.head; cur != nil; cur = cur.next {
			writeLine(w, cur.value)
		}
	})
}

func (l *DoublyList) LoadText(fileName string) error {
	return loadFile(fileName, func(r *bufio.Reader) error {
		tmp := NewDoublyList()
		for _, word := range readWords(r) {
			tmp.PushBack(word)
		}
		*l = *tmp
		return nil
	})
}

func (l *DoublyList) SaveBinary(fileName string) error {
	return saveFile(fileName, func(w *bufio.Writer) {
		writeInt(w, l.Size())
		for cur := l.head; cur != nil; cur = cur.next {
			writeString(w, cur.value)
		}
	})
}

func (l *DoublyList) LoadBinary(fileName string) error {
	return loadFile(fileName, func(r *bufio.Reader) error {
		count, err := readCount(r)
		if err != nil {
			return err
		}
		tmp := NewDoublyList()
		for i := 0; i < count; i++ {
			value, err := readString(r)
			if err != nil {
				return err
			}
			tmp.PushBack(value)
		}
		*l = *tmp
		return nil
	})
}
