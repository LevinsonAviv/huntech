#pragma once

/**
 * @brief template class of AVL tree with keys of type Key and values of type Value.
*/
template <class Key, class Value>
class AVLTree{

public:
    /**
     * @brief default constructor that creates an empty AVL tree.
    */
    AVLTree();

    // delete copy constructor
    AVLTree(const AVLTree& tree) = delete;

    // delete assignment operator
    AVLTree& operator=(const AVLTree& tree) = delete;

    /**
     * @brief destructor that deletes all memory allocated to the AVL tree.
    */
    ~AVLTree();

    /**
     * @brief finds a key in the AVL tree.
     * 
     * @param key the key to find in the AVL.
     * 
     * @return a pointer to the value of the key or nullptr if not found.
    */
    Value* find(const Key& key);

    /**
     * @brief finds a key in the AVL tree.
     * 
     * @param key the key to find in the AVL.
     * 
     * @return a pointer to the value of the key or nullptr if not found.
    */
    const Value* find(const Key& key) const;

    Value* select(int index);

    /**
     * @brief inserts a key with a value to the AVL tree.
     * 
     * @param key the key of the value to insert.
     * @param value the value to insert.
     * 
     * @return true if the insertion suceeded or false otherwise.
    */
    bool insert(const Key& key, const Value& value);

    /**
     * @brief removes a key and it's value from the AVL tree.
     * 
     * @param key the key to remove.
     * 
     * @return true if the remove suceeded or false otherwise.
    */
    bool remove(const Key& key);

    /**
     * @brief checks if the AVL tree is empty.
     * 
     * @return true if the AVL tree is empty or false otherwise.
    */
    bool isEmpty() const;

private:
    /**
     * @brief struct of node that stores a pair of key and value.
    */
    struct Node {
        /**
         * @brief key of the value stored in the node.
        */
        Key key;

        /**
         * @brief value stored in the node.
        */
        Value value;

        /**
         * @brief pointers to the left and right childs of the node.
        */
        Node *left, *right;

        /**
         * @brief the height of the node on the AVL tree.
        */
        int height;

        int size;
    };

    /**
     * @brief pointer to the root of the AVL tree.
    */
    Node *root;

    /**
     * @brief returns the height of a node.
     * 
     * @param node the node to return it's height.
     * 
     * @return the height of the node.
    */
    int getHeight(Node *node) const;

    int getSize(Node *node) const;

    /**
     * @brief update the height of a node.
     * 
     * @param node the node to update it's height.
    */
    void updateHeight(Node *node);

    void updateSize(Node *node);

    /**
     * @brief returns the balance factor of a node.
     * 
     * @param node the node to return it's balance factor.
     * 
     * @return the balance factor of the node.
    */
    int calcBF(Node *node) const;

    /**
     * @brief performs a LL rotation on the root.
     * 
     * @param root the root of the AVL tree or subtree.
     * 
     * @return a pointer to the new root after rotation.
    */
    Node* LLRotation(Node *root);

    /**
     * @brief performs a RR rotation on the root.
     * 
     * @param root the root of the AVL tree or subtree.
     * 
     * @return a pointer to the new root after rotation.
    */
    Node* RRRotation(Node *root);

    /**
     * @brief performs a LR rotation on the root.
     * 
     * @param root the root of the AVL tree or subtree.
     * 
     * @return a pointer to the new root after rotation.
    */
    Node* LRRotation(Node *root);

    /**
     * @brief performs a RL rotation on the root.
     * 
     * @param root the root of the AVL tree or subtree.
     * 
     * @return a pointer to the new root after rotation.
    */
    Node* RLRotation(Node *root);

    /**
     * @brief rebalancing the root if needed by determining and performing the appropriate rotation.
     * 
     * @param root the root of the AVL tree or subtree.
     * 
     * @return a pointer to the new root after rebalancing.
    */
    Node* rebalanceTree(Node *root);

    /**
     * @brief swaps between the root and the pivot. Pivot is a descendant of root and has only right child.
     * 
     * @param root the root of the AVL tree or subtree.
     * @param pivot the pivot to swap with the root.
     * @param pivotParent the parent of pivot in the AVL tree.
    */
    void swapNodes(Node *root, Node *pivot, Node *pivotParent);

