#ifndef COLA_PRIOR_H
#define COLA_PRIOR_H

#include "./aed2.h"

using namespace aed2;

namespace tp3 {

template<class T>
class colaPrior {
 private:
    Vector<T> heap;

    // Función de debug que verifica
    // que se cumpla el invariante de representación
    bool esHeapValido() {
        for (int i = 0; 2*i + 1 < heap.Longitud(); i++) {
            if (heap[i] < heap[2*i + 1]) {
                return false;
            } else if (2*i + 2 < heap.Longitud() && heap[i] < heap[2*i + 2]) {
                return false;
            }
        }
        return true;
    }

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
    Nat i = heap.Longitud() - 1;
    while (i != 0 && heap[(i + 1)/2 - 1] < heap[i]) {
        heap.Swap(i, (i + 1)/2 - 1);
        i = (i + 1)/2 - 1;
    }
    // assert(esHeapValido());  // debug
}

template<class T>
bool colaPrior<T>::esVacia() const {
    return heap.EsVacio();
}

template<class T>
T colaPrior<T>::desencolar() {
    assert(!esVacia());
    T res = T(heap[0]);
    heap.Swap(0, heap.Longitud() - 1);
    heap.Comienzo();
    Nat i = 0;
    while ((2*i + 1 < heap.Longitud() &&
        heap[i] < heap[2*i + 1]) ||
        (2*i + 2 < heap.Longitud() &&
        heap[i] < heap[2*i + 2])) {
        if (2*i + 2 < heap.Longitud()) {
            if (heap[2*i + 1] < heap[2*i + 2]) {
                heap.Swap(i, 2*i + 2);
                i = 2*i + 2;
            } else {
                heap.Swap(i, 2*i + 1);
                i = 2*i + 1;
            }
        } else {
            heap.Swap(i, 2*i + 1);
            i = 2*i + 1;
        }
    }
    // assert(esHeapValido());  // debug
    return res;
}

}  // namespace tp3

#endif  // COLA_PRIOR_H
