#include "anagram.h"
#include <iostream>
#include <fstream>
/*
THINGS TO DO NEXT: MAKE SURE addword DOESNT ADD DUPLICATES, COMPILE
*/




/***
An AnagramDict constructor. This Constructor takes in a filepath
and then creates a binary tree out of all the words inside the 
binary tree. 

Input: 
std::string filename - the path to the file

Output:
N/A
***/
AnagramDict::AnagramDict(std::string filename) {
    filename_ = filename;
    std::string path = "dictionaries/" + filename_;
    std::ifstream file;
    file.open(path.c_str());
    std::string new_word; //create variable to hold strings we will read from file
    file >> new_word; //read first string from file
    while (!file.eof() && file.is_open()) { //while we have not tried to read into the end of the file
        addWord(new_word); //add word to binary tree
        file >> new_word; //read next word into variable
    }
    file.close();
}

/***
An AnagramDict member function. This function sorts the given word in 
lexicographical order

Input: 
std::string word - the word that needs to be sorted

Output:
std::string - the sorted version of the input
***/
std::string AnagramDict::sortWord(std::string word) {
   sort(word.begin(), word.end()); //sort word in lexicographic order
   return word; //return now sorted word
}

/***
An AnagramDict member function. This function adds a word to the tree.
If the words sorted version already exists in the tree add it to the linked
list of that node. If the sorted word does not exist in the tree create a new
node and insert it into the tree
Duplicated words should not be added to the tree.
Input: 
std::string word - the word that needs to inserted

Output:
N/A
***/
void AnagramDict::addWord(std::string word) {
    std::string sorted_string = sortWord(word); //create sorted version of our passed string, since sort will modify the passed string
    Node<std::string, std::list<std::string> > *tree_node_ptr = tree.find_node(sorted_string); //pointer to node that has sorted_string as key
    int duplicate = 0; //variable to use to indicate if new word is already in data
    if (tree_node_ptr != NULL) { //if sorted string is already a key to a node
        std::list<std::string> node_data = tree_node_ptr->getData(); //get data from node
        for (std::list<std::string>::iterator it = node_data.begin(); it != node_data.end(); it++) {
            if (*it == word) {
                duplicate = 1; //if the word is a duplicate, set our variable to 1 accordingly
            }
        }
        if (!duplicate) { //if the word is not a duplicate, add to the data
            node_data.push_back(word); //add new word to data in node
            tree_node_ptr->setData(node_data); //set data in node as new data
        }
    }
    else { //if node has not been created yet, we need to add it to the tree
        std::list<std::string> node_list; //create a list containing our word to help create our new node
        node_list.push_back(word); //add word to list
        Node<std::string, std::list<std::string> > *node_ptr = new Node<std::string, std::list<std::string> >(sorted_string, node_list); //allocate memory for new node
        Node<std::string, std::list<std::string> > *root_ptr = tree.getRoot();
        tree.insert_node(root_ptr, node_ptr); //insert new node into tree
    }
}

/***
An AnagramDict member function. Does a preorder, postorder, or inorder traversal
and then prints out all the anagrams and words.

The output file should be the traversal order of the tree, but only the data on each line. 
View on wiki for more information. 

Input: 
std::string order - The type of order you want to traverse. Can be "pre", "post", "in"

Output:
N/A
***/
void AnagramDict::saveAnagrams(std::string order) {
    
    std::string path = "output/" + order+"_"+ filename_;
    std::ofstream file;
    file.open(path.c_str());
    if(!file.is_open()) {
        file.open(path.c_str(),std::ios::out);
    }
    std::list<Node<std::string, std::list<std::string> > > node_list; //create list for nodes to be read
    if (order == "pre") { //read pre order
        tree.pre_order(tree.getRoot(), node_list); //populate node_list with nodes from tree in proper order
    }
    
    else if (order == "post") { //read post order
        tree.post_order(tree.getRoot(), node_list); //populate node_list with nodes from tree in proper order
    }
    else if (order == "in") { //read in order
        tree.in_order(tree.getRoot(), node_list); //populate node_list with nodes from tree in proper order
    }

    for (std::list<Node<std::string, std::list<std::string> > >::iterator it_nodes = node_list.begin(); it_nodes != node_list.end(); it_nodes++) { //iterate through nodes
        std::list<std::string> node_data = (*it_nodes).getData();
        for (std::list<std::string>::iterator it_data = node_data.begin(); it_data != node_data.end(); it_data++) { //iterate through data in node
            file << *it_data << " "; //read string into file
        }
        file << std::endl; //read newline into file for different nodes
    }
    file.close();
}