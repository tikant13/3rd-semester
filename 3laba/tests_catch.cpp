#if __has_include(<catch2/catch_all.hpp>)
    #include <catch2/catch_all.hpp>
#elif __has_include(<catch2/catch.hpp>)
    #define CATCH_CONFIG_MAIN
    #include <catch2/catch.hpp>
#else
    #error "Catch2 не найден. Установите: sudo apt install catch2"
#endif
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
TEST_CASE("ArrayEmpty", "[Array]") {
    Array a;
    CHECK(a.Size() == 0);
    CHECK(a.ToString() == "");
    CHECK(a.At(0) == "");
    CHECK(a.At(-1) == "");
}

TEST_CASE("ArrayInsertAndDelete", "[Array]") {
    Array a;
    a.PushBack("one");
    a.PushBack("three");
    a.Insert(1, "two");
    a.Insert(0, "zero");
    CHECK(a.ToString() == "zero one two three");

    a.Set(2, "TWO");
    CHECK(a.At(2) == "TWO");
    a.DelIndx(0);
    a.DelIndx(2);
    CHECK(a.ToString() == "one TWO");
    CHECK(a.Size() == 2);
}

TEST_CASE("ArrayInvalidIndexes", "[Array]") {
    Array a;
    a.PushBack("hello");
    a.Insert(-1, "bad");
    a.Insert(10, "bad");
    a.Set(-1, "bad");
    a.Set(5, "bad");
    a.DelIndx(-1);
    a.DelIndx(10);
    CHECK(a.Size() == 1);
    CHECK(a.At(0) == "hello");
}

TEST_CASE("ArrayGrowthAndReset", "[Array]") {
    Array a;
    for (int i = 0; i < 30; ++i) {
        a.PushBack(to_string(i));
    }
    CHECK(a.Size() == 30);
    CHECK(a.At(0) == "0");
    CHECK(a.At(29) == "29");
    a.Create();
    CHECK(a.Size() == 0);
    a.PushBack("new");
    CHECK(a.At(0) == "new");
}

// 2. Односвязный список
TEST_CASE("LinkedListEmpty", "[LinkedList]") {
    linkedList l;
    CHECK(l.Size() == 0);
    CHECK(l.Find("x") == -1);
    CHECK(l.At(0) == "");
    l.delFront();
    l.delBack();
    CHECK(l.Size() == 0);
}

TEST_CASE("LinkedListInsert", "[LinkedList]") {
    linkedList l;
    l.pushBack("b");
    l.pushFront("a");
    l.pushAfter("b", "d");
    l.pushBefore("d", "c");
    CHECK(l.ToString() == "a b c d");
    CHECK(l.Find("c") == 2);
    CHECK(l.Find("x") == -1);
    l.pushAfter("missing", "x");
    CHECK(l.Size() == 4);
}

TEST_CASE("LinkedListDelete", "[LinkedList]") {
    linkedList l;
    l.pushBack("a");
    l.pushBack("b");
    l.pushBack("c");
    l.pushBack("d");
    l.DelVal("b");
    CHECK(l.ToString() == "a c d");
    l.delAfter("a");
    CHECK(l.ToString() == "a d");
    l.delBefore("d");
    CHECK(l.ToString() == "d");
    l.delBack();
    CHECK(l.Size() == 0);
}

// 3. Двусвязный список
TEST_CASE("DoublyListEmpty", "[DoublyList]") {
    DoublyList l;
    CHECK(l.Size() == 0);
    CHECK(l.ToString() == "");
    CHECK(l.Find("x") == -1);
}

TEST_CASE("DoublyListInsert", "[DoublyList]") {
    DoublyList l;
    l.pushBack("b");
    l.pushFront("a");
    l.pushAfter("b", "d");
    l.pushBefore("d", "c");
    CHECK(l.ToString() == "a b c d");
    CHECK(l.At(2) == "c");
    CHECK(l.Find("d") == 3);
}

TEST_CASE("DoublyListDelete", "[DoublyList]") {
    DoublyList l;
    l.pushBack("a");
    l.pushBack("b");
    l.pushBack("c");
    l.pushBack("d");
    l.DelVal("b");
    CHECK(l.ToString() == "a c d");
    l.delBefore("d");
    CHECK(l.ToString() == "a d");
    l.delAfter("a");
    CHECK(l.ToString() == "a");
    l.delFront();
    CHECK(l.Size() == 0);
}

// 4. Стек
TEST_CASE("StackEmpty", "[Stack]") {
    Stack s;
    CHECK(s.Size() == 0);
    CHECK(s.ToString() == "");
}

TEST_CASE("StackPushPop", "[Stack]") {
    Stack s;
    s.push("first");
    s.push("second");
    s.push("third");
    CHECK(s.Size() == 3);
    CHECK(s.ToString() == "first second third");
    s.pop();
    CHECK(s.ToString() == "first second");
    s.pop();
    s.pop();
    CHECK(s.Size() == 0);
    s.push("again");
    CHECK(s.ToString() == "again");
}

