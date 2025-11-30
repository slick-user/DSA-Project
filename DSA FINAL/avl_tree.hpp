#pragma once
#include "theme.hpp"

struct AVLNode {
    Theme theme;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(Theme t) : theme(t), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    AVLNode* root;
    int themeCount;

    int height(AVLNode* N) {
        if (N == nullptr) return 0;
        return N->height;
    }

    int max(int a, int b) {
        return (a > b) ? a : b;
    }

    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    int getBalance(AVLNode* N) {
        if (N == nullptr) return 0;
        return height(N->left) - height(N->right);
    }

    AVLNode* insert(AVLNode* node, Theme theme) {
        if (node == nullptr) {
            themeCount++;
            return new AVLNode(theme);
        }

        if (theme < node->theme)
            node->left = insert(node->left, theme);
        else if (theme > node->theme)
            node->right = insert(node->right, theme);
        else
            return node; // Duplicate keys not allowed

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && theme < node->left->theme)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && theme > node->right->theme)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && theme > node->left->theme) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && theme < node->right->theme) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void inorder(AVLNode* node, Theme* array, int& index) {
        if (node != nullptr) {
            inorder(node->left, array, index);
            if (index < themeCount) {
                array[index] = node->theme;
                index++;
            }
            inorder(node->right, array, index);
        }
    }

    void deleteTree(AVLNode* node) {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    AVLNode* copyTree(AVLNode* node) {
        if (node == nullptr) return nullptr;

        AVLNode* newNode = new AVLNode(node->theme);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);
        newNode->height = node->height;

        return newNode;
    }

    AVLNode* search(AVLNode* node, int themeId) {
        if (node == nullptr || node->theme.id == themeId)
            return node;

        if (themeId < node->theme.id)
            return search(node->left, themeId);
        else
            return search(node->right, themeId);
    }

public:

    // CONSTRUCTORS

    AVLTree() : root(nullptr) {}

    AVLTree(const AVLTree& other) : root(nullptr), themeCount(0) {
        if (other.root != nullptr) {
            root = copyTree(other.root);
            themeCount = other.themeCount;
        }
    }

    // DESTRUCTOR
    ~AVLTree() {
        deleteTree(root);
    }

    void insert(Theme theme) {
        root = insert(root, theme);
    }

    void getAllThemes(Theme* themesArray, int& count) {
        count = 0;
        if (root != nullptr) {
            inorder(root, themesArray, count);
        }
    }
    
    Theme getThemeByIndex(int index) {
        if (index < 0 || index >= themeCount) {
            return Theme();
        }

        Theme* themes = new Theme[themeCount];
        int count = 0;
        getAllThemes(themes, count);

        Theme result = themes[index];
        delete[] themes;
        return result;
    }
    
    Theme* search(int themeId) {
        AVLNode* result = search(root, themeId);
        if (result != nullptr) {
            return &(result->theme);
        }
        return nullptr;
    }

    // Optional: Also add a method to get index by ID
    int getIndexById(int themeId) {
        Theme* allThemes = new Theme[themeCount];
        int count = 0;
        getAllThemes(allThemes, count);

        for (int i = 0; i < count; i++) {
            if (allThemes[i].id == themeId) {
                delete[] allThemes;
                return i;
            }
        }

        delete[] allThemes;
        return -1;
    }

    int getSize() const {
        return themeCount;
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    void clear() {
        deleteTree(root);
        root = nullptr;
        themeCount = 0;
    }
};
