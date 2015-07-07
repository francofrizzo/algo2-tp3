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
        ab<entrada>* padre;

        entrada(K k, S s, Nat h, ab<entrada>* p) :
            clave(k), significado(s), altSubarbol(h), padre(p) {}
    };

    ab<entrada>* arbol;

    ab<entrada>* buscar(const K& k, ab<entrada>*& padre) const;
    void recalcularAltura(ab<entrada>*);
    int factorDeBalanceo(ab<entrada>*) const;
    void rotarAIzquierda(ab<entrada>*&);
    void rotarADerecha(ab<entrada>*);
    void rebalancearArbol(ab<entrada>*);

    bool estaBalanceado(ab<entrada>* a) const;

 public:
    diccLog();
    diccLog(const diccLog<K, S>& otro);
    ~diccLog();

    bool definido(const K& k) const;
    void definir(const K& k, const S& s);
    S& obtener(const K& k) const;
    void borrar(const K& k);
    Nat cantClaves() const;

    bool estaBalanceado() const;
    Lista<K> clavesEnOrden() const;
};

template<class K, class S>
diccLog<K, S>::diccLog() :
    arbol(new ab<entrada>()) {}

template<class K, class S>
diccLog<K, S>::diccLog(const diccLog<K, S>& otro) :
    arbol(new ab<entrada>(*(otro.arbol))) {}

template<class K, class S>
diccLog<K, S>::~diccLog() {
    delete arbol;
}

template<class K, class S>
bool diccLog<K, S>::definido(const K& k) const {
    ab<entrada>* padre;
    ab<entrada>* lugar = buscar(k, padre);
    return !(lugar->esNil());
}

template<class K, class S>
void diccLog<K, S>::definir(const K& k, const S& s) {
    ab<entrada>* padre;
    ab<entrada>* lugar = buscar(k, padre);
    if (!(lugar->esNil())) {
        // La clave ya estaba definida, reemplazo el significado por el nuevo
        lugar->raiz().significado = S(s);
    } else {
        if (padre == NULL) {
            // El árbol estaba vacío, agrego la nueva clave como árbol principal
            delete arbol;
            arbol = new ab<entrada>(new ab<entrada>(),
                entrada(k, s, 1, NULL), new ab<entrada>());
            // En este caso no hace falta rebalancear
        } else {
            // El árbol tenía nodos. Creo el nuevo nodo a insertar.
            ab<entrada>* nuevo = new ab<entrada>(new ab<entrada>(),
                entrada(k, s, 1, padre), new ab<entrada>());
            if (k < padre->raiz().clave) {
                delete padre->izq();
                padre->izq(nuevo);
            } else {
                delete padre->der();
                padre->der(nuevo);
            }
            // Ahora sí hay que rebalancear
            rebalancearArbol(padre);
        }
    }
}

template<class K, class S>
S& diccLog<K, S>::obtener(const K& k) const {
    assert(definido(k));
    ab<entrada>* padre;
    ab<entrada>* lugar = buscar(k, padre);
    return lugar->raiz().significado;
}

