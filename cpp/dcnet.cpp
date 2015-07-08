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

std::ostream& operator<<(std::ostream& os, const paquete& c)
{
  return os << c.ID;
}

// dcnet

// Constructores

dcnet::dcnet(const red& r) {
    _red = r;
    Nat cantC = r.cantCompus();
    cantPaqEnviados = Arreglo<Nat>(cantC);
    IPsCompusPorID = Arreglo<compu>(cantC);
    siguientesCompus = Arreglo<Arreglo<Nat> >(cantC);
    paquetesEnEspera = Arreglo<colas>(cantC);
    Conj<compu> c = r.computadoras();
    Conj<compu>::const_Iterador it1 = c.CrearIt();
    for (Nat j = 0; j < cantC; j++) {
        siguientesCompus.Definir(j, Arreglo<Nat>(cantC));
        cantPaqEnviados.Definir(j, 0);
        colas cs = colas();
        cs.enConjunto = Conj<paquete>();
        cs.porID = diccLog<id, paqPorID>();
        cs.porPrioridad = colaPrior<paqPorPrior>();
        cs.porEnviar.vacio = true;
        paquetesEnEspera.Definir(j, cs);
        IDsCompusPorIP.definir(it1.Siguiente().IP, j);
        IPsCompusPorID.Definir(j, it1.Siguiente());
        cout << "Agregando compu \"" << it1.Siguiente().IP << "\" con ID " << j << endl;  // DEBUG 
        it1.Avanzar();
    }
    cout << endl << "¡La Tablita!" << endl;  // DEBUG
    for (Nat j = 0; j < cantC; j++) {
        siguientesCompus.Definir(j, Arreglo<Nat>(cantC));
        for (Nat k = 0; k < cantC; k++) {
            if (j != k && r.hayCamino(IPsCompusPorID[j], IPsCompusPorID[k])) {
                Conj<Lista<compu> > caminos = r.caminosMinimos(
                    IPsCompusPorID[j], IPsCompusPorID[k]);
                Conj<Lista<compu> >::const_Iterador it2 = caminos.CrearIt();
                Lista<compu> camino = it2.Siguiente();
                camino.Fin();
                siguientesCompus[j].Definir(k, *(IDsCompusPorIP.obtener(camino.Primero().IP)));
                cout << *(IDsCompusPorIP.obtener(camino.Primero().IP)) << "  " ;  // DEBUG
            }
            else {  // DEBUG
                cout << "*  ";  // DEBUG
            }  // DEBUG
        }
        cout << endl;  // DEBUG
    }
    cout << endl << "Donde * significa: \"You shall not pass\"." << endl << endl;  // DEBUG
}

// Métodos públicos

void dcnet::crearPaquete(const paquete &p) {
    assert(laRed().hayCamino(p.origen, p.destino));  // DEBUG - ASSERTION

    Nat o = *(IDsCompusPorIP.obtener(p.origen.IP));
    Nat dest = *(IDsCompusPorIP.obtener(p.destino.IP));
    cout << "Creando paquete " << p.ID << ": " << o << " -> " << dest << "... ";  // DEBUG
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
    cout << "Paquete creado con éxito ✓" << endl << endl;  // DEBUG
}

