//
//  main.cpp
//  Homework 5
//
//  Created by Duncan Hackmann on 6/6/23.
//

#include <iostream>


// Definition of a node in the BST
struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int value) : data(value), left(nullptr), right(nullptr) {}
};

// Function to insert a new node in the BST
Node* insertNode(Node* root, int value) {
    // If the tree is empty, create a new node and return it as the root
    if (root == nullptr) {
        return new Node(value);
    }

    // Otherwise, find the appropriate position to insert the new node
    if (value < root->data) {
        // If the value is less than the current node's data, recursively insert it in the left subtree
        root->left = insertNode(root->left, value);
    } else {
        // If the value is greater than or equal to the current node's data, recursively insert it in the right subtree
        root->right = insertNode(root->right, value);
    }

    // Return the modified root of the BST
    return root;
}

// Function to print the BST in inorder traversal (ascending order)
void inorderTraversal(Node* root) {
    if (root == nullptr) {
        return;
    }

    inorderTraversal(root->left);
    std::cout << root->data << " ";
    inorderTraversal(root->right);
}

int main() {
    Node* root = nullptr;

    // Insert nodes into the BST
    root = insertNode(root, 5);
    root = insertNode(root, 3);
    root = insertNode(root, 7);
    root = insertNode(root, 1);
    root = insertNode(root, 4);

    // Print the BST
    std::cout << "Inorder traversal of the BST: ";
    inorderTraversal(root);
    std::cout << std::endl;

    return 0;
}

int main(int argc, const char * argv[]) {
    
}
