#ifndef GRAPH_H
#define GRAPH_H

#include <cstddef>
#include <map>
#include <vector>
#include <limits> /* para numeric_limits */
#include "../Node/AdjacentNode.hpp"
#include "../Node/VertexNode.hpp"

/* clase base abstracta para grafos dirigidos y no dirigidos */
template <typename T>
class Graph {
protected:
    /* atributos protegidos */
    VertexNode<T>* firstNode; /* puntero al primer nodo del grafo, no necesariamente ordenado */
    int vertexCount; /* contador del numero de vertices en el grafo */
    int edgeCount; /* contador del numero de aristas en el grafo */
    std::map<T, int> dataToIndex; /* mapeo desde el dato del vertice a su indice interno */
    std::vector<T> indexToData; /* vector que almacena los datos de los vertices por su indice */
    int nextIndex; /* entero que indica el siguiente indice disponible para un nuevo vertice */

    /* metodo virtual puro para copiar las aristas del otro grafo a este */
    virtual void copyEdges(const Graph<T>& otherGraph,
                             const std::map<VertexNode<T>*, VertexNode<T>*>& nodeMap) = 0;

    /* metodo protegido para agregar un dato al sistema de mapeo de datos a indices */
    void addToMappings(const T& data) {
        /* verifica si el dato ya existe en el mapa dataToIndex */
        if(dataToIndex.find(data) == dataToIndex.end()) {
            /* seguridad para evitar un overflow del indice */
            if (nextIndex == std::numeric_limits<int>::max()) {
                /**/ /* error: se alcanzo el maximo numero de vertices soportados */
                return;
            }
            /* asigna el siguiente indice disponible al dato */
            dataToIndex[data] = nextIndex;
            /* agrega el dato al vector indexToData en la posicion del nuevo indice */
            indexToData.push_back(data);
            /* incrementa el siguiente indice disponible */
            nextIndex++;
        }
        /* complejidad promedio: o(log n) debido a la busqueda en el mapa */
        /* complejidad peor caso: o(n) si el mapa degenera a una lista (poco probable con std::map) */
    }

    /* metodo protegido para eliminar un dato del sistema de mapeo */
    void removeFromMappings(const T& data) {
        /* crea un iterador para buscar el dato en el mapa dataToIndex */
        typename std::map<T, int>::iterator it = dataToIndex.find(data);
        /* si el iterador es valido (el dato se encontro en el mapa) */
        if(it != dataToIndex.end()) {
            /* obtiene el indice asociado al dato */
            int index = it->second;
            /* elimina la entrada del dato en el mapa dataToIndex */
            dataToIndex.erase(it);
            /* si el indice esta dentro de los limites del vector indexToData, marca esa posicion como "vacia" */
            if (index >= 0 && index < static_cast<int>(indexToData.size())) {
                indexToData[index] = T();
            }
        }
        /* complejidad promedio: o(log n) debido a la busqueda y eliminacion en el mapa */
        /* complejidad peor caso: o(n) si el mapa degenera a una lista */
    }

    /* metodo protegido para liberar la memoria de todos los nodos del grafo */
    void clearGraph() {
        /* comienza con el primer nodo de la lista de vertices */
        VertexNode<T>* current = firstNode;
        /* recorre la lista de vertices hasta el final */
        while(current != NULL) {
            /* guarda el puntero al siguiente vertice antes de eliminar el actual */
            VertexNode<T>* next = current->getNextVertex();

            /* comienza con el primer nodo adyacente del vertice actual */
            AdjacentNode<T>* adj = current->getNextAdjacent();
            /* recorre la lista de nodos adyacentes del vertice actual */
            while(adj != NULL) {
                /* guarda el puntero al siguiente nodo adyacente antes de eliminar el actual */
                AdjacentNode<T>* nextAdj = adj->getNext();
                /* elimina el nodo adyacente actual */
                delete adj;
                /* avanza al siguiente nodo adyacente */
                adj = nextAdj;
            }

            /* elimina el nodo vertice actual */
            delete current;
            /* avanza al siguiente nodo vertice */
            current = next;
        }
        /* resetea el puntero al primer nodo */
        firstNode = NULL;
        /* resetea el contador de vertices */
        vertexCount = 0;
        /* resetea el contador de aristas */
        edgeCount = 0;
        /* complejidad promedio: o(n + m) ya que se visitan todos los vertices y todas las aristas en las listas de adyacencia */
        /* complejidad peor caso: o(n + m) */
    }

