#include "Driver.h"
#include "mini_test.h"
#include "../aed2/Lista.h"
#include "../aed2/Conj.h"
#include "../aed2/Dicc.h"

#include <string>
#include <iostream>

using namespace aed2;

/**
 * Imprime un elemento a un string, en vez de a una pantalla,
 * a través del operador <<
 */
template <typename T>
std::string to_str(const T& t)
{
    std::stringstream ss;
    ss << t;

    return ss.str();
}

/**
 * Esta función se puede utilizar para comparar dos colecciones
 * iterables que representen conjuntos, es decir, que no tengan
 * elementos repetidos.
 */
template<typename T, typename S>
bool Comparar(const T& t, const S& s)
{
  typename T::const_Iterador it1 = t.CrearIt();
  typename S::const_Iterador it2 = s.CrearIt();

    // me fijo si tienen el mismo tamanho

    Nat len1 = 0;
    while( it1.HaySiguiente() ) {
        len1++;
        it1.Avanzar();
    }

    Nat len2 = 0;
    while( it2.HaySiguiente() ) {
        len2++;
        it2.Avanzar();
    }

    if ( len1 != len2 )
        return false;

    it1 = t.CrearIt();
    it2 = s.CrearIt();

    // me fijo que tengan los mismos elementos

    while( it1.HaySiguiente() )
    {
        bool esta = false;
        it2 = s.CrearIt();

        while( it2.HaySiguiente() ) {
          if ( it1.Siguiente() == it2.Siguiente() ) {
            esta = true;
            break;
          }
          it2.Avanzar();
        }

        if ( !esta ) {
            return false;
        }

        it1.Avanzar();
    }

  return true;
}


// ---------------------------------------------------------------------

/**
 * Ejemplo de caso de test, con llamadas a las rutinas de aserción
 * definidas en mini_test.h
 */

void test_red(){
	Driver re = Driver();
    ASSERT_EQ(re.CantidadComputadoras(), 0);
    Computadora c1 = "Manu";
    Conj<Interfaz> inter1 = Conj<Interfaz>();
    inter1.Agregar(1);
    inter1.Agregar(2);
    re.AgregarComputadora(c1, inter1);
    ASSERT_EQ(re.CantidadComputadoras(), 1);
    Computadora c2 = "Lu";
    Conj<Interfaz> inter2 = Conj<Interfaz>();
    inter2.Agregar(5);
    inter2.Agregar(6);
    inter2.Agregar(3);
    re.AgregarComputadora(c2, inter2);
    ASSERT_EQ(re.CantidadComputadoras(), 2);
    Computadora c3 = "Fran";
    Conj<Interfaz> inter3 = Conj<Interfaz>();
    inter3.Agregar(1);
    inter3.Agregar(65);
    inter3.Agregar(7);
    re.AgregarComputadora(c3, inter3);
    ASSERT_EQ(re.CantidadComputadoras(), 3);
    Computadora c4 = "Minion";
    Conj<Interfaz> inter4 = Conj<Interfaz>();
    inter4.Agregar(9);
    re.AgregarComputadora(c4, inter4);
    ASSERT_EQ(re.CantidadComputadoras(), 4);
    Computadora c5 = "Uri";
    Conj<Interfaz> inter5 = Conj<Interfaz>();
    inter5.Agregar(5);
    inter5.Agregar(6);
    inter5.Agregar(45);
    re.AgregarComputadora(c5, inter5);
    ASSERT_EQ(re.CantidadComputadoras(), 5);
    ASSERT_EQ(re.CantidadInterfacesDe(c1), 2);
    ASSERT_EQ(re.CantidadInterfacesDe(c2), 3);
    ASSERT_EQ(re.CantidadInterfacesDe(c3), 3);
    ASSERT_EQ(re.CantidadInterfacesDe(c4), 1);
    ASSERT_EQ(re.CantidadInterfacesDe(c5), 3);
    ASSERT_EQ(re.IesimaComputadora(0), c5);
    ASSERT_EQ(re.IesimaComputadora(1), c4);
    ASSERT_EQ(re.IesimaComputadora(2), c3);
    ASSERT_EQ(re.IesimaComputadora(3), c2);
    ASSERT_EQ(re.IesimaComputadora(4), c1);
    re.Conectar(c1, 1, c2, 5);
    re.Conectar(c3, 65, c2, 3);
    re.Conectar(c1, 2, c3, 7);
    re.Conectar(c3, 1, c4, 9);
    ASSERT(re.conectadas(c1, c2));
    ASSERT(re.conectadas(c2, c1));
    ASSERT(re.conectadas(c2, c3));
    ASSERT(re.conectadas(c3, c2));
    ASSERT(re.conectadas(c1, c3));
    ASSERT(re.conectadas(c3, c1));
    ASSERT_EQ(re.conectadas(c1, c4), false);
    ASSERT_EQ(re.conectadas(c2, c5), false);
    ASSERT_EQ(re.conectadas(c2, c4), false); 
    ASSERT_EQ(re.IntefazUsada(c1, c2), 1);
    ASSERT_EQ(re.IntefazUsada(c2, c3), 3);
    ASSERT_EQ(re.IntefazUsada(c2, c1), 5);
    ASSERT_EQ(re.IntefazUsada(c3, c2), 65);
    ASSERT_EQ(re.IntefazUsada(c3, c1), 7);
    ASSERT_EQ(re.IntefazUsada(c1, c3), 2);

}


