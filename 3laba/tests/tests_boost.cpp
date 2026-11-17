#define BOOST_TEST_MODULE Lab3BoostTests
#include <boost/test/included/unit_test.hpp>
#include <string>
#include <sstream>
#include <iostream>

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
BOOST_AUTO_TEST_CASE(ArrayEmpty) {
    Array a;
    BOOST_CHECK_EQUAL(a.Size(), 0);
    BOOST_CHECK_EQUAL(a.ToString(), "");
    BOOST_CHECK_EQUAL(a.At(0), "");
    BOOST_CHECK_EQUAL(a.At(-1), "");
}

BOOST_AUTO_TEST_CASE(ArrayInsertAndDelete) {
    Array a;
    a.PushBack("one");
    a.PushBack("three");
    a.Insert(1, "two");
    a.Insert(0, "zero");
    BOOST_CHECK_EQUAL(a.ToString(), "zero one two three");

    a.Set(2, "TWO");
    BOOST_CHECK_EQUAL(a.At(2), "TWO");
    a.DelIndx(0);
    a.DelIndx(2);
    BOOST_CHECK_EQUAL(a.ToString(), "one TWO");
    BOOST_CHECK_EQUAL(a.Size(), 2);
}

BOOST_AUTO_TEST_CASE(ArrayInvalidIndexes) {
    Array a;
    a.PushBack("hello");
    a.Insert(-1, "bad");
    a.Insert(10, "bad");
    a.Set(-1, "bad");
    a.Set(5, "bad");
    a.DelIndx(-1);
    a.DelIndx(10);
    BOOST_CHECK_EQUAL(a.Size(), 1);
    BOOST_CHECK_EQUAL(a.At(0), "hello");
}

BOOST_AUTO_TEST_CASE(ArrayGrowthAndReset) {
    Array a;
    for (int i = 0; i < 30; ++i) {
        a.PushBack(to_string(i));
    }
    BOOST_CHECK_EQUAL(a.Size(), 30);
    BOOST_CHECK_EQUAL(a.At(0), "0");
    BOOST_CHECK_EQUAL(a.At(29), "29");
    a.Create();
    BOOST_CHECK_EQUAL(a.Size(), 0);
    a.PushBack("new");
    BOOST_CHECK_EQUAL(a.At(0), "new");
}

// 2. Односвязный список
BOOST_AUTO_TEST_CASE(LinkedListEmpty) {
    linkedList l;
    BOOST_CHECK_EQUAL(l.Size(), 0);
    BOOST_CHECK_EQUAL(l.Find("x"), -1);
    BOOST_CHECK_EQUAL(l.At(0), "");
    l.delFront();
    l.delBack();
    BOOST_CHECK_EQUAL(l.Size(), 0);
}

BOOST_AUTO_TEST_CASE(LinkedListInsert) {
    linkedList l;
    l.pushBack("b");
    l.pushFront("a");
    l.pushAfter("b", "d");
    l.pushBefore("d", "c");
    BOOST_CHECK_EQUAL(l.ToString(), "a b c d");
    BOOST_CHECK_EQUAL(l.Find("c"), 2);
    BOOST_CHECK_EQUAL(l.Find("x"), -1);
    l.pushAfter("missing", "x");
    BOOST_CHECK_EQUAL(l.Size(), 4);
}

BOOST_AUTO_TEST_CASE(LinkedListDelete) {
    linkedList l;
    l.pushBack("a");
    l.pushBack("b");
    l.pushBack("c");
    l.pushBack("d");
    l.DelVal("b");
    BOOST_CHECK_EQUAL(l.ToString(), "a c d");
    l.delAfter("a");
    BOOST_CHECK_EQUAL(l.ToString(), "a d");
    l.delBefore("d");
    BOOST_CHECK_EQUAL(l.ToString(), "d");
    l.delBack();
    BOOST_CHECK_EQUAL(l.Size(), 0);
}

// 3. Двусвязный список
BOOST_AUTO_TEST_CASE(DoublyListEmpty) {
    DoublyList l;
    BOOST_CHECK_EQUAL(l.Size(), 0);
    BOOST_CHECK_EQUAL(l.ToString(), "");
    BOOST_CHECK_EQUAL(l.Find("x"), -1);
}

