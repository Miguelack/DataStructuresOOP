#ifndef NARYTREE_H
#define NARYTREE_H

#include "../Node/NaryTreeNode.hpp"
#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include <algorithm>
#include <climits>

template <typename T>
class NaryTree {
protected:
    NaryTreeNode<T>* root; /* puntero al nodo raíz del árbol n-ario */
    int size;             /* cantidad total de nodos en el árbol */

    /* verifica si un nodo es nulo */
    bool isNullNode(NaryTreeNode<T>* node) const {
        return node == NULL;
    }

    /* cuenta los nodos del árbol n-ario de forma recursiva.
     * Se recorre la lista de hijos (a partir del puntero izquierdo) y la cadena de hermanos (a través del puntero derecho).
     */
    int countNodes(NaryTreeNode<T>* node) const {
        if (!node) return 0;
        return 1 + countNodes(node->getLeft()) + countNodes(node->getRight());
    }

    /* Clona recursivamente el subárbol a partir de un nodo.
     * Se clona tanto la cadena de hijos como la de hermanos.
     */
    static NaryTreeNode<T>* cloneTree(const NaryTreeNode<T>* currentRecNode) {
        if (!currentRecNode) return NULL;
        return new NaryTreeNode<T>(
            currentRecNode->getData(),
            cloneTree(currentRecNode->getLeft()),
            cloneTree(currentRecNode->getRight())
        );
    }

    /* Elimina recursivamente todos los nodos del subárbol indicado.
     * Se recorren tanto la cadena de hijos (getLeft) como la de hermanos (getRight).
     */
    void auxiliarClear(NaryTreeNode<T>* currentNode) {
        if (!currentNode) return;
        // Se eliminan primero los hijos y luego los hermanos
        auxiliarClear(currentNode->getLeft());
        auxiliarClear(currentNode->getRight());
        delete currentNode;
        size = (size > 0) ? size - 1 : 0;
    }

    /* Calcula la altura del árbol tomando la mayor profundidad entre todos los hijos.
     * Se recorre la lista de hijos (con getLeft y getRight) para encontrar la máxima altura.
     */
    int auxiliarGetHeight(NaryTreeNode<T>* currentNode) const {
        if (!currentNode) return 0;
        int maxChildHeight = 0;
        NaryTreeNode<T>* child = currentNode->getLeft();
        while (child) {
            int h = auxiliarGetHeight(child);
            if (h > maxChildHeight)
                maxChildHeight = h;
            child = child->getRight();
        }
        return 1 + maxChildHeight;
    }

    /* Cuenta las hojas del árbol.
     * Un nodo se considera hoja si no tiene hijo (es decir, su puntero izquierdo es NULL).
     */
    int auxiliarCountLeaves(NaryTreeNode<T>* currentNode) const {
        if (!currentNode) return 0;
        int count = 0;
        if (currentNode->getLeft() == NULL)
            count = 1;
        else
            count = auxiliarCountLeaves(currentNode->getLeft());
        count += auxiliarCountLeaves(currentNode->getRight());
        return count;
    }

    /* Obtiene en una lista los valores de las hojas del árbol. */
    void auxiliarGetLeaves(NaryTreeNode<T>* currentNode, std::list<T>& leavesList) const {
        if (!currentNode) return;
        if (currentNode->getLeft() == NULL) {
            leavesList.push_back(currentNode->getData());
        } else {
            auxiliarGetLeaves(currentNode->getLeft(), leavesList);
        }
        auxiliarGetLeaves(currentNode->getRight(), leavesList);
    }

    /* Calcula el “peso” del árbol, es decir, la suma de todos los valores.
     * Se asume que la suma tiene sentido para el tipo T.
     */
    T auxiliarGetWeight(NaryTreeNode<T>* currentNode) const {
        if (!currentNode) return T();
        return currentNode->getData()
             + auxiliarGetWeight(currentNode->getLeft())
             + auxiliarGetWeight(currentNode->getRight());
    }

    /* Busca de forma recursiva un subárbol dentro del árbol n-ario. */
    bool findSubTree(NaryTreeNode<T>* current, NaryTreeNode<T>* subRoot) const {
        if (!subRoot) return true; // Un árbol vacío siempre está contenido.
        if (!current) return false;
        if (current->getData() == subRoot->getData()) {
            return findSubTree(current->getLeft(), subRoot->getLeft())
                && findSubTree(current->getRight(), subRoot->getRight());
        }
        return findSubTree(current->getLeft(), subRoot)
            || findSubTree(current->getRight(), subRoot);
    }

    /* Inserta un subárbol en la posición indicada dentro de la lista de hijos de la raíz.
     * Si asFirstChild es true, se inserta como el primer hijo; en caso contrario, como el último hijo.
     */
    void insertSubTreeHelper(NaryTreeNode<T>* rootNode, NaryTreeNode<T>* subTreeRoot, bool asFirstChild) {
        if (!subTreeRoot) return;
        NaryTreeNode<T>* clonedSubTree = cloneTree(subTreeRoot);
        int subTreeSize = countNodes(clonedSubTree);
        if (asFirstChild) {
            clonedSubTree->setRight(rootNode->getLeft());
            rootNode->setLeft(clonedSubTree);
        } else {
            if (!rootNode->getLeft()) {
                rootNode->setLeft(clonedSubTree);
            } else {
                NaryTreeNode<T>* child = rootNode->getLeft();
                while(child->getRight()) {
                    child = child->getRight();
                }
                child->setRight(clonedSubTree);
            }
        }
        size += subTreeSize;
    }

