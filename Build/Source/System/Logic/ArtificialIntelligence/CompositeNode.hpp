#pragma once
#include "AINode.hpp"
#include <vector>

namespace Engine5
{
    class CompositeNode : public AINode
    {
    public:
        CompositeNode();
        virtual ~CompositeNode();

        void AddChild(AINode* child);
        bool IsEmpty() const;

    protected:
        std::vector<AINode*>           m_children;
        std::vector<AINode*>::iterator m_iterator;
    };

    class SelectorNode final : public CompositeNode
    {
    public:
        SelectorNode();
        ~SelectorNode();

        eAINodeState OnUpdate(Real dt) override;

    private:
    };

    class SequenceNode final : public CompositeNode
    {
    public:
        SequenceNode();
        ~SequenceNode();

        eAINodeState OnUpdate(Real dt) override;
    private:
    };
}
