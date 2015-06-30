#include <iostream>
// #include "./aed2.h"
// #include "./red.h"
// #include "./ab.h"
#include "./dicc_log.h"

using namespace aed2;
using namespace tp3;

using std::cout;
using std::endl;

void test_ab() {
    ab<int> ii = ab<int>(ab<int>(), 1, ab<int>());
    ab<int> dd = ab<int>(ab<int>(), 4, ab<int>());
    ab<int> cc = ab<int>(ii, 8, dd);
    cout << cc.raiz() << endl;
    cout << "Altura de cc: " << cc.altura() << endl;
    cout << "Cant de nodos de cc: " << cc.cantNodos() << endl;
    cc.izq().izq() = ab<int>(ab<int>(), 6, ab<int>());
    cout << "Altura de cc: " << cc.altura() << endl;
    cout << "Cant de nodos de cc: " << cc.cantNodos() << endl;
    ab<int> nuevo = cc;
    cout << nuevo.izq().raiz() << endl;
    cc.izq().raiz() = 80;
    cout << nuevo.izq().raiz() << endl;
}

void test_dicc_log() {
    diccLog<int, int> dicc = diccLog<int, int>();
    dicc.definir(4, 100);
    cout << dicc.definido(4) << endl;
    dicc.definir(5, 100);
    dicc.definir(1, 25);
    dicc.definir(6, 41);
    dicc.definir(1, 16);
    cout << dicc.obtener(6) << endl;
    cout << dicc.obtener(4) << endl;
}

int main() {
    // test_ab();
    test_dicc_log();
    
    return 0;
}
