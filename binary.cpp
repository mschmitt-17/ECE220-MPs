#include "binary.h"
using namespace std;

/***
A Tree Node constructor. Set all the
member variables (key_, data_, left, right)
of the Node class. 

Input: 
T key - The key for the node
Y data - The actual value from the node

Output:
N/A
***/
template <class T, class Y>
Node<T, Y>::Node(T key, Y data){
  key_ = key; //set private key attribute for node
  data_ = data; //det private data attribute for node
  left = NULL; //initialize left child node to NULL
  right = NULL; //initialize right child node to NULL
} 

/***
A Node member getter function. This returns the
key of the node. 

Input: 
N/A

Output:
T - returns the key of the node that is of type T
***/
template <class T, class Y>
T Node<T, Y>::getKey(){
    return key_; //return private key attribute
}

/***
A Node member getter function. This returns the
actual data of the node. 

Input: 
N/A

Output:
Y - Returns the data of the node that is of type Y
***/
template <class T, class Y>
Y Node<T, Y>::getData(){
    return data_; //return private data attribute
}

/***
A Node member setter function. This sets the key
of a Node.

Input: 
T key - Sets the key of the Node of type T

Output:
N/A
***/
template <class T, class Y>
void Node<T, Y>::setKey(T key){
    key_ = key; //set private key attribute to passed key value
}

/***
A Node member setter function. This sets the actual
data of a Node.

Input: 
Y data - Sets the value of the Node of type Y

Output:
N/A
***/
template <class T, class Y>
void Node<T, Y>::setData(Y data){
    data_ = data; //set private data attribute of node to passed value
}


/***
The BinaryTree constructor. Implicitly set the 
values of the member variables


Input: 
N/A

Output:
N/A
***/

template <class T, class Y>
BinaryTree<T,Y>::BinaryTree(){
    root = NULL; //initialize root of tree to NULL
}

/***
A BinaryTree Member fuction. This function takes in two arguments
and inserts a node. If the key already exists in the tree do nothing.

Input: 
Node<T,Y> *parent_node - The current root node
Node<T,Y> *data - The Node that you want to insert into the tree

Output:
N/A
***/
template <class T, class Y>
void BinaryTree<T,Y>::insert_node(Node<T,Y> *parent_node, Node<T,Y> *data){
    if (root == NULL) { //if tree is empty, we need to set root node
        root = data;
        return;
    }
    if (parent_node == NULL) {
        parent_node = data;
        return;
    }
    else if (parent_node->getKey() == data->getKey()) {
        return;
    }
    else if (parent_node->getKey() < data->getKey()) {
        if (parent_node->right == NULL) {
            parent_node->right = data;
        }
        else {
            return insert_node(parent_node->right, data);
        }
    }
    else {
        if (parent_node->left == NULL) {
            parent_node->left = data;
        }
        else {
            return insert_node(parent_node->left, data);
        }
    }
}

/***
A BinaryTree Member fuction. This function takes in one arguments
and finds the parent of a node given a key.
If the key does not exist in the tree return NULL.

Input: 
T key - The key of the node you are searching for.

Output:
Node<T,Y> * - returns the parent of the node you are searching for
***/
template <class T, class Y>
Node<T,Y> * BinaryTree<T,Y>::find_node_parent(T key){
    return find_node_parent(root, key);
}

/***
A BinaryTree Member fuction. This function takes in one arguments
and finds the node given a key.
If the key does not exist in the tree return NULL.

Input: 
T key - The key of the node you are searching for.

Output:
Node<T,Y> * - returns the node you are searching for
***/
template <class T, class Y>
Node<T,Y> * BinaryTree<T,Y>::find_node(T key){
    return find_node(root, key);
}


/***
A BinaryTree Member fuction. This function deletes a node in
the binary tree. If the key does not exist do not do anything.

Input: 
T key - The key of the node you are trying to delete.

Output:
N/A
***/
template <class T, class Y>
void BinaryTree<T,Y>::delete_node(T key){
    root = delete_node(root, key);
}


/***
A BinaryTree helper fuction. This function deletes a node in
the binary tree. If the key does not exist in the tree return NULL.

Input: 
T key - The key of the node you are trying to delete.
Node<T,Y>*node - The root of the current subtree

Output:
Node<T,Y> * - The new root since the root can be deleted.
***/
template <class T, class Y>
Node<T,Y> * BinaryTree<T,Y>::delete_node(Node<T,Y>* node, T key){
    if (node == NULL) { //Base case: if passed node is NULL, return the node (NULL) as no further searching can be done
        return node;
    }
    else if (key < node->getKey()) { //Recursive case: if passed node is less than key of current node, set left subtree to result of call
        node->left = delete_node(node->left, key);
    }
    else if (key > node->getKey()) {
        node->right = delete_node(node->right, key); //Recursive case: if passed node is greater than key of current node, set right subtree to result of call
    }
    else { //if none of the above conditions are met, the node has been found
        if (node->right == NULL && node->left == NULL) { //if we reach this case, the node is the only node in the tree, so it must be deleted and NULL returned since there is no tree anymore
            delete node;
            return NULL;
        }
        else if (node->left == NULL) { //if node has no left subtree, we make the right subtree the new root and delete the current root
            Node<T,Y> *temp_node = node->right;
            delete node;
            return temp_node;
        }
        else if (node->right == NULL) { //if node has no right subtree, we make the left subtree the new root and delete the current root
            Node<T,Y> *temp_node = node->left;
            delete node;
            return temp_node;
        }
        else { //if this case is reached, the node we must delete has two children
            Node<T,Y> *temp_node = node->right; //set temp_node to start of right subtree
            while (temp_node->left != NULL) { //obtain the greatest value from the left subtree of the right subtree ()
                temp_node = temp_node->left;
            }
            node->setKey(temp_node->getKey()); //set root node key to key obtained from right subtree
            node->setData(temp_node->getData()); //set root node data to data obtained from right subtree
            node->right = delete_node(node->right, temp_node->getKey()); //after this, we must delete the node that we have changed our root node to
        }
    }
    return node; //return the (potentially new) root node
}


