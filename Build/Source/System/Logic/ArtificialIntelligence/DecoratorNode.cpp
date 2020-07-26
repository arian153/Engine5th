#include "DecoratorNode.hpp"

namespace Engine5
{
    DecoratorNode::DecoratorNode()
    {
    }

    DecoratorNode::~DecoratorNode()
    {
    }

    void DecoratorNode::AddChild(AINode* child)
    {
        m_child = child;
    }

    bool DecoratorNode::IsEmpty() const
    {
        return m_child == nullptr;
    }

    WhileNode::WhileNode()
    {
    }

    WhileNode::~WhileNode()
    {
    }

    eAINodeState WhileNode::OnUpdate(Real dt)
    {
        return eAINodeState::Failure;
    }

    IfNode::IfNode()
    {
    }

    IfNode::~IfNode()
    {
    }

    eAINodeState IfNode::OnUpdate(Real dt)
    {
        return eAINodeState::Failure;
    }

    RepeatNode::RepeatNode()
    {
    }

    RepeatNode::~RepeatNode()
    {
    }

    eAINodeState RepeatNode::OnUpdate(Real dt)
    {
        return eAINodeState::Failure;
    }
}
