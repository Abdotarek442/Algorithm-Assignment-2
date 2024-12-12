
#include <iostream>
#include <queue>

using namespace std;

template<class T>
struct Node {
    T data;
    Node* parent;
    Node* left;
    Node* right;
    char color;

    Node(T el, Node* parent) {
        this->data = el;
        this->color = 'R';
        this->left = nullptr;
        this->right = nullptr;
        this->parent = parent;
    }
};

template<class T>
class RBT {
private:
    Node<T>* root = nullptr;

    void rotateLeft(Node<T>* C) {
        Node<T>* B = C->right;
        C->right = B->left;
        if (B->left != nullptr) {
            B->left->parent = C;
        }
        B->parent = C->parent;
        if (C->parent == nullptr) {
            root = B;
        } else if (C == C->parent->left) {
            C->parent->left = B;
        } else {
            C->parent->right = B;
        }
        B->left = C;
        C->parent = B;
    }

    void rotateRight(Node<T>* C) {
        Node<T>* B = C->left;
        C->left = B->right;
        if (B->right != nullptr) {
            B->right->parent = C;
        }
        B->parent = C->parent;
        if (C->parent == nullptr) {
            root = B;
        } else if (C == C->parent->right) {
            C->parent->right = B;
        } else {
            C->parent->left = B;
        }
        B->right = C;
        C->parent = B;
    }

    void fixTree(Node<T>* currentNode) {
        while (currentNode->parent != nullptr && currentNode->parent->color == 'R') {
            if (currentNode->parent == currentNode->parent->parent->left) {
                Node<T>* sibling = currentNode->parent->parent->right;
                //first test case 
                if (sibling != nullptr && sibling->color == 'R') {
                    currentNode->parent->color = 'B';
                    sibling->color = 'B';
                    currentNode->parent->parent->color = 'R';
                    currentNode = currentNode->parent->parent;
                } 
                else {
                    // case 3 double rotation if it was right and the current is left 
                    if (currentNode == currentNode->parent->right) {
                        currentNode = currentNode->parent;
                        rotateLeft(currentNode);
                    }
                    currentNode->parent->color = 'B';
                    currentNode->parent->parent->color = 'R';
                    rotateRight(currentNode->parent->parent);
                }
            } 
            else {
                Node<T>* y = currentNode->parent->parent->left;
                if (y != nullptr && y->color == 'R') {
                    // case 1,2
                    currentNode->parent->color = 'B';
                    y->color = 'B';
                    currentNode->parent->parent->color = 'R';
                    currentNode = currentNode->parent->parent;
                } else {
                    // case 3,4
                    if (currentNode == currentNode->parent->left) {
                        currentNode = currentNode->parent;
                        rotateRight(currentNode);
                    }
                    currentNode->parent->color = 'B';
                    currentNode->parent->parent->color = 'R';
                    rotateLeft(currentNode->parent->parent);
                }
            }
        }
        root->color = 'B'; 
    }