    /* metodo protegido para resetear el sistema de mapeo de datos a indices */
    void clearMappings() {
        /* limpia el mapa dataToIndex */
        dataToIndex.clear();
        /* limpia el vector indexToData */
        indexToData.clear();
        /* resetea el siguiente indice disponible */
        nextIndex = 0;
        /* complejidad promedio: o(1) para clear de estructuras de datos estandar */
        /* complejidad peor caso: o(n) en implementaciones degeneradas */
    }

    /* metodo protegido para copiar los vertices del otro grafo a este con verificaciones de seguridad */
    bool copyVerticesFrom(const Graph<T>& otherGraph, std::map<VertexNode<T>*, VertexNode<T>*>& nodeMap) {
        /* comienza con el primer nodo del grafo original */
        VertexNode<T>* currentOther = otherGraph.firstNode;
        /* puntero para mantener el ultimo nodo copiado en el nuevo grafo */
        VertexNode<T>* lastCopied = NULL;
        /* contador para el numero de vertices copiados */
        int copiedCount = 0;

        /* define un limite seguro para evitar posibles bucles infinitos en grafos grandes */
        const int maxVertices = 1000000; /* un millon de vertices como limite */

        /* recorre todos los vertices del grafo original */
        while(currentOther != NULL) {
            /* verifica si se ha alcanzado el limite maximo de vertices */
            if (copiedCount >= maxVertices) {
                /**/ /* error: posible bucle infinito o grafo demasiado grande */
                return false;
            }

            /* intenta crear un nuevo nodo vertice con los datos del nodo actual del grafo original */
            VertexNode<T>* newNode = new (std::nothrow) VertexNode<T>(currentOther->getData());
            /* si la asignacion de memoria falla */
            if (!newNode) {
                /**/ /* error: fallo al asignar memoria para nuevo vertice */
                return false;
            }

            /* agrega el dato del nuevo nodo al sistema de mapeo */
            addToMappings(newNode->getData());
            /* mapea el nodo original al nuevo nodo creado */
            nodeMap[currentOther] = newNode;

            /* si es el primer nodo que se copia, se establece como el primer nodo del nuevo grafo */
            if(lastCopied == NULL) {
                firstNode = newNode;
            } else {
                /* enlaza el ultimo nodo copiado con el nuevo nodo */
                lastCopied->setNextVertex(newNode);
            }

            /* actualiza el puntero al ultimo nodo copiado */
            lastCopied = newNode;
            /* avanza al siguiente nodo en el grafo original */
            currentOther = currentOther->getNextVertex();
            /* incrementa el contador de vertices copiados */
            copiedCount++;
            /* incrementa el contador total de vertices del nuevo grafo */
            vertexCount++;
        }
        /* la copia de vertices se realizo con exito */
        return true;
        /* complejidad promedio: o(n) donde n es el numero de vertices en otherGraph */
        /* complejidad peor caso: o(n) */
    }

public:
    /* constructor vainilla de la clase Grafo */
    Graph() : firstNode(NULL), vertexCount(0), edgeCount(0), nextIndex(0) {};
    /* complejidad promedio: o(1) */
    /* complejidad peor caso: o(1) */

    /* constructor de copia de la clase Grafo */
/* constructor de copia de la clase Grafo */
    Graph(const Graph<T>& otherGraph) : firstNode(NULL), vertexCount(0), edgeCount(0), nextIndex(0) {
        /* si el grafo original esta vacio, no se necesita hacer nada */
        if(otherGraph.firstNode == NULL) return;

        /* mapa para almacenar la correspondencia entre los nodos del grafo original y los nodos del nuevo grafo */
        std::map<VertexNode<T>*, VertexNode<T>*> nodeMap;
        /* copia los vertices del grafo original al nuevo grafo */
        if (!copyVerticesFrom(otherGraph, nodeMap)) {
            /* si falla la copia de vertices, libera cualquier memoria alocada y retorna */
            clear();
            return;
        }
        /* Ahora la copia de aristas se hará en la clase derivada */
    }