    /**
     * @brief a helper function that finds a key in the AVL tree.
     * 
     * @param node the current node in the find path.
     * @param key the key to find in the AVL.
     * 
     * @return a pointer to the value of the key or nullptr if not found.
    */
    Node* findNode(Node *node, const Key& key);

    /**
     * @brief a helper function that finds a key in the AVL tree.
     * 
     * @param node the current node in the find path.
     * @param key the key to find in the AVL.
     * 
     * @return a pointer to the value of the key or nullptr if not found.
    */
    const Node* findNode(const Node *node, const Key& key) const;

    Node* selectNode(Node *node, int index);

    /**
     * @brief a helper function that inserts a key with a value to the AVL tree.
     * 
     * @param node the current node in the find path.
     * @param key the key of the value to insert.
     * @param value the value to insert.
     * 
     * @return a pointer to the new root after the insertion.
    */
    Node* insertNode(Node *node, const Key& key, const Value& value);

    /**
     * @brief a helper function that removes a key and it's value from the AVL tree.
     * 
     * @param node the current node in the find path.
     * @param key the key to remove.
     * 
     * @return a pointer to the new root after the remove.
    */
    Node* removeNode(Node *node, const Key& key);

    /**
     * @brief deletes all memory allocated to the nodes of the AVL tree.
     * 
     * @param node the root of the AVL tree.
    */
    void deleteNodes(Node *root);
};

template <class Key, class Value>
AVLTree<Key, Value>::AVLTree(): root(nullptr){}

template <class Key, class Value>
AVLTree<Key, Value>::~AVLTree(){
    deleteNodes(root);
}

template <class Key, class Value>
Value* AVLTree<Key, Value>::find(const Key& key){
    Node *result = findNode(root, key);
    if(result == nullptr){
        return nullptr;
    }
    return &(result->value);
}

template <class Key, class Value>
const Value* AVLTree<Key, Value>::find(const Key& key) const {
    const Node *result = findNode(root, key);
    if(result == nullptr){
        return nullptr;
    }
    return &(result->value);
}

template <class Key, class Value>
Value* AVLTree<Key, Value>::select(int index){
    Node *result = selectNode(root, index);
    if(result == nullptr){
        return nullptr;
    }
    return &(result->value);
}

template <class Key, class Value>
bool AVLTree<Key, Value>::insert(const Key& key, const Value& value){
    if(find(key) != nullptr){
        return false;
    }
    root = insertNode(root, key, value);
    return true;
}

template <class Key, class Value>
bool AVLTree<Key, Value>::remove(const Key& key){
    if(find(key) == nullptr){
        return false;
    }
    root = removeNode(root, key);
    return true;
}

template <class Key, class Value>
bool AVLTree<Key, Value>::isEmpty() const{
    return root == nullptr;
}

template <class Key, class Value>
int AVLTree<Key, Value>::getHeight(Node *node) const {
    if(node == nullptr){
        return -1;
    }
    return node->height;
}

template <class Key, class Value>
int AVLTree<Key, Value>::getSize(Node *node) const {
    if(node == nullptr){
        return 0;
    }
    return node->size;
}

template <class Key, class Value>
void AVLTree<Key, Value>::updateHeight(Node *node){
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    node->height = ((leftHeight > rightHeight) ? leftHeight : rightHeight) + 1;
}

template <class Key, class Value>
void AVLTree<Key, Value>::updateSize(Node *node){
    node->size = getSize(node->left) + getSize(node->right) + 1;
}

