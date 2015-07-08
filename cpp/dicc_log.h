#ifndef DICC_LOG_H
#define DICC_LOG_H

#include <iostream>
#include "./aed2.h"
#include "./ab.h"

using namespace aed2;
using std::cout;
using std::endl;

namespace tp3 {

template<class K, class S>
class diccLog{
 private:
    struct entrada {       // Cada uno de los nodos del árbol es de este tipo
        K clave;             // La clave almacenada
        S significado;       // El significado correspondiente
        Nat altSubarbol;     // Altura del subárbol del cual el nodo es raíz
        ab<entrada>* padre;  // Puntero al padre del nodo. Es NULL si y solo
                                // si el nodo es la raíz del árbol.

        entrada(K k, S s, Nat h, ab<entrada>* p) :
            clave(k), significado(s), altSubarbol(h), padre(p) {}
    };

    ab<entrada>* arbol;     // El diccionario se representa mediante
                               // un árbol binario con invariante de AVL.

    ab<entrada>* buscar(const K& k, ab<entrada>*& padre) const;
        // Devuelve un puntero al nodo cuya clave es k, o en su defecto
           // al nodo nil cuya clave debería ser k si estuviera definida.
           // Coloca en p la dirección del padre de dicho nodo.
    void recalcularAltura(ab<entrada>*);
        // Recalcula el valor del campo "altSubarbol" del nodo
           // cuya dirección recibe por parámetro.
    int factorDeBalanceo(ab<entrada>*) const;
        // Calcula el factor de balanceo del nodo
           // cuya dirección recibe por parámetro.
    ab<entrada>* rotarAIzquierda(ab<entrada>*);
        // Realiza una rotación entre el nodo cuya dirección recibe
           // por parámetro y su hijo izquierdo.
    ab<entrada>* rotarADerecha(ab<entrada>*);
        // Realiza una rotación entre el nodo cuya dirección recibe
           // por parámetro y su hijo derecho.
    void rebalancearArbol(ab<entrada>*);
        // Restablece el invariante de representación de AVL
           // de forma ascendente, a partir del nodo cuya dirección
           // recibe por parámetro.
    void reemplazarHijo(ab<entrada>*, ab<entrada>*, ab<entrada>*);
        // Si uno de los hijos del primer parámetro es igual al segundo
           // parámetro, lo reemplaza por el tercer parámetro.
    bool estaBalanceado(ab<entrada>* a) const;
        // Función de debug que decide si el árbol cuya raíz recibe
           // por parámetro cumple el invariante de representación de AVL.
    bool estaBalanceado() const;
        // Función de debug que decide si el árbol subyacente está balanceado.
    Lista<K> preorderClaves() const;
        // Función de debug que devuelve las claves del diccionario en preorder.

 public:
    diccLog();                            // Construye un diccionario vacío.
    diccLog(const diccLog<K, S>& otro);   // Construye un diccionario por copia.
    ~diccLog();                           // Destruye un diccionario.

    diccLog<K, S>& operator=(const diccLog<K, S>& otro);
        // Operador de asignación por copia

