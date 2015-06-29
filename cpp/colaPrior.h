#ifndef COLA_PRIOR_H
#define COLA_PRIOR_H

#include "aed2.h"

using namespace aed2;

namespace tp3 {

template<class T>
class colaPrior{
    private:
        Vector<T> heap;

    public:
        colaPrior();
        
        void encolar(const T&);
        bool esVacia() const;
        T desencolar();
};

template<class T>
colaPrior<T>::colaPrior() : heap(Vector<T>()) {}

template<class T>
void colaPrior<T>::encolar(const T& a) {
    heap.AgregarAtras(a);
    Nat i = heap.Longitud();
    while (i != 0 && heap[i] < heap[i / 2 - (i + 1) % 2]) {
        heap.Swap(i, i / 2 - (i + 1) % 2);
        i = i / 2 - (i + 1) % 2;
    }
}

template<class T>
bool colaPrior<T>::esVacia() const {
    return heap.esVacio();
}

template<class T>
T colaPrior<T>::desencolar() {

}

}