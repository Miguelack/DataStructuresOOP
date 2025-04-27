#ifndef NONDIRECTEDGRAPH_H
#define NONDIRECTEDGRAPH_H
#include <cstddef>
#include <map>

template <typename T>
class NonDirectedGraph(){

    /* atributos */
    VertexNode<T>* firstNode; /* apuntador al primer nodo vertice */
    int vertexCount, edgeCount; /* nVertices y nAristas respectivamente*/

public:

    NonDirectedGraph(): firstNode(NULL), vertexCount(0), edgeCount(0){};
    NonDirectedGraph(VertexNode<T>* indexedFirstNode): firstNode(indexedFirstNode), vertexCount(0), edgeCount(0){};
    NonDirectedGraph(const NonDirectedGraph<T>& otherGraph) 
        : firstNode(NULL), vertexCount(0), edgeCount(0) {
        
        if(otherGraph.firstNode == NULL) return;
        
        std::map<VertexNode<T>*, VertexNode<T>*> nodeMap; // Mapeo original -> copia
        
        // Primera pasada: copiar todos los vértices
        VertexNode<T>* current = otherGraph.firstNode;
        VertexNode<T>* lastCopied = NULL;
        
        while(current != NULL) {
            VertexNode<T>* newNode = new VertexNode<T>(current->getData());
            nodeMap[current] = newNode;
            
            if(lastCopied == NULL) {
                firstNode = newNode;
            } else {
                lastCopied->setNextVertex(newNode);
            }
            
            lastCopied = newNode;
            current = current->getNextVertex();
            vertexCount++;
        }
        
        // Segunda pasada: copiar todas las aristas
        current = otherGraph.firstNode;
        while(current != NULL) {
            VertexNode<T>* copiedVertex = nodeMap[current];
            AdjacentNode<T>* adj = current->getNextAdjacent();
            AdjacentNode<T>* lastAdjCopy = NULL;
            
            while(adj != NULL) {
                VertexNode<T>* originalAdjVertex = adj->getData();
                VertexNode<T>* copiedAdjVertex = nodeMap[originalAdjVertex];
                
                AdjacentNode<T>* newAdj = new AdjacentNode<T>(
                    copiedAdjVertex, 
                    adj->getWeight()
                );
                
                if(lastAdjCopy == NULL) {
                    copiedVertex->setNextAdjacent(newAdj);
                } else {
                    lastAdjCopy->setNext(newAdj);
                }
                
                lastAdjCopy = newAdj;
                adj = adj->getNext();
                edgeCount++;
            }
            
            current = current->getNextVertex();
        }
    }

    /* getters de los atributos */
    T getFirstNode(){ return fistNode->getData(); };
    int getVertexCount() { return vertexCount; };
    int getEdgeCount() { return edgeCount; };

    /* setters */
    void setFirstNode(VertexNode<T>* newNode){ this->firstNode = newNode;};

    
}
#endif