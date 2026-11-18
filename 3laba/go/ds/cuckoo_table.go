package ds

import (
	"bufio"
	"strings"
)

// maxKicks — сколько раз подряд можно выталкивать, потом считаем, что попали в цикл.
const maxKicks = 100

// CuckooTable — хеширование кукушки (как CuckooTable в C++): две таблицы и две
// хеш-функции. Ключ лежит либо в T1[h1(key)], либо в T2[h2(key)].
// Таблицы — массивы Array, пустая ячейка — пустая строка.
type CuckooTable struct {
	keys1, vals1 *Array // первая таблица
	keys2, vals2 *Array // вторая таблица
	capacity     int    // размер каждой таблицы
	count        int    // количество пар
}

func NewCuckooTable(capacity int) *CuckooTable {
	if capacity < 1 {
		capacity = 1
	}
	t := &CuckooTable{}
	t.allocate(capacity)
	return t
}

// allocate создаёт две пустые таблицы размера capacity.
func (t *CuckooTable) allocate(capacity int) {
	t.capacity = capacity
	t.keys1, t.vals1, t.keys2, t.vals2 = NewArray(), NewArray(), NewArray(), NewArray()
	for i := 0; i < capacity; i++ {
		t.keys1.PushBack("")
		t.vals1.PushBack("")
		t.keys2.PushBack("")
		t.vals2.PushBack("")
	}
}

// hash: полиномиальный хеш с основанием base.
func (t *CuckooTable) hash(key string, base uint64) int {
	var h uint64
	for i := 0; i < len(key); i++ {
		h = h*base + uint64(key[i])
	}
	return int(h % uint64(t.capacity))
}

func (t *CuckooTable) h1(key string) int { return t.hash(key, 31) }
func (t *CuckooTable) h2(key string) int { return t.hash(key, 37) }

// cell возвращает ключ и значение ячейки i таблицы (keys, vals).
func cell(keys, vals *Array, i int) (string, string) {
	k, _ := keys.Get(i)
	v, _ := vals.Get(i)
	return k, v
}

// keyAt возвращает ключ ячейки i ("" — ячейка пустая).
func keyAt(keys *Array, i int) string {
	k, _ := keys.Get(i)
	return k
}

// place кладёт пару в T1; если занято — выталкивает старую пару в T2 и так далее.
// При неудаче возвращает false и пару, оставшуюся без места.
func (t *CuckooTable) place(key, value string) (string, string, bool) {
	for step := 0; step < maxKicks; step++ {
		i := t.h1(key)
		oldKey, oldValue := cell(t.keys1, t.vals1, i)
		t.keys1.Set(i, key)
		t.vals1.Set(i, value)
		if oldKey == "" {
			return "", "", true
		}
		key, value = oldKey, oldValue

		j := t.h2(key)
		oldKey, oldValue = cell(t.keys2, t.vals2, j)
		t.keys2.Set(j, key)
		t.vals2.Set(j, value)
		if oldKey == "" {
			return "", "", true
		}
		key, value = oldKey, oldValue
	}
	return key, value, false
}

// rehash собирает все пары (и лишнюю extra, если есть) и раскладывает
// в таблицы размера newCapacity; если не влезло — увеличивает ещё в 2 раза.
func (t *CuckooTable) rehash(newCapacity int, extraKey, extraValue string) {
	var keys, values []string
	t.forEach(func(key, value string) {
		keys = append(keys, key)
		values = append(values, value)
	})
	if extraKey != "" {
		keys = append(keys, extraKey)
		values = append(values, extraValue)
	}
	for capacity := newCapacity; ; capacity *= 2 {
		t.allocate(capacity)
		ok := true
		for i := range keys {
			if _, _, placed := t.place(keys[i], values[i]); !placed {
				ok = false
				break
			}
		}
		if ok {
			return
		}
	}
}

// Insert: true — новый ключ, false — значение обновлено (или ключ пустой).
func (t *CuckooTable) Insert(key, value string) bool {
	if key == "" {
		return false // пустая строка обозначает пустую ячейку
	}
	if i := t.h1(key); keyAt(t.keys1, i) == key { // ключ уже есть — меняем значение
		t.vals1.Set(i, value)
		return false
	}
	if j := t.h2(key); keyAt(t.keys2, j) == key {
		t.vals2.Set(j, value)
		return false
	}
	// при заполнении больше 45% кукушка часто зацикливается — увеличиваем заранее
	if (t.count+1)*100 > t.capacity*2*45 {
		t.rehash(t.capacity*2, "", "")
	}
	if k, v, placed := t.place(key, value); !placed {
		t.rehash(t.capacity*2, k, v) // перестраиваем вместе с парой без места
	}
	t.count++
	return true
}

func (t *CuckooTable) Get(key string) (string, bool) {
	if key == "" {
		return "", false
	}
	if k, v := cell(t.keys1, t.vals1, t.h1(key)); k == key {
		return v, true
	}
	if k, v := cell(t.keys2, t.vals2, t.h2(key)); k == key {
		return v, true
	}
	return "", false
}

func (t *CuckooTable) Remove(key string) bool {
	if key == "" {
		return false
	}
	if i := t.h1(key); keyAt(t.keys1, i) == key {
		t.keys1.Set(i, "")
		t.vals1.Set(i, "")
		t.count--
		return true
	}
	if j := t.h2(key); keyAt(t.keys2, j) == key {
		t.keys2.Set(j, "")
		t.vals2.Set(j, "")
		t.count--
		return true
	}
	return false
}

func (t *CuckooTable) Size() int {
	return t.count
}

// forEach вызывает visit для каждой занятой ячейки T1 и T2.
func (t *CuckooTable) forEach(visit func(key, value string)) {
	for i := 0; i < t.capacity; i++ {
		if k, v := cell(t.keys1, t.vals1, i); k != "" {
			visit(k, v)
		}
		if k, v := cell(t.keys2, t.vals2, i); k != "" {
			visit(k, v)
		}
	}
}

// String — пары "ключ значение" через пробел.
func (t *CuckooTable) String() string {
	var parts []string
	t.forEach(func(key, value string) {
		parts = append(parts, key+" "+value)
	})
	return strings.Join(parts, " ")
}

// ---------- Сериализация ----------
// Сохраняются только пары; при чтении они вставляются заново,
// поэтому ячейки T1/T2 могут стать другими.

func (t *CuckooTable) SaveText(fileName string) error {
	return saveFile(fileName, func(w *bufio.Writer) {
		t.forEach(func(key, value string) {
			writeLine(w, key, value)
		})
	})
}

func (t *CuckooTable) LoadText(fileName string) error {
	return loadFile(fileName, func(r *bufio.Reader) error {
		words := readWords(r)
		if len(words)%2 != 0 {
			return errBadFile // у ключа нет значения
		}
		tmp := NewCuckooTable(8)
		for i := 0; i < len(words); i += 2 {
			tmp.Insert(words[i], words[i+1])
		}
		*t = *tmp
		return nil
	})
}

func (t *CuckooTable) SaveBinary(fileName string) error {
	return saveFile(fileName, func(w *bufio.Writer) {
		writeInt(w, t.count)
		t.forEach(func(key, value string) {
			writeString(w, key)
			writeString(w, value)
		})
	})
}

func (t *CuckooTable) LoadBinary(fileName string) error {
	return loadFile(fileName, func(r *bufio.Reader) error {
		count, err := readCount(r)
		if err != nil {
			return err
		}
		tmp := NewCuckooTable(8)
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
