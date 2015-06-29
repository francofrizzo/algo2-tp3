#include "dcnet.h"
namespace tp3 {

dcnet::iniciarDCNet(const red &r){
	red = r;
	cantPaqEnviados = Arreglo(r.cantCompus());
	IPsCompusPorID = Arreglo(r.cantCompus());
	siguientesCompus = Arreglo(r.cantCompus());
	paquetesEnEspera = Arreglo(r.cantCompus());
	Conj<compu> c = r.computadoras();
	Conj<compu>::const_Iterador it1 = c.crearIt();
	Nat j = 0;
	while (j < r.cantCompus()){
		siguientesCompus[j] = Arreglo(r.cantCompus());
		cantPaqEnviados[j] = 0;
		paquetesEnEspera[j] = colas();
		paquetesEnEspera[j].enConjunto = Conj();
		paquetesEnEspera[j].porID = vacio(); \\vacio de AVL
		paquetesEnEspera[j].porPrioridad = vacio(); \\vacio de ColaPrior
		IDsCompusPorIP.definir(j, it1.siguiente().IP);
		IPsCompusPorID[j] = it1.siguiente();
		j++;
		it1.avanzar();
	}
	Nat k = 0;
	j = 0;
	while (j < r.cantCompus()){
		while(k < r.cantCompus()){
			if (r.conectadas(IPsCompusPorID[j], IPsCompusPorID[k])) {
				Conj<Lista<Compu> >::const_Iterador it2 = r.CaminosMinimos(IPsCompusPorID[j], IPsCompusPorID[k]).crearIt();
				siguientesCompus[j][k] = it2.siguiente().fin().primero();
			}
			k++;
		}
		j++;
	}
};

void crearPaquete(const Paquete p){
 	Nat o = IDsCompusPorIP.obtener(p.origen);
 	Nat dest = IDsCompusPorIP.obtener(p.destino);
 	Conj<compu>::const_Iterador it = paquetesEnEspera[o].crearIt();
 	it = paquetesEnEspera[o].enConjunto.Agregar(p);
 	info i;
 	i.iPaquete = it;
 	i.codOrigen = o;
 	i.codDestino = dest;
 	paquetesEnEspera[o].porID.definir(p.ID, i);
 	priorIt pi;
 	pi.Prior = p.prioridad;
 	pi.iter = it;
 	paquetesEnEspera[o].porPrioridad.encolar(pi); 
}

void avanzarSegundo(){
	Nat j = 0;
	Nat o;
	Nat dest;
	Paquete paq;
	Conj<Paquete>::const_Iterador it;
	while (j < red.cantCompus()){
		if (!(paquetesEnEspera[j].enConjunto.esVacio())){
			paq = paquetesEnEspera[j].porPrioridad.desencolar().siguiente();
			o = paquetesEnEspera[j].porID.obtener(paq.ID).codOrigen;
			dest = paquetesEnEspera[j].porID.obtener(paq.ID).codDestino;
			it = paquetesEnEspera[j].porID.obtener(paq.ID).iPaquete;
			paquetesEnEspera[j].porID.borrar(paq.ID);
			it.eliminarSiguiente();
			cantPaqEnviados[j]++;
			if (!(siguientesCompus[j][dest] = dest)){
				Conj<Paquete>::const_Iterador it = paquetesEnEspera[siguientesCompus[j][dest]].enConjunto.crearIt();
				it= paquetesEnEspera[siguientesCompus[j][dest]].enConjunto.AgregarRapido(p);
				info i;
				i.iPaquete = it;
				i.codOrigen = siguientesCompus[j][dest];
				i.codDestino = dest;
				paquetesEnEspera[siguientesCompus[j][dest]].porID.definir(p.ID, i);
				priorIt pi;
				pi.Prior = paq.prioridad;
				pi.iter = it;
				paquetesEnEspera[siguientesCompus[j][dest]].porPrioridad.encolar(pi);

			}
		}
		j++;
	}
	Nat k = 0;
	Nat h = 0;
	while (k < red.cantCompus()){
		if (cantPaqEnviados[k] > cantPaqEnviados[h]){
			h = k;
		}
		k++;
	}
	laQueMasEnvio = IPsCompusPorID[h];
}

red& Red(){
	return red;
}

lista<Compu> caminoRecorrido(const Paquete p){
	Nat j = 0;
	Lista<Compu> res = vacia();
	while (!(paquetesEnEspera[j].porID.definido(p.ID))){
		j++;
	}
	Nat o = paquetesEnEspera[j].porID.obtener(p.ID).codOrigen();
	Nat dest = paquetesEnEspera[o].porID.obtener(p.ID).codDestino();
	while (!(paquetesEnEspera[o].porID.definido(p.ID))){
		res.AgregarAtras(IPsCompusPorID[o]);
		o = siguientesCompus[o][dest];
	}
	res.AgregarAtras(IPsCompusPorID[o]);
}

Nat cantidadEnviados(const compu c){
	Nat i = IDsCompusPorIP.obtener(c.IP);
	return cantPaqEnviados[i];
}

Conj<Paquete>& enEspera(const compu c){
	Nat i = IDsCompusPorIP.obtener(c.IP);
	return paquetesEnEspera[i].enConjunto; 
}

bool paqueteEnTransito(const Paquete p){
	bool res = false;
	Nat i = 0;
	while (i < paquetesEnEspera.Tamanho()){
		if (paquetesEnEspera[i].porID.definido(p)){
			res = true;
		}
		i++;
	}
	return res;
}

compu laQueMasEnvio(){
	return laQueMasEnvio;
}


}