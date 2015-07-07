#ifndef ARBOL_BIN_H
#define ARBOL_BIN_H

#include <algorithm>
#include <iostream>
#include "./aed2.h"

using std::cerr;
using std::endl;
using std::max;
using namespace aed2;

namespace tp3 {

template<class T>
class ab {
 private:
    struct nodo {
        T valor;
        ab<T>* _izq;
        ab<T>* _der;

        nodo(ab<T>* i, T v, ab<T>* d) :
            valor(v),
            _izq(i),
            _der(d) {
        }

        nodo(const nodo& otro) :
            valor(otro.valor) {
            _izq = otro._izq == NULL ? NULL : new ab<T>(*(otro._izq));
            _der = otro._der == NULL ? NULL : new ab<T>(*(otro._der));
        }

        ~nodo() {
            delete _izq;
            delete _der;
        }
    };

    nodo* _raiz;

 public:
    ab();                          // Constructor vacío (Nil)
    ab(const ab<T>&);              // Constructor por copia
    ab(ab<T>*, const T&, ab<T>*);  // Constructor Bin

    ~ab();

    ab<T>& operator=(const ab<T>&);  // Operador de asignación por copia

    bool esNil() const;       // Devuelve True si y solo si el árbol
                              //     no contiene nodos
    T& raiz() const;          // Devuelve el valor de la raíz del árbol
    ab<T>* constIzq() const;  // Devuelve el subárbol izquierdo
                              //     (no modifica la estructura interna)
    ab<T>* constDer() const;  // Devuelve el subárbol derecho
                              //     (no modifica la estructura interna)
    ab<T>* izq();             // Devuelve el subárbol izquierdo
    ab<T>* der();             // Devuelve el subárbol derecho
    void izq(ab<T>*);         // Reemplaza el subárbol izquierdo (NO libera memoria)
    void der(ab<T>*);         // Reemplaza el subárbol derecho (NO libera memoria)
    Nat altura() const;       // Devuelve la altura del árbol
    Nat cantNodos() const;    // Devuelve la cantidad de nodos del árbol
};

// Implementación de métodos públicos

template<class T>
ab<T>::ab() : _raiz(NULL) {
}

template<class T>
ab<T>::ab(const ab<T>& otro) {
    _raiz = otro._raiz == NULL ? NULL : new nodo(*(otro._raiz));
}

template<class T>
ab<T>::ab(ab<T>* i, const T& r, ab<T>* d) {
    _raiz = new nodo(i, r, d);
}

template<class T>
ab<T>::~ab() {
    delete _raiz;
}

template<class T>
ab<T>& ab<T>::operator=(const ab<T>& otro) {
    delete _raiz;
    _raiz = otro._raiz == NULL ? NULL : new nodo(*(otro._raiz));
    return *this;
}

template<class T>
bool ab<T>::esNil() const {
    return _raiz == NULL;
}

template<class T>
T& ab<T>::raiz() const {
    assert(!esNil());
    return _raiz->valor;
}

template<class T>
ab<T>* ab<T>::constIzq() const {
    assert(!esNil());
    return _raiz->_izq;
}

template<class T>
ab<T>* ab<T>::constDer() const {
    assert(!esNil());
    return _raiz->_der;
}

template<class T>
ab<T>* ab<T>::izq() {
    assert(!esNil());
    return _raiz->_izq;
}

template<class T>
ab<T>* ab<T>::der() {
    assert(!esNil());
    return _raiz->_der;
}

template<class T>
void ab<T>::izq(ab<T>* i) {
    assert(!esNil());
    // delete _raiz->_izq;
    _raiz->_izq = i;
}

template<class T>
void ab<T>::der(ab<T>* d) {
    assert(!esNil());
    // delete _raiz->_der;
    _raiz->_der = d;
}

template<class T>
Nat ab<T>::altura() const {
    if (esNil()) {
        return 0;
    } else {
        return 1 + max(constIzq()->altura(), constDer()->altura());
    }
}

template<class T>
Nat ab<T>::cantNodos() const {
    if (esNil()) {
        return 0;
    } else {
        return 1 + constIzq()->cantNodos() + constDer()->cantNodos();
    }
}

}  // namespace tp3

#endif  // ARBOL_BIN_H
