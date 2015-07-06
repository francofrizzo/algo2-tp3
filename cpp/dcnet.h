#ifndef DCNET_H
#define DCNET_H

#include "./aed2.h"
 #include "./dicc_trie.h"
 #include "./dicc_log.h"
#include "./cola_prior.h"
#include "./red.h"

using namespace aed2;

namespace tp3 {

typedef Nat prioridad;
typedef Nat id;

struct paquete {
    id ID;
    prioridad _prioridad;
    compu origen;
    compu destino;
};



class dcnet {
 private:
    struct paqPorID {
        Conj<paquete>::const_Iterador itPaquete;
        Nat codOrigen;
        Nat codDestino;
    };

    struct paqPorPrior {
        prioridad _prioridad;
        Conj<paquete>::const_Iterador itPaquete;
    };
    struct datosPaqAEnviar{
        paquete paq;
        Nat orig;
        Nat desti;
        bool vacio;
    };

    struct colas {
        Conj<paquete> enConjunto;
        diccLog<id, paqPorID> porID;
        colaPrior<paqPorPrior> porPrioridad;
        datosPaqAEnviar porEnviar;
    };

    red _red;
    DiccString<Nat> IDsCompusPorIP;
    Arreglo<Arreglo<Nat> > siguientesCompus;
    Arreglo<colas> paquetesEnEspera;
    Arreglo<Nat> cantPaqEnviados;
    compu _laQueMasEnvio;
    Arreglo<compu> IPsCompusPorID;

 public:
    explicit dcnet(const red& r);
    void crearPaquete(const paquete& p);
    void avanzarSegundo();
    const red& laRed() const;
    Lista<compu> caminoRecorrido(const paquete& p) const;
    Nat cantidadEnviados(const compu& c) const;
    const Conj<paquete>& enEspera(const compu& c) const;
    bool paqueteEnTransito(const paquete& p) const;
    const compu& laQueMasEnvio() const;
};

}  // namespace tp3

#endif  // DCNET_H
