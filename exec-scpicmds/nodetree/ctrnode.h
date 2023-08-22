#ifndef CTRNODE_H
#define CTRNODE_H

#include <vector>
#include <iterator>
#include <cstddef>
#include <functional>
#include "ictrnode.h"

class CtrNode : public ICtrNode
{
public:
    CtrNode();
    ~CtrNode();
    void exec(std::function<void(INode*)> *f = nullptr) override;
    void append(INode *node) override;
    bool remove(INode *node) override;
    void clear() override;
    bool isEmpty() override;
    bool hasLeaves() override;
    void traverse(std::function<void(INode*)> f) override;

    struct iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = INode;
        using pointer           = INode*;  // or also value_type*
        using reference         = INode&;  // or also value_type&

        iterator(std::vector<INode*> *nodesLinear) :
            m_nodesLinear(nodesLinear),
            m_ptr((nodesLinear != nullptr && nodesLinear->size() > 0) ? (*nodesLinear)[0] : nullptr) {
        }

        ~iterator() {
            delete m_nodesLinear;
        }

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }

        // Prefix increment
        iterator& operator++() { itNodeIdx++; m_ptr = (itNodeIdx < m_nodesLinear->size()) ? (*m_nodesLinear)[itNodeIdx] : nullptr; return *this; }
        // Postfix increment
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const iterator& a, const iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const iterator& a, const iterator& b) { return a.m_ptr != b.m_ptr; };

    private:
        std::vector<INode*> *m_nodesLinear = nullptr;
        int itNodeIdx = 0;
        pointer m_ptr = nullptr;
    };

    iterator begin() {
        auto m_nodesLinear = new std::vector<INode*>;
        this->traverse([m_nodesLinear] (INode *node) { m_nodesLinear->push_back(node); }); // Collect nodes

        if (m_nodesLinear->size() > 0)
            return iterator(m_nodesLinear);
         else
            return iterator(nullptr);
    }

    iterator end() {
        return iterator(nullptr);
    }

private:
    std::vector<INode*> m_nodes;
    bool m_isEmpty = true;
};

#endif // CTRNODE_H
