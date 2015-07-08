#ifndef RED_H
#define RED_H

#include "./aed2.h"
#include "./dicc_trie.h"

using namespace aed2;

namespace tp3 {

typedef String ip;
typedef Nat interfaz;
struct compu {
    ip IP;
    Conj<interfaz> interfaces;
    bool operator==(const compu&) const;
    bool operator!=(const compu&) const;
};

std::ostream& operator<<(std::ostream&, const compu&);  // DEBUG

class red {
 private:
    typedef Dicc<interfaz, ip> diccConexiones;

    Conj<compu> compus;
    DiccString<diccConexiones> conexiones;

    Conj<Lista<compu> > dameMinimos(const Conj<Lista<compu> >&) const;
    Conj<Lista<compu> > losDeLong(const Conj<Lista<compu> >&, Nat) const;
    Nat minimaLong(const Conj<Lista<compu> >&) const;
    Lista<compu> pasarConjALista(const Conj<compu>&) const;
    Conj<Lista<compu> > caminos(const compu&, const compu&,
        Lista<compu>&, Lista<compu>&) const;

 public:
    red();
    red(const red& r);

    void agregarCompu(const compu&);
    void conectar(const compu&, const interfaz&,
        const compu&, const interfaz&);
    bool conectadas(const compu&, const compu&) const;
    const interfaz& interfazUsada(const compu&, const compu&) const;
    Conj<compu> vecinos(const compu&) const;
    bool usaInterfaz(const compu&, const interfaz&) const;
    Conj<Lista<compu> > caminosMinimos(const compu&, const compu&) const;
    bool hayCamino(const compu&, const compu&) const;
    const Conj<compu>& computadoras() const;
    Nat cantCompus() const;
};

}  // namespace tp3

#endif  // RED_H
