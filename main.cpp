/* #include "Trees/BinaryTree.hpp"
#include <iostream>
#include <string>

using namespace std;

// Functor para imprimir
struct Printer {
    void operator()(const string& val) const {
        cout << val << " ";
    }
};

void testArbolString() {
    cout << "\n=== TEST ARBOL STRING ===" << endl;
    
    BinaryTree<string> arbol("Raiz");
    BinaryTree<string> subIzq("Izquierda");
    BinaryTree<string> subDer("Derecha");
    
    // Insertamos valores como objetos
    subIzq.insertSubTree(BinaryTree<string>("Hoja Izq 1"), true);
    subDer.insertSubTree(BinaryTree<string>("Hoja Der 1"), false);
    
    // Composición del árbol principal
    arbol.insertSubTree(subIzq, true);
    arbol.insertSubTree(subDer, false);
    
    // Recorrido seguro sin punteros
    cout << "Recorrido por niveles: ";
    Printer print;
    arbol.forEach(print);
    cout << endl;
    
    // Eliminación segura
    cout << "\nEliminando subárbol izquierdo..." << endl;
    arbol.removeSubTree(true, false);
    
    cout << "Recorrido después de eliminar: ";
    arbol.forEach(print);
    cout << endl;
}

void testArbolInt() {
    cout << "\n=== TEST ARBOL INT ===" << endl;
    
    BinaryTree<int> arbol(100);
    arbol.insertSubTree(BinaryTree<int>(50), true);
    arbol.insertSubTree(BinaryTree<int>(150), false);
    
    cout << "Tamaño del árbol: " << arbol.getSize() << endl;
    cout << "Altura: " << arbol.getHeight() << endl;
    cout << "Hojas: " << arbol.countLeaves() << endl;
    
    // Eliminación por valor (no por puntero)
    cout << "\nEliminando el valor 50..." << endl;
    arbol.remove(50);
    cout << "Tamaño después de eliminar: " << arbol.getSize() << endl;
}

int main() {
    testArbolString();
    testArbolInt();
    
    cout << "\n=== TODAS LAS PRUEBAS COMPLETADAS ===" << endl;
    return 0;
} */

#include "Trees/NaryTree.hpp"
#include <iostream>
#include <list>

int main() {
    // Crear un árbol n-ario con raíz 1
    NaryTree<int> tree(1);
    std::cout << "Raíz del árbol: " << tree.getRoot() << std::endl;

    // Crear subárboles para insertar en el árbol principal
    NaryTree<int> subtree1(2);
    NaryTree<int> subtree2(3);
    NaryTree<int> subtree3(4);

    // Insertar subtree1 como primer hijo de la raíz
    tree.insertSubTree(subtree1, true);  // Inserta subtree1 como primer hijo
    // Insertar subtree2 como último hijo de la raíz
    tree.insertSubTree(subtree2, false); // Inserta subtree2 como último hijo

    // Insertar subtree3 en subtree1 para crear una jerarquía más profunda
    subtree1.insertSubTree(subtree3, true); // subtree3 se inserta como primer hijo de subtree1
    // Se debe actualizar el árbol principal, ya que subtree1 fue modificado.
    // Por simplicidad, volvemos a insertar subtree1 en la raíz (el árbol principal clona el subárbol)
    tree.insertSubTree(subtree1, true);

    // Obtener y mostrar todos los datos del árbol (recorrido en niveles)
    std::list<int> dataList = tree.getDataList();
    std::cout << "Datos en el árbol (recorrido por niveles): ";
    for (std::list<int>::iterator it = dataList.begin(); it != dataList.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // Mostrar propiedades del árbol: tamaño, altura y peso (suma de valores)
    std::cout << "Tamaño del árbol: " << tree.getSize() << std::endl;
    std::cout << "Altura del árbol: " << tree.getHeight() << std::endl;
    std::cout << "Peso (suma de nodos) del árbol: " << tree.getWeight() << std::endl;

    // Eliminar un nodo (y su subárbol) a partir de su dato
    tree.remove(2);
    dataList = tree.getDataList();
    std::cout << "Datos en el árbol después de eliminar el nodo con dato 2: ";
    for (std::list<int>::iterator it = dataList.begin(); it != dataList.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // Consultas: verificar si el árbol contiene ciertos valores
    std::cout << "¿Contiene el árbol el dato 3? " << (tree.contains(3) ? "Sí" : "No") << std::endl;
    std::cout << "¿Contiene el árbol el dato 2? " << (tree.contains(2) ? "Sí" : "No") << std::endl;

    // Obtener y mostrar las hojas del árbol
    std::list<int> leaves = tree.getLeaves();
    std::cout << "Hojas del árbol: ";
    for (std::list<int>::iterator it = leaves.begin(); it != leaves.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}