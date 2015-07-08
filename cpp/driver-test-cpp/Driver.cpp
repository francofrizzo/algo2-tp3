#include "Driver.h"
#include "ArbolSintactico.h"

namespace aed2 {

Driver::Driver() :
    r(red()), d(NULL) {}

Driver::~Driver() {
    delete d;
}

// TAD RED
Nat Driver::CantidadComputadoras() const {
    return d.compus().cantCompus();
}

const Computadora& Driver::IesimaComputadora(const Nat i) const {
    assert(i < CantidadComputadoras());  // DEBUG - ASSERTION
    Conj<compu>::Iterador it = d.computadoras().CrearIt();
    for (Nat j = 0; j < i; j++) {
        it.Avanzar();
    }
    return it.Siguiente().IP;
}

Nat Driver::CantidadInterfacesDe(const Computadora& c) const {
    for (Conj<compu>::Iterador it = d.compus().CrearIt();
        it.Siguiente().IP != c; it.Avanzar()) {}
    return it.Siguiente().interfaces.Cardinal();
}

const Interfaz& Driver::IesimaInterfazDe(const Computadora& c, const Nat i) const{
    // TODO
    return 0;

}

const Interfaz& Driver::IntefazUsada(const Computadora& c1, const Computadora& c2) const {
    // TODO
    return 0;

}

bool Driver::conectadas(const Computadora& c1, const Computadora& c2) const {
    return d.laRed().conectadas(strACompu(c1), strACompu(c2));
}

// TAD DCNET
void Driver::AgregarComputadora(const Computadora& ip, const Conj<Interfaz>& ci) {
    // TODO
}

void Driver::Conectar(const Computadora& c1, const Interfaz& i1, const Computadora& c2, const Interfaz& i2) {
    // TODO
}


Nat Driver::CantidadNodosRecorridosPor(const Paquete& p) const {
    // TODO
    return 0;
}

const Computadora& Driver::IesimoNodoRecorridoPor(const Paquete& p, const Nat i) const {
    // TODO
}

Nat Driver::CantidadEnviadosPor(const Computadora& c) const {
    // TODO
    return 0;
}

Nat Driver::CantidadEnEsperaEn(const Computadora& c) const {
    // TODO
    return 0;
}

const Paquete& Driver::IesimoEnEsperaEn(const Computadora& c, const Nat i) const {
    // TODO
    return 0;
}

void Driver::CrearPaquete(const Computadora& origen, const Computadora& destino, Nat prioridad) {
    // TODO
}

void Driver::AvanzarSegundo() {
    // TODO
}

const Computadora& Driver::laQueMasEnvio() const {
    // TODO
    return 0;
}

const Computadora& Driver::origen(const Paquete& p) const {
    // TODO
    return 0;
}

const Computadora& Driver::destino(const Paquete& p) const {
    // TODO
    return 0;
}

Nat Driver::prioridad(const Paquete& p) const {
    // TODO
    return 0;
}



} // namespace aed2