BOOST_AUTO_TEST_CASE(DoublyListInsert) {
    DoublyList l;
    l.pushBack("b");
    l.pushFront("a");
    l.pushAfter("b", "d");
    l.pushBefore("d", "c");
    BOOST_CHECK_EQUAL(l.ToString(), "a b c d");
    BOOST_CHECK_EQUAL(l.At(2), "c");
    BOOST_CHECK_EQUAL(l.Find("d"), 3);
}

BOOST_AUTO_TEST_CASE(DoublyListDelete) {
    DoublyList l;
    l.pushBack("a");
    l.pushBack("b");
    l.pushBack("c");
    l.pushBack("d");
    l.DelVal("b");
    BOOST_CHECK_EQUAL(l.ToString(), "a c d");
    l.delBefore("d");
    BOOST_CHECK_EQUAL(l.ToString(), "a d");
    l.delAfter("a");
    BOOST_CHECK_EQUAL(l.ToString(), "a");
    l.delFront();
    BOOST_CHECK_EQUAL(l.Size(), 0);
}

// 4. Стек
BOOST_AUTO_TEST_CASE(StackEmpty) {
    Stack s;
    BOOST_CHECK_EQUAL(s.Size(), 0);
    BOOST_CHECK_EQUAL(s.ToString(), "");
}

BOOST_AUTO_TEST_CASE(StackPushPop) {
    Stack s;
    s.push("first");
    s.push("second");
    s.push("third");
    BOOST_CHECK_EQUAL(s.Size(), 3);
    BOOST_CHECK_EQUAL(s.ToString(), "first second third");
    s.pop();
    BOOST_CHECK_EQUAL(s.ToString(), "first second");
    s.pop();
    s.pop();
    BOOST_CHECK_EQUAL(s.Size(), 0);
    s.push("again");
    BOOST_CHECK_EQUAL(s.ToString(), "again");
}

// 5. Очередь
BOOST_AUTO_TEST_CASE(QueueEmpty) {
    Queue q;
    BOOST_CHECK_EQUAL(q.Size(), 0);
    BOOST_CHECK_EQUAL(q.ToString(), "");
}

BOOST_AUTO_TEST_CASE(QueueFirstInFirstOut) {
    Queue q;
    q.push("first");
    q.push("second");
    q.push("third");
    BOOST_CHECK_EQUAL(q.ToString(), "first second third");
    q.pop();
    BOOST_CHECK_EQUAL(q.ToString(), "second third");
    q.pop();
    q.pop();
    BOOST_CHECK_EQUAL(q.Size(), 0);
    q.push("again");
    BOOST_CHECK_EQUAL(q.ToString(), "again");
}

// 6. Красно-чёрное дерево
BOOST_AUTO_TEST_CASE(TreeEmpty) {
    RBTree t;
    BOOST_CHECK_EQUAL(t.Size(), 0);
    BOOST_CHECK(!t.Contains(10));
    t.Remove(10);
    BOOST_CHECK_EQUAL(t.Size(), 0);
}

BOOST_AUTO_TEST_CASE(TreeInsertAndSearch) {
    RBTree t;
    t.Insert(10);
    t.Insert(5);
    t.Insert(20);
    t.Insert(15);
    BOOST_CHECK_EQUAL(t.Size(), 4);
    BOOST_CHECK(t.Contains(5));
    BOOST_CHECK(t.Contains(15));
    BOOST_CHECK(!t.Contains(100));
    t.Insert(10);
    BOOST_CHECK_EQUAL(t.Size(), 4);
}

BOOST_AUTO_TEST_CASE(TreeRemove) {
    RBTree t;
    t.Insert(10);
    t.Insert(5);
    t.Insert(20);
    t.Insert(15);
    t.Remove(5);
    BOOST_CHECK(!t.Contains(5));
    BOOST_CHECK_EQUAL(t.Size(), 3);
    t.Remove(100);
    BOOST_CHECK_EQUAL(t.Size(), 3);
    t.Remove(15);
    BOOST_CHECK(!t.Contains(15));
    BOOST_CHECK(t.Contains(10));
}

