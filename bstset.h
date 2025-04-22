#ifndef BSTSET_H
#define BSTSET_H

template <typename T>
class BSTSet {
private:
    struct Node;

public:
    class SetIterator {
    public:
        SetIterator() 
            : m_current(nullptr) {}

        const T* get_and_advance() {
            if (!m_current) {
                return nullptr;
            }
            const T* ret = &(m_current->value);
            m_current = find_successor(m_current);
            return ret;
        }

        SetIterator(Node* node) : m_current(node) {}

    private:
        Node* m_current;

        Node* find_successor(Node* node) {
            if (!node) {
                return nullptr;
            }
            if (node->right) {
                Node* p = node->right;
                while (p->left) {
                    p = p->left;
                }
                return p;
            }
            Node* p = node;
            while (p->parent && p == p->parent->right) {
                p = p->parent;
            }
            return p->parent;
        }
    };

    BSTSet() : m_root(nullptr) {}

    ~BSTSet() {
        destroy(m_root);
    }

    

    SetIterator find(const T& value) const {
        Node* curr = m_root;
        while (curr != nullptr) {
            if (value < curr->value) {
                curr = curr->left;
            }
            else if (curr->value < value) {
                curr = curr->right;
            }
            else {
                return SetIterator(curr);
            }
        }
        return SetIterator();
    }

    SetIterator find_first_not_smaller(const T& value) const {
        Node* curr = m_root;
        Node* candidate = nullptr;
        while (curr) {
            if (!(curr->value < value)) {
                candidate = curr;
                curr = curr->left;
            }
            else {
                curr = curr->right;
            }
        }
        return SetIterator(candidate);
    }
    void insert(const T& value) {
        if (!m_root) {
            m_root = new Node(value, nullptr);
            return;
        }
        Node* curr = m_root;
        Node* parent = nullptr;
        while (curr != nullptr) {
            parent = curr;
            if (value < curr->value) {
                curr = curr->left;
            }
            else if (curr->value < value) {
                curr = curr->right;
            }
            else {
                curr->value = value;
                return;
            }
        }
        if (value < parent->value) {
            parent->left = new Node(value, parent);
        }
        else {
            parent->right = new Node(value, parent);
        }
    }
private:
    struct Node {
        T value;
        Node* left;
        Node* right;
        Node* parent;
        Node(const T& val, Node* par) : value(val), left(nullptr), right(nullptr), parent(par) {}
    };
    Node* m_root;

    void destroy(Node* node) {
        if (node == nullptr) {
            return;
        }
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
};

#endif