    // Insert a node and fix the tree
    void insertNode(const T& value) {
        Node<T>* y = nullptr;
        Node<T>* x = root;

        while (x != nullptr) {
            y = x;
            if (value < x->data) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        Node<T>* z = new Node<T>(value, y);
        if (y == nullptr) {
            root = z;
        } else if (value < y->data) {
            y->left = z;
        } else {
            y->right = z;
        }

        fixTree(z);
    }
    void deleteNode(Node<T>* node) {
        Node<T>* y = node;
        Node<T>* x;
        char originalColor = y->color;
        if (node->left == nullptr) 
        {
            x = node->right;
            swapNode(node, node->right);
        } else if (node->right == nullptr) 
        {
            x = node->left;
            swapNode(node, node->left);
        } 
        else {
            y = Predecessor(node->right);
            originalColor = y->color;
            x = y->right;
            if (y->parent == node) {
                if (x != nullptr) x->parent = y;
            } 
            else 
            {
                swapNode(y, y->right);
                y->right = node->right;
                y->right->parent = y;
            }
            swapNode(node, y);
            y->left = node->left;
            y->left->parent = y;
            y->color = node->color;
        }
        delete node;
        if (originalColor == 'B') {
            fixDelete(x);
        }
    }
      void fixDelete(Node<T>* x) {
        while (x != root && (x == nullptr || x->color == 'B')) {
            if (x == x->parent->left) {
                Node<T>* sibling = x->parent->right;
                if (sibling->color == 'R') {
                    sibling->color = 'B';
                    x->parent->color = 'R';
                    rotateLeft(x->parent);
                    sibling = x->parent->right;
                }
                if ((sibling->left == nullptr || sibling->left->color == 'B') &&
                    (sibling->right == nullptr || sibling->right->color == 'B')) {
                    sibling->color = 'R';
                    x = x->parent;
                } else {
                    if (sibling->right == nullptr || sibling->right->color == 'B') {
                        if (sibling->left != nullptr) sibling->left->color = 'B';
                        sibling->color = 'R';
                        rotateRight(sibling);
                        sibling = x->parent->right;
                    }
                    sibling->color = x->parent->color;
                    x->parent->color = 'B';
                    if (sibling->right != nullptr) sibling->right->color = 'B';
                    rotateLeft(x->parent);
                    x = root;
                }
            } 
            else {
                Node<T>* sibling = x->parent->left;
                if (sibling->color == 'R') {
                    sibling->color = 'B';
                    x->parent->color = 'R';
                    rotateRight(x->parent);
                    sibling = x->parent->left;
                }
                if ((sibling->right == nullptr || sibling->right->color == 'B') &&
                    (sibling->left == nullptr || sibling->left->color == 'B')) {
                    sibling->color = 'R';
                    x = x->parent;
                } else {
                    if (sibling->left == nullptr || sibling->left->color == 'B') {
                        if (sibling->right != nullptr) sibling->right->color = 'B';
                        sibling->color = 'R';
                        rotateLeft(sibling);
                        sibling = x->parent->left;
                    }
                    sibling->color = x->parent->color;
                    x->parent->color = 'B';
                    if (sibling->left != nullptr) sibling->left->color = 'B';
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        if (x != nullptr) x->color = 'B';
    }
    void swapNode(Node<T>* u, Node<T>* v) {
        if (u->parent == nullptr) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        if (v != nullptr) {
            v->parent = u->parent;
        }
    }

    Node<T>* Predecessor(Node<T>* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }
    Node<T>* search(const T& value, Node<T>* current) {
        if (current == nullptr || current->data == value) {
            return current;
        }
        if (value < current->data) {
            return search(value, current->left);
        } else {
            return search(value, current->right);
        }
    }

   
public:
    RBT() {}

    ~RBT() {
        deleteTree(root);
    }

    void deleteTree(Node<T>* node) {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    void Delete(const T& value) {
        Node<T>* node = search(value, root);
        if (node != nullptr) {
            deleteNode(node);
        }
    }

    void Insert(const T& value) {
        insertNode(value);
    }
    void printTreeLevelOrder() {
        if (root == nullptr) {
            cout << "The tree is empty." << endl;
            return;
        }
        queue<Node<T>*> q;
        q.push(root);
        while (!q.empty()) {
            int levelSize = q.size();
            for (int i = 0; i < levelSize; ++i) {
                Node<T>* currentNode = q.front();
                q.pop();

                cout << currentNode->data << "(" << currentNode->color << ") ";

                if (currentNode->left != nullptr) {
                    q.push(currentNode->left);
                }
                if (currentNode->right != nullptr) {
                    q.push(currentNode->right);
                }
            }
            cout << endl;
        }
    }

};

int main() {
    RBT<int> tree;
    tree.Insert(10);
    tree.Insert(5);
    tree.Insert(15);
    tree.Insert(3);
    tree.Insert(1);
    tree.printTreeLevelOrder();
    tree.Delete(3);
    tree.printTreeLevelOrder();

    return 0;
}