    bool definido(const K&) const;     // Decide si una clave está definida.
    void definir(const K&, const S&);  // Define el significado de una clave.
    S& obtener(const K&) const;        // Devuelve el significado de una clave.
    void borrar(const K&);             // Elimina una clave del diccionario.
    Nat cantClaves() const;            // Devuelve la cantidad de claves
                                          // del diccionario.
};

// Constructores

template<class K, class S>
diccLog<K, S>::diccLog() :
    arbol(new ab<entrada>()) {}

template<class K, class S>
diccLog<K, S>::diccLog(const diccLog<K, S>& otro) :
    arbol(new ab<entrada>(*(otro.arbol))) {}

// Destructor

template<class K, class S>
diccLog<K, S>::~diccLog() {
    delete arbol;
}

// Métodos públicos

template<class K, class S>
diccLog<K, S>& diccLog<K, S>::operator=(const diccLog<K, S>& otro) {
    delete arbol;
    arbol = otro.arbol == NULL ? NULL : new ab<entrada>(*(otro.arbol));
    return *this;
}

template<class K, class S>
bool diccLog<K, S>::definido(const K& k) const {
    ab<entrada>* padre;
    ab<entrada>* lugar = buscar(k, padre);
    return !(lugar->esNil());
}

template<class K, class S>
void diccLog<K, S>::definir(const K& k, const S& s) {
    // cout << endl << "Definiendo clave " << k << "." << endl;  // DEBUG
    ab<entrada>* padre;
    ab<entrada>* lugar = buscar(k, padre);
    if (!(lugar->esNil())) {
        // La clave ya estaba definida, reemplazo el significado por el nuevo
        // cout << "    Clave ya definida, reeplazando nodo." << endl;  // DEBUG
        lugar->raiz().significado = S(s);
    } else {
        // cout << "    Clave no definida, insertando nuevo nodo." << endl;  // DEBUG
        if (padre == NULL) {
            // El árbol estaba vacío, agrego la nueva clave como árbol principal
            delete arbol;
            arbol = new ab<entrada>(new ab<entrada>(),
                entrada(k, s, 1, NULL), new ab<entrada>());
            // En este caso no hace falta rebalancear
        } else {
            // El árbol tenía nodos. Creo el nuevo nodo a insertar.
            ab<entrada>* nuevo = new ab<entrada>(new ab<entrada>(),
                entrada(k, s, 1, padre), new ab<entrada>());
            if (k < padre->raiz().clave) {
                delete padre->izq();
                padre->izq(nuevo);
            } else {
                delete padre->der();
                padre->der(nuevo);
            }
            // Ahora sí hay que rebalancear
            rebalancearArbol(padre);
        }
    }
}

template<class K, class S>
S& diccLog<K, S>::obtener(const K& k) const {
    assert(definido(k));  // DEBUG - ASSERTION
    ab<entrada>* padre;
    ab<entrada>* lugar = buscar(k, padre);
    return lugar->raiz().significado;
}

template<class K, class S>
void diccLog<K, S>::borrar(const K& k) {
    // cout << endl << "Borrando clave " << k << "." << endl;  // DEBUG
    assert(definido(k));  // DEBUG - ASSERTION
    ab<entrada>* padre;
    ab<entrada>* lugar = buscar(k, padre);
    if ((lugar->izq() == NULL || lugar->izq()->esNil()) &&
        (lugar->der() == NULL || lugar->der()->esNil())) {
        // El nodo no tiene hijos. Puedo borrarlo directamente
        if (padre == NULL) {
            // Era el único nodo en el árbol. Ahora quedó vacío.
            arbol = new ab<entrada>();
        } else {
            // Arreglo el link del padre
            reemplazarHijo(padre, lugar, new ab<entrada>());
        }
        delete lugar;
        rebalancearArbol(padre);
    } else if (lugar->der() == NULL || lugar->der()->esNil()) {
        // El nodo tiene solo hijo izquierdo
        lugar->izq()->raiz().padre = padre;
        if (padre == NULL) {
            // Era la raíz del árbol. Ahora lo es su hijo izquierdo
            arbol = lugar->izq();
        } else {
            // Arreglo el link del padre
            reemplazarHijo(padre, lugar, lugar->izq());
        }
        lugar->izq(NULL);
        delete lugar;
        rebalancearArbol(padre);
    } else if (lugar->izq() == NULL || lugar->izq()->esNil()) {
        // El nodo tiene solo hijo derecho
        lugar->der()->raiz().padre = padre;
        if (padre == NULL) {
            // Era la raíz del árbol. Ahora lo es su hijo derecho
            arbol = lugar->der();
        } else {
            // Arreglo el link del padre
            reemplazarHijo(padre, lugar, lugar->der());
        }
        lugar->der(NULL);
        delete lugar;
        rebalancearArbol(padre);
    } else {
        // El nodo tiene ambos hijos
        ab<entrada>* reemplazo = lugar->der();
        if (reemplazo->izq() == NULL || reemplazo->izq()->esNil()) {
            // Caso en el que el reemplazo es hijo directo del nodo a borrar
            // Cuelgo al reemplazo de donde corresponde
            reemplazo->raiz().padre = padre;
            if (padre == NULL) {
                // Era la raíz del árbol. Ahora lo es el reemplazo
                arbol = reemplazo;
            } else {
                // Arreglo el link del padre
                reemplazarHijo(padre, lugar, reemplazo);
            }
            // Cuelgo del reemplazo al hijo izquierdo del nodo a reemplazar
            delete reemplazo->izq();
            reemplazo->izq(lugar->izq());
            if (reemplazo->izq() != NULL && !(reemplazo->izq()->esNil())) {
                reemplazo->izq()->raiz().padre = reemplazo;
            }
            // Borro el nodo reemplazado
            lugar->izq(NULL);
            lugar->der(NULL);
            delete lugar;
            rebalancearArbol(reemplazo);
        } else {
            // Caso en el que el reemplazo NO es hijo directo del nodo a reemplazar
            // Bajo hasta encontrar el mínimo del subárbol derecho
            while (reemplazo->izq() != NULL && !(reemplazo->izq()->esNil())) {
                reemplazo = reemplazo->izq();
            }
            ab<entrada>* padreReemplazo = reemplazo->raiz().padre;
            // Arreglo link entre padre del reemplazo y su subárbol derecho
            padreReemplazo->izq(reemplazo->der());
            if (padreReemplazo->izq() != NULL && !(padreReemplazo->izq()->esNil())) {
                padreReemplazo->izq()->raiz().padre = padreReemplazo;
            }
            // Cuelgo al reemplazo de donde corresponde
            reemplazo->raiz().padre = padre;
            if (padre == NULL) {
                // Era la raíz del árbol. Ahora lo es el reemplazo
                arbol = reemplazo;
            } else {
                // Arreglo el link del padre
                reemplazarHijo(padre, lugar, reemplazo);
            }
            // Le cuelgo al reemplazo los hijos del nodo a reemplazar
            delete reemplazo->izq();
            reemplazo->izq(lugar->izq());
            if (reemplazo->izq() != NULL && !(reemplazo->izq()->esNil())) {
                reemplazo->izq()->raiz().padre = reemplazo;
            }
            reemplazo->der(lugar->der());
            if (reemplazo->der() != NULL && !(reemplazo->der()->esNil())) {
                reemplazo->der()->raiz().padre = reemplazo;
            }
            // Borro el nodo reemplazado
            lugar->izq(NULL);
            lugar->der(NULL);
            delete lugar;
            rebalancearArbol(padreReemplazo);
        }
    }
}

template<class K, class S>
Nat diccLog<K, S>::cantClaves() const {
    return arbol->cantNodos();
}

// Métodos privados

template<class K, class S>
ab<typename diccLog<K, S>::entrada>* diccLog<K, S>::buscar(
    const K& k, ab<entrada>*& padre) const {
    padre = NULL;
    ab<entrada>* actual = arbol;
    while (!(actual->esNil()) && actual->raiz().clave != k) {
        padre = actual;
        if (k <= actual->raiz().clave) {
            actual = actual->izq();
        } else {
            actual = actual->der();
        }
    }
    return actual;
}

template<class K, class S>
void diccLog<K, S>::recalcularAltura(ab<entrada>* a) {
    // assert(a != NULL && !(a->esNil()));  // DEBUG
    // cout << "  - Recalculando altura de nodo con raíz " << a->raiz().clave << "." << endl;  // DEBUG
    if (!(a->izq()->esNil()) && !(a->der()->esNil())) {
        a->raiz().altSubarbol = 1 + max(
            a->izq()->raiz().altSubarbol, a->der()->raiz().altSubarbol);
        // cout << "    Ambos hijos no nulos.";  // DEBUG
    } else if (!(a->izq()->esNil())) {
        a->raiz().altSubarbol = 1 + a->izq()->raiz().altSubarbol;
        // cout << "    Hijo derecho nulo.";  // DEBUG
    } else if (!(a->der()->esNil())) {
        a->raiz().altSubarbol = 1 + a->der()->raiz().altSubarbol;
        // cout << "    Hijo izquierdo nulo.";  // DEBUG
    } else {
        a->raiz().altSubarbol = 1;
        // cout << "    Ambos hijos nulos.";  // DEBUG
    }
    // cout << " Altura calculada: " << a->raiz().altSubarbol << "." << endl;  // DEBUG
}

template<class K, class S>
int diccLog<K, S>::factorDeBalanceo(ab<entrada>* a) const {
    // assert(a != NULL);  // DEBUG
    int altIzq = a->izq()->esNil() ? 0 : a->izq()->raiz().altSubarbol;
    int altDer = a->der()->esNil() ? 0 : a->der()->raiz().altSubarbol;
    return altDer - altIzq;
}

template<class K, class S>
ab<typename diccLog<K, S>::entrada>* diccLog<K, S>::rotarAIzquierda(ab<entrada>* a) {
    // assert(!(a->esNil()));  // DEBUG
    // assert(a->der() != NULL && !(a->der()->esNil()));  // DEBUG
    // cout << "  - Rotación a IZQUIERDA. Baja nodo con clave " << a->raiz().clave;  // DEBUG
    // cout << ". Sube nodo con clave " << a->der()->raiz().clave << endl;  // DEBUG
    ab<entrada>* b = a->der();
    a->der(b->izq());
    b->izq(a);
    b->raiz().padre = a->raiz().padre;
    a->raiz().padre = b;
    if (a->der() != NULL && !(a->der()->esNil())) {
        a->der()->raiz().padre = a;
    }
    if (a->izq() != NULL && !(a->izq()->esNil())) {
        a->izq()->raiz().padre = a;
    }
    recalcularAltura(a);
    recalcularAltura(b);
    return b;
}

template<class K, class S>
ab<typename diccLog<K, S>::entrada>* diccLog<K, S>::rotarADerecha(ab<entrada>* a) {
    // assert(!(a->esNil()));  // DEBUG
    // assert(a->izq() != NULL && !(a->izq()->esNil()));  // DEBUG
    // cout << "  - Rotación a DERECHA. Baja nodo con clave " << a->raiz().clave << endl;  // DEBUG
    // cout << "Sube nodo con clave " << a->izq()->raiz().clave << endl;  // DEBUG
    ab<entrada>* b = a->izq();
    a->izq(b->der());
    b->der(a);
    b->raiz().padre = a->raiz().padre;
    a->raiz().padre = b;
    if (a->der() != NULL && !(a->der()->esNil())) {
        a->der()->raiz().padre = a;
    }
    if (a->izq() != NULL && !(a->izq()->esNil())) {
        a->izq()->raiz().padre = a;
    }
    recalcularAltura(a);
    recalcularAltura(b);
    return b;
}

template<class K, class S>
void diccLog<K, S>::rebalancearArbol(ab<entrada>* a) {
    // cout << endl << "Rebalanceando árbol." << endl;  // DEBUG
    ab<entrada>* p = a;
    while (p != NULL) {
        ab<entrada>* pAux;
        recalcularAltura(p);
        int fdb1 = factorDeBalanceo(p);
        if (fdb1 == 2 || fdb1 == -2) {
            if (fdb1 == 2) {
                ab<entrada>* q = p->der();
                int fdb2 = factorDeBalanceo(q);
                if (fdb2 == 1 || fdb2 == 0) {
                    pAux = rotarAIzquierda(p);
                } else if (fdb2 == -1) {
                    q = rotarADerecha(q);
                    p->der(q);
                    pAux = rotarAIzquierda(p);
                }
            } else if (fdb1 == -2) {
                ab<entrada>* q = p->izq();
                int fdb2 = factorDeBalanceo(q);
                if (fdb2 == -1 || fdb2 == 0) {
                    pAux = rotarADerecha(p);
                } else {
                    q = rotarAIzquierda(q);
                    p->izq(q);
                    pAux = rotarADerecha(p);
                }
            }
            if (pAux->raiz().padre != NULL) {
                reemplazarHijo(pAux->raiz().padre, p, pAux);
            } else {
                arbol = pAux;
            }
            p = pAux->raiz().padre;
        } else {
            p = p->raiz().padre;
        }
    }
    assert(estaBalanceado());  // DEBUG
}

template<class K, class S>
void diccLog<K, S>::reemplazarHijo(ab<entrada>* padre, ab<entrada>* hijo,
    ab<entrada>* reemplazo) {
    assert(padre != NULL);
    if (padre->izq() == hijo) {
        // Era hijo izquierdo de su padre
        padre->izq(reemplazo);
    } else {
        // Era hijo derecho de su padre
        padre->der(reemplazo);
    }
}

template<class K, class S>
bool diccLog<K, S>::estaBalanceado(ab<entrada>* a) const {
    if (a == NULL || a->esNil()) {
        // cout << "  - Visitando nodo vacío (está balanceado)." << endl;  // DEBUG
        return true;
    } else if (factorDeBalanceo(a) == -1 ||
        factorDeBalanceo(a) == 0 ||
        factorDeBalanceo(a) == 1) {
        // cout << "  - Visitando nodo con clave "<< a->raiz().clave << "." << endl;  // DEBUG
        // cout << "    Factor de balanceo: " << factorDeBalanceo(a) << " (está balanceado)." << endl;  // DEBUG
        // cout << "    Altura de subárbol: " << a->raiz().altSubarbol << "." << endl;  // DEBUG
        return estaBalanceado(a->izq()) && estaBalanceado(a->der());
    } else {
        // cout << "  - Visitando nodo con clave "<< a->raiz().clave << "." << endl;  // DEBUG
        // cout << "    Factor de balanceo: " << factorDeBalanceo(a) << " (no está balanceado)." << endl;  // DEBUG
        // cout << "    Altura de subárbol: " << a->raiz().altSubarbol << "." << endl;  // DEBUG
        return false;
    }
}

template<class K, class S>
bool diccLog<K, S>::estaBalanceado() const {
    // cout << endl << "Verificando si el árbol está balanceado." << endl;  // DEBUG
    return estaBalanceado(arbol);
}

template<class K, class S>
Lista<K> diccLog<K, S>::preorderClaves() const {
    Lista<entrada> entradas = arbol->preorder();
    Lista<K> res;
    for (typename Lista<entrada>::Iterador it = entradas.CrearIt(); it.HaySiguiente(); it.Avanzar()) {
        res.AgregarAtras(it.Siguiente().clave);
    }
    return res;
}

}  // namespace tp3

#endif  // DICC_LOG_H
