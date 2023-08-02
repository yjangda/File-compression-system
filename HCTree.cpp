#include "HCTree.hpp"
#include <stack>
using namespace std;
void deleteAll(HCNode* node) {
    if (node==nullptr) {
        return;
    }
        deleteAll(node->c0);
        deleteAll(node->c1);
        node->c0 = nullptr;
        node->c1 = nullptr;
        node->p = nullptr;
        delete node;
    
}
HCTree::~HCTree() {
    leaves.clear();
    deleteAll(root);
}

bool empty(const vector<int>& freqs) {
    for (int i = 0; i < 256; i++) {
        if (freqs[i]) {
            return false;
        }
    }
    return true;
}

/* Builds a Huffman tree from the given frequency vector */
void HCTree::build(const vector<int>& freqs) {
    if (empty(freqs)) {
        return;
    }
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    for (int i = 0; i < (int) freqs.size(); i++) {
        if (freqs[i] != 0) {
            HCNode* node = new HCNode(freqs[i], i);
            leaves[i] = node;
            pq.push(node);
        }
    }
    HCNode* lnode;
    HCNode* rnode;
    while (pq.size() > 1) {
        lnode = pq.top();
        pq.pop();
        rnode = pq.top();
        pq.pop();

        HCNode* temp = new HCNode(lnode->count + rnode->count, 0);
        temp->c0 = lnode;
        temp->c1 = rnode;
        lnode->p = temp;
        rnode->p = temp;
        pq.push(temp);
    }
    // last element becomes root
    root = pq.top();
}

/* Encodes the given symbol and writes to the given BitOutputStream */
void HCTree::encode(unsigned char symbol, FancyOutputStream& out) const {
    HCNode* node = leaves[(int) symbol];
    stack<int> temp;
    while (node != root) {
        if (node->p->c0 == node) {
            temp.push(0);
        }
        else {
            temp.push(1);
        }
        node = node->p;
    }

    while (!temp.empty()) {
        out.write_bit(temp.top());
        temp.pop();
    }
}

/* Decodes the given BitInputStream and returns the symbol */
unsigned char HCTree::decode(FancyInputStream& in) const {
    HCNode* node = root;
    while (node->c0 || node->c1) {
        if (in.read_bit()) {
            node = node->c1;
        } else {
            node = node->c0;
        }
    }
    return node->symbol;
}
