#ifndef DCNET_H
#define DCNET_H

#include "aed2.h"
#include "dicc_trie.h"

using namespace aed2;

namespace tp3 {



typedef aed2::Nat Prioridad;
typedef aed2::Nat ID;
typedef aed2::String IP;

struct Paquete
{ID id;
    Prioridad prioridad;
    Compu origen;
    Compu destino;
};



class dcnet {
private:
  
    red Red;
    DiccString<String, Nat> IDsCompusPorIP;
    Arreglo<Arreglo<Nat> > siguientesCompus;
    Arreglo<colas> paquetesEnEspera;
    Arreglo<Nat> cantPaqEnviados;
    Compu laQueMasEnvio;
    Arreglo<Compu> IPsCompusPorID;

    struct colas
    { Conj<Paquete> enConjunto;
        DiccString<ID, info> porID;
        colaPrior<priorIt> porPrioridad;        
    };

    struct info
    {Conj<Paquete>::Iterador iPaquete;
    Nat codOrigen;
    Nat codDestino;     
    };

    struct priorIt
    {Prioridad Prior;
    Conj<Paquete>::Iterador IterP;        
    };

public:
    iniciarDCNet(const red &r);
    void crearPaquete(const Paquete p);
    void avanzarSegundo();
    red& Red();
    lista<Compu> caminoRecorrido(const Paquete p);
    Nat cantidadEnviados(const compu c);
    Conj<Paquete>& enEspera(const compu c);
    bool paqueteEnTransito(const Paquete p);
    compu laQueMasEnvio();
};

}

#endif