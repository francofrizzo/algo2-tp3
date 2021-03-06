#include "./red.h"

namespace tp3 {

// compu

bool compu::operator==(const compu& otra) const {
    return (IP == otra.IP) && (interfaces == otra.interfaces);
}
bool compu::operator!=(const compu& otra) const {
    return !(this->operator==(otra));
}

std::ostream& operator<<(std::ostream& os, const compu& c)
{
  return os << c.IP;
}

// red

// Constructores

red::red() :
    compus(Conj<compu>()),
    conexiones(DiccString<diccConexiones>()) {}

red::red(const red& r) :
    compus(Conj<compu>(r.compus)),
    conexiones(DiccString<diccConexiones>(r.conexiones)) {}

// Métodos públicos

void red::agregarCompu(const compu& c) {
    compus.AgregarRapido(c);
    // diccConexiones* d = ;
    conexiones.definir(c.IP, diccConexiones());
}

void red::conectar(const compu& c1, const interfaz& i1,
    const compu& c2, const interfaz& i2) {
    assert(computadoras().Pertenece(c1));  // DEBUG - ASSERTION
    assert(computadoras().Pertenece(c2));  // DEBUG - ASSERTION
    assert(c1.interfaces.Pertenece(i1));   // DEBUG - ASSERTION
    assert(c2.interfaces.Pertenece(i2));   // DEBUG - ASSERTION
    assert(!usaInterfaz(c1, i1));          // DEBUG - ASSERTION
    assert(!usaInterfaz(c2, i2));          // DEBUG - ASSERTION
    conexiones.obtener(c1.IP)->DefinirRapido(i1, c2.IP);
    conexiones.obtener(c2.IP)->DefinirRapido(i2, c1.IP);
}

bool red::conectadas(const compu& c1, const compu& c2) const {
    assert(computadoras().Pertenece(c1));  // DEBUG - ASSERTION
    assert(computadoras().Pertenece(c2));  // DEBUG - ASSERTION
    diccConexiones* d = conexiones.obtener(c1.IP);
    diccConexiones::Iterador it = d->CrearIt();
    while (it.HaySiguiente() && it.SiguienteSignificado() != c2.IP) {
        it.Avanzar();
    }
    return it.HaySiguiente();
}

const interfaz& red::interfazUsada(const compu& c1, const compu& c2) const {
    assert(computadoras().Pertenece(c1));  // DEBUG - ASSERTION
    assert(computadoras().Pertenece(c2));  // DEBUG - ASSERTION
    Dicc<interfaz, ip>* d = conexiones.obtener(c1.IP);
    Dicc<interfaz, ip>::Iterador it = d->CrearIt();
    while (it.SiguienteSignificado() != c2.IP) {
        it.Avanzar();
    }
    return it.SiguienteClave();
}

Conj<compu> red::vecinos(const compu& c) const {
    assert(computadoras().Pertenece(c));  // DEBUG - ASSERTION
    Dicc<interfaz, ip>* d = conexiones.obtener(c.IP);
    Dicc<interfaz, ip>::Iterador it1 = d->CrearIt();
    Conj<compu> res = Conj<compu>();
    while (it1.HaySiguiente()) {
        Conj<compu>::const_Iterador it2 = compus.CrearIt();
        while (it2.Siguiente().IP != it1.SiguienteSignificado()) {
            it2.Avanzar();
        }
        res.AgregarRapido(it2.Siguiente());
        it1.Avanzar();
    }
    return res;
}

bool red::usaInterfaz(const compu& c, const interfaz& i) const {
    assert(computadoras().Pertenece(c));  // DEBUG - ASSERTION
    assert(c.interfaces.Pertenece(i));   // DEBUG - ASSERTION
    return conexiones.obtener(c.IP)->Definido(i);
}

Conj<Lista<compu> > red::caminosMinimos(const compu& c1, const compu& c2) const {
    assert(computadoras().Pertenece(c1));  // DEBUG - ASSERTION
    assert(computadoras().Pertenece(c2));  // DEBUG - ASSERTION
    assert(hayCamino(c1, c2));  // DEBUG - ASSERTION
    Conj<Lista<compu> > res = Conj<Lista<compu> >();
    if (vecinos(c1).Pertenece(c2)) {
        Lista<compu> l = Lista<compu>();
        l.AgregarAtras(c1);
        l.AgregarAtras(c2);
        res.Agregar(l);
    } else {
        Lista<compu> l = Lista<compu>();
        l.AgregarAtras(c1);
        Lista<compu> vec = pasarConjALista(vecinos(c1));
        res = dameMinimos(caminos(c1, c2, l, vec));
    }
    return res;
}

bool red::hayCamino(const compu& c1, const compu& c2) const {
    assert(computadoras().Pertenece(c1));  // DEBUG - ASSERTION
    assert(computadoras().Pertenece(c2));  // DEBUG - ASSERTION
    Lista<compu> l = Lista<compu>();
    l.AgregarAtras(c1);
    Lista<compu> vec = pasarConjALista(vecinos(c1));
    return !(caminos(c1, c2, l, vec).EsVacio());
}

const Conj<compu>& red::computadoras() const {
    return compus;
}

Nat red::cantCompus() const {
    return compus.Cardinal();
}

// Métodos privados

Conj<Lista<compu> > red::dameMinimos(const Conj<Lista<compu> >& c) const {
    return losDeLong(c, minimaLong(c));
}

Conj<Lista<compu> > red::losDeLong(const Conj<Lista<compu> >& c, Nat k) const {
    Conj<Lista<compu> >::const_Iterador it = c.CrearIt();
    Conj<Lista<compu> > res = Conj<Lista<compu> >();
    while (it.HaySiguiente()) {
        if (it.Siguiente().Longitud() == k) {
            res.Agregar(it.Siguiente());
        }
        it.Avanzar();
    }
    return res;
}

Nat red::minimaLong(const Conj<Lista<compu> >& c) const {
    Nat min = 0;
    Conj<Lista<compu> >::const_Iterador it = c.CrearIt();
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
}

Lista<compu> red::pasarConjALista(const Conj<compu>& c) const {
    Lista<compu> res = Lista<compu>();
    Conj<compu>::const_Iterador it = c.CrearIt();
    while (it.HaySiguiente()) {
        res.AgregarAtras(it.Siguiente());
        it.Avanzar();
    }
    return res;
}

Conj<Lista<compu> > red::caminos(const compu& c1, const compu& c2,
    Lista<compu>& visitadas, Lista<compu>& candidatos) const {
    if (candidatos.EsVacia()) {
        return Conj<Lista<compu> >();
    } else {
        if (visitadas.Ultimo() == c2) {
            Conj<Lista<compu> > res = Conj<Lista<compu> >();
            res.Agregar(visitadas);
            return res;
        } else {
            if (!(visitadas.Esta(candidatos.Primero()))) {
                compu primerCandidato = candidatos.Primero();
                Lista<compu> nuevosCandidatos = 
                    pasarConjALista(vecinos(primerCandidato));
                candidatos.Fin();
                Lista<compu> visitadasAntes = Lista<compu>(visitadas);
                visitadas.AgregarAtras(primerCandidato);
                return caminos(c1, c2, visitadas, nuevosCandidatos)
                    .Union(caminos(c1, c2, visitadasAntes, candidatos));
            } else {
                candidatos.Fin();
                return caminos(c1, c2, visitadas, candidatos);
            }
        }
    }
}

}  // namespace tp3
