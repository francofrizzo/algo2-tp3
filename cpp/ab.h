#ifndef ARBOL_BIN_H
#define ARBOL_BIN_H

#include <algorithm>
#include "./aed2.h"

using std::max;
using namespace aed2;

namespace tp3 {

template<class T>
class ab {
 private:
    struct subarbol {
        T valor;
        ab<T> izq;
        ab<T> der;

        subarbol(const ab<T>& i, const T& v, const ab<T>& d) :
            izq(i), valor(v), der(d) {}
    };

    subarbol* _raiz;

 public:
    ab();  // Constructor vacío (Nil)
    ab(const ab<T>&);  // Constructor por copia
    ab(const ab<T>&, const T&, const ab<T>&);  // Constructor Bin

    ab<T>& operator=(const ab<T>&); // Operador de asignación

    bool esNil() const;  // Devuelve True si y solo si el árbol no contiene nodos
    T& raiz() const;  // Devuelve el valor de la raíz del árbol
    ab<T>& izq() const;  // Devuelve el subárbol izquierdo
    ab<T>& der() const;  // Devuelve el subárbol derecho
    Nat altura() const;  // Devuelve la altura del árbol
    Nat cantNodos() const;  // Devuelve la cantidad de nodos del árbol
};

// Implementación de métodos públicos

template<class T>
ab<T>::ab() : _raiz(NULL) {}

template<class T>
ab<T>::ab(const ab<T>& otro) : _raiz(otro._raiz) {}

template<class T>
ab<T>::ab(const ab<T>& i, const T& r, const ab<T>& d) {
    _raiz = new subarbol(i, r, d);
}

template<class T>
ab<T>& ab<T>::operator=(const ab<T>& otro) {
    _raiz = otro._raiz;
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
ab<T>& ab<T>::izq() const {
    assert(!esNil());
    return _raiz->izq;
}

template<class T>
ab<T>& ab<T>::der() const {
    assert(!esNil());
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
        return 1 + izq().cantNodos() + der().cantNodos();
    }
}

}  // namespace tp3

#endif  // ARBOL_BIN_H
