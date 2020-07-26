#pragma once
#include "AINode.hpp"

namespace Engine5
{
    class DecoratorNode : public AINode
    {
    public:
        DecoratorNode();
        virtual ~DecoratorNode();

        void AddChild(AINode* child);
        bool IsEmpty() const;

    protected:
        AINode* m_child = nullptr;
    };

    class WhileNode final : public DecoratorNode
    {
    public:
        WhileNode();
        ~WhileNode();

        eAINodeState OnUpdate(Real dt) override;
    private:
    };

    class IfNode final : public DecoratorNode
    {
    public:
        IfNode();
        ~IfNode();

        eAINodeState OnUpdate(Real dt) override;
    private:
    };

    class RepeatNode final : public DecoratorNode
    {
    public:
        RepeatNode();
        ~RepeatNode();

        eAINodeState OnUpdate(Real dt) override;
    private:
    };
}
