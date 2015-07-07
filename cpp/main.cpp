#include <iostream>
// #include "./aed2.h"
#include "./dicc_trie.h"
// #include "./ab.h"
#include "./dicc_log.h"
// #include "./red.h"
// #include "./dcnet.h"
#include "./mini_test.h"

using namespace aed2;
using namespace tp3;

using std::cout;
using std::endl;

void test_ab() {
    ab<int>* nil1 = new ab<int>();
    ab<int>* nil2 = new ab<int>();
    ab<int>* nil3 = new ab<int>();
    ab<int>* nil4 = new ab<int>();
    ab<int>* I = new ab<int>(nil1, 1, nil2);
    ab<int>* D = new ab<int>(nil3, 4, nil4);
    ab<int>* C = new ab<int>(I, 8, D);

    ASSERT_EQ(C->raiz(), 8);
    ASSERT_EQ(C->izq()->raiz(), 1);
    ASSERT_EQ(C->der()->raiz(), 4);
    ASSERT_EQ(C->altura(), 2);
    ASSERT_EQ(C->cantNodos(), 3);

    nil1 = new ab<int>();
    nil2 = new ab<int>();
    ab<int>* A = new ab<int>(nil1, 16, nil2);

    ASSERT_EQ(A->raiz(), 16);
    ASSERT_EQ(A->altura(), 1);
    ASSERT_EQ(A->cantNodos(), 1);

    A->izq(new ab<int>());

    ASSERT(A->izq()->esNil());

    *A = *C;

    ASSERT_EQ(A->raiz(), 8);

    A->der(new ab<int>());

    ASSERT_EQ(A->cantNodos(), 2);
    ASSERT_EQ(C->cantNodos(), 3);

    delete(C);
    delete(A);
}

// Diccionario logarítmico

void test_definir() {
    diccLog<int, int> dicc = diccLog<int, int>();
    ASSERT_EQ(dicc.definido(4), false);
    dicc.definir(4, 100);
    ASSERT_EQ(dicc.cantClaves(), 1);
    ASSERT_EQ(dicc.definido(4), true);
    dicc.definir(5, 100);
    ASSERT_EQ(dicc.cantClaves(), 2);
    ASSERT_EQ(dicc.definido(4), true);
    ASSERT_EQ(dicc.definido(5), true);
    dicc.definir(1, 25);
    dicc.definir(6, 41);
    ASSERT_EQ(dicc.cantClaves(), 4);
    dicc.definir(1, 16);
    ASSERT_EQ(dicc.definido(4), true);
    ASSERT_EQ(dicc.definido(5), true);
    ASSERT_EQ(dicc.definido(1), true);
    ASSERT_EQ(dicc.definido(6), true);
    ASSERT_EQ(dicc.cantClaves(), 4);
}

void test_obtener() {
    diccLog<int, int> dicc = diccLog<int, int>();
    dicc.definir(4, 100);
    dicc.definir(5, 100);
    dicc.definir(1, 25);
    dicc.definir(6, 41);
    dicc.definir(1, 16);
    ASSERT_EQ(dicc.obtener(4), 100);
    ASSERT_EQ(dicc.obtener(5), 100);
    ASSERT_EQ(dicc.obtener(6), 41);
    ASSERT_EQ(dicc.obtener(1), 16);
}

/*

void test_red() {
    red r = red();
}

void test_heap() {
    colaPrior<int> cola;
    cola.encolar(4);
    cola.encolar(1);
    cola.encolar(6);
    cola.encolar(-80);
    cola.encolar(8);
    cola.encolar(150);
    cola.encolar(7);
    cola.encolar(99);
    cola.encolar(-12);
    cout << cola.desencolar() << endl;
    cout << cola.desencolar() << endl;
    cout << cola.desencolar() << endl;
    cout << cola.desencolar() << endl;
    cout << cola.desencolar() << endl;
    cout << cola.desencolar() << endl;
    cout << cola.desencolar() << endl;
    cout << cola.desencolar() << endl;
    cout << cola.desencolar() << endl;
}


void test_trie() {
    DiccString<int> trie;
    std::vector<string> vacio;
    std::vector<string> claves = trie.claves();
    ASSERT(claves == vacio);

    trie.definir("hola", 4);
    ASSERT(trie.definido("hola"));
    ASSERT_EQ(trie.obtener("hola"), 4);

    trie.definir("chau", 55);
    ASSERT(trie.definido("hola"));
    ASSERT(trie.definido("chau"));
    ASSERT_EQ(trie.obtener("hola"), 4);
    ASSERT_EQ(trie.obtener("chau"), 55);

    claves = trie.claves();
    ASSERT_EQ(claves.size(), 2);

    trie.borrar("hola");
    ASSERT(!trie.definido("hola"));
    ASSERT(trie.definido("chau"));
    ASSERT_EQ(trie.obtener("chau"), 55);

    claves = trie.claves();
    ASSERT_EQ(claves.size(), 1);
}

void test_dcnet() {
    red r;
    dcnet d = dcnet(r);
    ASSERT_EQ(d.laRed().cantCompus(), 0);
}
*/

int main() {
    RUN_TEST(test_ab);
    // RUN_TEST(test_definir);
    // RUN_TEST(test_obtener);
    RUN_TEST(test_recalcular_alturas);
    // RUN_TEST(test_dcnet);
    // RUN_TEST(test_trie);
    return 0;
}
