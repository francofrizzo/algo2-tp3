#include <iostream>
#include "./aed2.h"
#include "./dicc_trie.h"
#include "./ab.h"
#include "./dicc_log.h"
#include "./cola_prior.h"
#include "./red.h"
#include "./dcnet.h"
#include "./mini_test.h"

using namespace aed2;
using namespace tp3;

using std::cout;
using std::endl;

// Diccionario de strings

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

// Cola de prioridad

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
    ASSERT_EQ(cola.desencolar(), 150)
    ASSERT_EQ(cola.desencolar(), 99)
    ASSERT_EQ(cola.desencolar(), 8)
    ASSERT_EQ(cola.desencolar(), 7)
    ASSERT_EQ(cola.desencolar(), 6)
    ASSERT_EQ(cola.desencolar(), 4)
    ASSERT_EQ(cola.desencolar(), 1)
    ASSERT_EQ(cola.desencolar(), -12)
    ASSERT_EQ(cola.desencolar(), -80)
}

// Arbol binario

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

    delete A->izq();
    A->izq(new ab<int>());

    ASSERT(A->izq()->esNil());

    *A = *C;

    ASSERT_EQ(A->raiz(), 8);

    delete A->der();
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
    dicc.definir(0, 18);
    dicc.definir(8, 300);
    dicc.borrar(8);
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

void test_borrado_solo_raiz() {
    diccLog<int, int> dicc = diccLog<int, int>();
    dicc.definir(4, 5);
    ASSERT_EQ(dicc.definido(4), true);
    ASSERT_EQ(dicc.cantClaves(), 1);
    dicc.borrar(4);
    ASSERT_EQ(dicc.definido(4), false);
    ASSERT_EQ(dicc.cantClaves(), 0);
}

void test_borrado_hoja() {
    diccLog<int, int> dicc = diccLog<int, int>();
    dicc.definir(4, 5);
    dicc.definir(6, 25);
    dicc.definir(3, 7);
    dicc.definir(1, 36);
    ASSERT_EQ(dicc.definido(1), true);
    ASSERT_EQ(dicc.cantClaves(), 4);
    dicc.borrar(1);
    ASSERT_EQ(dicc.definido(1), false);
    ASSERT_EQ(dicc.cantClaves(), 3);
}

void test_borrado_solo_hijo_izq() {
    diccLog<int, int> dicc = diccLog<int, int>();
    dicc.definir(4, 5);
    dicc.definir(6, 25);
    dicc.definir(3, 7);
    dicc.definir(1, 36);
    ASSERT_EQ(dicc.definido(3), true);
    ASSERT_EQ(dicc.cantClaves(), 4);
    dicc.borrar(3);
    ASSERT_EQ(dicc.definido(3), false);
    ASSERT_EQ(dicc.cantClaves(), 3);
}

void test_borrado_solo_hijo_der() {
    diccLog<int, int> dicc = diccLog<int, int>();
    dicc.definir(4, 5);
    dicc.definir(6, 25);
    dicc.definir(3, 7);
    dicc.definir(1, 36);
    dicc.definir(8, 0);
    ASSERT_EQ(dicc.definido(6), true);
    ASSERT_EQ(dicc.cantClaves(), 5);
    dicc.borrar(6);
    ASSERT_EQ(dicc.definido(6), false);
    ASSERT_EQ(dicc.cantClaves(), 4);
}

void test_borrado_raiz() {
    diccLog<int, int> dicc = diccLog<int, int>();
    dicc.definir(4, 5);
    dicc.definir(3, 7);
    dicc.definir(6, 25);
    dicc.definir(5, 44);
    dicc.definir(1, 36);
    dicc.definir(8, 0);
    ASSERT_EQ(dicc.definido(4), true);
    ASSERT_EQ(dicc.cantClaves(), 6);
    dicc.borrar(4);
    ASSERT_EQ(dicc.definido(4), false);
    ASSERT_EQ(dicc.cantClaves(), 5);
    dicc.borrar(6);
    ASSERT_EQ(dicc.definido(6), false);
    ASSERT_EQ(dicc.cantClaves(), 4);
}

void test_rotac_izquierda() {
    diccLog<int, int> dicc = diccLog<int, int>();
    dicc.definir(1, 0);
    dicc.definir(5, 0);
    dicc.definir(10, 0);
    // ASSERT(dicc.estaBalanceado());
}

