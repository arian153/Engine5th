#include "BehaviorTree.hpp"
#include "CompositeNode.hpp"
#include "BlackBoard.hpp"

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
        if (m_black_board == nullptr)
        {
            m_black_board = new BlackBoard();
        }
        m_black_board->Clear();
    }

    void BehaviorTree::Update(Real dt)
    {
        m_state = m_root->Invoke(dt);
    }

    void BehaviorTree::Shutdown()
    {
        if (m_black_board != nullptr)
        {
            m_black_board->Clear();
            delete m_black_board;
            m_black_board = nullptr;
        }
    }

    void BehaviorTree::SetGlobalBoard(BlackBoard* black_board)
    {
        m_global_board = black_board;
    }
}
