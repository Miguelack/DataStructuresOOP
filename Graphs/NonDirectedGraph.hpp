#ifndef NONDIRECTEDGRAPH_H
#define NONDIRECTEDGRAPH_H

#include "Graph.hpp"
#include <map>

/*
 * @brief Clase derivada para representar un grafo no dirigido.
 * Hereda de la clase base Graph<T> e implementa la lógica específica
 * para aristas no dirigidas, donde una conexión entre dos vértices
 * es bidireccional.
 *
 * @tparam T El tipo de dato almacenado en los vértices del grafo.
 */
template <typename T>
class NonDirectedGraph : public Graph<T> {
public:
    /*
     * @brief Constructor predeterminado de la clase NonDirectedGraph.
     * Llama al constructor de la clase base Graph<T>.
     */
    NonDirectedGraph() : Graph<T>() {}

    /*
     * @brief Constructor de copia de la clase NonDirectedGraph.
     * Llama al constructor de copia de la clase base Graph<T> y luego copia las aristas.
     *
     * @param other El grafo no dirigido del cual se creará una copia.
     */
    NonDirectedGraph(const NonDirectedGraph<T>& other) : Graph<T>(other) {
        /* Mapa para almacenar la correspondencia entre los nodos del grafo original y los nodos del nuevo grafo. */
        std::map<VertexNode<T>*, VertexNode<T>*> nodeMap;
        /* Recorre los nodos del grafo original y los busca en el nuevo grafo para llenar el mapa. */
        VertexNode<T>* currentOther = other.firstNode;
        VertexNode<T>* currentThis = this->firstNode;
        while (currentOther != NULL && currentThis != NULL) {
            nodeMap[currentOther] = currentThis;
            currentOther = currentOther->getNextVertex();
            currentThis = currentThis->getNextVertex();
        }
        /* Llama a la función para copiar las aristas. */
        copyEdges(other, nodeMap);
        /* Copia el contador de aristas. */
        this->edgeCount = other.edgeCount;
    }

    /*
     * @brief Destructor virtual de la clase NonDirectedGraph.
     * Llama al destructor virtual de la clase base Graph<T>, asegurando
     * la correcta liberación de memoria.
     */
    virtual ~NonDirectedGraph() {}

    /*
     * @brief Agrega un nuevo vértice al grafo no dirigido.
     * Llama al método addToMappings de la clase base y crea un nuevo VertexNode.
     *
     * @param data El dato a almacenar en el nuevo vértice.
     */
    virtual void addVertex(const T& data) {
        /* Verifica si el vértice ya existe para evitar duplicados. */
        if (this->findVertex(data) == NULL) {
            /* Agrega el dato al sistema de mapeo de la clase base. */
            this->addToMappings(data);
            /* Crea un nuevo nodo de vértice. */
            VertexNode<T>* newNode = new (std::nothrow) VertexNode<T>(data, this->firstNode);
            /* Si la asignación de memoria fue exitosa. */
            if (newNode) {
                /* Actualiza el puntero al primer nodo. */
                this->firstNode = newNode;
                /* Incrementa el contador de vértices. */
                this->vertexCount++;
            } else {
                /* Manejo de error si falla la asignación de memoria. */
                /* Podrías lanzar una excepción o manejar el error de otra manera. */
                /* En este caso, simplemente no se agrega el vértice. */
            }
        }
        /* Si el vértice ya existe, no se hace nada. */
    }

    virtual void removeVertex(const T& data) {
        /* Busca el nodo del vértice a eliminar. */
        VertexNode<T>* toRemove = this->findVertex(data);
        if (toRemove) {
            /* 1. Elimina las aristas que tienen a este vértice como destino en otros vértices. */
            VertexNode<T>* currentVertex = this->firstNode;
            while (currentVertex != NULL) {
                removeEdgeInternal(currentVertex, toRemove);
                currentVertex = currentVertex->getNextVertex();
            }
    
            /* 2. Elimina las aristas adyacentes al vértice a eliminar y cuenta las aristas eliminadas. */
            AdjacentNode<T>* currentAdjacent = toRemove->getNextAdjacent();
            int deletedEdges = 0;
            while (currentAdjacent != NULL) {
                AdjacentNode<T>* nextAdjacent = currentAdjacent->getNext();
                delete currentAdjacent;
                currentAdjacent = nextAdjacent;
                deletedEdges++;
            }
            toRemove->setNextAdjacent(NULL);
            this->edgeCount -= deletedEdges;
    
            /* 3. Elimina el nodo del vértice de la lista enlazada de vértices. */
            if (toRemove == this->firstNode) {
                this->firstNode = toRemove->getNextVertex();
            } else {
                VertexNode<T>* prevVertex = this->firstNode;
                while (prevVertex != NULL && prevVertex->getNextVertex() != toRemove) {
                    prevVertex = prevVertex->getNextVertex();
                }
                if (prevVertex != NULL) {
                    prevVertex->setNextVertex(toRemove->getNextVertex());
                }
            }
    
            /* 4. Elimina el vértice del sistema de mapeo y decrementa el contador de vértices. */
            this->removeFromMappings(data);
            this->vertexCount--;
            delete toRemove;
        }
    }
    

