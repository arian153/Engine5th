#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "AINode.hpp"

namespace Engine5
{
    class SequenceNode;
    class BlackBoard;

    class RootNode final : public AINode
    {
    public:
        RootNode();
        ~RootNode();

        eAINodeState OnUpdate(Real dt) override;

    private:
        AINode* m_child = nullptr;
    };

    class BehaviorTree
    {
    public:
        BehaviorTree();
        ~BehaviorTree();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

        void SetGlobalBoard(BlackBoard* global_board);

    private:
        eAINodeState m_state        = eAINodeState::None;
        RootNode*    m_root         = nullptr;
        BlackBoard*  m_black_board  = nullptr;
        BlackBoard*  m_global_board = nullptr;
    };
}
