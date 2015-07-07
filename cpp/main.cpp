#include <iostream>
#include "./aed2.h"
#include "./dicc_trie.h"
#include "./ab.h"
#include "./dicc_log.h"
#include "./cola_prior.h"
#include "./red.h"
// #include "./dcnet.h"
#include "./mini_test.h"

using namespace aed2;
using namespace tp3;

using std::cout;
using std::endl;

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
    ASSERT_EQ(cola.desencolar(), 1)
    ASSERT_EQ(cola.desencolar(), 4)
    ASSERT_EQ(cola.desencolar(), -12)
    ASSERT_EQ(cola.desencolar(), -80)
}

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
    ASSERT(dicc.estaBalanceado());
    dicc.definir(0, 18);
    ASSERT(dicc.estaBalanceado());
    dicc.definir(8, 300);
    dicc.borrar(8);
    ASSERT(dicc.estaBalanceado());
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

void test_borrado_ambos_hijos() {
}

void test_borrado_raiz() {
    diccLog<int, int> dicc = diccLog<int, int>();
    dicc.definir(4, 5);
    dicc.definir(6, 25);
    dicc.definir(3, 7);
    dicc.definir(1, 36);
    dicc.definir(8, 0);
    ASSERT_EQ(dicc.definido(4), true);
    ASSERT_EQ(dicc.cantClaves(), 5);
    dicc.borrar(4);
    ASSERT_EQ(dicc.definido(4), false);
    ASSERT_EQ(dicc.cantClaves(), 4);
}

/*
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
    ASSERT(r.caminosMinimos(c1,c2) == minis);
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
    //crear red
    red r = red();
   //creo compu 1
    compu c1;
    Conj<interfaz> inter1 = Conj<interfaz>();
    inter1.Agregar(1);
    inter1.Agregar(2);
    c1.IP = "Manu";
    c1.interfaces = inter1;
    r.agregarCompu(c1);
    //creo compu 2
    compu c2;
    Conj<interfaz> inter2 = Conj<interfaz>();
    inter2.Agregar(5);
    inter2.Agregar(6);
    inter2.Agregar(3);
    c2.IP = "Lu";
    c2.interfaces = inter2;
    r.agregarCompu(c2);
    //creo compu 3
    compu c3;
    Conj<interfaz> inter3 = Conj<interfaz>();
    inter3.Agregar(1);
    inter3.Agregar(65);
    inter3.Agregar(7);
    c3.IP = "Fran";
    c3.interfaces = inter3;
    r.agregarCompu(c3);
    //creo compu 4
    compu c4;
    Conj<interfaz> inter4 = Conj<interfaz>();
    inter2.Agregar(9);
    c4.IP = "Minion";
    c4.interfaces = inter4;
    r.agregarCompu(c4);
    //creo compu 5
    compu c5;
    Conj<interfaz> inter5 = Conj<interfaz>();
    inter5.Agregar(5);
    inter5.Agregar(6);
    inter5.Agregar(45);
    c5.IP = "Uri";
    c5.interfaces = inter5;
    r.agregarCompu(c5);
    //conecto las compus
    r.conectar(c1, 1, c2, 5);
    r.conectar(c3, 65, c2, 3);
    r.conectar(c1, 2, c3, 7);
    //creo conj vecinos
    Conj<compu> vec1 = Conj<compu>();
    vec1.Agregar(c2);
    vec1.Agregar(c3);
    //creo camainos minimos
    Conj<Lista<compu> > minis = Conj<Lista<compu> > ();
    Lista <compu> lis = Lista<compu> ();
    lis.AgregarAtras(c1);
    lis.AgregarAtras(c2);
    minis.Agregar(lis);
    //creo cojunto compus
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
    lista<compu> l;
    l.AgregarAtras(c1);
    ASSERT_EQ(d.caminoRecorrido(p1), l);
    d.avanzarSegundo();
    ASSERT_EQ(d.laQueMasEnvio(), c1);
    l.AgregarAtras(c2);
    ASSERT_EQ(d.lared(), r);
    ASSERT_EQ(d.caminoRecorrido(p1),l);
    Conj<paquete> paqs = Conj<paquete>();
    paqs.Agregar(p1);
    ASSERT(d.enEspera(c2) == paqs);
    d.avanzarSegundo();
    ASSERT_EQ(d.paqueteEnTransito(p1), false);
    ASSERT_EQ(d.cantidadEnviados(c1), 1);
    
    //creo paquetes 2, 3 y 4. c1 mando 1 paquete, c2 mando un paquete. No hay paquetes en transito.
    
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
    d.avanzarSegundo();
    
    //p2 llego a destino. p4 esta en c2. p3 esta en c3. 
    //c2 mando 2 paquetes. c3 mando 1 paquete.  c1 mando 1 paquete.
    
    Lista<compu> l3;
    l3.AgregarAtras(c3);
    l3.AgregarAtras(c1);
    ASSERT_EQ(d.caminoRecorrido(p3), l3);
    Lista<compu> l4;
    l4.AgregarAtras(c3);
    l4.AgregarAtras(c2);
    ASSERT_EQ(d.caminoRecorrido(p4), l4);
    ASSERT_EQ(d.cantidadEnviados(c1), 1);
    ASSERT_EQ(d.cantidadEnviados(c2), 2);
    ASSERT_EQ(d.cantidadEnviados(c3), 2);
    Conj<paquete> paqs1 = Conj<paquete>();
    ASSERT(d.enEspera(c1) == paqs1);
    ASSERT(d.enEspera(c3) == paqs1);
    Conj<paquete> paqs2 = Conj<paquete>();
    paqs2.Agregar(p3);
    paqs2.Agregar(p4);
    ASSERT(d.enEspera(c3) == paqs2);
    ASSERT_EQ(d.paqueteEnTransito(p2), false);
    ASSERT(d.paqueteEnTransito(p3));
    ASSERT(d.paqueteEnTransito(p4));
    ASSERT_EQ(d.laQueMasEnvio(), c3); // aca puede dar c3 o c2, no se como quedan los nombres pasados por el trie.
    
}
*/

int main() {
    RUN_TEST(test_ab);
    RUN_TEST(test_definir);
    RUN_TEST(test_borrado_solo_raiz);
    RUN_TEST(test_borrado_hoja);
    RUN_TEST(test_borrado_solo_hijo_izq);
    RUN_TEST(test_borrado_solo_hijo_der);
    RUN_TEST(test_borrado_ambos_hijos);
    RUN_TEST(test_borrado_raiz);
    // RUN_TEST(test_obtener);
    // RUN_TEST(test_dcnet);
    // RUN_TEST(test_trie);
    // RUN_TEST(test_red);
    return 0;
}
