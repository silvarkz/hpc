#include <iostream>
#include <queue>
#include <omp.h>

using namespace std;

// Node structure for the tree
struct Node {
    int data;
    Node *left, *right;

    Node(int value) : data(value), left(NULL), right(NULL) {}
};

// Function to create a new node
Node* createNode(int value) {
    return new Node(value);
}

// Function to construct the tree based on user input
Node* constructTree() {
    int data;
    cout << "Enter root node value (-1 to stop): ";
    cin >> data;
    if (data == -1)
        return NULL;

    Node* root = createNode(data);

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        int leftData, rightData;
        cout << "Enter left child value of " << current->data << " (-1 for no left child): ";
        cin >> leftData;
        if (leftData != -1) {
            current->left = createNode(leftData);
            q.push(current->left);
        }

        cout << "Enter right child value of " << current->data << " (-1 for no right child): ";
        cin >> rightData;
        if (rightData != -1) {
            current->right = createNode(rightData);
            q.push(current->right);
        }
    }

    return root;
}

// Function to perform Depth First Search (DFS) in parallel
void parallelDFS(Node* root) {
    if (root == NULL)
        return;

    cout << root->data << " ";

    // Parallelize the exploration of left and right subtrees
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            #pragma omp task
            parallelDFS(root->left);
        }

        #pragma omp single nowait
        {
            #pragma omp task
            parallelDFS(root->right);
        }
    }
}

int main() {
    // Construct the tree
    Node* root = constructTree();

    // Perform parallel DFS
    cout << "Depth First Search (DFS): ";
    parallelDFS(root);
    cout << endl;

    return 0;
}