template<class K, class S>
void diccLog<K, S>::borrar(const K& k) {
    assert(definido(k));
    ab<entrada>* padre;
    ab<entrada>* lugar = buscar(k, padre);
    if ((lugar->izq() == NULL || lugar->izq()->esNil()) &&
        (lugar->der() == NULL || lugar->der()->esNil())) {
        // El nodo no tiene hijos. Puedo borrarlo directamente
        if (padre == NULL) {
            // Era el único nodo en el árbol. Ahora quedó vacío.
            arbol = new ab<entrada>();
        } else if (padre->izq() == lugar) {
            // Era hijo izquierdo de su padre
            padre->izq(new ab<entrada>());
        } else {
            // Era hijo derecho de su padre
            padre->der(new ab<entrada>());
        }
        delete lugar;
        rebalancearArbol(padre);
    } else if (lugar->der() == NULL || lugar->der()->esNil()) {
        // El nodo tiene solo hijo izquierdo
        lugar->izq()->raiz().padre = padre;
        if (padre == NULL) {
            // Era la raíz del árbol. Ahora lo es su hijo izquierdo
            arbol = lugar->izq();
        } else if (padre->izq() == lugar) {
            // Era hijo izquierdo de su padre
            padre->izq(lugar->izq());
        } else {
            // Era hijo derecho de su padre
            padre->der(lugar->izq());
        }
        lugar->izq(NULL);
        delete lugar;
        rebalancearArbol(padre);
    } else if (lugar->izq() == NULL || lugar->izq()->esNil()) {
        // El nodo tiene solo hijo derecho
        lugar->der()->raiz().padre = padre;
        if (padre == NULL) {
            // Era la raíz del árbol. Ahora lo es su hijo derecho
            arbol = lugar->der();
        } else if (padre->izq() == lugar) {
            // Era hijo izquierdo de su padre
            padre->izq(lugar->der());
        } else {
            // Era hijo derecho de su padre
            padre->der(lugar->der());
        }
        lugar->der(NULL);
        delete lugar;
        rebalancearArbol(padre);
    } else {
        // El nodo tiene ambos hijos
        ab<entrada>* reemplazo = lugar->der();
        if (reemplazo->izq() == NULL || reemplazo->izq()->esNil()) {
            // Caso en el que el reemplazo es hijo directo del nodo a borrar
            // Cuelgo al reemplazo de donde corresponde
            if (padre == NULL) {
                // Era la raíz del árbol. Ahora lo es el reemplazo
                arbol = reemplazo;
            } else if (padre->izq() == lugar) {
                // Era hijo izquierdo de su padre
                padre->izq(reemplazo);
            } else {
                // Era hijo derecho de su padre
                padre->der(reemplazo);
            }
            // Cuelgo del reemplazo al hijo izquierdo del nodo a reemplazar
            delete reemplazo->izq();
            reemplazo->izq(lugar->izq());
            if (reemplazo->izq() != NULL && !(reemplazo->izq()->esNil())) {
                reemplazo->izq()->raiz().padre = reemplazo;
            }
            // Borro el nodo reemplazado
            lugar->izq(NULL);
            lugar->der(NULL);
            delete lugar;
            rebalancearArbol(reemplazo);
        } else {
            // Caso en el que el reemplazo NO es hijo directo del nodo a reemplazar
            // Bajo hasta encontrar el mínimo del subárbol derecho
            while (reemplazo->izq() != NULL && !(reemplazo->izq()->esNil())) {
                reemplazo = reemplazo->izq();
            }
            ab<entrada>* padreReemplazo = reemplazo->raiz().padre;
            // Arreglo link entre padre del reemplazo y su subárbol derecho
            padreReemplazo->izq(reemplazo->der());
            if (padreReemplazo->izq() != NULL && !(padreReemplazo->izq()->esNil())) {
                padreReemplazo->izq()->raiz().padre = padreReemplazo;
            }
            // Cuelgo al reemplazo de donde corresponde
            reemplazo->raiz().padre = padre;
            if (padre == NULL) {
                // Era la raíz del árbol. Ahora lo es el reemplazo
                arbol = reemplazo;
            } else if (padre->izq() == lugar) {
                // Era hijo izquierdo de su padre
                padre->izq(reemplazo);
            } else {
                // Era hijo derecho de su padre
                padre->der(reemplazo);
            }
            // Le cuelgo al reemplazo los hijos del nodo a reemplazar
            delete reemplazo->izq();
            reemplazo->izq(lugar->izq());
            if (reemplazo->izq() != NULL && !(reemplazo->izq()->esNil())) {
                reemplazo->izq()->raiz().padre = reemplazo;
            }
            reemplazo->der(lugar->der());
            if (reemplazo->der() != NULL && !(reemplazo->der()->esNil())) {
                reemplazo->der()->raiz().padre = reemplazo;
            }
            // Borro el nodo reemplazado
            lugar->izq(NULL);
            lugar->der(NULL);
            delete lugar;
            rebalancearArbol(padreReemplazo);
        }
    }
}

template<class K, class S>
Nat diccLog<K, S>::cantClaves() const {
    return arbol->cantNodos();
}

