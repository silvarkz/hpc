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


// Function to perform Breadth First Search (BFS) in parallel
void parallelBFS(Node* root) {
    if (root == NULL)
        return;

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        int level_size = q.size();
        
        #pragma omp parallel
        {
            #pragma omp for
            for (int i = 0; i < level_size; ++i) {
                Node* current;
                #pragma omp critical
                {
                    current = q.front();
                    q.pop();
                }

                cout << current->data << " ";

                if (current->left != NULL)
                    #pragma omp critical
                    q.push(current->left);
                if (current->right != NULL)
                    #pragma omp critical
                    q.push(current->right);
            }
        }

    }
}

int main() {
    // Constructing the tree based on user input
    Node* root = constructTree();

    if (root == NULL) {
        cout << "Empty tree!" << endl;
        return 0;
    }

    cout << "Breadth First Search (BFS): ";
    parallelBFS(root);
    cout << endl;

    return 0;
}