// 7. Хеш-таблица с цепочками
BOOST_AUTO_TEST_CASE(ChainTableInsertAndGet) {
    ChainTable t;
    string value;
    BOOST_CHECK_EQUAL(t.Size(), 0);
    BOOST_CHECK(t.Insert("name", "Ivan"));
    BOOST_CHECK(t.Insert("age", "20"));
    BOOST_CHECK(t.Get("name", value));
    BOOST_CHECK_EQUAL(value, "Ivan");
    BOOST_CHECK_EQUAL(t.Size(), 2);
    BOOST_CHECK(!t.Get("missing", value));
}

BOOST_AUTO_TEST_CASE(ChainTableUpdateAndDelete) {
    ChainTable t;
    string value;
    BOOST_CHECK(!t.Insert("", "bad"));
    BOOST_CHECK(t.Insert("name", "Ivan"));
    BOOST_CHECK(!t.Insert("name", "Petr"));
    BOOST_CHECK_EQUAL(t.Size(), 1);
    BOOST_CHECK(t.Get("name", value));
    BOOST_CHECK_EQUAL(value, "Petr");
    BOOST_CHECK(t.Remove("name"));
    BOOST_CHECK(!t.Remove("name"));
    BOOST_CHECK_EQUAL(t.Size(), 0);
}

BOOST_AUTO_TEST_CASE(ChainTableRehash) {
    ChainTable t(2);
    for (int i = 0; i < 20; ++i) {
        BOOST_CHECK(t.Insert("key" + to_string(i), to_string(i)));
    }
    BOOST_CHECK_EQUAL(t.Size(), 20);
    BOOST_CHECK(t.Capacity() > 2);
    for (int i = 0; i < 20; ++i) {
        string value;
        BOOST_REQUIRE(t.Get("key" + to_string(i), value));
        BOOST_CHECK_EQUAL(value, to_string(i));
    }
}

// 8. Хеш-таблица с кукушкой
BOOST_AUTO_TEST_CASE(CuckooTableInsertAndGet) {
    CuckooTable t;
    string value;
    BOOST_CHECK_EQUAL(t.Size(), 0);
    BOOST_CHECK(t.Insert("name", "Ivan"));
    BOOST_CHECK(t.Insert("age", "20"));
    BOOST_CHECK(t.Get("name", value));
    BOOST_CHECK_EQUAL(value, "Ivan");
    BOOST_CHECK(!t.Get("missing", value));
}

BOOST_AUTO_TEST_CASE(CuckooTableUpdateAndDelete) {
    CuckooTable t;
    string value;
    BOOST_CHECK(!t.Insert("", "bad"));
    BOOST_CHECK(t.Insert("name", "Ivan"));
    BOOST_CHECK(!t.Insert("name", "Petr"));
    BOOST_CHECK_EQUAL(t.Size(), 1);
    BOOST_CHECK(t.Get("name", value));
    BOOST_CHECK_EQUAL(value, "Petr");
    BOOST_CHECK(t.Remove("name"));
    BOOST_CHECK(!t.Remove("name"));
    BOOST_CHECK_EQUAL(t.Size(), 0);
}

BOOST_AUTO_TEST_CASE(CuckooTableRehash) {
    CuckooTable t;
    for (int i = 0; i < 20; ++i) {
        BOOST_CHECK(t.Insert("key" + to_string(i), to_string(i)));
    }
    BOOST_CHECK_EQUAL(t.Size(), 20);
    for (int i = 0; i < 20; ++i) {
        string value;
        BOOST_REQUIRE(t.Get("key" + to_string(i), value));
        BOOST_CHECK_EQUAL(value, to_string(i));
    }
}

// Дополнительные тесты для увеличения покрытия кода.

BOOST_AUTO_TEST_CASE(ArrayPrintAndGet) {
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
    BOOST_CHECK_EQUAL(output.str(), "\none two \ntwo\nДлинна массива: 2\n");
}

