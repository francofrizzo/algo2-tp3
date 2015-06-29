#ifndef RED_H
#define RED_H

#include "aed2.h"
#include "dicc_trie.h"

using namespace aed2;

namespace tp3 {

typedef aed2::String ip;
typedef aed2::Nat interfaz;
struct compu {
    ip IP;
    Conj<interfaz> interfaces;
};

class red {
private:
    typedef Dicc<interfaz, ip> diccConexiones;
    
    Conj<compu> compus;
    DiccString<diccConexiones> conexiones;

    Conj<Lista<compu> > dameMinimos(const Conj<Lista<compu> > c) const;
    Conj<Lista<compu> > losDeLong(const Conj<Lista<compu> > c, Nat k) const;
    Nat minimaLong(const Conj<Lista<compu> > c) const;
    Lista<compu> pasarConjASecu(const Conj<compu> c) const;
    Conj<Lista<compu> > caminos(const compu c1, const compu c2, Lista<compu> l, Lista<compu> cantidatos) const;

public:
    red();
    red(const red& r);

    void agregarCompu(const compu c);
    void conectar(const compu c1, const interfaz i1, const compu c2, const interfaz i2);
    bool conectadas(const compu c1, const compu c2) const;
    interfaz interfazUsada(const compu c1, const compu c2) const;
    Conj<compu> vecinos(const compu c) const;
    bool usaInterfaz(const compu c, const interfaz i) const;
    Conj<Lista<compu> > caminosMinimos(const compu c1, const compu c2) const;
    bool hayCamino(const compu c1, const compu c2) const;
    Conj<compu> computadoras() const;
    Nat cantCompus() const;

};

}

#endif