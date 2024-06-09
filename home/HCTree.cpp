#include "HCTree.hpp"
#include <vector>
#include <stack>

/**
    Helps with the destructor
    Implements recursion 

    @param node 
*/
static void destructor_helper(HCNode* node){
    if(node == nullptr){
        return;
    }
    destructor_helper(node->c0);
    destructor_helper(node->c1);
    delete node;
}

/**
    Implementing destructor;
*/
HCTree::~HCTree(){
    destructor_helper(root);
    root = nullptr;
}

/**
    Use the Huffman algorithm to build a Huffman coding tree.
    PRECONDITION: 
        freqs is a vector of ints, 
        such that freqs[i] is the frequency of occurrence of byte i in the input file.
    POSTCONDITION: 
        root points to the root of the trie, 
        and leaves[i] points to the leaf node containing byte i.
*/ 
void HCTree::build(const vector<int>& freqs) 
{
    //cout << "Testing Build..." << endl;
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> huffman_forest;
    HCNode *new_root_ptr, *new_node_ptr, *new_left_node_ptr, *new_right_node_ptr;
    unsigned char filler_char;
    
    for (unsigned int i = 0; i < freqs.size(); i++){
        if (freqs[i] > 0){
            new_node_ptr = new HCNode(freqs[i], (unsigned char)i);
            huffman_forest.push(new_node_ptr);
            leaves[i] = new_node_ptr;
        }   
    }

    //cout << "HCTree: Created new leaf nodes" << endl;

    while (huffman_forest.size() > 1){
        filler_char = 't';
        new_left_node_ptr = huffman_forest.top();
        huffman_forest.pop();
        new_right_node_ptr = huffman_forest.top();
        huffman_forest.pop();

        new_root_ptr = new HCNode(new_left_node_ptr->count + new_right_node_ptr->count, filler_char);
        new_root_ptr->c0 = new_left_node_ptr;
        new_root_ptr->c1 = new_right_node_ptr;
        new_left_node_ptr->p = new_root_ptr;  
        new_right_node_ptr->p = new_root_ptr;
        huffman_forest.push(new_root_ptr);
    }

    //cout << "HCTree: Finished Building Huffman Trie" << endl;
    root = huffman_forest.top();
}

/**
    Write to the given FancyOutputStream the sequence of bits coding the given symbol.
    PRECONDITION: 
        build() has been called, 
        to create the coding tree, and initialize root pointer and leaves vector.
*/
void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const
{
    std::stack<int> output_encode;
    HCNode* traversal_node;
    
    //cout << "HCTree: starting encode..." << endl;
    //Set node to leaf node of the symbol
    //Traverse up the tree and pass on to stack
    traversal_node = leaves[(int) symbol];
    while (traversal_node != root){
        //If current is parent's left child, push 0 onto stack
        //If current is parent's right child, push 1 onto stack
        //cout << "Traversal Node count: " << traversal_node->count << endl;
        if (traversal_node == traversal_node->p->c0){
            output_encode.push(0);
        } else{
            output_encode.push(1);
        }
        traversal_node = traversal_node->p;
    }

    //cout << "HCTree: Finished gathering encoding patterns..." << endl;
    while(output_encode.size() != 0){
        out.write_bit(output_encode.top());
        output_encode.pop();
    }
}

/**
    Return symbol coded in the next sequence of bits from the stream.
    PRECONDITION: 
        build() has been called, to create the coding tree, 
        and initialize root pointer and leaves vector.
*/
unsigned char HCTree::decode(FancyInputStream & in) const
{
    HCNode* node;
    unsigned char bit_to_be_read;

    node = root;
    while (node->c0 != nullptr || node->c1 != nullptr){
        
        bit_to_be_read = in.read_bit();

        if (0 == bit_to_be_read){
            node = node->c0;
        } else if (1 == bit_to_be_read){
            node = node->c1;
        } else{
            return -1;
        }      
    }
    return node->symbol;
}