    /*
     * @brief Agrega una arista no dirigida entre dos vértices del grafo.
     * Crea nodos adyacentes en las listas de adyacencia de ambos vértices.
     *
     * @param source El dato del vértice de origen.
     * @param destination El dato del vértice de destino.
     * @param weight El peso de la arista (por defecto es 1.0).
     */
    virtual void addEdge(const T& source, const T& destination, double weight = 1.0) {
        /* Busca los nodos de los vértices de origen y destino. */
        VertexNode<T>* sourceVertex = this->findVertex(source);
        VertexNode<T>* destinationVertex = this->findVertex(destination);

        /* Verifica si ambos vértices existen. */
        if (sourceVertex && destinationVertex) {
            /* Verifica si la arista ya existe para evitar duplicados. */
            if (!this->findAdjacent(sourceVertex, destination) && !this->findAdjacent(destinationVertex, source)) {
                /* Crea un nuevo nodo adyacente para el destino en la lista de adyacencia del origen. */
                AdjacentNode<T>* newAdjacentSource = new (std::nothrow) AdjacentNode<T>(destinationVertex, weight);
                if (newAdjacentSource) {
                    newAdjacentSource->setNext(sourceVertex->getNextAdjacent());
                    sourceVertex->setNextAdjacent(newAdjacentSource);
                } else {
                    /* Manejo de error si falla la asignación de memoria. */
                    return;
                }

                /* Crea un nuevo nodo adyacente para el origen en la lista de adyacencia del destino. */
                AdjacentNode<T>* newAdjacentDestination = new (std::nothrow) AdjacentNode<T>(sourceVertex, weight);
                if (newAdjacentDestination) {
                    newAdjacentDestination->setNext(destinationVertex->getNextAdjacent());
                    destinationVertex->setNextAdjacent(newAdjacentDestination);
                } else {
                    /* Manejo de error: podría ser necesario deshacer la adición anterior. */
                    delete newAdjacentSource;
                    return;
                }
                /* Incrementa el contador de aristas (ya que es no dirigido, contamos una sola vez). */
                this->edgeCount++;
            }
            /* Si la arista ya existe, no se hace nada. */
        }
        /* Si alguno de los vértices no existe, no se puede agregar la arista. */
    }

    /*
     * @brief Elimina una arista no dirigida entre dos vértices del grafo.
     * Busca y elimina los nodos adyacentes correspondientes en las listas de adyacencia de ambos vértices.
     *
     * @param source El dato del vértice de origen.
     * @param destination El dato del vértice de destino.
     */
    virtual void removeEdge(const T& source, const T& destination) {
        VertexNode<T>* srcNode = this->findVertex(source);
        VertexNode<T>* destNode = this->findVertex(destination);
    
        if (srcNode && destNode) {
            /* Intenta eliminar la arista desde la fuente hacia el destino. */
            bool removedFromSource = this->removeEdgeInternal(srcNode, destNode);
            /* Intenta eliminar la arista desde el destino hacia la fuente. */
            bool removedFromDest = this->removeEdgeInternal(destNode, srcNode);
    
            /* Si al menos una de las eliminaciones fue exitosa, decrementa el contador. */
            if (removedFromSource || removedFromDest) {
                this->edgeCount--;
            }
        }
    }
    

