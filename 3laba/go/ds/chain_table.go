package ds

import (
	"bufio"
	"strings"
)

// ChainTable — хеш-таблица методом цепочек (как ChainTable в C++):
// массив корзин, каждая корзина — односвязный список с записями "ключ\tзначение".
type ChainTable struct {
	table []*LinkedList // корзины
	count int           // количество пар
}

func NewChainTable(capacity int) *ChainTable {
	if capacity < 1 {
		capacity = 1
	}
	t := &ChainTable{table: make([]*LinkedList, capacity)}
	for i := range t.table {
		t.table[i] = NewLinkedList()
	}
	return t
}

// hash: h = h*31 + символ, затем остаток от деления на число корзин.
func (t *ChainTable) hash(key string) int {
	var h uint64
	for i := 0; i < len(key); i++ {
		h = h*31 + uint64(key[i])
	}
	return int(h % uint64(len(t.table)))
}

// findInBucket ищет запись с ключом key в корзине b; возвращает её или "".
func (t *ChainTable) findInBucket(b int, key string) (string, bool) {
	bucket := t.table[b]
	for i := 0; i < bucket.Size(); i++ {
		entry, _ := bucket.Get(i)
		if k, _, _ := strings.Cut(entry, "\t"); k == key {
			return entry, true
		}
	}
	return "", false
}

// Insert: true — новый ключ, false — значение обновлено (или ключ пустой).
func (t *ChainTable) Insert(key, value string) bool {
	if key == "" {
		return false
	}
	b := t.hash(key)
	if entry, found := t.findInBucket(b, key); found {
		t.table[b].DelValue(entry)
		t.table[b].PushFront(key + "\t" + value)
		return false
	}
	t.table[b].PushFront(key + "\t" + value)
	t.count++
	if t.count*4 > len(t.table)*3 { // заполнено больше 75%
		t.rehash()
	}
	return true
}

func (t *ChainTable) Get(key string) (string, bool) {
	entry, found := t.findInBucket(t.hash(key), key)
	if !found {
		return "", false
	}
	_, value, _ := strings.Cut(entry, "\t")
	return value, true
}

func (t *ChainTable) Remove(key string) bool {
	b := t.hash(key)
	entry, found := t.findInBucket(b, key)
	if !found {
		return false
	}
	t.table[b].DelValue(entry)
	t.count--
	return true
}

// rehash увеличивает число корзин в 2 раза и раскладывает записи заново.
func (t *ChainTable) rehash() {
	old := t.table
	t.table = make([]*LinkedList, len(old)*2)
	for i := range t.table {
		t.table[i] = NewLinkedList()
	}
	for _, bucket := range old {
		for entry, ok := bucket.Head(); ok; entry, ok = bucket.Head() {
			key, _, _ := strings.Cut(entry, "\t")
			t.table[t.hash(key)].PushFront(entry)
			bucket.DelFront()
		}
	}
}

func (t *ChainTable) Size() int {
	return t.count
}

func (t *ChainTable) Capacity() int {
	return len(t.table)
}

// forEach вызывает visit для каждой пары.
func (t *ChainTable) forEach(visit func(key, value string)) {
	for _, bucket := range t.table {
		for i := 0; i < bucket.Size(); i++ {
			entry, _ := bucket.Get(i)
			key, value, _ := strings.Cut(entry, "\t")
			visit(key, value)
		}
	}
}

// String — пары "ключ значение" через пробел.
func (t *ChainTable) String() string {
	var parts []string
	t.forEach(func(key, value string) {
		parts = append(parts, key+" "+value)
	})
	return strings.Join(parts, " ")
}

// ---------- Сериализация ----------
// Сохраняются только пары; при чтении они вставляются заново,
// поэтому раскладка по корзинам может стать другой.

func (t *ChainTable) SaveText(fileName string) error {
	return saveFile(fileName, func(w *bufio.Writer) {
		t.forEach(func(key, value string) {
			writeLine(w, key, value)
		})
	})
}

func (t *ChainTable) LoadText(fileName string) error {
	return loadFile(fileName, func(r *bufio.Reader) error {
		words := readWords(r)
		if len(words)%2 != 0 {
			return errBadFile // у ключа нет значения
		}
		tmp := NewChainTable(8)
		for i := 0; i < len(words); i += 2 {
			tmp.Insert(words[i], words[i+1])
		}
		*t = *tmp
		return nil
	})
}

func (t *ChainTable) SaveBinary(fileName string) error {
	return saveFile(fileName, func(w *bufio.Writer) {
		writeInt(w, t.count)
		t.forEach(func(key, value string) {
			writeString(w, key)
			writeString(w, value)
		})
	})
}

func (t *ChainTable) LoadBinary(fileName string) error {
	return loadFile(fileName, func(r *bufio.Reader) error {
		count, err := readCount(r)
		if err != nil {
			return err
		}
		tmp := NewChainTable(8)
		for i := 0; i < count; i++ {
			key, err := readString(r)
			if err != nil {
				return err
			}
			value, err := readString(r)
			if err != nil {
				return err
			}
			tmp.Insert(key, value)
		}
		*t = *tmp
		return nil
	})
}
