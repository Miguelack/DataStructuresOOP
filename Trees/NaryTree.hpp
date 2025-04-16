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

    /* cuenta los nodos del árbol n-ario de forma recursiva */
    int countNodes(NaryTreeNode<T>* node) const {
        if (!node) return 0;
        return 1 + countNodes(node->getLeft()) + countNodes(node->getRight());
    }

    /* clona recursivamente el subárbol a partir de un nodo */
    static NaryTreeNode<T>* cloneTree(const NaryTreeNode<T>* currentRecNode) {
        if (!currentRecNode) return NULL;
        return new NaryTreeNode<T>(
            currentRecNode->getData(),
            cloneTree(currentRecNode->getLeft()),
            cloneTree(currentRecNode->getRight())
        );
    }

    /* elimina recursivamente todos los nodos del subárbol indicado */
    void auxiliarClear(NaryTreeNode<T>* currentNode) {
        if (!currentNode) return;
        auxiliarClear(currentNode->getLeft());
        auxiliarClear(currentNode->getRight());
        delete currentNode;
        size = (size > 0) ? size - 1 : 0;
    }

    /* calcula la altura del árbol tomando la mayor profundidad entre todos los hijos */
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

    /* cuenta las hojas del árbol */
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

    /* obtiene en una lista los valores de las hojas del árbol */
    void auxiliarGetLeaves(NaryTreeNode<T>* currentNode, std::list<T>& leavesList) const {
        if (!currentNode) return;
        if (currentNode->getLeft() == NULL) {
            leavesList.push_back(currentNode->getData());
        } else {
            auxiliarGetLeaves(currentNode->getLeft(), leavesList);
        }
        auxiliarGetLeaves(currentNode->getRight(), leavesList);
    }

    /* calcula el peso del árbol (suma de todos los valores) */
    T auxiliarGetWeight(NaryTreeNode<T>* currentNode) const {
        if (!currentNode) return T();
        return currentNode->getData()
             + auxiliarGetWeight(currentNode->getLeft())
             + auxiliarGetWeight(currentNode->getRight());
    }

    /* busca un subárbol dentro del árbol n-ario */
    bool findSubTree(NaryTreeNode<T>* current, NaryTreeNode<T>* subRoot) const {
        if (!subRoot) return true;
        if (!current) return false;
        if (current->getData() == subRoot->getData()) {
            return findSubTree(current->getLeft(), subRoot->getLeft())
                && findSubTree(current->getRight(), subRoot->getRight());
        }
        return findSubTree(current->getLeft(), subRoot)
            || findSubTree(current->getRight(), subRoot);
    }

    /* inserta un subárbol en la posición indicada */
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

    /* elimina un nodo (y su subárbol) que contenga el dato especificado */
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

    /* recorre el árbol en forma de niveles y almacena los datos en una lista */
    std::list<T> levelOrderToList() const {
        std::list<T> dataList;
        if (isEmpty()) return dataList;
        std::queue<NaryTreeNode<T>*> auxiliarQueue;
        auxiliarQueue.push(root);
        while(!auxiliarQueue.empty()) {
            NaryTreeNode<T>* current = auxiliarQueue.front();
            auxiliarQueue.pop();
            dataList.push_back(current->getData());
            NaryTreeNode<T>* child = current->getLeft();
            while(child) {
                auxiliarQueue.push(child);
                child = child->getRight();
            }
        }
        return dataList;
    }

    /* funcion auxiliar para encontrar un nodo en el arbol */
    NaryTreeNode<T>* findNode(NaryTreeNode<T>* current, const T& data) const {
        if(!current) return NULL;
        if(current->getData() == data) return current;
        
        NaryTreeNode<T>* found = findNode(current->getLeft(), data);
        if(found) return found;
        
        return findNode(current->getRight(), data);
    }
    
    /* funcion auxiliar para encontrar el padre de un nodo */
    NaryTreeNode<T>* findParent(NaryTreeNode<T>* current, const T& childData) const {
        if(!current) return NULL;
        
        NaryTreeNode<T>* child = current->getLeft();
        while(child) {
            if(child->getData() == childData) {
                return current;
            }
            child = child->getRight();
        }
        
        NaryTreeNode<T>* parent = findParent(current->getLeft(), childData);
        if(parent) return parent;
        
        return findParent(current->getRight(), childData);
    }

    /* construye el arbol a partir de listas inorden y preorden */
    NaryTreeNode<T>* buildFromInPre(
        typename std::list<T>::const_iterator& preIt,
        typename std::list<T>::const_iterator preEnd,
        typename std::list<T>::const_iterator inBegin,
        typename std::list<T>::const_iterator inEnd) {
        
        if(preIt == preEnd || inBegin == inEnd) return NULL;
        
        T rootValue = *preIt++;
        NaryTreeNode<T>* node = new NaryTreeNode<T>(rootValue);
        
        typename std::list<T>::const_iterator inRoot = std::find(inBegin, inEnd, rootValue);
        if(inRoot == inEnd) return node;
        
        /* construye el primer hijo (left) */
        node->setLeft(buildFromInPre(preIt, preEnd, inBegin, inRoot));
        
        /* construye los hermanos (right) */
        typename std::list<T>::const_iterator nextInRoot = inRoot;
        ++nextInRoot;
        node->setRight(buildFromInPre(preIt, preEnd, nextInRoot, inEnd));
        
        return node;
    }

    /* construye el arbol a partir de listas inorden y postorden */
    NaryTreeNode<T>* buildFromInPost(
        typename std::list<T>::const_iterator& postIt,
        typename std::list<T>::const_iterator postEnd,
        typename std::list<T>::const_iterator inBegin,
        typename std::list<T>::const_iterator inEnd) {
        
        if(postIt == postEnd || inBegin == inEnd) return NULL;
        
        T rootValue = *(--postIt);
        NaryTreeNode<T>* node = new NaryTreeNode<T>(rootValue);
        
        typename std::list<T>::const_iterator inRoot = std::find(inBegin, inEnd, rootValue);
        if(inRoot == inEnd) return node;
        
        /* construye los hermanos primero (right) */
        typename std::list<T>::const_iterator nextInRoot = inRoot;
        ++nextInRoot;
        node->setRight(buildFromInPost(postIt, postEnd, nextInRoot, inEnd));
        
        /* luego construye el primer hijo (left) */
        node->setLeft(buildFromInPost(postIt, postEnd, inBegin, inRoot));
        
        return node;
    }

