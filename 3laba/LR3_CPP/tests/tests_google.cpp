#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <fstream>

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
    EXPECT_TRUE(!t.Contains(10));
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
    EXPECT_TRUE(!t.Contains(100));
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
    EXPECT_TRUE(!t.Contains(5));
    EXPECT_EQ(t.Size(), 3);
    t.Remove(100);
    EXPECT_EQ(t.Size(), 3);
    t.Remove(15);
    EXPECT_TRUE(!t.Contains(15));
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
    EXPECT_TRUE(!t.Get("missing", value));
}

TEST(ChainTableTest, UpdateAndDelete) {
    ChainTable t;
    string value;
    EXPECT_TRUE(!t.Insert("", "bad"));
    EXPECT_TRUE(t.Insert("name", "Ivan"));
    EXPECT_TRUE(!t.Insert("name", "Petr"));
    EXPECT_EQ(t.Size(), 1);
    EXPECT_TRUE(t.Get("name", value));
    EXPECT_EQ(value, "Petr");
    EXPECT_TRUE(t.Remove("name"));
    EXPECT_TRUE(!t.Remove("name"));
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
    EXPECT_TRUE(!t.Get("missing", value));
}

TEST(CuckooTableTest, UpdateAndDelete) {
    CuckooTable t;
    string value;
    EXPECT_TRUE(!t.Insert("", "bad"));
    EXPECT_TRUE(t.Insert("name", "Ivan"));
    EXPECT_TRUE(!t.Insert("name", "Petr"));
    EXPECT_EQ(t.Size(), 1);
    EXPECT_TRUE(t.Get("name", value));
    EXPECT_EQ(value, "Petr");
    EXPECT_TRUE(t.Remove("name"));
    EXPECT_TRUE(!t.Remove("name"));
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

// Дополнительные тесты для увеличения покрытия кода.

TEST(ArrayTest, PrintAndGet) {
    Array a;
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    a.Print();
    a.Get(0);
    a.PushBack("one");
    a.PushBack("two");
    a.Print();
    a.Get(1);
    a.Get(-1);
    a.Get(10);
    a.Length();
    cout.rdbuf(old);
    EXPECT_EQ(output.str(), "\none two \ntwo\nДлинна массива: 2\n");
}

TEST(ArrayTest, InsertAtEndAndFull) {
    Array a;
    a.Insert(0, "first");
    for (int i = 1; i < 10; ++i) {
        a.PushBack(to_string(i));
    }
    a.Insert(5, "middle");  // массив заполнен: проверяем увеличение
    a.Insert(a.Size(), "last");
    EXPECT_EQ(a.Size(), 12);
    EXPECT_EQ(a.At(5), "middle");
    EXPECT_EQ(a.At(11), "last");
    a.DelIndx(a.Size() - 1);
    a.DelIndx(0);
    EXPECT_EQ(a.Size(), 10);
    EXPECT_EQ(a.At(-1), "");
    EXPECT_EQ(a.At(100), "");
}

TEST(LinkedListTest, EmptyOperations) {
    linkedList l;
    l.pushBefore("none", "x");
    l.pushAfter("none", "x");
    l.DelVal("none");
    l.delBefore("none");
    l.delAfter("none");
    l.GetHead();
    l.GetTail();
    l.GetIndx(-1);
    l.GetIndx(1);
    l.GetVal("none");
    EXPECT_EQ(l.Size(), 0);
    EXPECT_EQ(l.At(-1), "");
    EXPECT_EQ(l.At(2), "");
}

TEST(LinkedListTest, PrintAndGet) {
    linkedList l;
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    l.Print();
    l.pushBack("a");
    l.pushBack("b");
    l.Print();
    l.GetHead();
    l.GetTail();
    l.GetIndx(0);
    l.GetIndx(1);
    l.GetIndx(10);
    l.GetVal("b");
    l.GetVal("missing");
    cout.rdbuf(old);
    EXPECT_EQ(output.str(), "\na b \na\nb\na\nb\n1\n");
}

TEST(LinkedListTest, FirstLastAndMissing) {
    linkedList l;
    l.pushFront("middle");
    l.pushBefore("middle", "first");
    l.pushAfter("middle", "last");
    EXPECT_EQ(l.ToString(), "first middle last");
    l.pushBefore("missing", "x");
    l.delBefore("first");
    l.delAfter("last");
    l.DelVal("missing");
    EXPECT_EQ(l.ToString(), "first middle last");
    l.DelVal("last");
    EXPECT_EQ(l.ToString(), "first middle");
    l.delBack();
    EXPECT_EQ(l.ToString(), "first");
    l.DelVal("first");
    EXPECT_EQ(l.Size(), 0);
}

TEST(LinkedListTest, SingleElement) {
    linkedList a;
    a.pushFront("one");
    a.delFront();
    EXPECT_EQ(a.Size(), 0);
    a.pushBack("two");
    a.delBack();
    EXPECT_EQ(a.Size(), 0);
    a.pushBack("three");
    a.DelVal("three");
    EXPECT_EQ(a.Size(), 0);
    a.pushBack("four");
    a.pushBack("five");
    a.delAfter("four");
    EXPECT_EQ(a.ToString(), "four");
}

TEST(LinkedListTest, DeleteNeighbours) {
    linkedList l;
    l.pushBack("a");
    l.pushBack("b");
    l.pushBack("c");
    l.pushBack("d");
    l.delBefore("b"); // первый
    EXPECT_EQ(l.ToString(), "b c d");
    l.delBefore("d"); // средний
    EXPECT_EQ(l.ToString(), "b d");
    l.delAfter("b"); // последний
    EXPECT_EQ(l.ToString(), "b");
    l.delBefore("missing");
    EXPECT_EQ(l.ToString(), "b");
}

TEST(DoublyListTest, EmptyOperations) {
    DoublyList l;
    l.delFront();
    l.delBack();
    l.delBefore("x");
    l.delAfter("x");
    l.DelVal("x");
    l.pushBefore("x", "a");
    l.pushAfter("x", "a");
    EXPECT_EQ(l.Size(), 0);
    EXPECT_EQ(l.At(-1), "");
    EXPECT_EQ(l.At(5), "");
    EXPECT_EQ(l.Find("x"), -1);
}

TEST(DoublyListTest, Output) {
    DoublyList l;
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    l.Print();
    l.GetHead();
    l.GetTail();
    l.GetIndx(-1);
    l.GetIndx(0);
    l.GetVal("missing");
    l.pushBack("first");
    l.pushBack("second");
    l.Print();
    l.GetHead();
    l.GetTail();
    l.GetIndx(1);
    l.GetVal("second");
    cout.rdbuf(old);
    EXPECT_TRUE(output.str().find("first <-> second") != string::npos);
    EXPECT_TRUE(output.str().find("ЗНАЧЕНИЕ \"second\" ИНДЕКС 1") != string::npos);
    EXPECT_TRUE(output.str().find("ПУСТОЙ") != string::npos);
}

TEST(DoublyListTest, FirstAndLast) {
    DoublyList l;
    l.pushFront("b");
    l.pushBefore("b", "a");
    l.pushAfter("b", "c");
    EXPECT_EQ(l.ToString(), "a b c");
    l.delFront();
    EXPECT_EQ(l.ToString(), "b c");
    l.delBack();
    EXPECT_EQ(l.ToString(), "b");
    l.delBack();
    EXPECT_EQ(l.Size(), 0);
    l.pushBack("again");
    l.delFront();
    EXPECT_EQ(l.Size(), 0);
}

TEST(DoublyListTest, DeleteValues) {
    DoublyList l;
    l.pushBack("a");
    l.pushBack("b");
    l.pushBack("c");
    l.pushBack("d");
    l.pushBack("e");
    l.DelVal("a");
    l.DelVal("e");
    l.DelVal("c");
    EXPECT_EQ(l.ToString(), "b d");
    l.DelVal("missing");
    l.delBefore("b");
    l.delAfter("d");
    EXPECT_EQ(l.ToString(), "b d");
    l.DelVal("b");
    l.DelVal("d");
    EXPECT_EQ(l.Size(), 0);
}

TEST(DoublyListTest, Neighbours) {
    DoublyList l;
    l.pushBack("a");
    l.pushBack("b");
    l.pushBack("c");
    l.pushBack("d");
    l.pushBack("e");
    l.delBefore("d");   // удаляем средний узел c
    EXPECT_EQ(l.ToString(), "a b d e");
    l.delAfter("b");    // удаляем средний узел d
    EXPECT_EQ(l.ToString(), "a b e");
    l.delBefore("b");   // удаляем первый a
    EXPECT_EQ(l.ToString(), "b e");
    l.delAfter("b");    // удаляем последний e
    EXPECT_EQ(l.ToString(), "b");
    EXPECT_EQ(l.At(0), "b");
}

TEST(StackTest, EmptyOutput) {
    Stack s;
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    s.peek();
    s.Print();
    s.pop();
    cout.rdbuf(old);
    EXPECT_TRUE(output.str().find("Пуст") != string::npos);
    EXPECT_TRUE(output.str().find("Error!") != string::npos);
    EXPECT_EQ(s.Size(), 0);
}

TEST(StackTest, OutputAndOrder) {
    Stack s;
    s.push("one");
    s.push("two");
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    s.peek();
    s.Print();
    cout.rdbuf(old);
    EXPECT_EQ(output.str(), "two\ntwo\none\n");
    s.pop();
    EXPECT_EQ(s.ToString(), "one");
}

TEST(QueueTest, EmptyOutput) {
    Queue q;
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    q.peek();
    q.Print();
    q.pop();
    cout.rdbuf(old);
    EXPECT_TRUE(output.str().find("Пуста") != string::npos);
    EXPECT_TRUE(output.str().find("Error!") != string::npos);
    EXPECT_EQ(q.Size(), 0);
}

TEST(QueueTest, OutputAndOrder) {
    Queue q;
    q.push("one");
    q.push("two");
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    q.peek();
    q.Print();
    cout.rdbuf(old);
    EXPECT_EQ(output.str(), "one\none two \n");
    q.pop();
    EXPECT_EQ(q.ToString(), "two");
}

TEST(RBTreeTest, EmptyOutput) {
    RBTree t;
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    t.Print();
    t.PrintInorder();
    t.PrintPreorder();
    t.PrintPostorder();
    t.PrintBFS();
    cout.rdbuf(old);
    EXPECT_EQ(output.str(), "Пустое\nПустое\nПустое\nПустое\nПустое\n");
    EXPECT_EQ(t.ToString(), "");
}

TEST(RBTreeTest, Traversals) {
    RBTree t;
    for (int key : {10, 5, 15, 2, 7, 12, 20}) t.Insert(key);
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    t.Print();
    string picture = output.str();
    output.str("");
    t.PrintInorder();
    string inorder = output.str();
    output.str("");
    t.PrintPreorder();
    string preorder = output.str();
    output.str("");
    t.PrintPostorder();
    string postorder = output.str();
    output.str("");
    t.PrintBFS();
    string bfs = output.str();
    cout.rdbuf(old);

    // Проверяем именно порядок каждого обхода, а не только наличие чисел.
    EXPECT_EQ(inorder, "2 5 7 10 12 15 20 \n");
    EXPECT_EQ(preorder, "10 5 2 7 15 12 20 \n");
    EXPECT_EQ(postorder, "2 7 5 12 20 15 10 \n");
    EXPECT_EQ(bfs, "10 5 15 2 7 12 20 \n");
    EXPECT_EQ(t.Size(), 7);
    EXPECT_TRUE(picture.find("10(") != string::npos);
}

TEST(RBTreeTest, LoadAndSave) {
    RBTree t;
    t.Load(10, 'B');
    t.Load(5, 'R');
    t.Load(15, 'R');
    EXPECT_EQ(t.Size(), 3);
    EXPECT_EQ(t.ToString(), "10 B 5 R 15 R");
    EXPECT_TRUE(t.Contains(5));
    EXPECT_TRUE(t.Contains(15));
}

TEST(RBTreeTest, InsertRotations) {
    const int keys[6][3] = {
        {30, 20, 10}, {10, 20, 30}, {30, 10, 20},
        {10, 30, 20}, {20, 10, 30}, {20, 30, 10}
    };
    for (int i = 0; i < 6; ++i) {
        RBTree t;
        for (int j = 0; j < 3; ++j) t.Insert(keys[i][j]);
        EXPECT_EQ(t.Size(), 3);
        EXPECT_TRUE(t.Contains(10));
        EXPECT_TRUE(t.Contains(20));
        EXPECT_TRUE(t.Contains(30));
        EXPECT_TRUE(!t.Contains(40));
    }
}

TEST(RBTreeTest, RemoveRootCases) {
    {
        RBTree t;
        t.Insert(10);
        t.Remove(10);
        EXPECT_EQ(t.Size(), 0);
    }
    {
        RBTree t;
        t.Insert(10);
        t.Insert(5);
        t.Remove(10);  // у корня только левый потомок
        EXPECT_EQ(t.Size(), 1);
        EXPECT_TRUE(t.Contains(5));
    }
    {
        RBTree t;
        t.Insert(10);
        t.Insert(15);
        t.Remove(10);  // у корня только правый потомок
        EXPECT_EQ(t.Size(), 1);
        EXPECT_TRUE(t.Contains(15));
    }
    {
        RBTree t;
        t.Insert(10);
        t.Insert(5);
        t.Insert(15);
        t.Remove(10);  // у корня два потомка
        EXPECT_EQ(t.Size(), 2);
        EXPECT_TRUE(!t.Contains(10));
        EXPECT_TRUE(t.Contains(5));
        EXPECT_TRUE(t.Contains(15));
    }
}

TEST(RBTreeTest, RemoveMany) {
    RBTree t;
    for (int i = 0; i < 30; ++i) t.Insert(i);
    EXPECT_EQ(t.Size(), 30);
    for (int i = 0; i < 30; ++i) {
        EXPECT_TRUE(t.Contains(i));
        t.Remove(i);
        EXPECT_TRUE(!t.Contains(i));
        EXPECT_EQ(t.Size(), 29 - i);
    }
}

TEST(RBTreeTest, RemoveReverse) {
    RBTree t;
    for (int i = 29; i >= 0; --i) t.Insert(i);
    EXPECT_EQ(t.Size(), 30);
    for (int i = 29; i >= 0; --i) {
        t.Remove(i);
        EXPECT_TRUE(!t.Contains(i));
        EXPECT_EQ(t.Size(), i);
    }
}

TEST(ChainTableTest, EmptyAndPrint) {
    ChainTable t(0);
    EXPECT_EQ(t.Capacity(), 1);
    EXPECT_EQ(t.Size(), 0);
    EXPECT_EQ(t.ToString(), "");
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    t.Print();
    cout.rdbuf(old);
    EXPECT_EQ(output.str(), "Пустая\n");
    string value;
    EXPECT_TRUE(!t.Get("missing", value));
    EXPECT_TRUE(!t.Remove("missing"));
}

TEST(ChainTableTest, CollisionsAndPrint) {
    ChainTable t(128);
    EXPECT_TRUE(t.Insert("aa", "one"));
    EXPECT_TRUE(t.Insert("ee", "two"));   // коллизия: одинаковый остаток по модулю 128
    EXPECT_TRUE(t.Insert("ii", "three"));
    EXPECT_EQ(t.Size(), 3);
    string value;
    ASSERT_TRUE(t.Get("ee", value));
    EXPECT_EQ(value, "two");
    EXPECT_TRUE(t.Remove("ee"));
    EXPECT_TRUE(t.Get("aa", value));
    EXPECT_EQ(value, "one");
    EXPECT_TRUE(t.Get("ii", value));
    EXPECT_EQ(value, "three");
    EXPECT_TRUE(!t.Insert("aa", "new"));
    ASSERT_TRUE(t.Get("aa", value));
    EXPECT_EQ(value, "new");
    EXPECT_TRUE(t.ToString().find("aa new") != string::npos);
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    t.Print();
    cout.rdbuf(old);
    EXPECT_TRUE(output.str().find("aa=new") != string::npos);
}

TEST(CuckooTableTest, EmptyAndPrint) {
    CuckooTable t(0);
    EXPECT_EQ(t.Size(), 0);
    EXPECT_EQ(t.ToString(), "");
    string value;
    EXPECT_TRUE(!t.Get("", value));
    EXPECT_TRUE(!t.Remove(""));
    EXPECT_TRUE(!t.Remove("missing"));
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    t.Print();
    cout.rdbuf(old);
    EXPECT_EQ(output.str(), "Пустая\n");
}

TEST(CuckooTableTest, SecondTable) {
    CuckooTable t(16);
    EXPECT_TRUE(t.Insert("aa", "one"));
    EXPECT_TRUE(t.Insert("aq", "two")); // два ключа с одним h1 и h2
    EXPECT_EQ(t.Size(), 2);
    string value;
    ASSERT_TRUE(t.Get("aa", value));
    EXPECT_EQ(value, "one");
    ASSERT_TRUE(t.Get("aq", value));
    EXPECT_EQ(value, "two");
    EXPECT_TRUE(!t.Insert("aa", "updated"));
    EXPECT_TRUE(!t.Insert("aq", "updated2"));
    ASSERT_TRUE(t.Get("aq", value));
    EXPECT_EQ(value, "updated2");
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    t.Print();
    cout.rdbuf(old);
    EXPECT_TRUE(output.str().find("T1[") != string::npos);
    EXPECT_TRUE(output.str().find("T2[") != string::npos);
    EXPECT_TRUE(t.ToString().find("aa updated") != string::npos);
    EXPECT_TRUE(t.Remove("aq"));
    EXPECT_TRUE(!t.Get("aq", value));
    EXPECT_EQ(t.Size(), 1);
    EXPECT_TRUE(t.Remove("aa"));
    EXPECT_EQ(t.Size(), 0);
}

TEST(CuckooTableTest, CycleAndRehash) {
    CuckooTable t(16);
    EXPECT_TRUE(t.Insert("aa", "one"));
    EXPECT_TRUE(t.Insert("aq", "two"));
    EXPECT_TRUE(t.Insert("aA", "three")); // вызываем вытеснения и перестройку
    EXPECT_EQ(t.Size(), 3);
    string value;
    ASSERT_TRUE(t.Get("aa", value));
    EXPECT_EQ(value, "one");
    ASSERT_TRUE(t.Get("aq", value));
    EXPECT_EQ(value, "two");
    ASSERT_TRUE(t.Get("aA", value));
    EXPECT_EQ(value, "three");
}

TEST(SerializationTest, Array) {
    Array a; a.PushBack("one"); a.PushBack("with space"); a.PushBack("");
    Array b;
    ASSERT_TRUE(a.saveToFile("serial_array.txt"));
    ASSERT_TRUE(b.loadFromFile("serial_array.txt"));
    EXPECT_EQ(b.Size(), 3);
    EXPECT_EQ(b.At(1), "with space");
    ASSERT_TRUE(a.saveBinary("serial_array.bin"));
    ASSERT_TRUE(b.loadBinary("serial_array.bin"));
    EXPECT_EQ(b.ToString(), a.ToString());
    std::remove("serial_array.txt"); std::remove("serial_array.bin");
}

TEST(SerializationTest, LinkedList) {
    linkedList a; a.pushBack("first"); a.pushBack("second");
    linkedList b;
    ASSERT_TRUE(a.saveToFile("serial_list.txt"));
    ASSERT_TRUE(b.loadFromFile("serial_list.txt"));
    EXPECT_EQ(b.ToString(), a.ToString());
    ASSERT_TRUE(a.saveBinary("serial_list.bin"));
    ASSERT_TRUE(b.loadBinary("serial_list.bin"));
    EXPECT_EQ(b.ToString(), a.ToString());
    std::remove("serial_list.txt"); std::remove("serial_list.bin");
}

TEST(SerializationTest, DoublyList) {
    DoublyList a; a.pushFront("first"); a.pushBack("second");
    DoublyList b;
    ASSERT_TRUE(a.saveToFile("serial_dlist.txt"));
    ASSERT_TRUE(b.loadFromFile("serial_dlist.txt"));
    EXPECT_EQ(b.ToString(), a.ToString());
    ASSERT_TRUE(a.saveBinary("serial_dlist.bin"));
    ASSERT_TRUE(b.loadBinary("serial_dlist.bin"));
    EXPECT_EQ(b.ToString(), a.ToString());
    std::remove("serial_dlist.txt"); std::remove("serial_dlist.bin");
}

TEST(SerializationTest, Stack) {
    Stack a; a.push("bottom"); a.push("top");
    Stack b;
    ASSERT_TRUE(a.saveToFile("serial_stack.txt"));
    ASSERT_TRUE(b.loadFromFile("serial_stack.txt"));
    EXPECT_EQ(b.ToString(), "bottom top");
    ASSERT_TRUE(a.saveBinary("serial_stack.bin"));
    ASSERT_TRUE(b.loadBinary("serial_stack.bin"));
    EXPECT_EQ(b.ToString(), a.ToString());
    std::remove("serial_stack.txt"); std::remove("serial_stack.bin");
}

TEST(SerializationTest, Queue) {
    Queue a; a.push("first"); a.push("last");
    Queue b;
    ASSERT_TRUE(a.saveToFile("serial_queue.txt"));
    ASSERT_TRUE(b.loadFromFile("serial_queue.txt"));
    EXPECT_EQ(b.ToString(), a.ToString());
    ASSERT_TRUE(a.saveBinary("serial_queue.bin"));
    ASSERT_TRUE(b.loadBinary("serial_queue.bin"));
    EXPECT_EQ(b.ToString(), "first last");
    std::remove("serial_queue.txt"); std::remove("serial_queue.bin");
}

TEST(SerializationTest, RBTree) {
    RBTree a; a.Insert(10); a.Insert(5); a.Insert(15);
    RBTree b;
    ASSERT_TRUE(a.saveToFile("serial_tree.txt"));
    ASSERT_TRUE(b.loadFromFile("serial_tree.txt"));
    EXPECT_EQ(b.ToString(), a.ToString());
    ASSERT_TRUE(a.saveBinary("serial_tree.bin"));
    ASSERT_TRUE(b.loadBinary("serial_tree.bin"));
    EXPECT_EQ(b.ToString(), a.ToString());
    std::remove("serial_tree.txt"); std::remove("serial_tree.bin");
}

TEST(SerializationTest, ChainTable) {
    ChainTable a; a.Insert("one", "value with spaces"); a.Insert("two", "second");
    ChainTable b; string value;
    ASSERT_TRUE(a.saveToFile("serial_chain.txt"));
    ASSERT_TRUE(b.loadFromFile("serial_chain.txt"));
    ASSERT_TRUE(b.Get("one", value));
    EXPECT_EQ(value, "value with spaces");
    ASSERT_TRUE(a.saveBinary("serial_chain.bin"));
    ASSERT_TRUE(b.loadBinary("serial_chain.bin"));
    ASSERT_TRUE(b.Get("two", value));
    EXPECT_EQ(value, "second");
    std::remove("serial_chain.txt"); std::remove("serial_chain.bin");
}

TEST(SerializationTest, CuckooTable) {
    CuckooTable a; a.Insert("one", "value with spaces"); a.Insert("two", "second");
    CuckooTable b; string value;
    ASSERT_TRUE(a.saveToFile("serial_cuckoo.txt"));
    ASSERT_TRUE(b.loadFromFile("serial_cuckoo.txt"));
    ASSERT_TRUE(b.Get("one", value));
    EXPECT_EQ(value, "value with spaces");
    ASSERT_TRUE(a.saveBinary("serial_cuckoo.bin"));
    ASSERT_TRUE(b.loadBinary("serial_cuckoo.bin"));
    ASSERT_TRUE(b.Get("two", value));
    EXPECT_EQ(value, "second");
    std::remove("serial_cuckoo.txt"); std::remove("serial_cuckoo.bin");
}

TEST(SerializationTest, Errors) {
    { ofstream bad("serial_bad.bin", ios::binary); bad << "bad"; }
    Array a; a.PushBack("keep");
    EXPECT_TRUE(!a.loadBinary("serial_bad.bin"));
    EXPECT_EQ(a.ToString(), "keep");
    ASSERT_TRUE(a.saveBinary("serial_wrong.bin"));
    linkedList l; l.pushBack("keep");
    EXPECT_TRUE(!l.loadBinary("serial_wrong.bin"));
    EXPECT_EQ(l.ToString(), "keep");
    EXPECT_TRUE(!l.loadFromFile("serial_bad.bin"));
    EXPECT_EQ(l.ToString(), "keep");
    std::remove("serial_bad.bin"); std::remove("serial_wrong.bin");
}
