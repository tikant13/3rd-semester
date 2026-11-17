// Catch2 v2 (Ubuntu 22.04) и Catch2 v3 (Ubuntu 24.04 и новее)
#if __has_include(<catch2/catch_all.hpp>)
    #include <catch2/catch_all.hpp>
#elif __has_include(<catch2/catch.hpp>)
    #define CATCH_CONFIG_MAIN
    #include <catch2/catch.hpp>
#else
    #error "Catch2 не найден. Установите: sudo apt install catch2"
#endif
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

// Дополнительные тесты для увеличения покрытия кода.

TEST_CASE("ArrayPrintAndGet", "[Array]") {
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
    CHECK(output.str() == "\none two \ntwo\nДлинна массива: 2\n");
}

TEST_CASE("ArrayInsertAtEndAndFull", "[Array]") {
    Array a;
    a.Insert(0, "first");
    for (int i = 1; i < 10; ++i) {
        a.PushBack(to_string(i));
    }
    a.Insert(5, "middle");  // массив заполнен: проверяем увеличение
    a.Insert(a.Size(), "last");
    CHECK(a.Size() == 12);
    CHECK(a.At(5) == "middle");
    CHECK(a.At(11) == "last");
    a.DelIndx(a.Size() - 1);
    a.DelIndx(0);
    CHECK(a.Size() == 10);
    CHECK(a.At(-1) == "");
    CHECK(a.At(100) == "");
}

TEST_CASE("LinkedListEmptyOperations", "[LinkedList]") {
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
    CHECK(l.Size() == 0);
    CHECK(l.At(-1) == "");
    CHECK(l.At(2) == "");
}

TEST_CASE("LinkedListPrintAndGet", "[LinkedList]") {
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
    CHECK(output.str() == "\na b \na\nb\na\nb\n1\n");
}

TEST_CASE("LinkedListFirstLastAndMissing", "[LinkedList]") {
    linkedList l;
    l.pushFront("middle");
    l.pushBefore("middle", "first");
    l.pushAfter("middle", "last");
    CHECK(l.ToString() == "first middle last");
    l.pushBefore("missing", "x");
    l.delBefore("first");
    l.delAfter("last");
    l.DelVal("missing");
    CHECK(l.ToString() == "first middle last");
    l.DelVal("last");
    CHECK(l.ToString() == "first middle");
    l.delBack();
    CHECK(l.ToString() == "first");
    l.DelVal("first");
    CHECK(l.Size() == 0);
}

TEST_CASE("LinkedListSingleElement", "[LinkedList]") {
    linkedList a;
    a.pushFront("one");
    a.delFront();
    CHECK(a.Size() == 0);
    a.pushBack("two");
    a.delBack();
    CHECK(a.Size() == 0);
    a.pushBack("three");
    a.DelVal("three");
    CHECK(a.Size() == 0);
    a.pushBack("four");
    a.pushBack("five");
    a.delAfter("four");
    CHECK(a.ToString() == "four");
}

TEST_CASE("LinkedListDeleteNeighbours", "[LinkedList]") {
    linkedList l;
    l.pushBack("a");
    l.pushBack("b");
    l.pushBack("c");
    l.pushBack("d");
    l.delBefore("b"); // первый
    CHECK(l.ToString() == "b c d");
    l.delBefore("d"); // средний
    CHECK(l.ToString() == "b d");
    l.delAfter("b"); // последний
    CHECK(l.ToString() == "b");
    l.delBefore("missing");
    CHECK(l.ToString() == "b");
}

TEST_CASE("DoublyListEmptyOperations", "[DoublyList]") {
    DoublyList l;
    l.delFront();
    l.delBack();
    l.delBefore("x");
    l.delAfter("x");
    l.DelVal("x");
    l.pushBefore("x", "a");
    l.pushAfter("x", "a");
    CHECK(l.Size() == 0);
    CHECK(l.At(-1) == "");
    CHECK(l.At(5) == "");
    CHECK(l.Find("x") == -1);
}

TEST_CASE("DoublyListOutput", "[DoublyList]") {
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
    CHECK(output.str().find("first <-> second") != string::npos);
    CHECK(output.str().find("ЗНАЧЕНИЕ \"second\" ИНДЕКС 1") != string::npos);
    CHECK(output.str().find("ПУСТОЙ") != string::npos);
}

TEST_CASE("DoublyListFirstAndLast", "[DoublyList]") {
    DoublyList l;
    l.pushFront("b");
    l.pushBefore("b", "a");
    l.pushAfter("b", "c");
    CHECK(l.ToString() == "a b c");
    l.delFront();
    CHECK(l.ToString() == "b c");
    l.delBack();
    CHECK(l.ToString() == "b");
    l.delBack();
    CHECK(l.Size() == 0);
    l.pushBack("again");
    l.delFront();
    CHECK(l.Size() == 0);
}

