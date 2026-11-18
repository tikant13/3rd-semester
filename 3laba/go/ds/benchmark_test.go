package ds

import (
	"strconv"
	"testing"
)

// Те же замеры, что в C++ benchmark.cpp: заполнение до цикла,
// в цикле — только чтение/поиск или пара push/pop. Размеры: 100, 1000, 10000.
var sizes = []int{100, 1000, 10000}

// Результаты кладём в эти переменные, чтобы компилятор не выкинул вызовы.
var (
	sinkString string
	sinkBool   bool
)

func BenchmarkArrayRead(b *testing.B) {
	for _, n := range sizes {
		b.Run(strconv.Itoa(n), func(b *testing.B) {
			a := NewArray()
			for i := 0; i < n; i++ {
				a.PushBack("value")
			}
			b.ResetTimer()
			for i := 0; i < b.N; i++ {
				sinkString, sinkBool = a.Get(n / 2)
			}
		})
	}
}

func BenchmarkLinkedListRead(b *testing.B) {
	for _, n := range sizes {
		b.Run(strconv.Itoa(n), func(b *testing.B) {
			l := NewLinkedList()
			for i := 0; i < n; i++ {
				l.PushBack("value")
			}
			b.ResetTimer()
			for i := 0; i < b.N; i++ {
				sinkString, sinkBool = l.Get(n / 2)
			}
		})
	}
}

func BenchmarkDoublyListRead(b *testing.B) {
	for _, n := range sizes {
		b.Run(strconv.Itoa(n), func(b *testing.B) {
			l := NewDoublyList()
			for i := 0; i < n; i++ {
				l.PushBack("value")
			}
			b.ResetTimer()
			for i := 0; i < b.N; i++ {
				sinkString, sinkBool = l.Get(n / 2)
			}
		})
	}
}

func BenchmarkStackPushPop(b *testing.B) {
	for _, n := range sizes {
		b.Run(strconv.Itoa(n), func(b *testing.B) {
			s := NewStack()
			for i := 0; i < n; i++ {
				s.Push("value")
			}
			b.ResetTimer()
			for i := 0; i < b.N; i++ {
				s.Push("value")
				sinkString, sinkBool = s.Pop()
			}
		})
	}
}

func BenchmarkQueuePushPop(b *testing.B) {
	for _, n := range sizes {
		b.Run(strconv.Itoa(n), func(b *testing.B) {
			q := NewQueue()
			for i := 0; i < n; i++ {
				q.Push("value")
			}
			b.ResetTimer()
			for i := 0; i < b.N; i++ {
				q.Push("value")
				sinkString, sinkBool = q.Pop()
			}
		})
	}
}

func BenchmarkRBTreeSearch(b *testing.B) {
	for _, n := range sizes {
		b.Run(strconv.Itoa(n), func(b *testing.B) {
			tree := NewRBTree()
			for i := 0; i < n; i++ {
				tree.Insert(i)
			}
			b.ResetTimer()
			for i := 0; i < b.N; i++ {
				sinkBool = tree.Contains(i % n)
			}
		})
	}
}

// keys заранее готовит ключи, чтобы не считать в цикле strconv.
func keys(n int) []string {
	result := make([]string, n)
	for i := range result {
		result[i] = "key" + strconv.Itoa(i)
	}
	return result
}

func BenchmarkChainTableSearch(b *testing.B) {
	for _, n := range sizes {
		b.Run(strconv.Itoa(n), func(b *testing.B) {
			table := NewChainTable(8)
			k := keys(n)
			for _, key := range k {
				table.Insert(key, "value")
			}
			b.ResetTimer()
			for i := 0; i < b.N; i++ {
				sinkString, sinkBool = table.Get(k[i%n])
			}
		})
	}
}

func BenchmarkCuckooTableSearch(b *testing.B) {
	for _, n := range sizes {
		b.Run(strconv.Itoa(n), func(b *testing.B) {
			table := NewCuckooTable(8)
			k := keys(n)
			for _, key := range k {
				table.Insert(key, "value")
			}
			b.ResetTimer()
			for i := 0; i < b.N; i++ {
				sinkString, sinkBool = table.Get(k[i%n])
			}
		})
	}
}
