package ds

import (
	"bufio"
	"strings"
)

// listNode — узел односвязного списка (закрытый тип).
type listNode struct {
	value string
	next  *listNode
}

// LinkedList — односвязный список (как linkedList в C++).
type LinkedList struct {
	head *listNode
	tail *listNode
}

func NewLinkedList() *LinkedList {
	return &LinkedList{}
}

func (l *LinkedList) PushFront(value string) {
	l.head = &listNode{value: value, next: l.head}
	if l.tail == nil {
		l.tail = l.head
	}
}

func (l *LinkedList) PushBack(value string) {
	node := &listNode{value: value}
	if l.tail == nil {
		l.head = node
	} else {
		l.tail.next = node
	}
	l.tail = node
}

// findPrev возвращает узел перед первым узлом со значением target.
// found == false — такого значения нет; prev == nil — target в голове.
func (l *LinkedList) findPrev(target string) (prev *listNode, found bool) {
	for cur := l.head; cur != nil; cur = cur.next {
		if cur.value == target {
			return prev, true
		}
		prev = cur
	}
	return nil, false
}

// PushBefore добавляет value перед первым target.
func (l *LinkedList) PushBefore(target, value string) bool {
	prev, found := l.findPrev(target)
	if !found {
		return false
	}
	if prev == nil {
		l.PushFront(value)
		return true
	}
	prev.next = &listNode{value: value, next: prev.next}
	return true
}

// PushAfter добавляет value после первого target.
func (l *LinkedList) PushAfter(target, value string) bool {
	prev, found := l.findPrev(target)
	if !found {
		return false
	}
	node := l.head
	if prev != nil {
		node = prev.next
	}
	if node == l.tail {
		l.PushBack(value)
		return true
	}
	node.next = &listNode{value: value, next: node.next}
	return true
}

func (l *LinkedList) Head() (string, bool) {
	if l.head == nil {
		return "", false
	}
	return l.head.value, true
}

func (l *LinkedList) Tail() (string, bool) {
	if l.tail == nil {
		return "", false
	}
	return l.tail.value, true
}

func (l *LinkedList) Get(index int) (string, bool) {
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

func (l *LinkedList) DelFront() bool {
	if l.head == nil {
		return false
	}
	l.head = l.head.next
	if l.head == nil {
		l.tail = nil
	}
	return true
}

func (l *LinkedList) DelBack() bool {
	if l.head == nil {
		return false
	}
	if l.head == l.tail {
		l.head, l.tail = nil, nil
		return true
	}
	cur := l.head
	for cur.next != l.tail {
		cur = cur.next
	}
	cur.next = nil
	l.tail = cur
	return true
}

// removeAfter удаляет узел после prev (prev == nil — удалить голову).
func (l *LinkedList) removeAfter(prev *listNode) {
	if prev == nil {
		l.DelFront()
		return
	}
	del := prev.next
	prev.next = del.next
	if del == l.tail {
		l.tail = prev
	}
}

// DelValue удаляет первый узел со значением value.
func (l *LinkedList) DelValue(value string) bool {
	prev, found := l.findPrev(value)
	if !found {
		return false
	}
	l.removeAfter(prev)
	return true
}

// DelBefore удаляет элемент перед первым target.
func (l *LinkedList) DelBefore(target string) bool {
	var prevPrev, prev *listNode // два узла перед текущим
	for cur := l.head; cur != nil; cur = cur.next {
		if cur.value == target {
			if prev == nil {
				return false // перед головой ничего нет
			}
			l.removeAfter(prevPrev) // удаляем prev
			return true
		}
		prevPrev, prev = prev, cur
	}
	return false
}

// DelAfter удаляет элемент после первого target.
func (l *LinkedList) DelAfter(target string) bool {
	prev, found := l.findPrev(target)
	if !found {
		return false
	}
	node := l.head
	if prev != nil {
		node = prev.next
	}
	if node.next == nil {
		return false // после хвоста ничего нет
	}
	l.removeAfter(node)
	return true
}

// Find возвращает индекс первого value или -1.
func (l *LinkedList) Find(value string) int {
	index := 0
	for cur := l.head; cur != nil; cur = cur.next {
		if cur.value == value {
			return index
		}
		index++
	}
	return -1
}

func (l *LinkedList) Size() int {
	count := 0
	for cur := l.head; cur != nil; cur = cur.next {
		count++
	}
	return count
}

func (l *LinkedList) String() string {
	var parts []string
	for cur := l.head; cur != nil; cur = cur.next {
		parts = append(parts, cur.value)
	}
	return strings.Join(parts, " ")
}

// ---------- Сериализация (от головы к хвосту) ----------

func (l *LinkedList) SaveText(fileName string) error {
	return saveFile(fileName, func(w *bufio.Writer) {
		for cur := l.head; cur != nil; cur = cur.next {
			writeLine(w, cur.value)
		}
	})
}

func (l *LinkedList) LoadText(fileName string) error {
	return loadFile(fileName, func(r *bufio.Reader) error {
		tmp := NewLinkedList()
		for _, word := range readWords(r) {
			tmp.PushBack(word)
		}
		*l = *tmp
		return nil
	})
}

func (l *LinkedList) SaveBinary(fileName string) error {
	return saveFile(fileName, func(w *bufio.Writer) {
		writeInt(w, l.Size())
		for cur := l.head; cur != nil; cur = cur.next {
			writeString(w, cur.value)
		}
	})
}

func (l *LinkedList) LoadBinary(fileName string) error {
	return loadFile(fileName, func(r *bufio.Reader) error {
		count, err := readCount(r)
		if err != nil {
			return err
		}
		tmp := NewLinkedList()
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
