#include "red.h"


namespace tp3 {

red::red() {
    compus = Conj<compu>();
    conexiones = DiccString<diccConexiones>();
}; 
red::red(const red& r) {
    compus = Conj<compu>(r.compus);
    // conexiones = DiccString<diccConexiones>(r.conexiones);
    // DiccString<diccConexiones>::Iterador it = conexiones.CrearIt;
    // while ()
};

void red::agregarCompu(const compu c) {
    compus.AgregarRapido(c);
    diccConexiones* dicc = new diccConexiones();
    conexiones.definir(c.IP, *dicc);
};

void red::conectar(const compu c1, const interfaz i1, const compu c2, const interfaz i2){
    conexiones.obtener(c1.IP)->DefinirRapido(i1, c2.IP);
    conexiones.obtener(c2.IP)->DefinirRapido(i2, c1.IP);
};

bool red::conectadas(const compu c1, const compu c2) const {
    Dicc<interfaz, ip>* d = conexiones.obtener(c1.IP);
    Dicc<interfaz, ip>::Iterador it = d.CrearIt();
    while (it.HaySiguiente() && it.SiguienteSignificado() != c2.IP) {
        it.Avanzar();
    }
    return it.HaySiguiente();
};

interfaz red::interfazUsada(const compu c1, const compu c2) const {
    Dicc<interfaz, ip>* d = conexiones.obtener(c1.IP);
    Dicc<interfaz, ip>::Iterador it = d.CrearIt();
    while (it.SiguienteSignificado() != c2.IP) {
        it.Avanzar();
    }
    return it.SiguienteClave();
};

Conj<compu> red::vecinos(const compu c) const {
    Dicc<interfaz, ip>* d = conexiones.obtener(c.IP);
    Dicc<interfaz, ip>::Iterador it1 = d.CrearIt();
    Conj<compu> res = Conj<compu>();
    while (it1.HaySiguiente()) {
        Conj<compu> it2 = compus.CrearIt();
        while (it2.Siguiente().IP != it1.SiguienteSignificado()) {
            it2.Avanzar();
        }
        res.AgregarRapido(it2.Siguiente());
    }
    return res;
};

bool red::usaInterfaz(const compu c; const interfaz i) const {
    return conexiones.obtener(c.IP).Definido(i);
};

Conj<Lista<compu> > red::caminosMinimos(const compu c1, const compu c2) const {
    Conj<Lista<compu> > res = Conj<Lista<compu> >();
    if (Vecinos(c1).Pertenece(c2)) {
        res.Agregar(Lista<compu>().AgregarAtras(c1).AgregarAtras(c2));
    } else {
        res = DameMinimos(Caminos(c1, c2, Lista<compu>().AgregarAtras(c1), PasarConjASecu(Vecinos(c1)));
    }
};

bool red::hayCamino(const compu c1, const compu c2) const {
    return !(caminos(c1, c2, Lista<compu>().AgregarAtras(c1), Vecinos(c1)).EsVacio());
};

Conj<compu> red::computadoras() const {
    return compus;
};

Nat red::cantCompus() const;
    return compus.Cardinal();
}

Conj<Lista<compu> > red::dameMinimos(const Conj<Lista<compu> > c) const {
    return LosDeLong(c, MinimaLong(c));
};

Conj<Lista<compu> > red::losDeLong(const Conj<Lista<compu> > c, Nat k) const {
    Conj<Lista<compu> >::Iterador it = c.CrearIt();
    Conj<Lista<compu> > res = ConjConj<Lista<compu> >();
    while (it.HaySiguiente()) {
        if (it.Siguiente().Longitud() = k) {
            res.Agregar(it.Siguiente());
        }
        it.Avanzar();
    }
};

Nat red::minimaLong(const Conj<Lista<compu> > c) const {
    Nat min = 0;
    Conj<Lista<compu> >::Iterador it = c.CrearIt();
    if (it.HaySiguiente()) {
        min = it.Siguiente().Longitud();
    }
    while (it.HaySiguiente()) {
        if (it.Siguiente().Longitud() < min) {
            min = it.Siguiente().Longitud();
        }
        it.Avanzar();
    }
    return min;
};

Lista<compu> red::pasarConjASecu(const Conj<compu> c) const {
    Lista<compu> res = Lista<compu>();
    Conj<compu>::Iterador it = c.CrearIt();
    while (it.HaySiguiente()) {
        res.AgregarAtras(it.Siguiente());
    }
    return res;
};

Conj<Lista<compu> > red::caminos(const compu c1, const compu c2, Lista<compu> l, Lista<compu> candidatos) const {
    if (candidatos.EsVacia()) {
        return Conj<Lista<compu> >();
    } else {
        if (l.Ultimo() = c2) {
            return Conj<Lista<compu> >().Agregar(l);
        } else {
            if (!(candidatos.Primero().Esta(l))) {
                return Union(Caminos(c1, c2, l.AgregarAtras(candidatos.Primero()), PasarConjASecu(Vecinos(candidatos.Primero()))), Caminos(c1, c2, l, candidatos.Fin()));
            } else {
                return Caminos(c1, c2, l, candidatos.Fin());
            }
        }
    }
};