void test_dcnet(){
	Driver re = Driver();
    Computadora c1 = "Manu";
    Conj<Interfaz> inter1 = Conj<Interfaz>();
    inter1.Agregar(1);
    inter1.Agregar(2);
    re.AgregarComputadora(c1, inter1);
    Computadora c2 = "Lu";
    Conj<Interfaz> inter2 = Conj<Interfaz>();
    inter2.Agregar(5);
    inter2.Agregar(6);
    inter2.Agregar(3);
    re.AgregarComputadora(c2, inter2);
    Computadora c3 = "Fran";
    Conj<Interfaz> inter3 = Conj<Interfaz>();
    inter3.Agregar(1);
    inter3.Agregar(65);
    inter3.Agregar(7);
    re.AgregarComputadora(c3, inter3);
    Computadora c4 = "Minion";
    Conj<Interfaz> inter4 = Conj<Interfaz>();
    inter4.Agregar(9);
    re.AgregarComputadora(c4, inter4);
    Computadora c5 = "Uri";
    Conj<Interfaz> inter5 = Conj<Interfaz>();
    inter5.Agregar(5);
    inter5.Agregar(6);
    inter5.Agregar(45);
    re.AgregarComputadora(c5, inter5);
    re.Conectar(c1, 1, c2, 5);
    re.Conectar(c3, 65, c2, 3);
   // re.Conectar(c1, 2, c3, 7);
    //re.Conectar(c3, 1, c4, 9);

    // Ya se creo la red
    // Creamos una nueva DCNet

    Paquete p1 = 1;
    re.CrearPaquete(c1, c3, 1);
    ASSERT(re.CantidadNodosRecorridosPor(p1) == 1);
    re.AvanzarSegundo();
    ASSERT(re.laQueMasEnvio() == c1);
    ASSERT(re.CantidadNodosRecorridosPor(p1) == 2);
    ASSERT(re.CantidadEnEsperaEn(c2) == 1);
    re.AvanzarSegundo();
    re.AvanzarSegundo();
    ASSERT_EQ(re.CantidadEnviadosPor(c1), 1);

    // // creo paquetes 2, 3 y 4. c1 mando 1 paquete, c2 mando un paquete. No hay paquetes en transito.

    Paquete p2 = 2;
    re.CrearPaquete(c2, c3, 2);
    Paquete p3 = 3;
    re.CrearPaquete(c3, c1, 3);
    Paquete p4 = 4;
    re.CrearPaquete(c3, c1, 4);

    // veamos que en c3 estan en espera los dos paquetes que tienen como origen a c3, es decir, p3 y p4.

    ASSERT(re.CantidadEnEsperaEn(c3) == 2);
    bool estaElPaquete3 = false;
    bool estaElPaquete4 = false;
    for (int i =0; i < re.CantidadEnEsperaEn(c3) ; i++) { 
        if (re.IesimoEnEsperaEn(c3, i) == p3){
        	estaElPaquete3 = true;
        }
        if (re.IesimoEnEsperaEn(c3, i) == p4){
        	estaElPaquete4 = true;
        }
    }
    ASSERT(estaElPaquete3);
    ASSERT(estaElPaquete4);
   
    re.AvanzarSegundo();

    // p2 llego a destino. p4 esta en c2. p3 esta en c3.
    //c1 mando 1 paquete. c2 mando 2 paquetes.  c3 mando 1 paquete.


    ASSERT(re.CantidadNodosRecorridosPor(p3) == 1);
    ASSERT(re.CantidadNodosRecorridosPor(p4) == 2);


    // cantidad de paquetes enviados de cada compu
    

   	ASSERT(re.CantidadEnviadosPor(c1) == 1);
    ASSERT(re.CantidadEnviadosPor(c2) == 2);
    ASSERT(re.CantidadEnviadosPor(c3) == 1);


    // quienes son los paquetes en espera de las compus.

    ASSERT(re.CantidadEnEsperaEn(c1) == 0);
    ASSERT(re.CantidadEnEsperaEn(c2) == 1);

    estaElPaquete4 = false;

    for (int i =0; i < re.CantidadEnEsperaEn(c2) ; i++) { 
        if (re.IesimoEnEsperaEn(c2, i) == p4){
        	estaElPaquete4 = true;
        }
    }

    ASSERT(estaElPaquete4);

    ASSERT(re.CantidadEnEsperaEn(c3) == 1);

    estaElPaquete3 = false;

    for (int i =0; i < re.CantidadEnEsperaEn(c3) ; i++) { 
        if (re.IesimoEnEsperaEn(c3, i) == p3){
        	estaElPaquete3 = true;
        }
    }

    ASSERT(estaElPaquete3);

    //la que mas envio es c2.

    ASSERT(re.laQueMasEnvio() == c2);

    re.AvanzarSegundo();

    // p4 llego a destino. p3 esta en c2
    // c1 envio 1 paq. c2 envio 3 paquetes. c3 envio 2 paquetes

    ASSERT(re.CantidadNodosRecorridosPor(p3) == 2);

    ASSERT_EQ(re.CantidadEnviadosPor(c1), 1);
    ASSERT_EQ(re.CantidadEnviadosPor(c2), 3);
    ASSERT_EQ(re.CantidadEnviadosPor(c3), 2);

    ASSERT(re.CantidadEnEsperaEn(c1) == 0);
    ASSERT(re.CantidadEnEsperaEn(c2) == 1);
    ASSERT(re.CantidadEnEsperaEn(c3) == 0);


    estaElPaquete3 = false;

    for (int i =0; i < re.CantidadEnEsperaEn(c2) ; i++) { 
        if (re.IesimoEnEsperaEn(c2, i) == p3){
        	estaElPaquete3 = true;
        }
    }

    ASSERT(estaElPaquete3);



    ASSERT(re.laQueMasEnvio() == c2);

    re.AvanzarSegundo();

    // No hay paquetes en transito.
    // c1 envio 1 paq. c2 envio 4 paquetes. c3 envio 2 paquetes.

    ASSERT_EQ(re.CantidadEnviadosPor(c1), 1);
    ASSERT_EQ(re.CantidadEnviadosPor(c2), 4);
    ASSERT_EQ(re.CantidadEnviadosPor(c3), 2);

	ASSERT(re.CantidadEnEsperaEn(c1) == 0);
    ASSERT(re.CantidadEnEsperaEn(c2) == 0);
    ASSERT(re.CantidadEnEsperaEn(c3) == 0);

    ASSERT(re.laQueMasEnvio() == c2);


}

