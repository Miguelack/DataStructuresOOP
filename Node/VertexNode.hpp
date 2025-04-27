#ifndef NODOVERT_H
#define NODOVERT_H

#include "AdjacentNode.hpp"

template <typename T> class VertexNode{

    T data; /* dato contenido por el grafo */
    VertexNode<T>* nextVertex; /* puntero al siguiente nodo vertice */
    AdjacentNode<T>* nextAdjacent; /* puntero al primer nodo adyacente */

public:
    /* constructores publicos de la clase: permiten instanciar un objeto desde determinadas condiciones */
    VertexNode() : data( T() ), nextVertex(NULL), nextAdjacent(NULL){};
    VertexNode(T newData) : data(newData), nextVertex(NULL), nextAdjacent(NULL){};
    VertexNode(T newData, VertexNode<T>* newNextVertex): data(newData), nextVertex(newNextVertex), nextAdjacent(NULL){};
    VertexNode(T newData,AdjacentNode<T>* nextAdjacent): data(newData), nextVertex(NULL), nextAdjacent(nextAdjacent){};
    VertexNode(T newData,VertexNode<T>* newNextVertex,AdjacentNode<T>* nextAdjacent): data(newData), nextVertex(newNextVertex), nextAdjacent(nextAdjacent){};

    /* metodos getters: permiten obtener los atributos privados */
    const T& getData() const { return data; }; 
    VertexNode<T> *getNextVertex() const { return nextVertex; };
    AdjacentNode<T> *getNextAdjacent() const { return nextAdjacent; };

    /* metodos setters: permiten modificar los atributos privados 
    NOTA: incluye seguridad para evitar autoreferenciado */
    void setData(T newData){ 
        if(data != newData){ /* evita la asignacion de un dato repetido */
        data = newData;
        }
    };
    void setNextVertex(VertexNode<T> *newNextVertex){ 
        if(newNextVertex != nextVertex && newNextVertex != this){
        nextVertex = newNextVertex;
        }
    };
    void setNextAdjacent(AdjacentNode<T> *newNextAdjacent){ 
        if(newNextAdjacent != nextAdjacent){
        nextAdjacent = newNextAdjacent; 
        }
    };

private:
    /* elimina las operaciones de copia para prevenir un uso incorrecto de memoria e incorrecto manejo de punteros*/
    VertexNode(const VertexNode&);
    VertexNode& operator=(const VertexNode&);
};

#endif