TEST_CASE("DoublyListDeleteValues", "[DoublyList]") {
    DoublyList l;
    l.pushBack("a");
    l.pushBack("b");
    l.pushBack("c");
    l.pushBack("d");
    l.pushBack("e");
    l.DelVal("a");
    l.DelVal("e");
    l.DelVal("c");
    CHECK(l.ToString() == "b d");
    l.DelVal("missing");
    l.delBefore("b");
    l.delAfter("d");
    CHECK(l.ToString() == "b d");
    l.DelVal("b");
    l.DelVal("d");
    CHECK(l.Size() == 0);
}

TEST_CASE("DoublyListNeighbours", "[DoublyList]") {
    DoublyList l;
    l.pushBack("a");
    l.pushBack("b");
    l.pushBack("c");
    l.pushBack("d");
    l.pushBack("e");
    l.delBefore("d");   // удаляем средний узел c
    CHECK(l.ToString() == "a b d e");
    l.delAfter("b");    // удаляем средний узел d
    CHECK(l.ToString() == "a b e");
    l.delBefore("b");   // удаляем первый a
    CHECK(l.ToString() == "b e");
    l.delAfter("b");    // удаляем последний e
    CHECK(l.ToString() == "b");
    CHECK(l.At(0) == "b");
}

TEST_CASE("StackEmptyOutput", "[Stack]") {
    Stack s;
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    s.peek();
    s.Print();
    s.pop();
    cout.rdbuf(old);
    CHECK(output.str().find("Пуст") != string::npos);
    CHECK(output.str().find("Error!") != string::npos);
    CHECK(s.Size() == 0);
}

TEST_CASE("StackOutputAndOrder", "[Stack]") {
    Stack s;
    s.push("one");
    s.push("two");
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    s.peek();
    s.Print();
    cout.rdbuf(old);
    CHECK(output.str() == "two\ntwo\none\n");
    s.pop();
    CHECK(s.ToString() == "one");
}

TEST_CASE("QueueEmptyOutput", "[Queue]") {
    Queue q;
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    q.peek();
    q.Print();
    q.pop();
    cout.rdbuf(old);
    CHECK(output.str().find("Пуста") != string::npos);
    CHECK(output.str().find("Error!") != string::npos);
    CHECK(q.Size() == 0);
}

TEST_CASE("QueueOutputAndOrder", "[Queue]") {
    Queue q;
    q.push("one");
    q.push("two");
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    q.peek();
    q.Print();
    cout.rdbuf(old);
    CHECK(output.str() == "one\none two \n");
    q.pop();
    CHECK(q.ToString() == "two");
}

TEST_CASE("TreeEmptyOutput", "[RBTree]") {
    RBTree t;
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    t.Print();
    t.PrintInorder();
    t.PrintPreorder();
    t.PrintPostorder();
    t.PrintBFS();
    cout.rdbuf(old);
    CHECK(output.str().find("Пустое") != string::npos);
    CHECK(t.ToString() == "");
}

TEST_CASE("TreeTraversals", "[RBTree]") {
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
    CHECK(output.str().find("5(") != string::npos);
    CHECK(output.str().find("10(") != string::npos);
    CHECK(output.str().find("15(") != string::npos);
    CHECK(t.Size() == 3);
    CHECK(!t.ToString().empty());
}

TEST_CASE("TreeLoadAndSave", "[RBTree]") {
    RBTree t;
    t.Load(10, 'B');
    t.Load(5, 'R');
    t.Load(15, 'R');
    CHECK(t.Size() == 3);
    CHECK(t.ToString() == "10 B 5 R 15 R");
    CHECK(t.Contains(5));
    CHECK(t.Contains(15));
}

TEST_CASE("TreeInsertRotations", "[RBTree]") {
    const int keys[6][3] = {
        {30, 20, 10}, {10, 20, 30}, {30, 10, 20},
        {10, 30, 20}, {20, 10, 30}, {20, 30, 10}
    };
    for (int i = 0; i < 6; ++i) {
        RBTree t;
        for (int j = 0; j < 3; ++j) t.Insert(keys[i][j]);
        CHECK(t.Size() == 3);
        CHECK(t.Contains(10));
        CHECK(t.Contains(20));
        CHECK(t.Contains(30));
        CHECK(!t.Contains(40));
    }
}

TEST_CASE("TreeRemoveRootCases", "[RBTree]") {
    {
        RBTree t;
        t.Insert(10);
        t.Remove(10);
        CHECK(t.Size() == 0);
    }
    {
        RBTree t;
        t.Insert(10);
        t.Insert(5);
        t.Remove(10);  // у корня только левый потомок
        CHECK(t.Size() == 1);
        CHECK(t.Contains(5));
    }
    {
        RBTree t;
        t.Insert(10);
        t.Insert(15);
        t.Remove(10);  // у корня только правый потомок
        CHECK(t.Size() == 1);
        CHECK(t.Contains(15));
    }
    {
        RBTree t;
        t.Insert(10);
        t.Insert(5);
        t.Insert(15);
        t.Remove(10);  // у корня два потомка
        CHECK(t.Size() == 2);
        CHECK(!t.Contains(10));
        CHECK(t.Contains(5));
        CHECK(t.Contains(15));
    }
}

