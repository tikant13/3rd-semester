#include <gtest/gtest.h>
#include <string>

#include "array.h"
#include "linkedList.h"
#include "doublyList.h"
#include "stack.h"
#include "Queue.h"
#include "Rbtree.h"
#include "Chaintable.h"
#include "Cuckootable.h"

using namespace std;

// 1. Массив
TEST(ArrayTest, Empty) {
    Array a;
    EXPECT_EQ(a.Size(), 0);
    EXPECT_EQ(a.ToString(), "");
    EXPECT_EQ(a.At(0), "");
    EXPECT_EQ(a.At(-1), "");
}

TEST(ArrayTest, InsertAndDelete) {
    Array a;
    a.PushBack("one");
    a.PushBack("three");
    a.Insert(1, "two");
    a.Insert(0, "zero");
    EXPECT_EQ(a.ToString(), "zero one two three");

    a.Set(2, "TWO");
    EXPECT_EQ(a.At(2), "TWO");
    a.DelIndx(0);
    a.DelIndx(2);
    EXPECT_EQ(a.ToString(), "one TWO");
    EXPECT_EQ(a.Size(), 2);
}

TEST(ArrayTest, InvalidIndexes) {
    Array a;
    a.PushBack("hello");
    a.Insert(-1, "bad");
    a.Insert(10, "bad");
    a.Set(-1, "bad");
    a.Set(5, "bad");
    a.DelIndx(-1);
    a.DelIndx(10);
    EXPECT_EQ(a.Size(), 1);
    EXPECT_EQ(a.At(0), "hello");
}

TEST(ArrayTest, GrowthAndReset) {
    Array a;
    for (int i = 0; i < 30; ++i) {
        a.PushBack(to_string(i));
    }
    EXPECT_EQ(a.Size(), 30);
    EXPECT_EQ(a.At(0), "0");
    EXPECT_EQ(a.At(29), "29");
    a.Create();
    EXPECT_EQ(a.Size(), 0);
    a.PushBack("new");
    EXPECT_EQ(a.At(0), "new");
}

// 2. Односвязный список
TEST(LinkedListTest, Empty) {
    linkedList l;
    EXPECT_EQ(l.Size(), 0);
    EXPECT_EQ(l.Find("x"), -1);
    EXPECT_EQ(l.At(0), "");
    l.delFront();
    l.delBack();
    EXPECT_EQ(l.Size(), 0);
}

TEST(LinkedListTest, Insert) {
    linkedList l;
    l.pushBack("b");
    l.pushFront("a");
    l.pushAfter("b", "d");
    l.pushBefore("d", "c");
    EXPECT_EQ(l.ToString(), "a b c d");
    EXPECT_EQ(l.Find("c"), 2);
    EXPECT_EQ(l.Find("x"), -1);
    l.pushAfter("missing", "x");
    EXPECT_EQ(l.Size(), 4);
}

TEST(LinkedListTest, Delete) {
    linkedList l;
    l.pushBack("a");
    l.pushBack("b");
    l.pushBack("c");
    l.pushBack("d");
    l.DelVal("b");
    EXPECT_EQ(l.ToString(), "a c d");
    l.delAfter("a");
    EXPECT_EQ(l.ToString(), "a d");
    l.delBefore("d");
    EXPECT_EQ(l.ToString(), "d");
    l.delBack();
    EXPECT_EQ(l.Size(), 0);
}

// 3. Двусвязный список
TEST(DoublyListTest, Empty) {
    DoublyList l;
    EXPECT_EQ(l.Size(), 0);
    EXPECT_EQ(l.ToString(), "");
    EXPECT_EQ(l.Find("x"), -1);
}

TEST(DoublyListTest, Insert) {
    DoublyList l;
    l.pushBack("b");
    l.pushFront("a");
    l.pushAfter("b", "d");
    l.pushBefore("d", "c");
    EXPECT_EQ(l.ToString(), "a b c d");
    EXPECT_EQ(l.At(2), "c");
    EXPECT_EQ(l.Find("d"), 3);
}

TEST(DoublyListTest, Delete) {
    DoublyList l;
    l.pushBack("a");
    l.pushBack("b");
    l.pushBack("c");
    l.pushBack("d");
    l.DelVal("b");
    EXPECT_EQ(l.ToString(), "a c d");
    l.delBefore("d");
    EXPECT_EQ(l.ToString(), "a d");
    l.delAfter("a");
    EXPECT_EQ(l.ToString(), "a");
    l.delFront();
    EXPECT_EQ(l.Size(), 0);
}

// 4. Стек
TEST(StackTest, Empty) {
    Stack s;
    EXPECT_EQ(s.Size(), 0);
    EXPECT_EQ(s.ToString(), "");
}

TEST(StackTest, PushPop) {
    Stack s;
    s.push("first");
    s.push("second");
    s.push("third");
    EXPECT_EQ(s.Size(), 3);
    EXPECT_EQ(s.ToString(), "first second third");
    s.pop();
    EXPECT_EQ(s.ToString(), "first second");
    s.pop();
    s.pop();
    EXPECT_EQ(s.Size(), 0);
    s.push("again");
    EXPECT_EQ(s.ToString(), "again");
}

