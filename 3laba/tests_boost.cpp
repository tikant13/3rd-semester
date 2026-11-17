#define BOOST_TEST_MODULE Lab3BoostTests
#include <boost/test/included/unit_test.hpp>
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