template <class Key, class Value>
int AVLTree<Key, Value>::calcBF(Node *node) const {
    if(node == nullptr){
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

template <class Key, class Value>
typename AVLTree<Key, Value>::Node* AVLTree<Key, Value>::LLRotation(Node *root){
    Node *a = root->left;
    root->left = a->right;
    a->right = root;
    updateHeight(root);
    updateSize(root);
    updateHeight(a);
    updateSize(a);
    return a;
}

template <class Key, class Value>
typename AVLTree<Key, Value>::Node* AVLTree<Key, Value>::RRRotation(Node *root){
    Node *a = root->right;
    root->right = a->left;
    a->left = root;
    updateHeight(root);
    updateSize(root);
    updateHeight(a);
    updateSize(a);
    return a;
}


template <class Key, class Value>
typename AVLTree<Key, Value>::Node* AVLTree<Key, Value>::LRRotation(Node *root){
    Node *a = root->left;
    root->left = RRRotation(a);
    return LLRotation(root);
}

template <class Key, class Value>
typename AVLTree<Key, Value>::Node* AVLTree<Key, Value>::RLRotation(Node *root){
    Node *a = root->right;
    root->right = LLRotation(a);
    return RRRotation(root);
}

template <class Key, class Value>
typename AVLTree<Key, Value>::Node* AVLTree<Key, Value>::rebalanceTree(Node *root){
    int rootBF = calcBF(root);
    if(rootBF == 2){
        if(calcBF(root->left) > -1){
            return LLRotation(root);
        }else{
            return LRRotation(root);
        }
    }else if(rootBF == -2){
        if(calcBF(root->right) < 1){
            return RRRotation(root);
        }else{
            return RLRotation(root);
        }
    }
    return root;
}

template <class Key, class Value>
void AVLTree<Key, Value>::swapNodes(Node *root, Node *pivot, Node *pivotParent){
    Node *pivotRight = pivot->right;
    if(pivotParent == root){
        pivot->right = root;
    }else{
        pivotParent->left = root;
        pivot->right = root->right;
    }
    pivot->left = root->left;
    root->left = nullptr;
    root->right = pivotRight;
    int pivotHeight = pivot->height;
    pivot->height = root->height;
    root->height = pivotHeight;
    int pivotSize = pivot->size;
    pivot->size = root->size;
    root->size = pivotSize;
}

template <class Key, class Value>
typename AVLTree<Key, Value>::Node* AVLTree<Key, Value>::findNode(Node *node, const Key& key){
    if(node == nullptr || node->key == key){
        return node;
    }
    if(key < node->key){
        return findNode(node->left, key);
    }else{
        return findNode(node->right, key);
    }
}

template <class Key, class Value>
const typename AVLTree<Key, Value>::Node* AVLTree<Key, Value>::findNode(const Node *node, const Key& key) const{
    if(node == nullptr || node->key == key){
        return node;
    }
    if(key < node->key){
        return findNode(node->left, key);
    }else{
        return findNode(node->right, key);
    }
}

template <class Key, class Value>
typename AVLTree<Key, Value>::Node* AVLTree<Key, Value>::selectNode(Node *node, int index){
    if(node == nullptr || getSize(node->left) == index - 1){
        return node;
    }
    if(getSize(node->left) > index - 1){
        return selectNode(node->left, index);
    }else{
        return selectNode(node->right, index - getSize(node->left) - 1);
    }
}

template <class Key, class Value>
typename AVLTree<Key, Value>::Node* AVLTree<Key, Value>::insertNode(Node *node, const Key& key, const Value& value){
    if(node == nullptr){
        return new Node{key, value, nullptr, nullptr, 0, 1};
    }
    if(key < node->key){
        node->left = insertNode(node->left, key, value);
    }else if(key > node->key){
        node->right = insertNode(node->right, key, value);
    }
    updateHeight(node);
    node = rebalanceTree(node);
    updateSize(node);
    return node;
}

template <class Key, class Value>
typename AVLTree<Key, Value>::Node* AVLTree<Key, Value>::removeNode(Node *node, const Key& key){
    if(node == nullptr){
        return nullptr;
    }
    if(key < node->key){
        node->left = removeNode(node->left, key);
    }
    else if(key > node->key){
        node->right = removeNode(node->right, key);
    }
    else{
        if(node->left == nullptr){
            Node *child = node->right;
            delete node;
            return child;
        }
        if(node->right == nullptr){
            Node *child = node->left;
            delete node;
            return child;
        }
        Node *pivot = node->right, *pivotParent = node;
        while(pivot->left != nullptr){
            pivotParent = pivot;
            pivot = pivot->left;
        }
        swapNodes(node, pivot, pivotParent);
        node = pivot;
        node->right = removeNode(node->right, key);
    }
    updateHeight(node);
    node = rebalanceTree(node);
    updateSize(node);
    return node;
}

template <class Key, class Value>
void AVLTree<Key, Value>::deleteNodes(Node *root){
    if(root != nullptr){
        deleteNodes(root->right);
        deleteNodes(root->left);
        delete root;
    }
}