void test_rotac_derecha() {
    diccLog<int, int> dicc = diccLog<int, int>();
    dicc.definir(10, 0);
    dicc.definir(5, 0);
    dicc.definir(1, 0);
    // ASSERT(dicc.estaBalanceado());
}

void test_rebalanceo() {
    diccLog<int, int> dicc = diccLog<int, int>();
    dicc.definir(50, 0);
    dicc.definir(75, 0);
    dicc.definir(100, 0);
    // ASSERT(dicc.estaBalanceado());

    dicc.definir(40, 0);
    dicc.definir(20, 0);
    // ASSERT(dicc.estaBalanceado());

    dicc.definir(45, 0);
    // ASSERT(dicc.estaBalanceado());

    dicc.definir(80, 0);
    // ASSERT(dicc.estaBalanceado());

    dicc.borrar(50);
    // ASSERT(dicc.estaBalanceado());

    dicc.borrar(75);
    // ASSERT(dicc.estaBalanceado());

    dicc.borrar(80);
    // ASSERT(dicc.estaBalanceado());

    dicc.borrar(100);
    // ASSERT(dicc.estaBalanceado());
}

// Red y DCNet

void test_red() {
    red r = red();
    ASSERT_EQ(r.cantCompus(), 0);
    compu c1;
    Conj<interfaz> inter1 = Conj<interfaz>();
    inter1.Agregar(1);
    inter1.Agregar(2);
    c1.IP = "Manu";
    c1.interfaces = inter1;
    r.agregarCompu(c1);
    ASSERT_EQ(r.cantCompus(), 1);
    ASSERT(r.vecinos(c1).EsVacio());
    compu c2;
    Conj<interfaz> inter2 = Conj<interfaz>();
    inter2.Agregar(5);
    inter2.Agregar(6);
    inter2.Agregar(3);
    c2.IP = "Lu";
    c2.interfaces = inter2;
    r.agregarCompu(c2);
    compu c3;
    Conj<interfaz> inter3 = Conj<interfaz>();
    inter3.Agregar(1);
    inter3.Agregar(65);
    inter3.Agregar(7);
    c3.IP = "Fran";
    c3.interfaces = inter3;
    r.agregarCompu(c3);
    compu c4;
    Conj<interfaz> inter4 = Conj<interfaz>();
    inter2.Agregar(9);
    c4.IP = "Minion";
    c4.interfaces = inter4;
    r.agregarCompu(c4);
    compu c5;
    Conj<interfaz> inter5 = Conj<interfaz>();
    inter5.Agregar(5);
    inter5.Agregar(6);
    inter5.Agregar(45);
    c5.IP = "Uri";
    c5.interfaces = inter5;
    r.agregarCompu(c5);
    r.conectar(c1, 1, c2, 5);
    r.conectar(c3, 65, c2, 3);
    r.conectar(c1, 2, c3, 7);
    ASSERT(r.conectadas(c1, c2));
    ASSERT_EQ(r.interfazUsada(c1, c2), 1);
    ASSERT_EQ(r.interfazUsada(c2, c3), 3);
    Conj<compu> vec1 = Conj<compu>();
    vec1.Agregar(c2);
    vec1.Agregar(c3);
    ASSERT(r.vecinos(c1) == vec1);
    ASSERT(r.usaInterfaz(c1, 1));
    ASSERT_EQ(r.usaInterfaz(c2, 6), false);
    ASSERT(r.hayCamino(c1, c2));
    ASSERT(r.hayCamino(c2, c3));
    ASSERT_EQ(r.hayCamino(c1, c4), false);
    ASSERT(r.hayCamino(c2, c1));
    ASSERT(r.hayCamino(c3, c3));
    Conj<Lista<compu> > minis = Conj<Lista<compu> > ();
    Lista <compu> lis = Lista<compu> ();
    lis.AgregarAtras(c1);
    lis.AgregarAtras(c2);
    minis.Agregar(lis);
    ASSERT(r.caminosMinimos(c1, c2) == minis);
    Conj<compu> pcs = Conj<compu>();
    pcs.Agregar(c1);
    pcs.Agregar(c2);
    pcs.Agregar(c3);
    pcs.Agregar(c4);
    pcs.Agregar(c5);
    ASSERT(r.computadoras() == pcs);
    ASSERT_EQ(r.cantCompus(), 5);
}