    /* destructor virtual de la clase Grafo para asegurar la correcta liberacion de memoria en las clases derivadas */
    virtual ~Graph() {
        /* libera la memoria del grafo al destruirse el objeto */
        clear();
        /* complejidad promedio: o(n + m) */
        /* complejidad peor caso: o(n + m) */
    }

    /* metodos virtuales puros que deben ser implementados por las clases derivadas */
    virtual void addVertex(const T& data) = 0;
    virtual void removeVertex(const T& data) = 0;
    virtual void addEdge(const T& source, const T& destination, double weight = 1.0) = 0;
    virtual void removeEdge(const T& source, const T& destination) = 0;
    virtual bool areAdjacent(const T& source, const T& destination) const = 0;

    /* metodos comunes para obtener informacion del grafo */
    virtual int getVertexCount() const { return vertexCount; }
    virtual int getEdgeCount() const { return edgeCount; }
    virtual bool isEmpty() const { return vertexCount == 0; }
    /* complejidad promedio: o(1) */
    /* complejidad peor caso: o(1) */

    /* metodo virtual para liberar toda la memoria del grafo y resetear su estado */
    virtual void clear() {
        /* libera la memoria de los nodos del grafo */
        clearGraph();
        /* resetea el sistema de mapeo */
        clearMappings();
        /* complejidad promedio: o(n + m) */
        /* complejidad peor caso: o(n + m) */
    }

    /* metodo virtual para copiar el contenido de otro grafo a este */
    virtual bool copy(const Graph<T>& otherGraph) {
        /* evita la autoasignacion */
        if(this == &otherGraph) return true;

        /* libera la memoria del grafo actual antes de la copia */
        clear();
        /* si el grafo original esta vacio, no hay nada que copiar */
        if(otherGraph.firstNode == NULL) return true;

        /* mapa para almacenar la correspondencia entre los nodos del grafo original y los nodos del nuevo grafo */
        std::map<VertexNode<T>*, VertexNode<T>*> nodeMap;
        /* copia los vertices del grafo original */
        if (!copyVerticesFrom(otherGraph, nodeMap)) {
            /* si falla la copia de vertices, libera cualquier memoria alocada y retorna false */
            clear();
            return false;
        }
        /* copia las aristas del grafo original */
        copyEdges(otherGraph, nodeMap);
        /* copia el contador de aristas */
        edgeCount = otherGraph.edgeCount;
        /* complejidad promedio: o(n + m) */
        /* complejidad peor caso: o(n^2) */
        return true;
    }

    /* operador de asignacion sobrecargado para permitir la asignacion entre objetos Grafo */
    Graph<T>& operator=(const Graph<T>& otherGraph) {
        /* evita la autoasignacion */
        if(this != &otherGraph) {
            /* utiliza el metodo copy para realizar la copia */
            copy(otherGraph);
        }
        /* devuelve una referencia al objeto actual para permitir el encadenamiento de asignaciones */
        return *this;
        /* complejidad promedio: o(n + m) */
        /* complejidad peor caso: o(n^2) */
    }

    /* metodos virtuales para verificar la existencia de vertices y aristas */
    virtual bool containsVertex(const T& data) const {
        /* utiliza el metodo auxiliar findVertex para buscar el vertice */
        return findVertex(data) != NULL;
        /* complejidad promedio: o(n) */
        /* complejidad peor caso: o(n) */
    }

    virtual bool containsEdge(const T& source, const T& destination) const {
        /* busca el nodo vertice de origen */
        VertexNode<T>* srcVertex = findVertex(source);
        /* si el vertice de origen existe, busca la arista en su lista de adyacencia */
        return srcVertex ? findAdjacent(srcVertex, destination) != NULL : false;
        /* complejidad promedio: o(grado(source)) donde grado es el grado del vertice de origen */
        /* complejidad peor caso: o(n) si todos los vertices son adyacentes al vertice de origen */
    }

    virtual double edgeWeight(const T& source, const T& destination) const {
        /* busca el nodo vertice de origen */
        VertexNode<T>* srcVertex = findVertex(source);
        /* si el vertice de origen no existe, retorna un valor indicativo de no encontrado */
        if(!srcVertex) return -1.0;

        /* busca el nodo adyacente correspondiente al destino */
        AdjacentNode<T>* adj = findAdjacent(srcVertex, destination);
        /* si la arista se encuentra, devuelve su peso, sino retorna un valor indicativo de no encontrado */
        return adj ? adj->getWeight() : -1.0;
        /* complejidad promedio: o(grado(source)) */
        /* complejidad peor caso: o(n) */
    }

