#ifndef DICC_LOG_H
#define DICC_LOG_H

#include "aed2.h"
#include "ab.h"

using namespace aed2;

namespace tp3 {

template<class K, class S>
class diccLog{
    private:
        struct entrada {
            entrada(K k, S s, Nat alt, ab<entrada>* p) : clave(k), significado(s), altSubarbol(alt), padre(p) {};
            K clave;
            S significado;
            Nat altSubarbol;
            ab<entrada>* padre;
        };

        ab<entrada> arbol;

        ab<entrada>* buscar(const K& k, ab<entrada>* padre);
        void recalcularAltura();
        int factorDeBalanceo();
        void rotarAIzquierda();
        void rotarADerecha();
        void rebalancearArbol();

    public:
        diccLog();
        diccLog(const diccLog<K,S>& otro);

        bool definido(const K& k) const;
        void definir(const K& k, const S& s);
        S& obtener(const K& k) const;
        void borrar(const K& k);
        Nat cantClaves() const;
};

template<class K, class S>
diccLog<K,S>::diccLog() : arbol(ab<entrada>()) {}

template<class K, class S>
diccLog<K,S>::diccLog(const diccLog<K,S>& otro) {
    arbol = ab<entrada>(otro.arbol);
}

template<class K, class S>
bool diccLog<K,S>::definido(const K& k) const {
    ab<entrada>* padre;
    ab<entrada>* lugar = buscar(k, padre);
    return lugar->esNil();
}

template<class K, class S>
void diccLog<K,S>::definir(const K& k, const S& s) {
    ab<entrada>* padre;
    ab<entrada>* lugar = buscar(k, padre);
    if (!(lugar->esNil())) {
        lugar->raiz().significado = S(s);
    } else {
        entrada nuevaEntrada = entrada(k, s, 1, padre);
        ab<entrada>* nuevo = new ab<entrada>(ab<entrada>(), nuevaEntrada, ab<entrada>());
        if (k <= padre->raiz().clave) {
            padre* = ab<entrada>(nuevo, padre.raiz(), padre.der());
        } else {
            padre* = ab<entrada>(padre.izq(), padre.raiz(), nuevo);
        }
        padre->rebalancearArbol();
    }
}

template<class K, class S>
S& diccLog<K,S>::obtener(const K& k) const {
    ab<entrada>* padre;
    ab<entrada>* lugar = buscar(k, padre);
    return lugar->raiz().significado;
}

template<class K, class S>
void diccLog<K,S>::borrar(const K& k) {
    ab<entrada>* padre;
    ab<entrada>* lugar = buscar(k, padre);
    if (lugar->izq().esNil() && lugar->der().esNil()) {
        if (padre->izq() = lugar*) {
            padre* = ab<entrada>(ab<entrada>(), padre->raiz(), padre->der());
        } else {
            padre* = ab<entrada>(padre->izq(), padre->raiz(), ab<entrada>());
        }
        padre->rebalancearArbol();
    } else {
        arbol = ab<entrada>();
    }
    delete(lugar);
} else if (lugar->der.esNil()) {
    lugar->izq().raiz().padre* = padre;
    if (!(padre->esNil())) {
        if (padre->izq() = lugar) {
            padre* = ab<entrada>(lugar->izq(), padre->raiz(), padre->der());
        } else {
            padre* = ab<entrada>(padre->izq(), padre->raiz(), lugar->izq());
        }
        padre->rebalancearArbol();
    } else {
        arbol = lugar->izq();
    }
    delete(lugar);
} else if (lugar->izq.esNil()) {
    lugar->der().raiz().padre* = padre;
    if (!(padre->esNil())) {
        if (padre->izq() = lugar) {
            padre* = ab<entrada>(lugar->der(), padre->raiz(), padre->der());
        } else {
            padre* = ab<entrada>(padre->izq(), padre->raiz(), lugar->der());
        }
        padre->rebalancearArbol();
    } else {
        arbol = lugar->der();
    }
    delete(lugar);
} else {
    ab<entrada>* reemplazo = lugar->der();
    while (!(reemplazo->esNil())) {
        reemplazo = &(reemplazo->izq);
    }
    ab<entrada>* padreReemplazo = reemplazo->raiz().padre;
    padreReemplazo* = ab<entrada>(reemplazo->der(), padreReemplazo->raiz(), padreReemplazo->der());
    if (!(padreReemplazo->izq().esNil())) {
        padreReemplazo->raiz().padre* = padreReemplazo;
    }
    lugar* = ab(lugar->izq(), reemplazo->raiz(), lugar->der());
    padre* = lugar->raiz().padre;
    padreReemplazo->rebalancearArbol();
    delete(reemplazo);
}

template<class K, class S>
Nat diccLog<K,S>::cantClaves() const {
    return arbol.cantNodos();
}

template<class K, class S>
diccLog<K,S>::ab<entrada>* diccLog<K,S>::buscar(const K& k, ab<entrada>* padre) {
    padre = new ab<entrada>();
    ab<entrada>* actual = &arbol;
    while (!(actual.esNil() && actual->raiz().clave != k)) {
        padre = actual;
        if (k <= actual->raiz().clave) {
            actual = actual->izq();
        } else {
            actual = actual->der();
        }
    }
    return actual;
}

template<class K, class S>
void diccLog<K,S>::recalcularAltura() {
    if (!(arbol.izq().esNil()) && !(arbol.der().esNil())) {
        arbol.raiz().altSubarbol = 1 + max(arbol.raiz().izq().altSubarbol, arbol.raiz().der().altSubarbol);
    } else (!(arbol.izq().esNil())) {
        arbol.raiz().altSubarbol = 1 + arbol.raiz().izq().altSubarbol;
    } else (!(arbol.der().esNil())) {
        arbol.raiz().altSubarbol = 1 + arbol.raiz().der().altSubarbol;
    } else {
        arbol.raiz().altSubarbol = 1;
    }
}

template<class K, class S>
int diccLog<K,S>::factorDeBalanceo() {
    int altIzq = arbol.izq().esNil() ? 0 : arbol.izq().raiz().altSubarbol;
    int altDer = arbol.der().esNil() ? 0 : arbol.der().raiz().altSubarbol;
    return altDer - altIzq;
}

template<class K, class S>
void diccLog<K,S>::rotarAIzquierda() {
    ab<entrada>* nuevo1 = new ab<entrada>(arbol.izq(), arbol.raiz(), arbol.der().izq());
    ab<entrada>* nuevo2 = new ab<entrada>(nuevo1, arbol.der().raiz(), arbol.der().der());
    delete(arbol.der());
    delete(arbol);
    arbol = nuevo2;
    arbol.raiz().padre = arbol.izq().raiz().padre;
    arbol.izq().raiz().padre = arbol;
    arbol.der().raiz().padre = arbol;
    arbol.izq().izq().raiz().padre = arbol.izq();
    arbol.izq().der().raiz().padre = arbol.izq();
    arbol.izq()->recalcularAltura();
    arbol->recalcularAltura();
}

template<class K, class S>
void diccLog<K,S>::rotarADerecha() {
    ab<entrada>* nuevo1 = new ab<entrada>(arbol.izq().der(), arbol.raiz(), arbol.der());
    ab<entrada>* nuevo2 = new ab<entrada>(arbol.izq().izq(), arbol.izq().raiz(), nuevo1);
    delete(arbol.izq());
    delete(arbol);
    arbol = nuevo2;
    arbol.raiz().padre = arbol.der().raiz().padre;
    arbol.izq().raiz().padre = arbol;
    arbol.der().raiz().padre = arbol;
    arbol.izq().der().raiz().padre = arbol.der();
    arbol.der().der().raiz().padre = arbol.der();
    arbol.der()->recalcularAltura();
    arbol->recalcularAltura();
}

template<class K, class S>
void diccLog<K,S>::rebalancearArbol() {
    ab<entrada>* p = arbol;
    while(!(p->esNil())) {
        p->recalcularAltura();
        int fdb1 = p->factorDeBalanceo();
        if (fdb1 == 2) {
            ab<entrada>* q = p->der();
            int fdb2 = q->factorDeBalanceo();
            if (fdb2 == 1 || fdb2 == 0) {
                p->rotarAIzquierda();
            } else if (fdb2 = -1) {
                q->rotarADerecha();
                p->rotarAIzquierda();
            }
        } else if (fdb1 == -2) {
            ab<entrada>* q = p->izq();
            int fdb2 = q->factorDeBalanceo();
            if (fdb2 == -1 || fdb2 == 0) {
                p->rotarADerecha();
            } else {
                q->rotarAIzquierda();
                p->rotarADerecha();
            }
        }
        p = p->raiz().padre;
    }
}

}

#endif