// 5. Очередь
TEST(QueueTest, Empty) {
    Queue q;
    EXPECT_EQ(q.Size(), 0);
    EXPECT_EQ(q.ToString(), "");
}

TEST(QueueTest, FirstInFirstOut) {
    Queue q;
    q.push("first");
    q.push("second");
    q.push("third");
    EXPECT_EQ(q.ToString(), "first second third");
    q.pop();
    EXPECT_EQ(q.ToString(), "second third");
    q.pop();
    q.pop();
    EXPECT_EQ(q.Size(), 0);
    q.push("again");
    EXPECT_EQ(q.ToString(), "again");
}

// 6. Красно-чёрное дерево
TEST(RBTreeTest, Empty) {
    RBTree t;
    EXPECT_EQ(t.Size(), 0);
    EXPECT_FALSE(t.Contains(10));
    t.Remove(10);
    EXPECT_EQ(t.Size(), 0);
}

TEST(RBTreeTest, InsertAndSearch) {
    RBTree t;
    t.Insert(10);
    t.Insert(5);
    t.Insert(20);
    t.Insert(15);
    EXPECT_EQ(t.Size(), 4);
    EXPECT_TRUE(t.Contains(5));
    EXPECT_TRUE(t.Contains(15));
    EXPECT_FALSE(t.Contains(100));
    t.Insert(10);
    EXPECT_EQ(t.Size(), 4);
}

TEST(RBTreeTest, Remove) {
    RBTree t;
    t.Insert(10);
    t.Insert(5);
    t.Insert(20);
    t.Insert(15);
    t.Remove(5);
    EXPECT_FALSE(t.Contains(5));
    EXPECT_EQ(t.Size(), 3);
    t.Remove(100);
    EXPECT_EQ(t.Size(), 3);
    t.Remove(15);
    EXPECT_FALSE(t.Contains(15));
    EXPECT_TRUE(t.Contains(10));
}

// 7. Хеш-таблица с цепочками
TEST(ChainTableTest, InsertAndGet) {
    ChainTable t;
    string value;
    EXPECT_EQ(t.Size(), 0);
    EXPECT_TRUE(t.Insert("name", "Ivan"));
    EXPECT_TRUE(t.Insert("age", "20"));
    EXPECT_TRUE(t.Get("name", value));
    EXPECT_EQ(value, "Ivan");
    EXPECT_EQ(t.Size(), 2);
    EXPECT_FALSE(t.Get("missing", value));
}

TEST(ChainTableTest, UpdateAndDelete) {
    ChainTable t;
    string value;
    EXPECT_FALSE(t.Insert("", "bad"));
    EXPECT_TRUE(t.Insert("name", "Ivan"));
    EXPECT_FALSE(t.Insert("name", "Petr"));
    EXPECT_EQ(t.Size(), 1);
    EXPECT_TRUE(t.Get("name", value));
    EXPECT_EQ(value, "Petr");
    EXPECT_TRUE(t.Remove("name"));
    EXPECT_FALSE(t.Remove("name"));
    EXPECT_EQ(t.Size(), 0);
}

TEST(ChainTableTest, Rehash) {
    ChainTable t(2);
    for (int i = 0; i < 20; ++i) {
        EXPECT_TRUE(t.Insert("key" + to_string(i), to_string(i)));
    }
    EXPECT_EQ(t.Size(), 20);
    EXPECT_TRUE(t.Capacity() > 2);
    for (int i = 0; i < 20; ++i) {
        string value;
        ASSERT_TRUE(t.Get("key" + to_string(i), value));
        EXPECT_EQ(value, to_string(i));
    }
}

// 8. Хеш-таблица с кукушкой
TEST(CuckooTableTest, InsertAndGet) {
    CuckooTable t;
    string value;
    EXPECT_EQ(t.Size(), 0);
    EXPECT_TRUE(t.Insert("name", "Ivan"));
    EXPECT_TRUE(t.Insert("age", "20"));
    EXPECT_TRUE(t.Get("name", value));
    EXPECT_EQ(value, "Ivan");
    EXPECT_FALSE(t.Get("missing", value));
}

TEST(CuckooTableTest, UpdateAndDelete) {
    CuckooTable t;
    string value;
    EXPECT_FALSE(t.Insert("", "bad"));
    EXPECT_TRUE(t.Insert("name", "Ivan"));
    EXPECT_FALSE(t.Insert("name", "Petr"));
    EXPECT_EQ(t.Size(), 1);
    EXPECT_TRUE(t.Get("name", value));
    EXPECT_EQ(value, "Petr");
    EXPECT_TRUE(t.Remove("name"));
    EXPECT_FALSE(t.Remove("name"));
    EXPECT_EQ(t.Size(), 0);
}

TEST(CuckooTableTest, Rehash) {
    CuckooTable t;
    for (int i = 0; i < 20; ++i) {
        EXPECT_TRUE(t.Insert("key" + to_string(i), to_string(i)));
    }
    EXPECT_EQ(t.Size(), 20);
    for (int i = 0; i < 20; ++i) {
        string value;
        ASSERT_TRUE(t.Get("key" + to_string(i), value));
        EXPECT_EQ(value, to_string(i));
    }
}
