#ifndef DICC_LOG_H
#define DICC_LOG_H

#include <iostream>
#include "./aed2.h"
#include "./ab.h"

using namespace aed2;

namespace tp3 {

template<class K, class S>
class diccLog{
 private:
    struct entrada {
        K clave;
        S significado;
        Nat altSubarbol;
        ab<entrada> padre;

        entrada(K k, S s, Nat h, ab<entrada> p) :
            clave(k), significado(s), altSubarbol(h), padre(p) {}
    };

    ab<entrada> arbol;

    ab<entrada> buscar(const K& k, ab<entrada>& padre) const;
    void recalcularAltura(ab<entrada>&);
    int factorDeBalanceo(ab<entrada>&);
    void rotarAIzquierda(ab<entrada>&);
    void rotarADerecha(ab<entrada>&);
    void rebalancearArbol(ab<entrada>&);

 public:
    diccLog();
    diccLog(const diccLog<K, S>& otro);

    bool definido(const K& k) const;
    void definir(const K& k, const S& s);
    S& obtener(const K& k) const;
    void borrar(const K& k);
    Nat cantClaves() const;
};

template<class K, class S>
diccLog<K, S>::diccLog() :
    arbol(ab<entrada>()) {}

template<class K, class S>
diccLog<K, S>::diccLog(const diccLog<K, S>& otro) : 
    arbol(ab<entrada>(otro.arbol)) {}

template<class K, class S>
bool diccLog<K, S>::definido(const K& k) const {
    ab<entrada> padre;
    ab<entrada> lugar = buscar(k, padre);
    return !(lugar.esNil());
}

template<class K, class S>
void diccLog<K, S>::definir(const K& k, const S& s) {
    // std::cout << "Intentando definir " << k << std::endl;
    ab<entrada> padre;
    ab<entrada> lugar = buscar(k, padre);
    if (!(lugar.esNil())) {
        // std::cout << k << " ya estaba definido" << std::endl;
        lugar.raiz().significado = S(s);
        // std::cout << "Ahora " << k << " está definido como " << s << std::endl;
    } else {
        // std::cout << k << " no estaba definido" << std::endl;
        if (padre.esNil()) {
            // std::cout << "El árbol no tenía ningun nodo, agregando a " << k << " como raíz" << std::endl;
            arbol = ab<entrada>(
                ab<entrada>(), entrada(k, s, 1, ab<entrada>()), ab<entrada>());
        } else if (k <= padre.raiz().clave) {
            // std::cout << "Agregando a " << k << " como hijo izquierdo de " << padre.raiz().clave << std::endl;
            padre.izq() = ab<entrada>(
            ab<entrada>(), entrada(k, s, 1, padre), ab<entrada>());
        } else {
            // std::cout << "Agregando a " << k << " como hijo derecho de " << padre.raiz().clave << std::endl;
            padre.der() = ab<entrada>(
            ab<entrada>(), entrada(k, s, 1, padre), ab<entrada>());
        }
        rebalancearArbol(padre);
    }
}

template<class K, class S>
S& diccLog<K, S>::obtener(const K& k) const {
    assert(definido(k));
    ab<entrada> padre;
    ab<entrada> lugar = buscar(k, padre);
    return lugar.raiz().significado;
}

// template<class K, class S>
// void diccLog<K, S>::borrar(const K& k) {
//     ab<entrada>* padre;
//     ab<entrada>* lugar = buscar(k, padre);
//     if (lugar->izq().esNil() && lugar->der().esNil()) {
//         if (padre->izq() = lugar*) {
//             *padre = ab<entrada>(ab<entrada>(), padre->raiz(), padre->der());
//         } else {
//             *padre = ab<entrada>(padre->izq(), padre->raiz(), ab<entrada>());
//         }
//         padre->rebalancearArbol();
//     } else {
//         arbol = ab<entrada>();
//     }
//     delete(lugar);
// } else if (lugar->der.esNil()) {
//     lugar->izq().raiz().*padre = padre;
//     if (!(padre->esNil())) {
//         if (padre->izq() = lugar) {
//             *padre = ab<entrada>(lugar->izq(), padre->raiz(), padre->der());
//         } else {
//             *padre = ab<entrada>(padre->izq(), padre->raiz(), lugar->izq());
//         }
//         padre->rebalancearArbol();
//     } else {
//         arbol = lugar->izq();
//     }
//     delete(lugar);
// } else if (lugar->izq.esNil()) {
//     lugar->der().raiz().*padre = padre;
//     if (!(padre->esNil())) {
//         if (padre->izq() = lugar) {
//             *padre = ab<entrada>(lugar->der(), padre->raiz(), padre->der());
//         } else {
//             *padre = ab<entrada>(padre->izq(), padre->raiz(), lugar->der());
//         }
//         padre->rebalancearArbol();
//     } else {
//         arbol = lugar->der();
//     }
//     delete(lugar);
// } else {
//     ab<entrada>* reemplazo = lugar->der();
//     while (!(reemplazo->esNil())) {
//         reemplazo = &(reemplazo->izq);
//     }
//     ab<entrada>* padreReemplazo = reemplazo->raiz().padre;
//     padreReemplazo* = ab<entrada>(
//         reemplazo->der(), padreReemplazo->raiz(), padreReemplazo->der());
//     if (!(padreReemplazo->izq().esNil())) {
//         padreReemplazo->raiz().*padre = padreReemplazo;
//     }
//     lugar* = ab(lugar->izq(), reemplazo->raiz(), lugar->der());
//     *padre = lugar->raiz().padre;
//     padreReemplazo->rebalancearArbol();
//     delete(reemplazo);
// }

// template<class K, class S>
// Nat diccLog<K, S>::cantClaves() const {
//     return arbol.cantNodos();
// }

template<class K, class S>
ab<typename diccLog<K, S>::entrada> diccLog<K, S>::buscar(
    const K& k, ab<entrada>& padre) const {
    // std::cout << "Buscando a " << k << std::endl;
    padre = ab<entrada>();
    ab<entrada> actual = arbol;
    while (!(actual.esNil()) && actual.raiz().clave != k) {
        padre = actual;
        if (k <= actual.raiz().clave) {
            actual = actual.izq();
        } else {
            actual = actual.der();
        }
    }
    return actual;
}

template<class K, class S>
void diccLog<K, S>::recalcularAltura(ab<entrada>& a) {
    if (!(a.izq().esNil()) && !(a.der().esNil())) {
        a.raiz().altSubarbol = 1 + max(
            a.izq().raiz().altSubarbol, a.der().raiz().altSubarbol);
    } else if (!(a.izq().esNil())) {
        a.raiz().altSubarbol = 1 + a.izq().raiz().altSubarbol;
    } else if (!(a.der().esNil())) {
        a.raiz().altSubarbol = 1 + a.der().raiz().altSubarbol;
    } else {
        a.raiz().altSubarbol = 1;
    }
}

template<class K, class S>
int diccLog<K, S>::factorDeBalanceo(ab<entrada>& a) {
    int altIzq = a.izq().esNil() ? 0 : a.izq().raiz().altSubarbol;
    int altDer = a.der().esNil() ? 0 : a.der().raiz().altSubarbol;
    return altDer - altIzq;
}

template<class K, class S>
void diccLog<K, S>::rotarAIzquierda(ab<entrada>& a) {
//     ab<entrada>* nuevo1 = new ab<entrada>(
//         arbol.izq(), arbol.raiz(), arbol.der().izq());
//     ab<entrada>* nuevo2 = new ab<entrada>(
//         nuevo1, arbol.der().raiz(), arbol.der().der());
//     delete(arbol.der());
//     delete(arbol);
//     arbol = nuevo2;
//     arbol.raiz().padre = arbol.izq().raiz().padre;
//     arbol.izq().raiz().padre = arbol;
//     arbol.der().raiz().padre = arbol;
//     arbol.izq().izq().raiz().padre = arbol.izq();
//     arbol.izq().der().raiz().padre = arbol.izq();
//     arbol.izq()->recalcularAltura();
//     arbol->recalcularAltura();
}

template<class K, class S>
void diccLog<K, S>::rotarADerecha(ab<entrada>& a) {
//     ab<entrada>* nuevo1 = new ab<entrada>(
//         arbol.izq().der(), arbol.raiz(), arbol.der());
//     ab<entrada>* nuevo2 = new ab<entrada>(
//         arbol.izq().izq(), arbol.izq().raiz(), nuevo1);
//     delete(arbol.izq());
//     delete(arbol);
//     arbol = nuevo2;
//     arbol.raiz().padre = arbol.der().raiz().padre;
//     arbol.izq().raiz().padre = arbol;
//     arbol.der().raiz().padre = arbol;
//     arbol.izq().der().raiz().padre = arbol.der();
//     arbol.der().der().raiz().padre = arbol.der();
//     arbol.der()->recalcularAltura();
//     arbol->recalcularAltura();
}

template<class K, class S>
void diccLog<K, S>::rebalancearArbol(ab<entrada>& a) {
    std::cout << "Rebalanceando árbol" << std::endl;
    ab<entrada> p = a;
    while (!(p.esNil())) {
        recalcularAltura(p);
        int fdb1 = factorDeBalanceo(p);
        if (fdb1 == 2) {
            ab<entrada> q = p.der();
            int fdb2 = factorDeBalanceo(q);
            if (fdb2 == 1 || fdb2 == 0) {
                rotarAIzquierda(p);
            } else if (fdb2 == -1) {
                rotarADerecha(q);
                rotarAIzquierda(p);
            }
        } else if (fdb1 == -2) {
            ab<entrada> q = p.izq();
            int fdb2 = factorDeBalanceo(q);
            if (fdb2 == -1 || fdb2 == 0) {
                rotarADerecha(p);
            } else {
                rotarAIzquierda(q);
                rotarADerecha(p);
            }
        }
        p = p.raiz().padre;
    }
}

}  // namespace tp3

#endif  // DICC_LOG_H