void test_dcnet() {
    // crear red
    red r = red();
    // creo compu 1
    compu c1;
    Conj<interfaz> inter1 = Conj<interfaz>();
    inter1.Agregar(1);
    inter1.Agregar(2);
    c1.IP = "Manu";
    c1.interfaces = inter1;
    r.agregarCompu(c1);
    // creo compu 2
    compu c2;
    Conj<interfaz> inter2 = Conj<interfaz>();
    inter2.Agregar(5);
    inter2.Agregar(6);
    inter2.Agregar(3);
    c2.IP = "Lu";
    c2.interfaces = inter2;
    r.agregarCompu(c2);
    // creo compu 3
    compu c3;
    Conj<interfaz> inter3 = Conj<interfaz>();
    inter3.Agregar(1);
    inter3.Agregar(65);
    inter3.Agregar(7);
    c3.IP = "Fran";
    c3.interfaces = inter3;
    r.agregarCompu(c3);
    // creo compu 4
    compu c4;
    Conj<interfaz> inter4 = Conj<interfaz>();
    inter4.Agregar(9);
    c4.IP = "Minion";
    c4.interfaces = inter4;
    r.agregarCompu(c4);
    // creo compu 5
    compu c5;
    Conj<interfaz> inter5 = Conj<interfaz>();
    inter5.Agregar(5);
    inter5.Agregar(6);
    inter5.Agregar(45);
    c5.IP = "Uri";
    c5.interfaces = inter5;
    r.agregarCompu(c5);
    // conecto las compus
    r.conectar(c1, 1, c2, 5);
    r.conectar(c3, 65, c2, 3);
    // r.conectar(c1, 2, c3, 7);
    r.conectar(c3, 1, c4, 9);
    // creo conj vecinos
    Conj<compu> vec1 = Conj<compu>();
    vec1.Agregar(c2);
    vec1.Agregar(c3);
    // creo camainos minimos
    Conj<Lista<compu> > minis = Conj<Lista<compu> > ();
    Lista <compu> lis = Lista<compu> ();
    lis.AgregarAtras(c1);
    lis.AgregarAtras(c2);
    minis.Agregar(lis);
    // creo cojunto compus
    Conj<compu> pcs = Conj<compu>();
    pcs.Agregar(c1);
    pcs.Agregar(c2);
    pcs.Agregar(c3);
    pcs.Agregar(c4);
    pcs.Agregar(c5);

    dcnet d = dcnet(r);

    ASSERT_EQ(d.laRed().cantCompus(), 5);
    paquete p1;
    p1.ID = 1;
    p1._prioridad = 1;
    p1.origen = c1;
    p1.destino = c3;
    d.crearPaquete(p1);
    Lista<compu> l;
    l.AgregarAtras(c1);
    ASSERT(d.caminoRecorrido(p1) == l);
    d.avanzarSegundo();
    ASSERT(d.laQueMasEnvio() == c1);
    l.AgregarAtras(c2);
    // // ASSERT(d.laRed() == r);  // ¡No sabemos comparar redes!
    ASSERT(d.caminoRecorrido(p1) == l);
    Conj<paquete> paqs = Conj<paquete>();
    paqs.Agregar(p1);
    ASSERT(d.enEspera(c2) == paqs);
    ASSERT(d.caminoRecorrido(p1) == l);
    d.avanzarSegundo();
    ASSERT_EQ(d.paqueteEnTransito(p1), false);
    d.avanzarSegundo();
    ASSERT_EQ(d.cantidadEnviados(c1), 1);

    // IMPORTANTE
    // Todo funciona bien, pero los paquetes pasan un segundo el el destino antes de morir.
    // Revisar la especificación, porque no estoy seguro de que eso era lo que queríamos.

    // // creo paquetes 2, 3 y 4. c1 mando 1 paquete, c2 mando un paquete. No hay paquetes en transito.

    paquete p2;
    p2.ID = 2;
    p2._prioridad = 2;
    p2.origen = c2;
    p2.destino = c3;
    d.crearPaquete(p2);
    paquete p3;
    p3.ID = 3;
    p3._prioridad = 3;
    p3.origen = c3;
    p3.destino = c1;
    d.crearPaquete(p3);
    paquete p4;
    p4.ID = 4;
    p4._prioridad = 4;
    p4.origen = c3;
    p4.destino = c1;
    d.crearPaquete(p4);

    // veamos que en c3 estan en espera los dos paquetes que tienen como origen a c3, es decir, p3 y p4.

    paqs = Conj<paquete>();
    paqs.Agregar(p3);
    paqs.Agregar(p4);
    ASSERT(d.enEspera(c3) == paqs);

    d.avanzarSegundo();

    // p2 llego a destino. p4 esta en c2. p3 esta en c3.
    //c1 mando 1 paquete. c2 mando 2 paquetes.  c3 mando 1 paquete.

    Lista<compu> l3;
    l3.AgregarAtras(c3);
    ASSERT(d.caminoRecorrido(p3) == l3);
    Lista<compu> l4;
    l4.AgregarAtras(c3);
    l4.AgregarAtras(c2);
    ASSERT(d.caminoRecorrido(p4) == l4);

    // cantidad de paquetes enviados de cada compu

    ASSERT_EQ(d.cantidadEnviados(c1), 1);
    ASSERT_EQ(d.cantidadEnviados(c2), 2);
    ASSERT_EQ(d.cantidadEnviados(c3), 1);

    // quienes son los paquetes en espera de las compus.

    Conj<paquete> paqs1 = Conj<paquete>();
    ASSERT(d.enEspera(c1) == paqs1);
    Conj<paquete> paqs2 = Conj<paquete> ();
    paqs2.Agregar(p4);
    ASSERT(d.enEspera(c2) == paqs2);
    Conj<paquete> paqs3 = Conj<paquete> ();
    paqs3.Agregar(p3);
    ASSERT(d.enEspera(c3) == paqs3);

    //los paquetes que estan en transito.

    ASSERT_EQ(d.paqueteEnTransito(p2), false);
    ASSERT(d.paqueteEnTransito(p3));
    ASSERT(d.paqueteEnTransito(p4));

    //la que mas envio es c2.

    ASSERT(d.laQueMasEnvio() == c2);

    d.avanzarSegundo();

    // p4 llego a destino. p3 esta en c2
    // c1 envio 1 paq. c2 envio 3 paquetes. c3 envio 2 paquetes

    l3.AgregarAtras(c2);
    ASSERT(d.caminoRecorrido(p3) == l3);

    ASSERT_EQ(d.cantidadEnviados(c1), 1);
    ASSERT_EQ(d.cantidadEnviados(c2), 3);
    ASSERT_EQ(d.cantidadEnviados(c3), 2);

    ASSERT(d.enEspera(c1) == paqs1);
    paqs2 = Conj<paquete> ();
    paqs2.Agregar(p3);
    ASSERT(d.enEspera(c2) == paqs2);
    Conj<paquete> paqs33 = Conj<paquete> ();
    ASSERT(d.enEspera(c3) == paqs33);

    ASSERT_EQ(d.paqueteEnTransito(p4), false);
    ASSERT(d.paqueteEnTransito(p3));

    ASSERT(d.laQueMasEnvio() == c2);

    d.avanzarSegundo();

    // No hay paquetes en transito.
    // c1 envio 1 paq. c2 envio 4 paquetes. c3 envio 2 paquetes.

    ASSERT_EQ(d.cantidadEnviados(c1), 1);
    ASSERT_EQ(d.cantidadEnviados(c2), 4);
    ASSERT_EQ(d.cantidadEnviados(c3), 2);

    ASSERT(d.enEspera(c1) == paqs1);
    ASSERT(d.enEspera(c2) == paqs1);
    ASSERT(d.enEspera(c3) == paqs1);

    ASSERT_EQ(d.paqueteEnTransito(p3), false);

    ASSERT(d.laQueMasEnvio() == c2);
}

int main() {
    // // Diccionario de strings
    RUN_TEST(test_trie);

    // // Cola de prioridad
    RUN_TEST(test_heap);

    // // Árbol binario
    RUN_TEST(test_ab);

    // // Diccionario logarítmico
    RUN_TEST(test_definir);
    RUN_TEST(test_borrado_solo_raiz);
    RUN_TEST(test_borrado_hoja);
    RUN_TEST(test_borrado_solo_hijo_izq);
    RUN_TEST(test_borrado_solo_hijo_der);
    RUN_TEST(test_borrado_raiz);
    RUN_TEST(test_obtener);
    RUN_TEST(test_rotac_izquierda);
    RUN_TEST(test_rotac_derecha);
    RUN_TEST(test_rebalanceo);

    // Red y DCNet
    RUN_TEST(test_red);
    RUN_TEST(test_dcnet);

    return 0;
}

// NOTA IMPORTANTE:
// Necesitamos ver qué onda los IDs de los paquetes... La idea era que los
// asignara el sistema, no que vinieran de afuera, y no hacerlo de esa forma nos
// puede traer problemas si nos intentan cagar con paquetes con IDs repetidos.
// Y no sabemos con qué nos podemos encontrar en los tests (todavía no hice nada del driver).
