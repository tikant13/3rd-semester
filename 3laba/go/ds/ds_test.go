package ds

import (
	"encoding/binary"
	"os"
	"path/filepath"
	"strconv"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// 1. Массив

func TestArrayOperations(t *testing.T) {
	a := NewArray()
	assert.Equal(t, 0, a.Size())
	assert.Equal(t, "", a.String())

	a.PushBack("one")
	a.PushBack("three")
	assert.True(t, a.Insert(1, "two"))
	assert.True(t, a.Insert(0, "zero"))
	assert.True(t, a.Insert(4, "end")) // индекс == размер — вставка в конец
	assert.Equal(t, "zero one two three end", a.String())

	value, ok := a.Get(2)
	assert.True(t, ok)
	assert.Equal(t, "two", value)
	assert.True(t, a.Set(2, "TWO"))
	assert.True(t, a.Delete(0))
	assert.True(t, a.Delete(3))
	assert.Equal(t, "one TWO three", a.String())
	assert.Equal(t, 3, a.Size())
}

func TestArrayInvalidIndexes(t *testing.T) {
	a := NewArray()
	a.PushBack("hello")
	assert.False(t, a.Insert(-1, "bad"))
	assert.False(t, a.Insert(5, "bad"))
	assert.False(t, a.Set(-1, "bad"))
	assert.False(t, a.Set(1, "bad"))
	assert.False(t, a.Delete(-1))
	assert.False(t, a.Delete(1))
	_, ok := a.Get(1)
	assert.False(t, ok)
	_, ok = a.Get(-1)
	assert.False(t, ok)
	assert.Equal(t, "hello", a.String())
}

func TestArrayGrowth(t *testing.T) {
	a := NewArray()
	for i := 0; i < 30; i++ {
		a.PushBack(strconv.Itoa(i))
	}
	assert.Equal(t, 30, a.Size())
	last, _ := a.Get(29)
	assert.Equal(t, "29", last)

	full := NewArray() // вставка в заполненный массив тоже увеличивает его
	for i := 0; i < 10; i++ {
		full.PushBack("x")
	}
	assert.True(t, full.Insert(5, "middle"))
	assert.Equal(t, 11, full.Size())

	var zero Array // нулевое значение тоже рабочее
	zero.PushBack("a")
	assert.Equal(t, "a", zero.String())
}

// 2. Односвязный список

func TestLinkedListEmpty(t *testing.T) {
	l := NewLinkedList()
	assert.Equal(t, 0, l.Size())
	_, ok := l.Head()
	assert.False(t, ok)
	_, ok = l.Tail()
	assert.False(t, ok)
	_, ok = l.Get(0)
	assert.False(t, ok)
	assert.False(t, l.DelFront())
	assert.False(t, l.DelBack())
	assert.False(t, l.DelValue("x"))
	assert.False(t, l.DelBefore("x"))
	assert.False(t, l.DelAfter("x"))
	assert.False(t, l.PushBefore("x", "y"))
	assert.False(t, l.PushAfter("x", "y"))
	assert.Equal(t, -1, l.Find("x"))
}

func TestLinkedListInsert(t *testing.T) {
	l := NewLinkedList()
	l.PushBack("b")
	l.PushFront("a")
	l.PushBack("d")
	assert.True(t, l.PushBefore("d", "c"))
	assert.True(t, l.PushBefore("a", "start")) // перед головой
	assert.True(t, l.PushAfter("d", "end"))    // после хвоста
	assert.True(t, l.PushAfter("a", "a2"))
	assert.Equal(t, "start a a2 b c d end", l.String())

	head, _ := l.Head()
	tail, _ := l.Tail()
	assert.Equal(t, "start", head)
	assert.Equal(t, "end", tail)
	value, ok := l.Get(3)
	assert.True(t, ok)
	assert.Equal(t, "b", value)
	_, ok = l.Get(-1)
	assert.False(t, ok)
	_, ok = l.Get(7)
	assert.False(t, ok)
	assert.Equal(t, 4, l.Find("c"))
	assert.Equal(t, 7, l.Size())

	single := NewLinkedList()
	single.PushFront("only") // PushFront в пустой список задаёт и хвост
	tail, _ = single.Tail()
	assert.Equal(t, "only", tail)
}

func TestLinkedListDelete(t *testing.T) {
	l := NewLinkedList()
	for _, v := range []string{"a", "b", "c", "d", "e", "f"} {
		l.PushBack(v)
	}
	assert.True(t, l.DelFront())
	assert.True(t, l.DelBack())
	assert.Equal(t, "b c d e", l.String())

	assert.True(t, l.DelBefore("c")) // удаляем голову b
	assert.True(t, l.DelBefore("e")) // удаляем d из середины
	assert.False(t, l.DelBefore("c"))
	assert.False(t, l.DelBefore("missing"))
	assert.Equal(t, "c e", l.String())

	l.PushBack("g")
	assert.True(t, l.DelAfter("c")) // e из середины
	assert.True(t, l.DelAfter("c")) // g — хвост
	assert.False(t, l.DelAfter("c"))
	assert.False(t, l.DelAfter("missing"))
	tail, _ := l.Tail()
	assert.Equal(t, "c", tail)

	l.PushBack("x")
	l.PushBack("y")
	assert.True(t, l.DelValue("y")) // хвост
	assert.True(t, l.DelValue("c")) // голова
	assert.False(t, l.DelValue("missing"))
	assert.True(t, l.DelBack()) // последний элемент
	assert.Equal(t, 0, l.Size())
	_, ok := l.Tail()
	assert.False(t, ok)

	l.PushBack("p")
	l.PushBack("q")
	l.PushBack("r")
	assert.True(t, l.DelValue("q")) // середина
	assert.True(t, l.DelAfter("p")) // хвост через DelAfter
	tail, _ = l.Tail()
	assert.Equal(t, "p", tail)
	assert.True(t, l.DelFront())
	_, ok = l.Tail()
	assert.False(t, ok)
}

// 3. Двусвязный список

func TestDoublyListEmpty(t *testing.T) {
	l := NewDoublyList()
	assert.Equal(t, 0, l.Size())
	_, ok := l.Head()
	assert.False(t, ok)
	_, ok = l.Tail()
	assert.False(t, ok)
	_, ok = l.Get(0)
	assert.False(t, ok)
	_, ok = l.Get(-1)
	assert.False(t, ok)
	assert.False(t, l.DelFront())
	assert.False(t, l.DelBack())
	assert.False(t, l.DelValue("x"))
	assert.False(t, l.DelBefore("x"))
	assert.False(t, l.DelAfter("x"))
	assert.False(t, l.PushBefore("x", "y"))
	assert.False(t, l.PushAfter("x", "y"))
	assert.Equal(t, -1, l.Find("x"))
}

func TestDoublyListInsertAndDelete(t *testing.T) {
	l := NewDoublyList()
	l.PushFront("b")
	l.PushFront("a")
	l.PushBack("d")
	assert.True(t, l.PushBefore("d", "c"))
	assert.True(t, l.PushBefore("a", "start"))
	assert.True(t, l.PushAfter("d", "end"))
	assert.True(t, l.PushAfter("a", "a2"))
	assert.Equal(t, "start a a2 b c d end", l.String())
	assert.Equal(t, "end d c b a2 a start", l.Backward()) // связи prev верные
	head, _ := l.Head()
	tail, _ := l.Tail()
	assert.Equal(t, "start", head)
	assert.Equal(t, "end", tail)
	value, ok := l.Get(4)
	assert.True(t, ok)
	assert.Equal(t, "c", value)
	_, ok = l.Get(10)
	assert.False(t, ok)
	assert.Equal(t, 5, l.Find("d"))

	assert.True(t, l.DelFront())
	assert.True(t, l.DelBack())
	assert.True(t, l.DelBefore("a2")) // голова a
	assert.False(t, l.DelBefore("a2"))
	assert.True(t, l.DelAfter("b")) // c
	assert.True(t, l.DelAfter("b")) // d — хвост
	assert.False(t, l.DelAfter("b"))
	assert.True(t, l.DelValue("a2")) // голова
	assert.Equal(t, "b", l.String())
	assert.True(t, l.DelValue("b")) // последний
	assert.Equal(t, 0, l.Size())
	assert.Equal(t, "", l.Backward())
}

// 4. Стек и очередь

func TestStack(t *testing.T) {
	s := NewStack()
	_, ok := s.Pop()
	assert.False(t, ok)
	_, ok = s.Peek()
	assert.False(t, ok)

	s.Push("a")
	s.Push("b")
	s.Push("c")
	assert.Equal(t, 3, s.Size())
	assert.Equal(t, "a b c", s.String()) // от дна к вершине
	top, _ := s.Peek()
	assert.Equal(t, "c", top)
	value, ok := s.Pop()
	assert.True(t, ok)
	assert.Equal(t, "c", value)
	value, _ = s.Pop()
	assert.Equal(t, "b", value)
	assert.Equal(t, "a", s.String())
}

func TestQueue(t *testing.T) {
	q := NewQueue()
	_, ok := q.Pop()
	assert.False(t, ok)
	_, ok = q.Peek()
	assert.False(t, ok)

	q.Push("1")
	q.Push("2")
	q.Push("3")
	assert.Equal(t, 3, q.Size())
	assert.Equal(t, "1 2 3", q.String())
	first, _ := q.Peek()
	assert.Equal(t, "1", first)
	value, ok := q.Pop()
	assert.True(t, ok)
	assert.Equal(t, "1", value)
	q.Pop()
	q.Pop()
	assert.Equal(t, 0, q.Size())
	q.Push("4") // после опустошения хвост сброшен правильно
	assert.Equal(t, "4", q.String())
}

// 5. Красно-чёрное дерево

// checkRB проверяет свойства красно-чёрного дерева и возвращает чёрную высоту.
func checkRB(t *testing.T, node *treeNode) int {
	if node == nil {
		return 1
	}
	if node.left != nil {
		require.Equal(t, node, node.left.parent)
		require.Less(t, node.left.key, node.key)
	}
	if node.right != nil {
		require.Equal(t, node, node.right.parent)
		require.Less(t, node.key, node.right.key)
	}
	if isRed(node) { // у красного узла дети чёрные
		require.False(t, isRed(node.left))
		require.False(t, isRed(node.right))
	}
	left := checkRB(t, node.left)
	right := checkRB(t, node.right)
	require.Equal(t, left, right) // одинаковое число чёрных узлов на всех путях
	if node.color == black {
		return left + 1
	}
	return left
}

func checkTree(t *testing.T, tree *RBTree) {
	if tree.root != nil {
		require.Equal(t, byte(black), tree.root.color)
		require.Nil(t, tree.root.parent)
	}
	checkRB(t, tree.root)
}

func TestRBTreeEmpty(t *testing.T) {
	tree := NewRBTree()
	assert.Equal(t, 0, tree.Size())
	assert.False(t, tree.Contains(1))
	assert.False(t, tree.Remove(1))
	assert.Empty(t, tree.Inorder())
	assert.Empty(t, tree.BFS())
	assert.Equal(t, "", tree.String())
}

func TestRBTreeInsertAndTraversals(t *testing.T) {
	tree := NewRBTree()
	for _, key := range []int{10, 20, 30, 15, 25, 5, 1} {
		assert.True(t, tree.Insert(key))
		checkTree(t, tree)
	}
	assert.False(t, tree.Insert(10)) // повтор
	assert.Equal(t, 7, tree.Size())
	assert.True(t, tree.Contains(25))
	assert.False(t, tree.Contains(100))
	assert.Equal(t, []int{1, 5, 10, 15, 20, 25, 30}, tree.Inorder())
	assert.Equal(t, []int{20, 10, 5, 1, 15, 30, 25}, tree.Preorder())
	assert.Equal(t, []int{1, 5, 15, 10, 25, 30, 20}, tree.Postorder())
	assert.Equal(t, []int{20, 10, 30, 5, 15, 25, 1}, tree.BFS())
	assert.Equal(t, "20 B 10 R 5 B 1 R 15 B 30 B 25 R", tree.String())
}

func TestRBTreeRotations(t *testing.T) {
	orders := [][]int{{30, 20, 10}, {10, 20, 30}, {30, 10, 20}, {10, 30, 20}}
	for _, keys := range orders {
		tree := NewRBTree()
		for _, key := range keys {
			tree.Insert(key)
		}
		checkTree(t, tree)
		assert.Equal(t, []int{20, 10, 30}, tree.Preorder())
	}
}

func TestRBTreeRemove(t *testing.T) {
	// разные порядки удаления проходят все случаи балансировки
	orders := map[string]func(i int) int{
		"по возрастанию": func(i int) int { return i },
		"по убыванию":    func(i int) int { return 63 - i },
		"вперемешку":     func(i int) int { return (i * 37) % 64 },
	}
	for name, order := range orders {
		tree := NewRBTree()
		for i := 0; i < 64; i++ {
			tree.Insert((i * 29) % 64) // ключи 0..63 в перемешанном порядке
		}
		for i := 0; i < 64; i++ {
			key := order(i)
			require.True(t, tree.Remove(key), name)
			require.False(t, tree.Contains(key))
			checkTree(t, tree)
		}
		assert.Equal(t, 0, tree.Size())
	}
}

// 6. Хеш-таблица методом цепочек

func TestChainTable(t *testing.T) {
	table := NewChainTable(0) // размер меньше 1 исправляется на 1
	assert.Equal(t, 1, table.Capacity())
	assert.True(t, table.Insert("name", "Ivan"))
	assert.False(t, table.Insert("name", "Petr")) // обновление
	assert.False(t, table.Insert("", "bad"))
	value, ok := table.Get("name")
	assert.True(t, ok)
	assert.Equal(t, "Petr", value)
	_, ok = table.Get("missing")
	assert.False(t, ok)

	for i := 0; i < 50; i++ {
		table.Insert("key"+strconv.Itoa(i), strconv.Itoa(i))
	}
	assert.Equal(t, 51, table.Size())
	assert.Greater(t, table.Capacity(), 50) // была перестройка
	for i := 0; i < 50; i++ {
		value, ok := table.Get("key" + strconv.Itoa(i))
		require.True(t, ok)
		assert.Equal(t, strconv.Itoa(i), value)
	}
	assert.True(t, table.Remove("name"))
	assert.False(t, table.Remove("name"))
	assert.Equal(t, 50, table.Size())
	assert.Contains(t, table.String(), "key7 7")
}

// 7. Хеширование кукушки

func TestCuckooTable(t *testing.T) {
	table := NewCuckooTable(0)
	assert.True(t, table.Insert("name", "Ivan"))
	assert.False(t, table.Insert("name", "Petr"))
	assert.False(t, table.Insert("", "bad"))
	value, ok := table.Get("name")
	assert.True(t, ok)
	assert.Equal(t, "Petr", value)
	_, ok = table.Get("missing")
	assert.False(t, ok)
	_, ok = table.Get("")
	assert.False(t, ok)
	assert.False(t, table.Remove(""))
	assert.False(t, table.Remove("missing"))

	for i := 0; i < 200; i++ {
		table.Insert("key"+strconv.Itoa(i), strconv.Itoa(i))
	}
	assert.Equal(t, 201, table.Size())
	for i := 0; i < 200; i++ {
		value, ok := table.Get("key" + strconv.Itoa(i))
		require.True(t, ok)
		assert.Equal(t, strconv.Itoa(i), value)
	}
	for i := 0; i < 200; i++ {
		require.True(t, table.Remove("key"+strconv.Itoa(i)))
	}
	assert.Equal(t, "name Petr", table.String())
}

func TestCuckooTableSecondTable(t *testing.T) {
	table := NewCuckooTable(16)
	assert.True(t, table.Insert("aa", "one"))
	assert.True(t, table.Insert("aq", "two")) // одинаковый h1: второй ключ уходит в T2
	assert.False(t, table.Insert("aa", "one!"))
	assert.False(t, table.Insert("aq", "two!"))
	value, _ := table.Get("aa")
	assert.Equal(t, "one!", value)
	value, _ = table.Get("aq")
	assert.Equal(t, "two!", value)
	assert.True(t, table.Remove("aq"))
	assert.True(t, table.Remove("aa"))
	assert.Equal(t, 0, table.Size())
}

func TestCuckooTableCycle(t *testing.T) {
	table := NewCuckooTable(16)
	// у трёх ключей одинаковые h1 и h2 — места нет, нужна перестройка
	assert.True(t, table.Insert("aa", "1"))
	assert.True(t, table.Insert("aq", "2"))
	assert.True(t, table.Insert("aA", "3"))
	assert.Equal(t, 3, table.Size())
	for key, want := range map[string]string{"aa": "1", "aq": "2", "aA": "3"} {
		value, ok := table.Get(key)
		require.True(t, ok)
		assert.Equal(t, want, value)
	}
}

// 8. Сериализация

// serializable — общий интерфейс сериализации; его реализуют все 8 структур.
type serializable interface {
	SaveText(fileName string) error
	LoadText(fileName string) error
	SaveBinary(fileName string) error
	LoadBinary(fileName string) error
	String() string
}

func intBytes(values ...int32) []byte {
	var buf []byte
	for _, v := range values {
		buf = binary.LittleEndian.AppendUint32(buf, uint32(v))
	}
	return buf
}

// checkFileErrors — ошибки, общие для всех структур. После неудачной загрузки
// объект не должен меняться.
func checkFileErrors(t *testing.T, obj serializable) {
	dir := t.TempDir()
	before := obj.String()
	assert.Error(t, obj.LoadText(filepath.Join(dir, "missing.txt")))
	assert.Error(t, obj.LoadBinary(filepath.Join(dir, "missing.bin")))
	assert.Error(t, obj.SaveText(filepath.Join(dir, "no_dir", "file.txt")))
	assert.Error(t, obj.SaveBinary(filepath.Join(dir, "no_dir", "file.bin")))

	bad := filepath.Join(dir, "bad.bin")
	cases := [][]byte{
		[]byte("ab"),                            // нет даже количества
		intBytes(-1),                            // отрицательное количество
		intBytes(1, -1),                         // отрицательная длина строки
		append(intBytes(3, 5), []byte("ab")...), // файл обрезан
	}
	for _, data := range cases {
		require.NoError(t, os.WriteFile(bad, data, 0o644))
		assert.Error(t, obj.LoadBinary(bad))
	}
	assert.Equal(t, before, obj.String())
}

// roundTrip сохраняет obj в оба формата и загружает в fromText и fromBinary.
func roundTrip(t *testing.T, obj, fromText, fromBinary serializable) {
	dir := t.TempDir()
	textFile := filepath.Join(dir, "data.txt")
	binFile := filepath.Join(dir, "data.bin")
	require.NoError(t, obj.SaveText(textFile))
	require.NoError(t, fromText.LoadText(textFile))
	require.NoError(t, obj.SaveBinary(binFile))
	require.NoError(t, fromBinary.LoadBinary(binFile))
}

func TestArraySerialization(t *testing.T) {
	a := NewArray()
	a.PushBack("one")
	a.PushBack("two")
	fromText, fromBinary := NewArray(), NewArray()
	fromText.PushBack("old") // старое содержимое заменяется
	roundTrip(t, a, fromText, fromBinary)
	assert.Equal(t, "one two", fromText.String())
	assert.Equal(t, "one two", fromBinary.String())

	a.PushBack("with space") // бинарный формат хранит строку целиком
	a.PushBack("")
	roundTrip(t, a, NewArray(), fromBinary)
	assert.Equal(t, 4, fromBinary.Size())
	value, _ := fromBinary.Get(2)
	assert.Equal(t, "with space", value)
	checkFileErrors(t, fromBinary)
}

func TestListSerialization(t *testing.T) {
	l := NewLinkedList()
	d := NewDoublyList()
	for _, v := range []string{"a", "b", "c"} {
		l.PushBack(v)
		d.PushBack(v)
	}
	lText, lBin := NewLinkedList(), NewLinkedList()
	roundTrip(t, l, lText, lBin)
	assert.Equal(t, "a b c", lText.String())
	lBin.PushBack("d") // хвост восстановлен
	assert.Equal(t, "a b c d", lBin.String())
	checkFileErrors(t, lBin)

	dText, dBin := NewDoublyList(), NewDoublyList()
	roundTrip(t, d, dText, dBin)
	assert.Equal(t, "a b c", dText.String())
	assert.Equal(t, "c b a", dBin.Backward()) // связи prev восстановлены
	checkFileErrors(t, dBin)
}

func TestStackQueueSerialization(t *testing.T) {
	s := NewStack()
	q := NewQueue()
	for _, v := range []string{"1", "2", "3"} {
		s.Push(v)
		q.Push(v)
	}
	sText, sBin := NewStack(), NewStack()
	roundTrip(t, s, sText, sBin)
	assert.Equal(t, "1 2 3", sText.String())
	top, _ := sBin.Pop() // вершина та же
	assert.Equal(t, "3", top)
	checkFileErrors(t, sBin)

	qText, qBin := NewQueue(), NewQueue()
	roundTrip(t, q, qText, qBin)
	assert.Equal(t, "1 2 3", qText.String())
	qBin.Push("4")
	first, _ := qBin.Pop()
	assert.Equal(t, "1", first)
	assert.Equal(t, "2 3 4", qBin.String())
	checkFileErrors(t, qBin)
}

func TestRBTreeSerialization(t *testing.T) {
	tree := NewRBTree()
	for i := 1; i <= 10; i++ {
		tree.Insert(i)
	}
	fromText, fromBinary := NewRBTree(), NewRBTree()
	roundTrip(t, tree, fromText, fromBinary)
	assert.Equal(t, tree.String(), fromText.String()) // та же форма и те же цвета
	assert.Equal(t, tree.String(), fromBinary.String())
	checkTree(t, fromBinary)
	assert.True(t, fromBinary.Remove(4)) // с деревом можно работать дальше
	assert.True(t, fromBinary.Insert(11))
	checkTree(t, fromBinary)
	checkFileErrors(t, fromBinary)

	dir := t.TempDir()
	file := filepath.Join(dir, "tree.txt")
	for _, text := range []string{"10 B\n5 X\n", "10 B\n5\n", "10 B\nabc R\n"} {
		require.NoError(t, os.WriteFile(file, []byte(text), 0o644))
		assert.Error(t, fromText.LoadText(file)) // плохой цвет, нет цвета, ключ не число
	}
	assert.Equal(t, tree.String(), fromText.String())
}

// getter — у обеих хеш-таблиц есть Get, поэтому проверку пишем один раз.
type getter interface {
	Get(key string) (string, bool)
}

func checkPairs(t *testing.T, table getter, n int) {
	for i := 0; i < n; i++ {
		value, ok := table.Get("key" + strconv.Itoa(i))
		require.True(t, ok)
		assert.Equal(t, "value"+strconv.Itoa(i), value)
	}
}

// checkOddText: в текстовом файле у последнего ключа нет значения.
func checkOddText(t *testing.T, obj serializable) {
	file := filepath.Join(t.TempDir(), "bad.txt")
	require.NoError(t, os.WriteFile(file, []byte("key1 value1\nkey2\n"), 0o644))
	before := obj.String()
	assert.Error(t, obj.LoadText(file))
	assert.Equal(t, before, obj.String())
}

func TestChainTableSerialization(t *testing.T) {
	table := NewChainTable(8)
	for i := 0; i < 20; i++ { // с перестройкой таблицы
		table.Insert("key"+strconv.Itoa(i), "value"+strconv.Itoa(i))
	}
	fromText, fromBinary := NewChainTable(8), NewChainTable(8)
	roundTrip(t, table, fromText, fromBinary)
	assert.Equal(t, 20, fromText.Size())
	assert.Equal(t, 20, fromBinary.Size())
	checkPairs(t, fromText, 20)
	checkPairs(t, fromBinary, 20)
	checkOddText(t, fromText)
	checkFileErrors(t, fromBinary)
}

func TestCuckooTableSerialization(t *testing.T) {
	table := NewCuckooTable(16)
	table.Insert("aa", "one")
	table.Insert("aq", "two") // одинаковый h1: одна пара лежит в T2
	for i := 0; i < 5; i++ {
		table.Insert("key"+strconv.Itoa(i), "value"+strconv.Itoa(i))
	}
	fromText, fromBinary := NewCuckooTable(8), NewCuckooTable(8)
	roundTrip(t, table, fromText, fromBinary)
	assert.Equal(t, 7, fromText.Size())
	assert.Equal(t, 7, fromBinary.Size())
	checkPairs(t, fromText, 5)
	checkPairs(t, fromBinary, 5)
	value, ok := fromBinary.Get("aq")
	assert.True(t, ok)
	assert.Equal(t, "two", value)
	checkOddText(t, fromText)
	checkFileErrors(t, fromBinary)
}
