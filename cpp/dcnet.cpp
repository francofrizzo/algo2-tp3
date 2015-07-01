#include "./dcnet.h"

namespace tp3 {

// Constructores

explicit dcnet::dcnet(const red& r) {
    _red = r;
    cantCompus = r.cantCompus();
    cantPaqEnviados = Arreglo(cantCompus);
    IPsCompusPorID = Arreglo(cantCompus);
    siguientesCompus = Arreglo(cantCompus);
    paquetesEnEspera = Arreglo(cantCompus);
    Conj<compu> c = r.computadoras();
    Conj<compu>::const_Iterador it1 = c.crearIt();
    for (Nat j = 0; j < cantCompus; j++) {
        siguientesCompus[j] = Arreglo(cantCompus);
        cantPaqEnviados[j] = 0;
        paquetesEnEspera[j] = colas();
        paquetesEnEspera[j].enConjunto = Conj<paquete>();
        paquetesEnEspera[j].porID = vacio();  \\ vacio de diccLog
        paquetesEnEspera[j].porPrioridad = vacio();  \\ vacio de colaPrior
        IDsCompusPorIP.definir(j, it1.siguiente().IP);
        IPsCompusPorID[j] = it1.siguiente();
        it1.avanzar();
    }
    for (Nat j = 0; j < cantCompus; j++) {
        for (Nat k = 0; k < cantCompus; k++) {
            if (r.conectadas(IPsCompusPorID[j], IPsCompusPorID[k])) {
                Conj<Lista<compu> >::const_Iterador it2 = r.CaminosMinimos(
                    IPsCompusPorID[j], IPsCompusPorID[k]).CrearIt();
                Lista<compu> camino = it2.Siguiente().Fin()
                siguientesCompus[j][k] = camino.Primero();
            }
        }
    }
}

// Métodos públicos

void crearPaquete(const paquete p) {
    Nat o = IDsCompusPorIP.obtener(p.origen);
    Nat dest = IDsCompusPorIP.obtener(p.destino);
    Conj<compu>::const_Iterador it = paquetesEnEspera[o].crearIt();
    it = paquetesEnEspera[o].enConjunto.Agregar(p);
    paqPorID i;
    i.itPaquete = it;
    i.codOrigen = o;
    i.codDestino = dest;
    paquetesEnEspera[o].porID.definir(p.ID, i);
    paqPorPrior pi;
    pi._prioridad = p._prioridad;
    pi.iter = it;
    paquetesEnEspera[o].porPrioridad.encolar(pi);
}

void avanzarSegundo() {
    Nat o;
    Nat dest;
    paquete paq;
    Conj<paquete>::const_Iterador it;
    for (Nat j = 0; j < red.cantCompus(); j++) {
        if (!(paquetesEnEspera[j].enConjunto.EsVacio())) {
            paq = paquetesEnEspera[j].porPrioridad.desencolar().siguiente();
            o = paquetesEnEspera[j].porID.obtener(paq.ID).codOrigen;
            dest = paquetesEnEspera[j].porID.obtener(paq.ID).codDestino;
            it = paquetesEnEspera[j].porID.obtener(paq.ID).itPaquete;
            paquetesEnEspera[j].porID.borrar(paq.ID);
            it.EliminarSiguiente();
            cantPaqEnviados[j]++;
            if (!(siguientesCompus[j][dest] = dest)) {
                Conj<paquete>::const_Iterador it =
                    paquetesEnEspera[siguientesCompus[j][dest]]
                    .enConjunto.CrearIt();
                it = paquetesEnEspera[siguientesCompus[j][dest]]
                    .enConjunto.AgregarRapido(p);
                paqPorID i = paqPorId();
                i.itPaquete = it;
                i.codOrigen = siguientesCompus[j][dest];
                i.codDestino = dest;
                paquetesEnEspera[siguientesCompus[j][dest]]
                    .porID.definir(p.ID, i);
                paqPorPrior pi = paqPorPrior();
                pi._prioridad = paq._prioridad;
                pi.iter = it;
                paquetesEnEspera[siguientesCompus[j][dest]]
                    .porPrioridad.encolar(pi);
            }
        }
    }
    Nat h = 0;
    for (Nat k = 0; k < red.cantCompus(); k++) {
        if (cantPaqEnviados[k] > cantPaqEnviados[h]) {
            h = k;
        }
    }
    _laQueMasEnvio = IPsCompusPorID[h];
}

const red& laRed() const {
    return _red;
}

Lista<compu> caminoRecorrido(const paquete p) const {
    Lista<compu> res = vacia();
    for (Nat j = 0; !(paquetesEnEspera[j].porID.definido(p.ID)); j++) {
        j++;
    }
    Nat o = paquetesEnEspera[j].porID.obtener(p.ID).codOrigen();
    Nat dest = paquetesEnEspera[o].porID.obtener(p.ID).codDestino();
    while (!(paquetesEnEspera[o].porID.definido(p.ID))) {
        res.AgregarAtras(IPsCompusPorID[o]);
        o = siguientesCompus[o][dest];
    }
    res.AgregarAtras(IPsCompusPorID[o]);
    return res;
}

Nat cantidadEnviados(const compu c) const {
    Nat i = IDsCompusPorIP.obtener(c.IP);
    return cantPaqEnviados[i];
}

const Conj<paquete>& enEspera(const compu c) const {
    Nat i = IDsCompusPorIP.obtener(c.IP);
    return paquetesEnEspera[i].enConjunto;
}

bool paqueteEnTransito(const paquete p) const {
    for (Nat i = 0; i < paquetesEnEspera.Tamanho(); i++) {
        if (paquetesEnEspera[i].porID.definido(p)) {
            return true;
        }
    }
    return false;
}

const &compu laQueMasEnvio() const {
    return _laQueMasEnvio;
}

}  // namespace tp3