void dcnet::avanzarSegundo() {
    Nat o;
    Nat dest;
    paquete paq;
    Conj<paquete>::Iterador it;
    for (Nat j = 0; j < _red.cantCompus(); j++) {
        if (!(paquetesEnEspera[j].enConjunto.EsVacio())) {
            paq = paquetesEnEspera[j].porPrioridad.desencolar().itPaquete.Siguiente();
            o = paquetesEnEspera[j].porID.obtener(paq.ID).codOrigen;  // Esto es necesario?
            dest = paquetesEnEspera[j].porID.obtener(paq.ID).codDestino;
            cout << "Procesando paquete " << paq.ID << " (" << o << " -> " << dest << "), actualmente en " << j << ". ";  // DEBUG
            it = paquetesEnEspera[j].porID.obtener(paq.ID).itPaquete;
            paquetesEnEspera[j].porID.borrar(paq.ID);
            it.EliminarSiguiente();
            if (dest != j) {
                cout << "Paquete listo para enviar." << endl;  // DEBUG
                cantPaqEnviados[j]++;
                paquetesEnEspera[j].porEnviar.paq = paq;
                paquetesEnEspera[j].porEnviar.vacio = false;
                paquetesEnEspera[j].porEnviar.orig = o;  // Lo mismo con esto, hace falta?
                paquetesEnEspera[j].porEnviar.desti = dest;
            }
            else {  // DEBUG
                cout << "El paquete ya llegó a destino y será eliminado. Será como si jamás hubiera existido. :(" << endl << endl;  // DEBUG
            }  // DEBUG
        }
    }
    cout << endl;  // DEBUG
    for (Nat j = 0; j < _red.cantCompus(); j++) {
        if (!(paquetesEnEspera[j].porEnviar.vacio)) {
            Nat sigCompu = siguientesCompus[j]
                [paquetesEnEspera[j].porEnviar.desti];
            cout << "Enviando paquete " << paquetesEnEspera[j].porEnviar.paq.ID << ". Próximo paso: " << j << " -> " << sigCompu << "... ";  // DEBUG
            Conj<paquete>::Iterador it = paquetesEnEspera[sigCompu]
                .enConjunto.CrearIt();
            it = paquetesEnEspera[sigCompu].enConjunto
                .AgregarRapido(paquetesEnEspera[j].porEnviar.paq);
            paqPorID i;
            i.itPaquete = it;
            i.codOrigen = paquetesEnEspera[j].porEnviar.orig;
            i.codDestino = paquetesEnEspera[j].porEnviar.desti;
            paquetesEnEspera[sigCompu].porID.definir(it.Siguiente().ID, i);
            paqPorPrior pi = paqPorPrior();
            pi._prioridad = paquetesEnEspera[j].porEnviar.paq._prioridad;
            pi.itPaquete = it;
            paquetesEnEspera[sigCompu].porPrioridad.encolar(pi);
            paquetesEnEspera[j].porEnviar.vacio = true;
            cout << "Paquete enviado con éxito ✓" << endl;  // DEBUG
        }
    }
    cout << endl;  // DEBUG
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
    cout << "Analizando el recorrido del paquete " << p.ID << "... " << endl;  // DEBUG
    Lista<compu> res = Lista<compu>();
    Nat j;
    for (j = 0; !(paquetesEnEspera[j].porID.definido(p.ID)); j++) {}
    Nat o = paquetesEnEspera[j].porID.obtener(p.ID).codOrigen;
    Nat dest = paquetesEnEspera[j].porID.obtener(p.ID).codDestino;
    cout << "El recorrido del paquete fue: ";  // DEBUG
    while (!(paquetesEnEspera[o].porID.definido(p.ID))) {
        res.AgregarAtras(IPsCompusPorID[o]);
        cout << o << " -> ";  // DEBUG
        o = siguientesCompus[o][dest];
    }
    res.AgregarAtras(IPsCompusPorID[o]);
    cout << o << endl << endl;  // DEBUG
    return res;
}

Nat dcnet::cantidadEnviados(const compu &c) const {
    Nat i = *(IDsCompusPorIP.obtener(c.IP));
    cout << "La compu " << i << " envió " << cantPaqEnviados[i] << " paquetes." << endl << endl;  // DEBUG
    return cantPaqEnviados[i];
}

const Conj<paquete>& dcnet::enEspera(const compu &c) const {
    Nat i = *(IDsCompusPorIP.obtener(c.IP));
    cout << "La compu " << i << " tiene estos paquetes en su cola: " << paquetesEnEspera[i].enConjunto << endl << endl;  // DEBUG
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
