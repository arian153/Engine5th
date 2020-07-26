#include "CompositeNode.hpp"

namespace Engine5
{
    CompositeNode::CompositeNode()
        : m_iterator(m_children.begin())
    {
    }

    CompositeNode::~CompositeNode()
    {
    }

    void CompositeNode::AddChild(AINode* child)
    {
        m_children.push_back(child);
    }

    bool CompositeNode::IsEmpty() const
    {
        return m_children.empty();
    }

    SelectorNode::SelectorNode()
    {
    }

    SelectorNode::~SelectorNode()
    {
    }

    eAINodeState SelectorNode::OnUpdate(Real dt)
    {
        return eAINodeState::Failure;
    }

    SequenceNode::SequenceNode()
    {
    }

    SequenceNode::~SequenceNode()
    {
    }

    eAINodeState SequenceNode::OnUpdate(Real dt)
    {
        return eAINodeState::Failure;
    }
}
