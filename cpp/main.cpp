#include <iostream>
// #include "./aed2.h"
// #include "./dicc_trie.h"
// #include "./ab.h"
// #include "./dicc_log.h"
// #include "./cola_prior.h"
// #include "./red.h"
#include "./dcnet.h"
#include "./mini_test.h"

using namespace aed2;
using namespace tp3;

using std::cout;
using std::endl;

/*void test_ab() {
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

    // *A = ab<int>(new ab<int>(), A->raiz(), A->der());

    // ASSERT_EQ(A->cantNodos(), 2);
    // ASSERT_EQ(C->cantNodos(), 3);

    delete(C);
    delete(A);
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
*/

/*void test_dcnet() {
    dcnet d = dcnet();
    ASSERT_EQ(d.laRed().cantCompus(), 0);
}
*/
int main() {
    // RUN_TEST(test_ab);
    //RUN_TEST(test_dcnet);
    return 0;
}