    /*
     * @brief Verifica si dos vértices son adyacentes en el grafo no dirigido.
     * Comprueba si el destino aparece en la lista de adyacencia del origen.
     *
     * @param source El dato del vértice de origen.
     * @param destination El dato del vértice de destino.
     * @return true si son adyacentes, false en caso contrario.
     */
    virtual bool areAdjacent(const T& source, const T& destination) const {
        /* Busca el nodo del vértice de origen. */
        VertexNode<T>* sourceVertex = this->findVertex(source);
        /* Si el vértice de origen existe, busca el destino en su lista de adyacencia. */
        return sourceVertex ? this->findAdjacent(sourceVertex, destination) != NULL : false;
    }

protected:
    /*
     * @brief Implementación específica para copiar las aristas de otro grafo no dirigido a este.
     * Utiliza el mapa de nodos para establecer las conexiones correctas en el nuevo grafo.
     *
     * @param otherGraph El grafo no dirigido del cual se copiarán las aristas.
     * @param nodeMap Un mapa que relaciona los nodos del grafo original con los nodos del nuevo grafo.
     */
    virtual void copyEdges(const Graph<T>& otherGraph,
                           const std::map<VertexNode<T>*, VertexNode<T>*>& nodeMap) {
        /* Realiza un downcast seguro al tipo NonDirectedGraph. */
        const NonDirectedGraph<T>* otherNonDirected = dynamic_cast<const NonDirectedGraph<T>*>(&otherGraph);
        if (otherNonDirected) {
            /* Recorre los vértices del grafo original. */
            VertexNode<T>* currentOtherVertex = otherNonDirected->firstNode;
            while (currentOtherVertex != NULL) {
                /* Busca el nodo correspondiente en el nuevo grafo. */
                typename std::map<VertexNode<T>*, VertexNode<T>*>::const_iterator it = nodeMap.find(currentOtherVertex);
                if (it != nodeMap.end()) {
                    VertexNode<T>* currentNewVertex = it->second;
                    /* Recorre los nodos adyacentes del vértice original. */
                    AdjacentNode<T>* currentOtherAdjacent = currentOtherVertex->getNextAdjacent();
                    while (currentOtherAdjacent != NULL) {
                        /* Busca el nodo adyacente correspondiente en el nuevo grafo. */
                        typename std::map<VertexNode<T>*, VertexNode<T>*>::const_iterator adjacentIt = nodeMap.find(currentOtherAdjacent->getData());
                        if (adjacentIt != nodeMap.end()) {
                            VertexNode<T>* adjacentNewVertex = adjacentIt->second;
                            double weight = currentOtherAdjacent->getWeight();
                            /* Agrega la arista en el nuevo grafo (solo si no existe ya). */
                            if (!this->findAdjacent(currentNewVertex, adjacentNewVertex->getData())) {
                                AdjacentNode<T>* newAdjacent = new (std::nothrow) AdjacentNode<T>(adjacentNewVertex, weight);
                                if (newAdjacent) {
                                    newAdjacent->setNext(currentNewVertex->getNextAdjacent());
                                    currentNewVertex->setNextAdjacent(newAdjacent);
                                    /* Para grafos no dirigidos, la arista ya se habrá agregado desde el otro extremo. */
                                    /* Evitamos doble conteo aquí. */
                                }
                            }
                        }
                        currentOtherAdjacent = currentOtherAdjacent->getNext();
                    }
                }
                currentOtherVertex = currentOtherVertex->getNextVertex();
            }
            /* El contador de aristas se copia directamente en el constructor de copia de Graph. */
        }
    }

private:
    /*
     * @brief Método auxiliar privado para eliminar una arista entre un vértice de origen y un vértice de destino.
     * Recorre la lista de adyacencia del vértice de origen y elimina el nodo adyacente al destino.
     *
     * @param sourceVertex El nodo del vértice de origen.
     * @param destinationVertex El nodo del vértice de destino.
     * @return true si la arista fue eliminada, false si no se encontró.
     */
    bool removeEdgeInternal(VertexNode<T>* source, VertexNode<T>* destination) {
        if (!source || !destination) {
            return false;
        }

        AdjacentNode<T>* prev = NULL;
        AdjacentNode<T>* current = source->getNextAdjacent();

        while (current != NULL) {
            if (current->getData() == destination) {
                if (prev == NULL) {
                    source->setNextAdjacent(current->getNext());
                } else {
                    prev->setNext(current->getNext());
                }
                delete current;
                return true;
            }
            prev = current;
            current = current->getNext();
        }
        return false;
    }
};

#endif
