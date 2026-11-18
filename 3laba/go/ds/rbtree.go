package ds

import (
	"bufio"
	"strconv"
	"strings"
)

const (
	red   = 'R'
	black = 'B'
)

// treeNode — узел красно-чёрного дерева (закрытый тип).
type treeNode struct {
	key    int
	color  byte
	left   *treeNode
	right  *treeNode
	parent *treeNode
}

// RBTree — красно-чёрное дерево целых ключей (как RBTree в C++).
type RBTree struct {
	root *treeNode
}

func NewRBTree() *RBTree {
	return &RBTree{}
}

// isRed: пустой узел (nil) считается чёрным.
func isRed(node *treeNode) bool {
	return node != nil && node.color == red
}

// Левый поворот вокруг x: правый ребёнок y поднимается на место x.
func (t *RBTree) leftRotate(x *treeNode) {
	y := x.right
	x.right = y.left
	if y.left != nil {
		y.left.parent = x
	}
	t.replaceChild(x, y)
	y.left = x
	x.parent = y
}

func (t *RBTree) rightRotate(x *treeNode) {
	y := x.left
	x.left = y.right
	if y.right != nil {
		y.right.parent = x
	}
	t.replaceChild(x, y)
	y.right = x
	x.parent = y
}

// replaceChild ставит v на место u у родителя u (transplant).
func (t *RBTree) replaceChild(u, v *treeNode) {
	if u.parent == nil {
		t.root = v
	} else if u == u.parent.left {
		u.parent.left = v
	} else {
		u.parent.right = v
	}
	if v != nil {
		v.parent = u.parent
	}
}

// Insert добавляет ключ; false — такой ключ уже есть.
func (t *RBTree) Insert(key int) bool {
	var parent *treeNode
	cur := t.root
	for cur != nil {
		parent = cur
		if key == cur.key {
			return false
		}
		if key < cur.key {
			cur = cur.left
		} else {
			cur = cur.right
		}
	}
	node := &treeNode{key: key, color: red, parent: parent} // новый узел всегда красный
	if parent == nil {
		t.root = node
	} else if key < parent.key {
		parent.left = node
	} else {
		parent.right = node
	}
	t.insertFixup(node)
	return true
}

// insertFixup убирает два красных узла подряд.
func (t *RBTree) insertFixup(node *treeNode) {
	for node != t.root && node.parent.color == red {
		parent := node.parent
		grand := parent.parent
		if parent == grand.left {
			uncle := grand.right
			if isRed(uncle) { // дядя красный: перекрашиваем и поднимаемся
				parent.color, uncle.color, grand.color = black, black, red
				node = grand
				continue
			}
			if node == parent.right {
				node = parent
				t.leftRotate(node)
				parent = node.parent
			}
			parent.color, grand.color = black, red
			t.rightRotate(grand)
		} else {
			uncle := grand.left
			if isRed(uncle) {
				parent.color, uncle.color, grand.color = black, black, red
				node = grand
				continue
			}
			if node == parent.left {
				node = parent
				t.rightRotate(node)
				parent = node.parent
			}
			parent.color, grand.color = black, red
			t.leftRotate(grand)
		}
	}
	t.root.color = black
}

func (t *RBTree) findNode(key int) *treeNode {
	cur := t.root
	for cur != nil && cur.key != key {
		if key < cur.key {
			cur = cur.left
		} else {
			cur = cur.right
		}
	}
	return cur
}

func (t *RBTree) Contains(key int) bool {
	return t.findNode(key) != nil
}

// Remove удаляет ключ с сохранением свойств дерева; false — ключа нет.
func (t *RBTree) Remove(key int) bool {
	z := t.findNode(key)
	if z == nil {
		return false
	}
	y := z // узел, который уходит со своего места
	yColor := y.color
	var x, xParent *treeNode // x встаёт на место y; x может быть nil, поэтому храним родителя
	switch {
	case z.left == nil:
		x, xParent = z.right, z.parent
		t.replaceChild(z, z.right)
	case z.right == nil:
		x, xParent = z.left, z.parent
		t.replaceChild(z, z.left)
	default:
		y = z.right // минимальный в правом поддереве
		for y.left != nil {
			y = y.left
		}
		yColor = y.color
		x = y.right
		if y.parent == z {
			xParent = y
		} else {
			xParent = y.parent
			t.replaceChild(y, y.right)
			y.right = z.right
			y.right.parent = y
		}
		t.replaceChild(z, y)
		y.left = z.left
		y.left.parent = y
		y.color = z.color
	}
	if yColor == black { // ушёл чёрный узел — восстанавливаем чёрную высоту
		t.deleteFixup(x, xParent)
	}
	return true
}