/***
A BinaryTree helper fuction. This function finds the parent node of
a node with a given key for the binary tree. If the key does not exist in the tree return NULL.

Input: 
T key - The key of the node you are trying to find.
Node<T,Y>*node - The root of the current subtree

Output:
Node<T,Y> * - Returns the parent of the key you were searching for
***/
template <class T, class Y>
Node<T,Y> * BinaryTree<T,Y>::find_node_parent(Node<T,Y> *node,T key){
    if (node == NULL) { //Base case: if root node is null we have an empty tree, so there are no nodes
        return NULL;
    }
    else if (node->getKey() == key) { //Base case: this case will only be reached if the passed key correspondds to the root node, which has no parent
        return NULL;
    } 
    else if (node->getKey() < key) { //if the key of the root is less than the passed key, we examine the right subtree
        if (node->right == NULL) { // Base case: if there is no right subtree, return NULL
            return NULL;
        }
        if (node->right->getKey() == key) { //Base case: if right node is the node we are looking for, return the current node which is its parent
            return node;
        }
        return find_node_parent(node->right, key); //Recursive case: call the function on the right subtree
    }
    else { //if this case is reached, our key will be in the right subtree if it is present at all
        if (node->left == NULL) { //Base case: if there is no left subtree, return NULL
            return NULL;
        }
        if (node->left->getKey() == key) { //Base case: if left node is the node we are looking for, return parent node
            return node;
        }
        return find_node_parent(node->left, key); //Recursive case: call the function on the left subtree
    }
}

/***
A BinaryTree helper fuction. This function finds the node with 
a given key for the binary tree. If the key does not exist in 
the tree return NULL.

Input: 
T key - The key of the node you are trying to find.
Node<T,Y>*node - The root of the current subtree

Output:
Node<T,Y> * - Returns the Node in the Tree
              for the key you were searching for
***/
template <class T, class Y>
Node<T,Y> * BinaryTree<T,Y>::find_node(Node<T,Y> *node,T key){
    if (node == NULL) { //Base case: if node is NULL, return null
        return NULL;
    }
    else if (node->getKey() == key) { //Base case: if key of node is desired key, return the node
        return node;
    }
    else if (node->getKey() < key) { //Recursive case: if key of current node is less than current tree, examine right subtree
        return find_node(node->right, key);
    }
    else { //Recursive case: if the above cases aren't met, the only option left is to examine the left subtree
        return find_node(node->left, key);
    }
}

/***
A BinaryTree member fuction. This function performs
pre_order traversal on the tree and stores the nodes
inside a list

Input: 
Node<T,Y> *node - The current root of the subtree
List<Node<T,Y>> - A list to store all the nodes.

Output:
N/A
***/
template <class T, class Y>
void BinaryTree<T,Y>::pre_order(Node<T,Y> *node, std::list<Node<T,Y> > &list){
    if (node != NULL) { //if current node is null, we don't want to keep recursing
        list.push_back(*node); //call push_back to add node to end of list
        pre_order(node->left, list); //traverse left subtree
        pre_order(node->right, list); //traverse right subtree
    }
}

/***
A BinaryTree member fuction. This function performs
in_order traversal on the tree and stores the nodes
inside a list

Input: 
Node<T,Y> *node - The current root of the subtree
List<Node<T,Y>> - A list to store all the nodes.

Output:
N/A
***/
template <class T, class Y>
void BinaryTree<T,Y>::in_order(Node<T,Y> *node, std::list<Node<T,Y> > &list){
    if (node != NULL) {
        in_order(node->left, list); //traverse left subtree
        list.push_back(*node); //call push_back to add node to end of list
        in_order(node->right, list); //traverse right subtree
    }
}

/***
A BinaryTree member fuction. This function performs
post_order traversal on the tree and stores the nodes
inside a list

Input: 
Node<T,Y> *node - The current root of the subtree
List<Node<T,Y>> - A list to store all the nodes.

Output:
N/A
***/
template <class T, class Y>
void BinaryTree<T,Y>::post_order(Node<T,Y> *node, std::list<Node<T,Y> > &list){
    if (node != NULL) {
        post_order(node->left, list); //traverse left subtree
        post_order(node->right, list); //traverse right subtree
        list.push_back(*node); //call push_back to add node to end of list
    }
}

/***
A BinaryTree getter fuction. This function gets
the current root of the binary tree.

Input: 
N/A
Output:
Outputs the root of the binary tree
***/
template <class T, class Y>
Node<T,Y> * BinaryTree<T,Y>::getRoot(){
    return root; //return private root attribute
}


/***
The BinaryTree destructor. Make sure all
the TreeNodes that have been created get destroyed
and make sure there are no memory leaks.

Input: 
N/A

Output:
N/A
***/
template <class T, class Y>
BinaryTree<T,Y>::~BinaryTree() {
    while (getRoot() != NULL) {
        delete_node(getRoot()->getKey());
    }
}

template class Node<int, std::string>;
template class Node<std::string, std::string>;
template class Node<int, int>;
template class Node<double, double>;
template class Node<std::string, long>;
template class Node<std::string, std::list<std::string> >;
template class Node<std::string, int >;

template class BinaryTree<std::string, std::list<std::string> >;
template class BinaryTree<std::string, std::string>;
template class BinaryTree<int, int>;
template class BinaryTree<int, std::string>;
template class BinaryTree<std::string, int>;
