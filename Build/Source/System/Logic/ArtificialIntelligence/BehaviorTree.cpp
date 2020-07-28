#include "BehaviorTree.hpp"
#include "CompositeNode.hpp"

namespace Engine5
{
    RootNode::RootNode()
    {
    }

    RootNode::~RootNode()
    {
    }

    eAINodeState RootNode::OnUpdate(Real dt)
    {
        return m_child->Invoke(dt);
    }

    BehaviorTree::BehaviorTree()
    {
    }

    BehaviorTree::~BehaviorTree()
    {
    }

    void BehaviorTree::Initialize()
    {
    }

    void BehaviorTree::Update(Real dt)
    {
        m_state = m_root->Invoke(dt);
    }

    void BehaviorTree::Shutdown()
    {
    }
}