TEST_CASE("TreeRemoveMany", "[RBTree]") {
    RBTree t;
    for (int i = 0; i < 30; ++i) t.Insert(i);
    CHECK(t.Size() == 30);
    for (int i = 0; i < 30; ++i) {
        CHECK(t.Contains(i));
        t.Remove(i);
        CHECK(!t.Contains(i));
        CHECK(t.Size() == 29 - i);
    }
}

TEST_CASE("TreeRemoveReverse", "[RBTree]") {
    RBTree t;
    for (int i = 29; i >= 0; --i) t.Insert(i);
    CHECK(t.Size() == 30);
    for (int i = 29; i >= 0; --i) {
        t.Remove(i);
        CHECK(!t.Contains(i));
        CHECK(t.Size() == i);
    }
}

TEST_CASE("ChainTableEmptyAndPrint", "[ChainTable]") {
    ChainTable t(0);
    CHECK(t.Capacity() == 1);
    CHECK(t.Size() == 0);
    CHECK(t.ToString() == "");
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    t.Print();
    cout.rdbuf(old);
    CHECK(output.str() == "Пустая\n");
    string value;
    CHECK(!t.Get("missing", value));
    CHECK(!t.Remove("missing"));
}

TEST_CASE("ChainTableCollisionsAndPrint", "[ChainTable]") {
    ChainTable t(128);
    CHECK(t.Insert("aa", "one"));
    CHECK(t.Insert("ee", "two"));   // коллизия: одинаковый остаток по модулю 128
    CHECK(t.Insert("ii", "three"));
    CHECK(t.Size() == 3);
    string value;
    REQUIRE(t.Get("ee", value));
    CHECK(value == "two");
    CHECK(t.Remove("ee"));
    CHECK(t.Get("aa", value));
    CHECK(value == "one");
    CHECK(t.Get("ii", value));
    CHECK(value == "three");
    CHECK(!t.Insert("aa", "new"));
    REQUIRE(t.Get("aa", value));
    CHECK(value == "new");
    CHECK(t.ToString().find("aa new") != string::npos);
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    t.Print();
    cout.rdbuf(old);
    CHECK(output.str().find("aa=new") != string::npos);
}

TEST_CASE("CuckooTableEmptyAndPrint", "[CuckooTable]") {
    CuckooTable t(0);
    CHECK(t.Size() == 0);
    CHECK(t.ToString() == "");
    string value;
    CHECK(!t.Get("", value));
    CHECK(!t.Remove(""));
    CHECK(!t.Remove("missing"));
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    t.Print();
    cout.rdbuf(old);
    CHECK(output.str() == "Пустая\n");
}

TEST_CASE("CuckooTableSecondTable", "[CuckooTable]") {
    CuckooTable t(16);
    CHECK(t.Insert("aa", "one"));
    CHECK(t.Insert("aq", "two")); // два ключа с одним h1 и h2
    CHECK(t.Size() == 2);
    string value;
    REQUIRE(t.Get("aa", value));
    CHECK(value == "one");
    REQUIRE(t.Get("aq", value));
    CHECK(value == "two");
    CHECK(!t.Insert("aa", "updated"));
    CHECK(!t.Insert("aq", "updated2"));
    REQUIRE(t.Get("aq", value));
    CHECK(value == "updated2");
    ostringstream output;
    streambuf* old = cout.rdbuf(output.rdbuf());
    t.Print();
    cout.rdbuf(old);
    CHECK(output.str().find("T1[") != string::npos);
    CHECK(output.str().find("T2[") != string::npos);
    CHECK(t.ToString().find("aa updated") != string::npos);
    CHECK(t.Remove("aq"));
    CHECK(!t.Get("aq", value));
    CHECK(t.Size() == 1);
    CHECK(t.Remove("aa"));
    CHECK(t.Size() == 0);
}

TEST_CASE("CuckooTableCycleAndRehash", "[CuckooTable]") {
    CuckooTable t(16);
    CHECK(t.Insert("aa", "one"));
    CHECK(t.Insert("aq", "two"));
    CHECK(t.Insert("aA", "three")); // вызываем вытеснения и перестройку
    CHECK(t.Size() == 3);
    string value;
    REQUIRE(t.Get("aa", value));
    CHECK(value == "one");
    REQUIRE(t.Get("aq", value));
    CHECK(value == "two");
    REQUIRE(t.Get("aA", value));
    CHECK(value == "three");
}
