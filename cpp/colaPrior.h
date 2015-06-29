#ifndef COLA_PRIOR_H
#define COLA_PRIOR_H

#include "aed2.h"

using namespace aed2;

namespace tp3 {

template<class T>
class colaPrior{
    private:
        Vector<T> heap;

        void swap(Nat i, Nat j);

    public:
        colaPrior();
        
        void encolar(const &T a);
        bool esVacia() const;
        T desencolar();
};

template<class T>
colaPrior<T>::colaPrior() {
    heap = Vector<T>;
};

}