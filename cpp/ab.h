#ifndef ARBOL_BIN_H
#define ARBOL_BIN_H

#include "aed2.h"

using namespace aed2;

namespace tp3 {

template<class T>
class ab {
    private:
        struct nodo {
            T* valor;
            ab<T>* izq;
            ab<T>* der;
        };

        nodo* _raiz;

        // ab(nodo*);

    public:
        ab();
        ab(const ab<T>&);
        ab(const ab<T>&, T&, const ab<T>&);

        bool esNil() const;
        T& raiz() const;
        ab<T>& izq() const;
        ab<T>& der() const;
        Nat altura() const;
        Nat cantNodos() const;
        // Lista<T> inorder();
        // Lista<T> preorder();
        // Lista<T> postorder();
};

// Implementación del árbol binario

// template<class T>
// ab<T>::ab(nodo* n) {
//     _raiz = n;
// }

template<class T>
ab<T>::ab() : _raiz() {}

template<class T>
ab<T>::ab(const ab<T>& otro) {
    _raiz = otro._raiz;
}

template<class T>
ab<T>::ab(const ab<T>& i, T& r, const ab<T>& d) {
    _raiz = new nodo();
    _raiz->valor = &r;
    _raiz->izq = i._raiz;
    _raiz->der = d._raiz;
}

template<class T>
bool ab<T>::esNil() const {
    return _raiz == NULL;
}

template<class T>
T& ab<T>::raiz() const {
    return _raiz->valor;
}

template<class T>
ab<T>& ab<T>::izq() const {
    return _raiz->izq;
}

template<class T>
ab<T>& ab<T>::der() const {
    return _raiz->der;
}

template<class T>
Nat ab<T>::altura() const {
    if (esNil()) {
        return 0;
    } else {
        return 1 + max(izq().altura(), der().altura());
    }
}

template<class T>
Nat ab<T>::cantNodos() const {
    if (esNil()) {
        return 0;
    } else {
        return 1 + izq().CantNodos() + der().CantNodos();
    }
}

}

#endif