BOOST_AUTO_TEST_CASE(ArrayInsertAtEndAndFull) {
    Array a;
    a.Insert(0, "first");
    for (int i = 1; i < 10; ++i) {
        a.PushBack(to_string(i));
    }
    a.Insert(5, "middle");  // массив заполнен: проверяем увеличение
    a.Insert(a.Size(), "last");
    BOOST_CHECK_EQUAL(a.Size(), 12);
    BOOST_CHECK_EQUAL(a.At(5), "middle");
    BOOST_CHECK_EQUAL(a.At(11), "last");
    a.DelIndx(a.Size() - 1);
    a.DelIndx(0);
    BOOST_CHECK_EQUAL(a.Size(), 10);
    BOOST_CHECK_EQUAL(a.At(-1), "");
    BOOST_CHECK_EQUAL(a.At(100), "");
}

BOOST_AUTO_TEST_CASE(LinkedListEmptyOperations) {
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
    BOOST_CHECK_EQUAL(l.Size(), 0);
    BOOST_CHECK_EQUAL(l.At(-1), "");
    BOOST_CHECK_EQUAL(l.At(2), "");
}

BOOST_AUTO_TEST_CASE(LinkedListPrintAndGet) {
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
    BOOST_CHECK_EQUAL(output.str(), "\na b \na\nb\na\nb\n1\n");
}

BOOST_AUTO_TEST_CASE(LinkedListFirstLastAndMissing) {
    linkedList l;
    l.pushFront("middle");
    l.pushBefore("middle", "first");
    l.pushAfter("middle", "last");
    BOOST_CHECK_EQUAL(l.ToString(), "first middle last");
    l.pushBefore("missing", "x");
    l.delBefore("first");
    l.delAfter("last");
    l.DelVal("missing");
    BOOST_CHECK_EQUAL(l.ToString(), "first middle last");
    l.DelVal("last");
    BOOST_CHECK_EQUAL(l.ToString(), "first middle");
    l.delBack();
    BOOST_CHECK_EQUAL(l.ToString(), "first");
    l.DelVal("first");
    BOOST_CHECK_EQUAL(l.Size(), 0);
}

BOOST_AUTO_TEST_CASE(LinkedListSingleElement) {
    linkedList a;
    a.pushFront("one");
    a.delFront();
    BOOST_CHECK_EQUAL(a.Size(), 0);
    a.pushBack("two");
    a.delBack();
    BOOST_CHECK_EQUAL(a.Size(), 0);
    a.pushBack("three");
    a.DelVal("three");
    BOOST_CHECK_EQUAL(a.Size(), 0);
    a.pushBack("four");
    a.pushBack("five");
    a.delAfter("four");
    BOOST_CHECK_EQUAL(a.ToString(), "four");
}

BOOST_AUTO_TEST_CASE(LinkedListDeleteNeighbours) {
    linkedList l;
    l.pushBack("a");
    l.pushBack("b");
    l.pushBack("c");
    l.pushBack("d");
    l.delBefore("b"); // первый
    BOOST_CHECK_EQUAL(l.ToString(), "b c d");
    l.delBefore("d"); // средний
    BOOST_CHECK_EQUAL(l.ToString(), "b d");
    l.delAfter("b"); // последний
    BOOST_CHECK_EQUAL(l.ToString(), "b");
    l.delBefore("missing");
    BOOST_CHECK_EQUAL(l.ToString(), "b");
}

BOOST_AUTO_TEST_CASE(DoublyListEmptyOperations) {
    DoublyList l;
    l.delFront();
    l.delBack();
    l.delBefore("x");
    l.delAfter("x");
    l.DelVal("x");
    l.pushBefore("x", "a");
    l.pushAfter("x", "a");
    BOOST_CHECK_EQUAL(l.Size(), 0);
    BOOST_CHECK_EQUAL(l.At(-1), "");
    BOOST_CHECK_EQUAL(l.At(5), "");
    BOOST_CHECK_EQUAL(l.Find("x"), -1);
}

BOOST_AUTO_TEST_CASE(DoublyListOutput) {
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
    BOOST_CHECK(output.str().find("first <-> second") != string::npos);
    BOOST_CHECK(output.str().find("ЗНАЧЕНИЕ \"second\" ИНДЕКС 1") != string::npos);
    BOOST_CHECK(output.str().find("ПУСТОЙ") != string::npos);
}