template<class K, class S>
ab<typename diccLog<K, S>::entrada>* diccLog<K, S>::buscar(
    const K& k, ab<entrada>*& padre) const {
    padre = NULL;
    ab<entrada>* actual = arbol;
    while (!(actual->esNil()) && actual->raiz().clave != k) {
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
void diccLog<K, S>::recalcularAltura(ab<entrada>* a) {
    assert(a != NULL && !(a->esNil()));
    cout << "Recalculando altura de nodo con raíz " << a->raiz().clave << endl;
    if (!(a->izq()->esNil()) && !(a->der()->esNil())) {
        a->raiz().altSubarbol = 1 + max(
            a->izq()->raiz().altSubarbol, a->der()->raiz().altSubarbol);
        cout << "Ambos hijos no nulos.";
    } else if (!(a->izq()->esNil())) {
        a->raiz().altSubarbol = 1 + a->izq()->raiz().altSubarbol;
        cout << "Hijo derecho nulo.";
    } else if (!(a->der()->esNil())) {
        a->raiz().altSubarbol = 1 + a->der()->raiz().altSubarbol;
        cout << "Hijo izquierdo nulo.";
    } else {
        a->raiz().altSubarbol = 1;
        cout << "Ambos hijos nulos.";
    }
    cout << " Altura calculada: " << a->raiz().altSubarbol << endl << endl;
}

template<class K, class S>
int diccLog<K, S>::factorDeBalanceo(ab<entrada>* a) const {
    assert(a != NULL);
    int altIzq = a->izq()->esNil() ? 0 : a->izq()->raiz().altSubarbol;
    int altDer = a->der()->esNil() ? 0 : a->der()->raiz().altSubarbol;
    return altDer - altIzq;
}

template<class K, class S>
void diccLog<K, S>::rotarAIzquierda(ab<entrada>*& a) {
    assert(!(a->esNil()));
    assert(a->der() != NULL && !(a->der()->esNil()));
    cout << "ROTACIÓN A IZQUIERDA. Baja nodo con clave " << a->raiz().clave << endl;
    cout << "Sube nodo con clave " << a->der()->raiz().clave << endl;
    // ab<entrada>* b = a->der();
    // a->der(b->izq());
    // b->izq(a);
    // b->raiz().padre = a->raiz().padre;
    // a->raiz().padre = b;
    // if (a->der() != NULL && !(a->der()->esNil())) {
    //     a->der()->raiz().padre = a;
    // }
    // if (a->izq() != NULL && !(a->der()->esNil())) {
    //     a->izq()->raiz().padre = a;
    // }
    // recalcularAltura(a);
    // recalcularAltura(b);
    // a = b;
}

template<class K, class S>
void diccLog<K, S>::rotarADerecha(ab<entrada>* a) {
    assert(!(a->esNil()));
    assert(a->izq() != NULL && !(a->izq()->esNil()));
    cout << "ROTACIÓN A DERECHA. Baja nodo con clave " << a->raiz().clave << endl;
    cout << "Sube nodo con clave " << a->izq()->raiz().clave << endl;
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
void diccLog<K, S>::rebalancearArbol(ab<entrada>* a) {
    std::cout << "Rebalanceando árbol" << std::endl;
    ab<entrada>* p = a;
    while (p != NULL) {
        recalcularAltura(p);
        int fdb1 = factorDeBalanceo(p);
        if (fdb1 == 2) {
            ab<entrada>* q = p->der();
            int fdb2 = factorDeBalanceo(q);
            if (fdb2 == 1 || fdb2 == 0) {
                rotarAIzquierda(p);
            } else if (fdb2 == -1) {
                rotarADerecha(q);
                rotarAIzquierda(p);
            }
        } else if (fdb1 == -2) {
            ab<entrada>* q = p->izq();
            int fdb2 = factorDeBalanceo(q);
            if (fdb2 == -1 || fdb2 == 0) {
                rotarADerecha(p);
            } else {
                rotarAIzquierda(q);
                rotarADerecha(p);
            }
        }
        p = p->raiz().padre;
    }
}

template<class K, class S>
bool diccLog<K, S>::estaBalanceado() const {
    return estaBalanceado(arbol);
}

template<class K, class S>
bool diccLog<K, S>::estaBalanceado(ab<entrada>* a) const {
    if (a == NULL || a->esNil()) {
        cout << "Visitando nodo vacío (Balanceado)." << endl;
        return true;
    } else if (factorDeBalanceo(a) == -1 ||
        factorDeBalanceo(a) == 0 ||
        factorDeBalanceo(a) == 1) {
        cout << "Visitando nodo con clave "<< a->raiz().clave <<". Factor de balanceo: "
            << factorDeBalanceo(a) << " (está balanceado)." << endl;
        cout << "Altura de subárbol: " << a->raiz().altSubarbol << endl;
        return estaBalanceado(a->izq()) && estaBalanceado(a->der());
    } else {
        return false;
        cout << "Visitando nodo con clave "<< a->raiz().clave <<". Factor de balanceo: "
            << factorDeBalanceo(a) << " (no está balanceado)." << endl;
        cout << "Altura de subárbol: " << a->raiz().altSubarbol << endl;
    }
}

template<class K, class S>
Lista<K> diccLog<K, S>::clavesEnOrden() const {
    Lista<entrada> entradas = arbol->preorder();
    Lista<K> res;
    for (typename Lista<entrada>::Iterador it = entradas.CrearIt(); it.HaySiguiente(); it.Avanzar()) {
        res.AgregarAtras(it.Siguiente().clave);
    }
    return res;
}

}  // namespace tp3

#endif  // DICC_LOG_H
