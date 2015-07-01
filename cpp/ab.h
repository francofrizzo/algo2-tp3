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
    bool _esNil;
    T valor;
    ab<T>* _izq;
    ab<T>* _der;

 public:
    ab();  // Constructor vacío (Nil)
    ab(const ab<T>&);  // Constructor por copia

    // Variantes del constructor Bin:
    ab(const T&);
    ab(ab<T>&, const T&);
    ab(const T&, ab<T>&);
    ab(ab<T>&, const T&, ab<T>&);

    ~ab();

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
ab<T>::ab(const T& r) {
    ab<T>* i = new ab<T>();
    ab<T>* d = new ab<T>();
    _raiz = new subarbol(*i, r, *d);
}

template<class T>
ab<T>::ab(ab<T>& i, const T& r) {
    ab<T>* d = new ab<T>();
    _raiz = new subarbol(i, r, *d);
}

template<class T>
ab<T>::ab(const T& r, ab<T>& d) {
    ab<T>* i = new ab<T>();
    _raiz = new subarbol(*i, r, d);
}

template<class T>
ab<T>::ab(ab<T>& i, const T& r, ab<T>& d) {
    _raiz = new subarbol(i, r, d);
}

template<class T>
ab<T>::~ab() {
    delete _raiz;
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
    return *(_raiz->izq);
}

template<class T>
ab<T>& ab<T>::der() const {
    assert(!esNil());
    return *(_raiz->der);
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