BOOST_AUTO_TEST_CASE(DoublyListFirstAndLast) {
    DoublyList l;
    l.pushFront("b");
    l.pushBefore("b", "a");
    l.pushAfter("b", "c");
    BOOST_CHECK_EQUAL(l.ToString(), "a b c");
    l.delFront();
    BOOST_CHECK_EQUAL(l.ToString(), "b c");
    l.delBack();
    BOOST_CHECK_EQUAL(l.ToString(), "b");
    l.delBack();
    BOOST_CHECK_EQUAL(l.Size(), 0);
    l.pushBack("again");
    l.delFront();
    BOOST_CHECK_EQUAL(l.Size(), 0);
}

BOOST_AUTO_TEST_CASE(DoublyListDeleteValues) {
    DoublyList l;
    l.pushBack("a");
    l.pushBack("b");
    l.pushBack("c");
    l.pushBack("d");
    l.pushBack("e");
    l.DelVal("a");
    l.DelVal("e");
    l.DelVal("c");
    BOOST_CHECK_EQUAL(l.ToString(), "b d");
    l.DelVal("missing");
    l.delBefore("b");
    l.delAfter("d");
    BOOST_CHECK_EQUAL(l.ToString(), "b d");
    l.DelVal("b");
    l.DelVal("d");
    BOOST_CHECK_EQUAL(l.Size(), 0);
}

BOOST_AUTO_TEST_CASE(DoublyListNeighbours) {
    DoublyList l;
    l.pushBack("a");
    l.pushBack("b");
    l.pushBack("c");
    l.pushBack("d");
    l.pushBack("e");
    l.delBefore("d");   // удаляем средний узел c
    BOOST_CHECK_EQUAL(l.ToString(), "a b d e");
    l.delAfter("b");    // удаляем средний узел d
    BOOST_CHECK_EQUAL(l.ToString(), "a b e");
    l.delBefore("b");   // удаляем первый a
    BOOST_CHECK_EQUAL(l.ToString(), "b e");
    l.delAfter("b");    // удаляем последний e
    BOOST_CHECK_EQUAL(l.ToString(), "b");
    BOOST_CHECK_EQUAL(l.At(0), "b");
}

BOOST_AUTO_TEST_CASE(StackEmptyOutput) {
    Stack s;
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    s.peek();
    s.Print();
    s.pop();
    cout.rdbuf(old);
    BOOST_CHECK(output.str().find("Пуст") != string::npos);
    BOOST_CHECK(output.str().find("Error!") != string::npos);
    BOOST_CHECK_EQUAL(s.Size(), 0);
}

BOOST_AUTO_TEST_CASE(StackOutputAndOrder) {
    Stack s;
    s.push("one");
    s.push("two");
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    s.peek();
    s.Print();
    cout.rdbuf(old);
    BOOST_CHECK_EQUAL(output.str(), "two\ntwo\none\n");
    s.pop();
    BOOST_CHECK_EQUAL(s.ToString(), "one");
}

BOOST_AUTO_TEST_CASE(QueueEmptyOutput) {
    Queue q;
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    q.peek();
    q.Print();
    q.pop();
    cout.rdbuf(old);
    BOOST_CHECK(output.str().find("Пуста") != string::npos);
    BOOST_CHECK(output.str().find("Error!") != string::npos);
    BOOST_CHECK_EQUAL(q.Size(), 0);
}

BOOST_AUTO_TEST_CASE(QueueOutputAndOrder) {
    Queue q;
    q.push("one");
    q.push("two");
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    q.peek();
    q.Print();
    cout.rdbuf(old);
    BOOST_CHECK_EQUAL(output.str(), "one\none two \n");
    q.pop();
    BOOST_CHECK_EQUAL(q.ToString(), "two");
}

BOOST_AUTO_TEST_CASE(TreeEmptyOutput) {
    RBTree t;
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    t.Print();
    t.PrintInorder();
    t.PrintPreorder();
    t.PrintPostorder();
    t.PrintBFS();
    cout.rdbuf(old);
    BOOST_CHECK(output.str().find("Пустое") != string::npos);
    BOOST_CHECK_EQUAL(t.ToString(), "");
}