func (t *RBTree) deleteFixup(x, xParent *treeNode) {
	for x != t.root && !isRed(x) {
		if x == xParent.left {
			brother := xParent.right
			if isRed(brother) {
				brother.color, xParent.color = black, red
				t.leftRotate(xParent)
				brother = xParent.right
			}
			if !isRed(brother.left) && !isRed(brother.right) {
				brother.color = red
				x, xParent = xParent, xParent.parent
				continue
			}
			if !isRed(brother.right) {
				brother.left.color, brother.color = black, red
				t.rightRotate(brother)
				brother = xParent.right
			}
			brother.color, xParent.color = xParent.color, black
			brother.right.color = black
			t.leftRotate(xParent)
			x = t.root
		} else { // то же самое зеркально
			brother := xParent.left
			if isRed(brother) {
				brother.color, xParent.color = black, red
				t.rightRotate(xParent)
				brother = xParent.left
			}
			if !isRed(brother.left) && !isRed(brother.right) {
				brother.color = red
				x, xParent = xParent, xParent.parent
				continue
			}
			if !isRed(brother.left) {
				brother.right.color, brother.color = black, red
				t.leftRotate(brother)
				brother = xParent.left
			}
			brother.color, xParent.color = xParent.color, black
			brother.left.color = black
			t.rightRotate(xParent)
			x = t.root
		}
	}
	if x != nil {
		x.color = black
	}
}

// Обходы возвращают ключи в нужном порядке.

func (t *RBTree) Inorder() []int {
	var keys []int
	var walk func(node *treeNode)
	walk = func(node *treeNode) {
		if node != nil {
			walk(node.left)
			keys = append(keys, node.key)
			walk(node.right)
		}
	}
	walk(t.root)
	return keys
}

func (t *RBTree) Preorder() []int {
	var keys []int
	t.forEachPreorder(func(node *treeNode) {
		keys = append(keys, node.key)
	})
	return keys
}

func (t *RBTree) Postorder() []int {
	var keys []int
	var walk func(node *treeNode)
	walk = func(node *treeNode) {
		if node != nil {
			walk(node.left)
			walk(node.right)
			keys = append(keys, node.key)
		}
	}
	walk(t.root)
	return keys
}

// BFS — обход в ширину (по уровням).
func (t *RBTree) BFS() []int {
	var keys []int
	if t.root == nil {
		return keys
	}
	queue := []*treeNode{t.root}
	for len(queue) > 0 {
		node := queue[0]
		queue = queue[1:]
		keys = append(keys, node.key)
		if node.left != nil {
			queue = append(queue, node.left)
		}
		if node.right != nil {
			queue = append(queue, node.right)
		}
	}
	return keys
}

func (t *RBTree) Size() int {
	return len(t.Preorder())
}

// String — пары "ключ цвет" в прямом обходе (как ToString в C++).
func (t *RBTree) String() string {
	var parts []string
	t.forEachPreorder(func(node *treeNode) {
		parts = append(parts, strconv.Itoa(node.key)+" "+string(node.color))
	})
	return strings.Join(parts, " ")
}

func (t *RBTree) forEachPreorder(visit func(node *treeNode)) {
	var walk func(node *treeNode)
	walk = func(node *treeNode) {
		if node != nil {
			visit(node)
			walk(node.left)
			walk(node.right)
		}
	}
	walk(t.root)
}

// load ставит узел с готовым цветом на место по правилу BST (для чтения файла).
// Если вставлять узлы в прямом обходе, получится то же самое дерево.
func (t *RBTree) load(key int, color byte) {
	var parent *treeNode
	cur := t.root
	for cur != nil {
		parent = cur
		if key < cur.key {
			cur = cur.left
		} else {
			cur = cur.right
		}
	}
	node := &treeNode{key: key, color: color, parent: parent}
	if parent == nil {
		t.root = node
	} else if key < parent.key {
		parent.left = node
	} else {
		parent.right = node
	}
}

// ---------- Сериализация (прямой обход: ключ и цвет) ----------

func (t *RBTree) SaveText(fileName string) error {
	return saveFile(fileName, func(w *bufio.Writer) {
		t.forEachPreorder(func(node *treeNode) {
			writeLine(w, node.key, string(node.color))
		})
	})
}

func (t *RBTree) LoadText(fileName string) error {
	return loadFile(fileName, func(r *bufio.Reader) error {
		words := readWords(r)
		if len(words)%2 != 0 {
			return errBadFile // у последнего ключа нет цвета
		}
		tmp := NewRBTree()
		for i := 0; i < len(words); i += 2 {
			key, err := strconv.Atoi(words[i])
			if err != nil {
				return errBadFile // ключ не число
			}
			color := words[i+1]
			if color != "R" && color != "B" {
				return errBadFile
			}
			tmp.load(key, color[0])
		}
		*t = *tmp
		return nil
	})
}

func (t *RBTree) SaveBinary(fileName string) error {
	return saveFile(fileName, func(w *bufio.Writer) {
		writeInt(w, t.Size())
		t.forEachPreorder(func(node *treeNode) {
			writeInt(w, node.key)
			_ = w.WriteByte(node.color)
		})
	})
}

func (t *RBTree) LoadBinary(fileName string) error {
	return loadFile(fileName, func(r *bufio.Reader) error {
		count, err := readCount(r)
		if err != nil {
			return err
		}
		tmp := NewRBTree()
		for i := 0; i < count; i++ {
			key, err := readInt(r)
			if err != nil {
				return err
			}
			color, err := r.ReadByte()
			if err != nil {
				return err
			}
			if color != red && color != black {
				return errBadFile
			}
			tmp.load(key, color)
		}
		*t = *tmp
		return nil
	})
}