void test_dcnet_ejemplo() {
    Conj<Interfaz> conjIc1;
    Conj<Interfaz> conjIc2;
    Conj<Interfaz> conjIc3;

    conjIc1.Agregar(1);
    conjIc1.Agregar(2);
    conjIc1.Agregar(3);

    conjIc2.Agregar(1);
    conjIc2.Agregar(2);
    conjIc2.Agregar(3);

    conjIc3.Agregar(1);
    conjIc3.Agregar(2);

    Computadora c1 = "dc.uba.ar";
    Computadora c2 = "uba.ar";
    Computadora c3 = "dm.uba.ar";

    Driver dcnet;

    dcnet.AgregarComputadora(c1, conjIc1);
    dcnet.AgregarComputadora(c2, conjIc2);
    dcnet.AgregarComputadora(c3, conjIc3);

    // ejemplo - Indexado en 0
    Interfaz i1 = dcnet.IesimaInterfazDe(c1, 0);
    Interfaz i2 = dcnet.IesimaInterfazDe(c2, 2);

    dcnet.Conectar(c1, i1, c2, i2);
    dcnet.CrearPaquete(c1, c2, 3);
    dcnet.AvanzarSegundo();

    ASSERT_EQ(dcnet.laQueMasEnvio(), c1);

}

int main(int argc, char **argv)
{
    RUN_TEST(test_dcnet_ejemplo);
    RUN_TEST(test_red);
    RUN_TEST(test_dcnet);


    /******************************************************************
     * TODO: escribir casos de test exhaustivos para todas            *
     * las funcionalidades del módulo.                                *
     * La interacción con el TAD DcNet se debe hacer exclusivamente  *
     * a través de la interfaz del driver.                            *
     ******************************************************************/
   

    return 0;
}