// 5. Очередь
TEST_CASE("QueueEmpty", "[Queue]") {
    Queue q;
    CHECK(q.Size() == 0);
    CHECK(q.ToString() == "");
}

TEST_CASE("QueueFirstInFirstOut", "[Queue]") {
    Queue q;
    q.push("first");
    q.push("second");
    q.push("third");
    CHECK(q.ToString() == "first second third");
    q.pop();
    CHECK(q.ToString() == "second third");
    q.pop();
    q.pop();
    CHECK(q.Size() == 0);
    q.push("again");
    CHECK(q.ToString() == "again");
}

// 6. Красно-чёрное дерево
TEST_CASE("RBTreeEmpty", "[RBTree]") {
    RBTree t;
    CHECK(t.Size() == 0);
    CHECK_FALSE(t.Contains(10));
    t.Remove(10);
    CHECK(t.Size() == 0);
}

TEST_CASE("RBTreeInsertAndSearch", "[RBTree]") {
    RBTree t;
    t.Insert(10);
    t.Insert(5);
    t.Insert(20);
    t.Insert(15);
    CHECK(t.Size() == 4);
    CHECK(t.Contains(5));
    CHECK(t.Contains(15));
    CHECK_FALSE(t.Contains(100));
    t.Insert(10);
    CHECK(t.Size() == 4);
}

TEST_CASE("RBTreeRemove", "[RBTree]") {
    RBTree t;
    t.Insert(10);
    t.Insert(5);
    t.Insert(20);
    t.Insert(15);
    t.Remove(5);
    CHECK_FALSE(t.Contains(5));
    CHECK(t.Size() == 3);
    t.Remove(100);
    CHECK(t.Size() == 3);
    t.Remove(15);
    CHECK_FALSE(t.Contains(15));
    CHECK(t.Contains(10));
}

// 7. Хеш-таблица с цепочками
TEST_CASE("ChainTableInsertAndGet", "[ChainTable]") {
    ChainTable t;
    string value;
    CHECK(t.Size() == 0);
    CHECK(t.Insert("name", "Ivan"));
    CHECK(t.Insert("age", "20"));
    CHECK(t.Get("name", value));
    CHECK(value == "Ivan");
    CHECK(t.Size() == 2);
    CHECK_FALSE(t.Get("missing", value));
}

TEST_CASE("ChainTableUpdateAndDelete", "[ChainTable]") {
    ChainTable t;
    string value;
    CHECK_FALSE(t.Insert("", "bad"));
    CHECK(t.Insert("name", "Ivan"));
    CHECK_FALSE(t.Insert("name", "Petr"));
    CHECK(t.Size() == 1);
    CHECK(t.Get("name", value));
    CHECK(value == "Petr");
    CHECK(t.Remove("name"));
    CHECK_FALSE(t.Remove("name"));
    CHECK(t.Size() == 0);
}

TEST_CASE("ChainTableRehash", "[ChainTable]") {
    ChainTable t(2);
    for (int i = 0; i < 20; ++i) {
        CHECK(t.Insert("key" + to_string(i), to_string(i)));
    }
    CHECK(t.Size() == 20);
    CHECK(t.Capacity() > 2);
    for (int i = 0; i < 20; ++i) {
        string value;
        REQUIRE(t.Get("key" + to_string(i), value));
        CHECK(value == to_string(i));
    }
}

// 8. Хеш-таблица с кукушкой
TEST_CASE("CuckooTableInsertAndGet", "[CuckooTable]") {
    CuckooTable t;
    string value;
    CHECK(t.Size() == 0);
    CHECK(t.Insert("name", "Ivan"));
    CHECK(t.Insert("age", "20"));
    CHECK(t.Get("name", value));
    CHECK(value == "Ivan");
    CHECK_FALSE(t.Get("missing", value));
}

TEST_CASE("CuckooTableUpdateAndDelete", "[CuckooTable]") {
    CuckooTable t;
    string value;
    CHECK_FALSE(t.Insert("", "bad"));
    CHECK(t.Insert("name", "Ivan"));
    CHECK_FALSE(t.Insert("name", "Petr"));
    CHECK(t.Size() == 1);
    CHECK(t.Get("name", value));
    CHECK(value == "Petr");
    CHECK(t.Remove("name"));
    CHECK_FALSE(t.Remove("name"));
    CHECK(t.Size() == 0);
}

TEST_CASE("CuckooTableRehash", "[CuckooTable]") {
    CuckooTable t;
    for (int i = 0; i < 20; ++i) {
        CHECK(t.Insert("key" + to_string(i), to_string(i)));
    }
    CHECK(t.Size() == 20);
    for (int i = 0; i < 20; ++i) {
        string value;
        REQUIRE(t.Get("key" + to_string(i), value));
        CHECK(value == to_string(i));
    }
}
