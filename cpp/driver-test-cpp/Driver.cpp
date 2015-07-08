#include "Driver.h"
#include "ArbolSintactico.h"

namespace aed2 {

Driver::Driver() :
    r(red()), d(NULL), sigId(0) {}

Driver::~Driver() {
    delete d;
}

// TAD RED
Nat Driver::CantidadComputadoras() const {
    return r.cantCompus();
}

const Computadora& Driver::IesimaComputadora(const Nat i) const {
    assert(i < CantidadComputadoras());  // DEBUG - ASSERTION
    Conj<compu>::const_Iterador it = r.computadoras().CrearIt();
    for (Nat j = 0; j < i; j++) {
        it.Avanzar();
    }
    return it.Siguiente().IP;
}

Nat Driver::CantidadInterfacesDe(const Computadora& c) const {
    return strACompu(c).interfaces.Cardinal();
}

const Interfaz& Driver::IesimaInterfazDe(const Computadora& c, const Nat i) const{
    assert(i < CantidadInterfacesDe(c));
    Conj<interfaz>::const_Iterador it = strACompu(c).interfaces.CrearIt();
    for (Nat j = 0; j < i; j++) {
        it.Avanzar();
    }
    return it.Siguiente();
}

const Interfaz& Driver::IntefazUsada(const Computadora& c1, const Computadora& c2) const {
    return r.interfazUsada(strACompu(c1), strACompu(c2));
}

bool Driver::conectadas(const Computadora& c1, const Computadora& c2) const {
    return r.conectadas(strACompu(c1), strACompu(c2));
}

// TAD DCNET
void Driver::AgregarComputadora(const Computadora& ip, const Conj<Interfaz>& ci) {
    assert(d == NULL);
    compu c;
    c.IP = ip;
    c.interfaces = ci;
    r.agregarCompu(c);
}

void Driver::Conectar(const Computadora& c1, const Interfaz& i1, const Computadora& c2, const Interfaz& i2) {
    assert(d == NULL);
    r.conectar(strACompu(c1), i1, strACompu(c2), i2);
}


Nat Driver::CantidadNodosRecorridosPor(const Paquete& p) const {
    assert(d != NULL);
    const paquete& p2 = encontrarPaquete(p);
    return d->caminoRecorrido(p2).Longitud();
}

const Computadora& Driver::IesimoNodoRecorridoPor(const Paquete& p, const Nat i) const {
    assert(d != NULL);
    assert(i < CantidadNodosRecorridosPor(p));
    const paquete& p2 = encontrarPaquete(p);
    return strACompu(d->caminoRecorrido(p2)[i].IP).IP;
}

Nat Driver::CantidadEnviadosPor(const Computadora& c) const {
    assert(d != NULL);
    return d->cantidadEnviados(strACompu(c));
}

Nat Driver::CantidadEnEsperaEn(const Computadora& c) const {
    assert(d != NULL);
    return d->enEspera(strACompu(c)).Cardinal();
}

const Paquete& Driver::IesimoEnEsperaEn(const Computadora& c, const Nat i) const {
    assert(d != NULL);
    assert(i < CantidadEnEsperaEn(c));
    const Conj<paquete>& cola = d->enEspera(strACompu(c));
    Conj<paquete>::const_Iterador it = cola.CrearIt();
    for (Nat j = 0; j < i; j++) {
        it.Avanzar();
    }
    return it.Siguiente().ID;
}

void Driver::CrearPaquete(const Computadora& origen, const Computadora& destino, Nat prioridad) {
    if (d == NULL) {
        d = new dcnet(r);
    }
    paquete p;
    p.ID = sigId;
    p.origen = strACompu(origen);
    p.destino = strACompu(destino);
    p._prioridad = prioridad;
    d->crearPaquete(p);
    sigId++;
}

void Driver::AvanzarSegundo() {
    if (d == NULL) {
        d = new dcnet(r);
    }
    d->avanzarSegundo();
}

const Computadora& Driver::laQueMasEnvio() const {
    assert(d != NULL);
    return d->laQueMasEnvio().IP;
}

const Computadora& Driver::origen(const Paquete& p) const {
    assert(d != NULL);
    return encontrarPaquete(p).origen.IP;
}

const Computadora& Driver::destino(const Paquete& p) const {
    assert(d != NULL);
    return encontrarPaquete(p).destino.IP;
}

Nat Driver::prioridad(const Paquete& p) const {
    assert(d != NULL);
    return encontrarPaquete(p)._prioridad;
}

const compu& Driver::strACompu(const Computadora& c) const {
    Conj<compu>::const_Iterador it = r.computadoras().CrearIt();
    while (it.Siguiente().IP != c) {
        it.Avanzar();
    }
    return it.Siguiente();
}

const paquete& Driver::encontrarPaquete(const Paquete& p) const {
    assert(d != NULL);
    Conj<compu>::const_Iterador it1 = r.computadoras().CrearIt();
    while (it1.HaySiguiente()) {
        const Conj<paquete>& cola = d->enEspera(it1.Siguiente());
        Conj<paquete>::const_Iterador it2 = cola.CrearIt();
        while (it2.HaySiguiente()) {
            if (it2.Siguiente().ID == p) {
                return it2.Siguiente();
            }
            it2.Avanzar();
        }
        it1.Avanzar();
    }
}

} // namespace aed2

