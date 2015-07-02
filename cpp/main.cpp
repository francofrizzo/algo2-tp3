#include <iostream>
// #include "./aed2.h"
// #include "./dicc_trie.h"
#include "./ab.h"
// #include "./dicc_log.h"
// #include "./cola_prior.h"
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

    *A = *C;

    ASSERT_EQ(A->raiz(), 8);

    delete(C);
    delete(A);

    // cout << cc.raiz() << endl;
    // cout << "Altura de cc: " << cc.altura() << endl;
    // cout << "Cant de nodos de cc: " << cc.cantNodos() << endl;
    // ab<int> nil5 = ab<int>();
    // ab<int> nil6 = ab<int>();
    // cc.izq()->izq() = ab<int>(nil5, 6, nil6);
    // cout << "Altura de cc: " << cc.altura() << endl;
    // cout << "Cant de nodos de cc: " << cc.cantNodos() << endl;
    // ab<int> nuevo = cc;
    // cout << nuevo.izq()->raiz() << endl;
    // cc.izq()->raiz() = 80;
    // cout << nuevo.izq()->raiz() << endl;
}

void test_dicc_log() {
    // diccLog<int, int> dicc = diccLog<int, int>();
    // dicc.definir(4, 100);
    // cout << dicc.definido(4) << endl;
    // dicc.definir(5, 100);
    // dicc.definir(1, 25);
    // dicc.definir(6, 41);
    // dicc.definir(1, 16);
    // cout << dicc.obtener(6) << endl;
    // cout << dicc.obtener(4) << endl;
}

void test_red() {
    // red r = red();
}

void test_heap() {
    // colaPrior<int> cola;
    // cola.encolar(4);
    // cola.encolar(1);
    // cola.encolar(6);
    // cola.encolar(-80);
    // cola.encolar(8);
    // cola.encolar(150);
    // cola.encolar(7);
    // cola.encolar(99);
    // cola.encolar(-12);
    // cout << cola.desencolar() << endl;
    // cout << cola.desencolar() << endl;
    // cout << cola.desencolar() << endl;
    // cout << cola.desencolar() << endl;
    // cout << cola.desencolar() << endl;
    // cout << cola.desencolar() << endl;
    // cout << cola.desencolar() << endl;
    // cout << cola.desencolar() << endl;
    // cout << cola.desencolar() << endl;
}

int main() {
    RUN_TEST(test_ab);
    
    return 0;
}