public:
    /* constructor por defecto */
    NaryTree() : root(NULL), size(0) {}

    /* constructor con valor inicial en la raíz */
    NaryTree(const T& rootData) : root(new NaryTreeNode<T>(rootData)), size(1) {}

    /* constructor copia */
    NaryTree(const NaryTree<T>& originalTree) : root(NULL), size(0) {
        if (!originalTree.isEmpty()) {
            root = cloneTree(originalTree.root);
            size = originalTree.size;
        }
    }

    /* constructor desde inorden y preorden */
    NaryTree(const std::list<T>& inOrder, const std::list<T>& preOrder) : root(NULL), size(0) {
        if(inOrder.size() != preOrder.size() || inOrder.empty()) return;
        
        typename std::list<T>::const_iterator preIt = preOrder.begin();
        root = buildFromInPre(preIt, preOrder.end(), inOrder.begin(), inOrder.end());
        size = countNodes(root);
    }

    /* constructor desde inorden y postorden */
    NaryTree(const std::list<T>& inOrder, const std::list<T>& postOrder, bool) : root(NULL), size(0) {
        if(inOrder.size() != postOrder.size() || inOrder.empty()) return;
        
        typename std::list<T>::const_iterator postIt = postOrder.end();
        root = buildFromInPost(postIt, postOrder.begin(), inOrder.begin(), inOrder.end());
        size = countNodes(root);
    }

    /* destructor */
    ~NaryTree() {
        clear();
    }

    /* operador de asignación */
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

    /* obtiene el dato de la raíz */
    T getRoot() const {
        if (isEmpty()) return T();
        return root->getData();
    }

    /* retorna la cantidad de nodos en el árbol */
    int getSize() const {
        return size;
    }

    /* inserta un subárbol completo en la raíz */
    void insertSubTree(const NaryTree<T>& subTree, bool asFirstChild) {
        if (subTree.isEmpty()) return;
        if (isEmpty()) {
            root = cloneTree(subTree.root);
            size = countNodes(root);
            return;
        }
        insertSubTreeHelper(root, subTree.root, asFirstChild);
    }

    /* elimina un nodo (y su subárbol) que coincida */
    void remove(const T& data) {
        root = auxiliarRemove(root, data);
    }

    /* elimina todos los nodos del árbol */
    void clear() {
        auxiliarClear(root);
        root = NULL;
        size = 0;
    }

    /* retorna la altura del árbol */
    int getHeight() const {
        return auxiliarGetHeight(root);
    }

    /* retorna la cantidad de hojas en el árbol */
    int countLeaves() const {
        return auxiliarCountLeaves(root);
    }

    /* retorna una lista con los datos de las hojas */
    std::list<T> getLeaves() const {
        std::list<T> leavesList;
        auxiliarGetLeaves(root, leavesList);
        return leavesList;
    }

    /* calcula la suma de todos los valores del árbol */
    T getWeight() const {
        return auxiliarGetWeight(root);
    }

    /* verifica si el árbol contiene un valor determinado */
    bool contains(const T& data) const {
        if (isEmpty()) return false;
        std::queue<NaryTreeNode<T>*> auxiliarQueue;
        auxiliarQueue.push(root);
        while (!auxiliarQueue.empty()) {
            NaryTreeNode<T>* current = auxiliarQueue.front();
            auxiliarQueue.pop();
            if (current->getData() == data)
                return true;
            NaryTreeNode<T>* child = current->getLeft();
            while (child) {
                auxiliarQueue.push(child);
                child = child->getRight();
            }
        }
        return false;
    }

    /* verifica si el árbol contiene un subárbol */
    bool containsSubTree(const NaryTree<T>& subTree) const {
        if (subTree.isEmpty()) return true;
        if (isEmpty()) return false;
        return findSubTree(root, subTree.root);
    }

    /* retorna una lista con todos los datos en orden de niveles */
    std::list<T> getDataList() const {
        return levelOrderToList();
    }

    /* verifica si el árbol está vacío */
    bool isEmpty() const {
        return root == NULL;
    }

    /* obtiene todos los hermanos de un nodo dado */
    std::list<NaryTree<T> > getSiblings(const T& nodeData) const {
        std::list<NaryTree<T> > siblingsList;
        
        if(isEmpty()) return siblingsList;
        
        if(root->getData() == nodeData) {
            siblingsList.push_back(*this);
            return siblingsList;
        }
        
        NaryTreeNode<T>* parent = findParent(root, nodeData);
        
        if(!parent) return siblingsList;
        
        NaryTreeNode<T>* currentSibling = parent->getLeft();
        
        while(currentSibling) {
            NaryTree<T> siblingTree;
            siblingTree.root = cloneTree(currentSibling);
            siblingTree.size = countNodes(siblingTree.root);
            siblingsList.push_back(siblingTree);
            
            currentSibling = currentSibling->getRight();
        }
        
        return siblingsList;
    }

    /* obtiene todos los hijos directos de un nodo dado */
    std::list<NaryTree<T> > getChildren(const T& nodeData) const {
        std::list<NaryTree<T> > childrenList;
        
        if(isEmpty()) return childrenList;
        
        NaryTreeNode<T>* node = findNode(root, nodeData);
        if(!node) return childrenList;
        
        NaryTreeNode<T>* firstChild = node->getLeft();
        
        if(firstChild) {
            NaryTreeNode<T>* child = firstChild;
            while(child) {
                NaryTree<T> childTree;
                childTree.root = cloneTree(child);
                childTree.size = countNodes(childTree.root);
                childrenList.push_back(childTree);
                
                child = child->getRight();
            }
        }
        
        return childrenList;
    }
};

#endif