BOOST_AUTO_TEST_CASE(TreeTraversals) {
    RBTree t;
    t.Insert(10);
    t.Insert(5);
    t.Insert(15);
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    t.Print();
    t.PrintInorder();
    t.PrintPreorder();
    t.PrintPostorder();
    t.PrintBFS();
    cout.rdbuf(old);
    BOOST_CHECK(output.str().find("5(") != string::npos);
    BOOST_CHECK(output.str().find("10(") != string::npos);
    BOOST_CHECK(output.str().find("15(") != string::npos);
    BOOST_CHECK_EQUAL(t.Size(), 3);
    BOOST_CHECK(!t.ToString().empty());
}

BOOST_AUTO_TEST_CASE(TreeLoadAndSave) {
    RBTree t;
    t.Load(10, 'B');
    t.Load(5, 'R');
    t.Load(15, 'R');
    BOOST_CHECK_EQUAL(t.Size(), 3);
    BOOST_CHECK_EQUAL(t.ToString(), "10 B 5 R 15 R");
    BOOST_CHECK(t.Contains(5));
    BOOST_CHECK(t.Contains(15));
}

BOOST_AUTO_TEST_CASE(TreeInsertRotations) {
    const int keys[6][3] = {
        {30, 20, 10}, {10, 20, 30}, {30, 10, 20},
        {10, 30, 20}, {20, 10, 30}, {20, 30, 10}
    };
    for (int i = 0; i < 6; ++i) {
        RBTree t;
        for (int j = 0; j < 3; ++j) t.Insert(keys[i][j]);
        BOOST_CHECK_EQUAL(t.Size(), 3);
        BOOST_CHECK(t.Contains(10));
        BOOST_CHECK(t.Contains(20));
        BOOST_CHECK(t.Contains(30));
        BOOST_CHECK(!t.Contains(40));
    }
}

BOOST_AUTO_TEST_CASE(TreeRemoveRootCases) {
    {
        RBTree t;
        t.Insert(10);
        t.Remove(10);
        BOOST_CHECK_EQUAL(t.Size(), 0);
    }
    {
        RBTree t;
        t.Insert(10);
        t.Insert(5);
        t.Remove(10);  // у корня только левый потомок
        BOOST_CHECK_EQUAL(t.Size(), 1);
        BOOST_CHECK(t.Contains(5));
    }
    {
        RBTree t;
        t.Insert(10);
        t.Insert(15);
        t.Remove(10);  // у корня только правый потомок
        BOOST_CHECK_EQUAL(t.Size(), 1);
        BOOST_CHECK(t.Contains(15));
    }
    {
        RBTree t;
        t.Insert(10);
        t.Insert(5);
        t.Insert(15);
        t.Remove(10);  // у корня два потомка
        BOOST_CHECK_EQUAL(t.Size(), 2);
        BOOST_CHECK(!t.Contains(10));
        BOOST_CHECK(t.Contains(5));
        BOOST_CHECK(t.Contains(15));
    }
}

BOOST_AUTO_TEST_CASE(TreeRemoveMany) {
    RBTree t;
    for (int i = 0; i < 30; ++i) t.Insert(i);
    BOOST_CHECK_EQUAL(t.Size(), 30);
    for (int i = 0; i < 30; ++i) {
        BOOST_CHECK(t.Contains(i));
        t.Remove(i);
        BOOST_CHECK(!t.Contains(i));
        BOOST_CHECK_EQUAL(t.Size(), 29 - i);
    }
}

BOOST_AUTO_TEST_CASE(TreeRemoveReverse) {
    RBTree t;
    for (int i = 29; i >= 0; --i) t.Insert(i);
    BOOST_CHECK_EQUAL(t.Size(), 30);
    for (int i = 29; i >= 0; --i) {
        t.Remove(i);
        BOOST_CHECK(!t.Contains(i));
        BOOST_CHECK_EQUAL(t.Size(), i);
    }
}

BOOST_AUTO_TEST_CASE(ChainTableEmptyAndPrint) {
    ChainTable t(0);
    BOOST_CHECK_EQUAL(t.Capacity(), 1);
    BOOST_CHECK_EQUAL(t.Size(), 0);
    BOOST_CHECK_EQUAL(t.ToString(), "");
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    t.Print();
    cout.rdbuf(old);
    BOOST_CHECK_EQUAL(output.str(), "Пустая\n");
    string value;
    BOOST_CHECK(!t.Get("missing", value));
    BOOST_CHECK(!t.Remove("missing"));
}

