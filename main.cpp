#include "Graphs/NonDirectedGraph.hpp"
#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>
#include <ctime> // Para medir el tiempo de ejecución

using namespace std;

// Función para medir el tiempo transcurrido en milisegundos (C++98)
double getMilliseconds() {
    clock_t start = clock();
    return static_cast<double>(start) / (CLOCKS_PER_SEC / 1000.0);
}

int main() {
    std::cout << "--- Prueba de Estrés de NonDirectedGraph con Muchos Nodos (C++98) ---" << std::endl;

    NonDirectedGraph<int> graph;
    int numVertices = 10000; // Número grande de vértices
    int numAristas = 50000; // Número grande de aristas

    // 1. Prueba de addVertex con muchos vértices
    double start_time = getMilliseconds();
    for (int i = 0; i < numVertices; ++i) {
        graph.addVertex(i);
        assert(graph.containsVertex(i));
    }
    double end_time = getMilliseconds();
    cout << "1. addVertex con " << numVertices << " vértices: OK. Tiempo: " << end_time - start_time << " ms" << endl;
    assert(graph.getVertexCount() == numVertices);

    // 2. Prueba de addEdge con muchas aristas
    start_time = getMilliseconds();
    for (int i = 0; i < numAristas; ++i) {
        int source = i % numVertices;         // Distribución simple de las aristas
        int dest = (i + 1) % numVertices;     // para conectar vértices.
        double weight = (i % 100) / 10.0; // Peso entre 0.0 y 9.9
        graph.addEdge(source, dest, weight);
        if (!graph.containsEdge(source,dest)){
           cout << "ERROR: No se agrego la arista " << source << " <-> " << dest << endl;
        }
        assert(graph.containsEdge(source, dest));
        assert(graph.containsEdge(dest, source));
        assert(abs(graph.edgeWeight(source, dest) - weight) < 0.0001);
        assert(abs(graph.edgeWeight(dest, source) - weight) < 0.0001);
    }
    end_time = getMilliseconds();
    cout << "2. addEdge con " << numAristas << " aristas: OK. Tiempo: " << end_time - start_time << " ms" << endl;
    assert(graph.getEdgeCount() == numAristas);

    // 3. Prueba de removeEdge con muchas aristas
    start_time = getMilliseconds();
    for (int i = 0; i < numAristas / 2; ++i) { // Eliminar la mitad de las aristas
        int source = i % numVertices;
        int dest = (i + 1) % numVertices;
        if (graph.containsEdge(source, dest)) { // Importante verificar antes de eliminar
            graph.removeEdge(source, dest);
            assert(!graph.containsEdge(source, dest));
            assert(!graph.containsEdge(dest, source));
        }
    }
    end_time = getMilliseconds();
    cout << "3. removeEdge con " << numAristas / 2 << " aristas: OK. Tiempo: " << end_time - start_time << " ms" << endl;
    assert(graph.getEdgeCount() == numAristas - (numAristas / 2));

    // 4. Prueba de removeVertex con muchos vértices
    start_time = getMilliseconds();
    for (int i = 0; i < numVertices / 2; ++i) { // Eliminar la mitad de los vértices
        if (graph.containsVertex(i)) {
            graph.removeVertex(i);
            assert(!graph.containsVertex(i));
        }
    }
    end_time = getMilliseconds();
    cout << "4. removeVertex con " << numVertices / 2 << " vértices: OK. Tiempo: " << end_time - start_time << " ms" << endl;
    assert(graph.getVertexCount() == numVertices - (numVertices / 2));

    // 5. Prueba de clear
    start_time = getMilliseconds();
    graph.clear();
    end_time = getMilliseconds();
    cout << "5. clear: OK. Tiempo: " << end_time - start_time << " ms" << endl;
    assert(graph.isEmpty());
    assert(graph.getVertexCount() == 0);
    assert(graph.getEdgeCount() == 0);

    std::cout << "--- Fin de la Prueba de Estrés con Muchos Nodos (C++98) ---" << std::endl;

    return 0;
}