    /* metodos virtuales para acceder a los datos del grafo utilizando indices */
    virtual T getDataByIndex(int index) const {
        /* verifica si el indice esta dentro de los limites del vector indexToData */
        return (index >= 0 && index < static_cast<int>(indexToData.size())) ? indexToData[index] : T();
        /* complejidad promedio: o(1) */
        /* complejidad peor caso: o(1) */
    }

    virtual int getIndexByData(const T& data) const {
        /* busca el dato en el mapa dataToIndex y devuelve su indice si se encuentra */
        typename std::map<T, int>::const_iterator it = dataToIndex.find(data);
        return it != dataToIndex.end() ? it->second : -1;
        /* complejidad promedio: o(log n) */
        /* complejidad peor caso: o(n) */
    }

    virtual bool containsVertexByIndex(int index) const {
        /* verifica si el indice es valido y si hay un dato asociado a ese indice */
        return (index >= 0 && index < static_cast<int>(indexToData.size()) && !(indexToData[index] == T()));
        /* complejidad promedio: o(1) */
        /* complejidad peor caso: o(1) */
    }

    virtual bool containsEdgeByIndex(int sourceIndex, int destIndex) const {
        /* verifica si ambos indices corresponden a vertices validos y luego verifica la existencia de la arista por datos */
        return containsVertexByIndex(sourceIndex) && containsVertexByIndex(destIndex) &&
               containsEdge(getDataByIndex(sourceIndex), getDataByIndex(destIndex));
        /* complejidad promedio: o(n) en el peor caso (debido a containsEdge con datos) *//* complejidad peor caso: o(n) */
    }

    virtual double edgeWeightByIndex(int sourceIndex, int destIndex) const {
        /* verifica si ambos indices son validos y luego obtiene el peso de la arista por datos */
        return containsVertexByIndex(sourceIndex) && containsVertexByIndex(destIndex) ?
               edgeWeight(getDataByIndex(sourceIndex), getDataByIndex(destIndex)) : -1.0;
        /* complejidad promedio: o(n) en el peor caso */
        /* complejidad peor caso: o(n) */
    }

protected:
    /* metodo protegido para buscar un nodo vertice por su dato */
    VertexNode<T>* findVertex(const T& data) const {
        /* comienza la busqueda desde el primer nodo */
        VertexNode<T>* current = firstNode;
        /* recorre la lista de vertices hasta encontrar el dato o llegar al final */
        while (current != NULL) {
            /* si el dato del nodo actual coincide con el dato buscado, devuelve el nodo */
            if (current->getData() == data) {
                return current;
            }
            /* avanza al siguiente nodo */
            current = current->getNextVertex();
        }
        /* si el dato no se encuentra, devuelve NULL */
        return NULL;
        /* complejidad promedio: o(n) */
        /* complejidad peor caso: o(n) */
    }

    /* metodo protegido para buscar un nodo adyacente en la lista de adyacencia de un vertice */
    AdjacentNode<T>* findAdjacent(VertexNode<T>* vertex, const T& targetData) const {
        /* si el vertice es NULL o su lista de adyacencia esta vacia, no hay adyacentes */
        if (!vertex || !vertex->getNextAdjacent()) {
            return NULL;
        }
        /* comienza la busqueda desde el primer nodo adyacente */
        AdjacentNode<T>* current = vertex->getNextAdjacent();
        /* recorre la lista de adyacencia hasta encontrar el nodo con el dato destino o llegar al final */
        while (current != NULL) {
            /* si el nodo adyacente actual apunta al vertice con el dato destino, devuelve el nodo adyacente */
            if (current->getData()->getData() == targetData) {
                return current;
            }
            /* avanza al siguiente nodo adyacente */
            current = current->getNext();
        }
        /* si no se encuentra el nodo adyacente, devuelve NULL */
        return NULL;
        /* complejidad promedio: o(grado(vertex)) */
        /* complejidad peor caso: o(n) */
    }
};

#endif