    /* Elimina recursivamente un nodo (y su subárbol) que contenga el dato especificado.
     * Se promueven los hijos, enlazándolos con el posible hermano del nodo eliminado.
     */
    NaryTreeNode<T>* auxiliarRemove(NaryTreeNode<T>* currentNode, const T& dataToDelete) {
        if (!currentNode) return NULL;
        if (currentNode->getData() == dataToDelete) {
            if (!currentNode->getLeft()) {
                NaryTreeNode<T>* temp = currentNode->getRight();
                delete currentNode;
                size = (size > 0) ? size - 1 : 0;
                return temp;
            } else { 
                NaryTreeNode<T>* promoted = currentNode->getLeft();
                NaryTreeNode<T>* temp = promoted;
                while (temp->getRight()) {
                    temp = temp->getRight();
                }
                temp->setRight(currentNode->getRight());
                delete currentNode;
                size = (size > 0) ? size - 1 : 0;
                return promoted;
            }
        }
        currentNode->setLeft(auxiliarRemove(currentNode->getLeft(), dataToDelete));
        currentNode->setRight(auxiliarRemove(currentNode->getRight(), dataToDelete));
        return currentNode;
    }

    /* Recorre el árbol en forma de niveles y almacena los datos en una lista.
     * Se recorre la estructura hijo-izquierdo / hermano-derecho.
     */
    std::list<T> levelOrderToList() const {
        std::list<T> dataList;
        if (isEmpty()) return dataList;
        std::queue<NaryTreeNode<T>*> q;
        q.push(root);
        while(!q.empty()) {
            NaryTreeNode<T>* current = q.front();
            q.pop();
            dataList.push_back(current->getData());
            // Encolar todos los hijos del nodo actual
            NaryTreeNode<T>* child = current->getLeft();
            while(child) {
                q.push(child);
                child = child->getRight();
            }
        }
        return dataList;
    }

public:
    /* Constructor por defecto */
    NaryTree() : root(NULL), size(0) {}

    /* Constructor con valor inicial en la raíz */
    NaryTree(const T& rootData) : root(new NaryTreeNode<T>(rootData)), size(1) {}

    /* Constructor copia */
    NaryTree(const NaryTree<T>& originalTree) : root(NULL), size(0) {
        if (!originalTree.isEmpty()) {
            root = cloneTree(originalTree.root);
            size = originalTree.size;
        }
    }

    /* Destructor */
    ~NaryTree() {
        clear();
    }

    /* Operador de asignación */
    NaryTree<T>& operator=(const NaryTree<T>& originalTree) {
        if (this != &originalTree) {
            clear();
            if (!originalTree.isEmpty()) {
                root = cloneTree(originalTree.root);
                size = originalTree.size;
            }
        }
        return *this;
    }

    /* Obtiene el dato de la raíz.
     * Si el árbol está vacío se retorna el valor por defecto de T.
     */
    T getRoot() const {
        if (isEmpty()) return T();
        return root->getData();
    }

    /* Retorna la cantidad de nodos en el árbol */
    int getSize() const {
        return size;
    }

    /* Inserta un subárbol completo en la raíz.
     * El parámetro asFirstChild indica si se inserta como primer hijo (true) o
     * como último hijo (false). Si el árbol original está vacío, la raíz se clona.
     */
    void insertSubTree(const NaryTree<T>& subTree, bool asFirstChild) {
        if (subTree.isEmpty()) return;
        if (isEmpty()) {
            root = cloneTree(subTree.root);
            size = countNodes(root);
            return;
        }
        insertSubTreeHelper(root, subTree.root, asFirstChild);
    }

    /* Elimina, a partir del dato, el nodo (y su subárbol) que coincida. */
    void remove(const T& data) {
        root = auxiliarRemove(root, data);
    }

    /* Elimina todos los nodos del árbol. */
    void clear() {
        auxiliarClear(root);
        root = NULL;
        size = 0;
    }

    /* Retorna la altura del árbol.
     * La altura se define como la mayor cantidad de nodos en alguna rama.
     */
    int getHeight() const {
        return auxiliarGetHeight(root);
    }

    /* Retorna la cantidad de hojas en el árbol. */
    int countLeaves() const {
        return auxiliarCountLeaves(root);
    }

    /* Retorna una lista con los datos almacenados en las hojas del árbol. */
    std::list<T> getLeaves() const {
        std::list<T> leavesList;
        auxiliarGetLeaves(root, leavesList);
        return leavesList;
    }

    /* Calcula la suma de todos los valores del árbol.
     * Se asume que para T es válida la operación de suma.
     */
    T getWeight() const {
        return auxiliarGetWeight(root);
    }

    /* Verifica si el árbol contiene un valor determinado.
     * Se recorre el árbol en forma de niveles utilizando la representación hijo-izquierdo / hermano-derecho.
     */
    bool contains(const T& data) const {
        if (isEmpty()) return false;
        std::queue<NaryTreeNode<T>*> q;
        q.push(root);
        while (!q.empty()) {
            NaryTreeNode<T>* current = q.front();
            q.pop();
            if (current->getData() == data)
                return true;
            NaryTreeNode<T>* child = current->getLeft();
            while (child) {
                q.push(child);
                child = child->getRight();
            }
        }
        return false;
    }

    /* Verifica si el árbol contiene (como subárbol) a otro árbol. */
    bool containsSubTree(const NaryTree<T>& subTree) const {
        if (subTree.isEmpty()) return true;
        if (isEmpty()) return false;
        return findSubTree(root, subTree.root);
    }

    /* Retorna en una lista todos los datos contenidos en el árbol, en orden de niveles.
     * Esta función reemplaza el recorrido que utilizaba apuntadores a funciones.
     */
    std::list<T> getDataList() const {
        return levelOrderToList();
    }

    /* Verifica si el árbol está vacío */
    bool isEmpty() const {
        return root == NULL;
    }
};

#endif