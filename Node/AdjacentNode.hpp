#ifndef ADJACENTNODE_H
#define ADJACENTNODE_H
#include <cstddef>

template <typename T> class VertexNode;

template <typename T> class AdjacentNode{

double weight; /* peso de la arista hacia el vertice almacenado, tambien conocido como su costo */
VertexNode<T>* data; /* puntero al vertice adyacente, indica con cual nodo forma una arista este objeto*/
AdjacentNode<T>* next;/* puntero al siguiente nodo adyacente, emula comportamiento de una lista */

public:

    /* constructores publicos de la clase: permiten instanciar un objeto desde determinadas condiciones */
    AdjacentNode() : weight(0.0), data(NULL), next(NULL){}; /* constructor predeterminado, sirve para construir un nodo nuevo vacio*/
    AdjacentNode(VertexNode<T> *newData, double newWeight)
      : weight(newWeight), data(newData), next(NULL){};

    /* destructor: no elimina la data porque es un apuntador a un vertice perteneciente al grafo */
    ~AdjacentNode(){
        next = NULL;
    }

    /* metodos getters: permiten obtener los atributos privados */
    double getWeight() const { return weight; };
    VertexNode<T> *getData() const { return data; };
    AdjacentNode<T> *getNext() const { return next; };

    /* metodos setters: permiten modificar los atributos privados 
    NOTA: incluye seguridad para evitar autoreferenciado */
    void setWeight(double newWeight) {
        if(newWeight >= 0){ /* evita la asignacion de un peso negativo */
        weight = newWeight;
        }
    };
    void setData(VertexNode<T> *newData){ 
        if(data != newData){ /* evita la asignacion de un dato repetido */
        data = newData;
        }
    };
    void setNext(AdjacentNode<T> * newNext) {
        if(newNext != next && newNext != this){ /* evita la re-asignacion y la auto-referencia */
        next = newNext;
        }
    };

private:
    /* elimina las operaciones de copia para prevenir un uso incorrecto de memoria e incorrecto manejo de punteros*/
    AdjacentNode(const AdjacentNode&);
    AdjacentNode& operator=(const AdjacentNode&);
};
#endif