BOOST_AUTO_TEST_CASE(ChainTableCollisionsAndPrint) {
    ChainTable t(128);
    BOOST_CHECK(t.Insert("aa", "one"));
    BOOST_CHECK(t.Insert("ee", "two"));   // коллизия: одинаковый остаток по модулю 128
    BOOST_CHECK(t.Insert("ii", "three"));
    BOOST_CHECK_EQUAL(t.Size(), 3);
    string value;
    BOOST_REQUIRE(t.Get("ee", value));
    BOOST_CHECK_EQUAL(value, "two");
    BOOST_CHECK(t.Remove("ee"));
    BOOST_CHECK(t.Get("aa", value));
    BOOST_CHECK_EQUAL(value, "one");
    BOOST_CHECK(t.Get("ii", value));
    BOOST_CHECK_EQUAL(value, "three");
    BOOST_CHECK(!t.Insert("aa", "new"));
    BOOST_REQUIRE(t.Get("aa", value));
    BOOST_CHECK_EQUAL(value, "new");
    BOOST_CHECK(t.ToString().find("aa new") != string::npos);
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    t.Print();
    cout.rdbuf(old);
    BOOST_CHECK(output.str().find("aa=new") != string::npos);
}

BOOST_AUTO_TEST_CASE(CuckooTableEmptyAndPrint) {
    CuckooTable t(0);
    BOOST_CHECK_EQUAL(t.Size(), 0);
    BOOST_CHECK_EQUAL(t.ToString(), "");
    string value;
    BOOST_CHECK(!t.Get("", value));
    BOOST_CHECK(!t.Remove(""));
    BOOST_CHECK(!t.Remove("missing"));
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    t.Print();
    cout.rdbuf(old);
    BOOST_CHECK_EQUAL(output.str(), "Пустая\n");
}

BOOST_AUTO_TEST_CASE(CuckooTableSecondTable) {
    CuckooTable t(16);
    BOOST_CHECK(t.Insert("aa", "one"));
    BOOST_CHECK(t.Insert("aq", "two")); // два ключа с одним h1 и h2
    BOOST_CHECK_EQUAL(t.Size(), 2);
    string value;
    BOOST_REQUIRE(t.Get("aa", value));
    BOOST_CHECK_EQUAL(value, "one");
    BOOST_REQUIRE(t.Get("aq", value));
    BOOST_CHECK_EQUAL(value, "two");
    BOOST_CHECK(!t.Insert("aa", "updated"));
    BOOST_CHECK(!t.Insert("aq", "updated2"));
    BOOST_REQUIRE(t.Get("aq", value));
    BOOST_CHECK_EQUAL(value, "updated2");
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    t.Print();
    cout.rdbuf(old);
    BOOST_CHECK(output.str().find("T1[") != string::npos);
    BOOST_CHECK(output.str().find("T2[") != string::npos);
    BOOST_CHECK(t.ToString().find("aa updated") != string::npos);
    BOOST_CHECK(t.Remove("aq"));
    BOOST_CHECK(!t.Get("aq", value));
    BOOST_CHECK_EQUAL(t.Size(), 1);
    BOOST_CHECK(t.Remove("aa"));
    BOOST_CHECK_EQUAL(t.Size(), 0);
}

BOOST_AUTO_TEST_CASE(CuckooTableCycleAndRehash) {
    CuckooTable t(16);
    BOOST_CHECK(t.Insert("aa", "one"));
    BOOST_CHECK(t.Insert("aq", "two"));
    BOOST_CHECK(t.Insert("aA", "three")); // вызываем вытеснения и перестройку
    BOOST_CHECK_EQUAL(t.Size(), 3);
    string value;
    BOOST_REQUIRE(t.Get("aa", value));
    BOOST_CHECK_EQUAL(value, "one");
    BOOST_REQUIRE(t.Get("aq", value));
    BOOST_CHECK_EQUAL(value, "two");
    BOOST_REQUIRE(t.Get("aA", value));
    BOOST_CHECK_EQUAL(value, "three");
}
