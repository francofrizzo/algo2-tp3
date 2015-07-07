#include "./dcnet.h"

namespace tp3 {

// paquete

bool paquete::operator==(const paquete& otro) const {
    return ID == otro.ID && _prioridad == otro._prioridad &&
        origen == otro.origen && destino == otro.destino;
}

bool paquete::operator!=(const paquete& otro) const {
    return !(this->operator==(otro));
}

bool dcnet::paqPorPrior::operator<(const dcnet::paqPorPrior& otro) const {
    return _prioridad < otro._prioridad;
}

bool dcnet::paqPorPrior::operator>(const dcnet::paqPorPrior& otro) const {
    return _prioridad > otro._prioridad;
}

// dcnet

// Constructores

dcnet::dcnet(const red& r) {
    _red = r;
    Nat cantC = r.cantCompus();
    cantPaqEnviados = Arreglo<Nat>(cantC);
    IPsCompusPorID = Arreglo<compu>(cantC);
    siguientesCompus = Arreglo<Arreglo <Nat> >(cantC);
    paquetesEnEspera = Arreglo<colas>(cantC);
    Conj<compu> c = r.computadoras();
    Conj<compu>::const_Iterador it1 = c.CrearIt();
    for (Nat j = 0; j < cantC; j++) {
        siguientesCompus[j] = Arreglo<Nat>(cantC);
        cantPaqEnviados[j] = 0;
        paquetesEnEspera[j] = colas();
        paquetesEnEspera[j].enConjunto = Conj<paquete>();
        paquetesEnEspera[j].porID = diccLog<id, paqPorID>();
        paquetesEnEspera[j].porPrioridad = colaPrior<paqPorPrior>();
        IDsCompusPorIP.definir(it1.Siguiente().IP, j);
        IPsCompusPorID[j] = it1.Siguiente();
        it1.Avanzar();
    }
    for (Nat j = 0; j < cantC; j++) {
        for (Nat k = 0; k < cantC; k++) {
            if (r.conectadas(IPsCompusPorID[j], IPsCompusPorID[k])) {
                Conj<Lista<compu> >::const_Iterador it2 = r.caminosMinimos(
                    IPsCompusPorID[j], IPsCompusPorID[k]).CrearIt();
                Lista<compu> camino = it2.Siguiente();
                camino.Fin();
                siguientesCompus[j][k] = *(IDsCompusPorIP.obtener(camino.Primero().IP));
            }
        }
    }
}

// Métodos públicos

void dcnet::crearPaquete(const paquete &p) {
    Nat o = *(IDsCompusPorIP.obtener(p.origen.IP));
    Nat dest = *(IDsCompusPorIP.obtener(p.destino.IP));
    Conj<paquete>::Iterador it = paquetesEnEspera[o].enConjunto.CrearIt();
    it = paquetesEnEspera[o].enConjunto.AgregarRapido(p);
    paqPorID i;
    i.itPaquete = it;
    i.codOrigen = o;
    i.codDestino = dest;
    paquetesEnEspera[o].porID.definir(p.ID, i);
    paqPorPrior pi;
    pi._prioridad = p._prioridad;
    pi.itPaquete = it;
    paquetesEnEspera[o].porPrioridad.encolar(pi);
}

void dcnet::avanzarSegundo() {
    Nat o;
    Nat dest;
    paquete paq;
    Conj<paquete>::Iterador it;
    for (Nat j = 0; j < _red.cantCompus(); j++) {
        if (!(paquetesEnEspera[j].enConjunto.EsVacio())) {
            paq = paquetesEnEspera[j].porPrioridad.desencolar().itPaquete.Siguiente();
            o = paquetesEnEspera[j].porID.obtener(paq.ID).codOrigen;
            dest = paquetesEnEspera[j].porID.obtener(paq.ID).codDestino;
            it = paquetesEnEspera[j].porID.obtener(paq.ID).itPaquete;
            paquetesEnEspera[j].porID.borrar(paq.ID);
            it.EliminarSiguiente();
            cantPaqEnviados[j]++;
            paquetesEnEspera[siguientesCompus[j][dest]].porEnviar.paq = paq;
            paquetesEnEspera[siguientesCompus[j][dest]].porEnviar.vacio = false;
            paquetesEnEspera[siguientesCompus[j][dest]].porEnviar.orig = o;
            paquetesEnEspera[siguientesCompus[j][dest]].porEnviar.desti = dest;
        }
    }
    for (Nat j = 0; j < _red.cantCompus(); j++) {
        if (!(paquetesEnEspera[j].porEnviar.vacio)) {
            Conj<paquete>::Iterador it = paquetesEnEspera[siguientesCompus[j]
                [paquetesEnEspera[j].porEnviar.desti]].enConjunto.CrearIt();
            it = paquetesEnEspera[siguientesCompus[j][paquetesEnEspera[j].porEnviar
                .desti]].enConjunto.AgregarRapido(paquetesEnEspera[j].porEnviar.paq);
            paqPorID i;
            i.itPaquete = it;
            i.codOrigen = paquetesEnEspera[j].porEnviar.orig;
            i.codDestino = paquetesEnEspera[j].porEnviar.desti;
            paquetesEnEspera[siguientesCompus[j][paquetesEnEspera[j]
                .porEnviar.desti]].porID.definir(it.Siguiente().ID, i);
            paqPorPrior pi = paqPorPrior();
            pi._prioridad = paquetesEnEspera[j].porEnviar.paq._prioridad;
            pi.itPaquete = it;
            paquetesEnEspera[siguientesCompus[j][paquetesEnEspera[j]
                .porEnviar.desti]].porPrioridad.encolar(pi);
            paquetesEnEspera[j].porEnviar.vacio = true;
        }
    }
    Nat h = 0;
    for (Nat k = 0; k < _red.cantCompus(); k++) {
        if (cantPaqEnviados[k] > cantPaqEnviados[h]) {
            h = k;
        }
    }
    _laQueMasEnvio = IPsCompusPorID[h];
}

const red& dcnet::laRed() const {
    return _red;
}

Lista<compu> dcnet::caminoRecorrido(const paquete &p) const {
    Lista<compu> res =  Lista<compu>();
    Nat j;
    for (j = 0; !(paquetesEnEspera[j].porID.definido(p.ID)); j++) {
        j++;
    }
    Nat o = paquetesEnEspera[j].porID.obtener(p.ID).codOrigen;
    Nat dest = paquetesEnEspera[o].porID.obtener(p.ID).codDestino;
    while (!(paquetesEnEspera[o].porID.definido(p.ID))) {
        res.AgregarAtras(IPsCompusPorID[o]);
        o = siguientesCompus[o][dest];
    }
    res.AgregarAtras(IPsCompusPorID[o]);
    return res;
}

Nat dcnet::cantidadEnviados(const compu &c) const {
    Nat i = *(IDsCompusPorIP.obtener(c.IP));
    return cantPaqEnviados[i];
}

const Conj<paquete>& dcnet::enEspera(const compu &c) const {
    Nat i = *(IDsCompusPorIP.obtener(c.IP));
    return paquetesEnEspera[i].enConjunto;
}

bool dcnet::paqueteEnTransito(const paquete &p) const {
    for (Nat i = 0; i < paquetesEnEspera.Tamanho(); i++) {
        if (paquetesEnEspera[i].porID.definido(p.ID)) {
            return true;
        }
    }
    return false;
}

const compu& dcnet::laQueMasEnvio() const {
    return _laQueMasEnvio;
}

}  // namespace tp3
