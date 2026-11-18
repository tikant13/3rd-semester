package ds

import (
	"bufio"
	"slices"
	"strings"
)

// Stack — стек на односвязном списке (как Stack в C++).
type Stack struct {
	top *listNode // вершина
}

func NewStack() *Stack {
	return &Stack{}
}

func (s *Stack) Push(value string) {
	s.top = &listNode{value: value, next: s.top}
}

// Pop снимает вершину; false — стек пуст.
func (s *Stack) Pop() (string, bool) {
	if s.top == nil {
		return "", false
	}
	value := s.top.value
	s.top = s.top.next
	return value, true
}

func (s *Stack) Peek() (string, bool) {
	if s.top == nil {
		return "", false
	}
	return s.top.value, true
}

func (s *Stack) Size() int {
	count := 0
	for cur := s.top; cur != nil; cur = cur.next {
		count++
	}
	return count
}

// String — элементы от дна к вершине (как ToString в C++).
func (s *Stack) String() string {
	var parts []string
	for cur := s.top; cur != nil; cur = cur.next {
		parts = append(parts, cur.value)
	}
	slices.Reverse(parts)
	return strings.Join(parts, " ")
}

// ---------- Сериализация (от вершины ко дну) ----------
// При чтении элементы идут от вершины ко дну, поэтому Push делаем с конца списка.

func (s *Stack) SaveText(fileName string) error {
	return saveFile(fileName, func(w *bufio.Writer) {
		for cur := s.top; cur != nil; cur = cur.next {
			writeLine(w, cur.value)
		}
	})
}

func (s *Stack) LoadText(fileName string) error {
	return loadFile(fileName, func(r *bufio.Reader) error {
		words := readWords(r)
		tmp := NewStack()
		for i := len(words) - 1; i >= 0; i-- {
			tmp.Push(words[i])
		}
		*s = *tmp
		return nil
	})
}

func (s *Stack) SaveBinary(fileName string) error {
	return saveFile(fileName, func(w *bufio.Writer) {
		writeInt(w, s.Size())
		for cur := s.top; cur != nil; cur = cur.next {
			writeString(w, cur.value)
		}
	})
}

func (s *Stack) LoadBinary(fileName string) error {
	return loadFile(fileName, func(r *bufio.Reader) error {
		count, err := readCount(r)
		if err != nil {
			return err
		}
		var values []string
		for i := 0; i < count; i++ {
			value, err := readString(r)
			if err != nil {
				return err
			}
			values = append(values, value)
		}
		tmp := NewStack()
		for i := len(values) - 1; i >= 0; i-- {
			tmp.Push(values[i])
		}
		*s = *tmp
		return nil
	})
}
