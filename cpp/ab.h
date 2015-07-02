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
            _der(d) {}

        nodo(const nodo& otro) :
            valor(otro.valor) {
            if (otro._izq != NULL) {
                _izq = new ab<T>(*(otro._izq));
            } else {
                _izq = NULL;
            }
            if (otro._der != NULL) {
                _der = new ab<T>(*(otro._der));
            } else {
                _der = NULL;
            }
        }

        ~nodo() {
            delete _izq;
            delete _der;
        }
    };

    nodo* _raiz;

 public:
    ab();  // Constructor vacío (Nil)
    ab(const ab<T>&);  // Constructor por copia
    ab(ab<T>*, const T&, ab<T>*);  // Constructor Bin

    ~ab();

    ab<T>& operator=(const ab<T>&);  // Operador de asignación por copia

    bool esNil() const;  // Devuelve True si y solo si el árbol no contiene nodos
    T& raiz() const;  // Devuelve el valor de la raíz del árbol
    ab<T>* izq() const;  // Devuelve el subárbol izquierdo
    ab<T>* der() const;  // Devuelve el subárbol derecho
    Nat altura() const;  // Devuelve la altura del árbol
    Nat cantNodos() const;  // Devuelve la cantidad de nodos del árbol
};

// Implementación de métodos públicos

template<class T>
ab<T>::ab() : _raiz(NULL) {}

template<class T>
ab<T>::ab(const ab<T>& otro) {
    *this = otro;
}

template<class T>
ab<T>::ab(ab<T>* i, const T& r, ab<T>* d) {
    _raiz = new nodo(i, r, d);
}

template<class T>
ab<T>::~ab() {
    // delete _raiz;
}

template<class T>
ab<T>& ab<T>::operator=(const ab<T>& otro) {
    delete _raiz;
    _raiz = NULL;
    if (otro._raiz != NULL) {
        ab<T>* izquierda, * derecha;
        if (otro._raiz->_izq != NULL) {
            izquierda = new ab<T>(*(otro._raiz->_izq));
        }
        if (otro._raiz->_der != NULL) {
            derecha = new ab<T>(*(otro._raiz->_der));
        }
        _raiz = new nodo(izquierda, otro._raiz->valor, derecha);
    } else {
        _raiz = NULL;
    }
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
ab<T>* ab<T>::izq() const {
    assert(!esNil());
    return _raiz->_izq;
}

template<class T>
ab<T>* ab<T>::der() const {
    assert(!esNil());
    return _raiz->_der;
}

template<class T>
Nat ab<T>::altura() const {
    if (esNil()) {
        return 0;
    } else {
        return 1 + max(izq()->altura(), der()->altura());
    }
}

template<class T>
Nat ab<T>::cantNodos() const {
    if (esNil()) {
        return 0;
    } else {
        return 1 + izq()->cantNodos() + der()->cantNodos();
    }
}

}  // namespace tp3

#endif  // ARBOL_BIN_H
