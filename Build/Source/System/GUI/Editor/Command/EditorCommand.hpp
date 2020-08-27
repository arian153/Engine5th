#pragma once
#include <string>

namespace Engine5
{
    class CommandRegistry;

    class EditorCommand
    {
    public:
        EditorCommand();
        virtual ~EditorCommand();

        virtual void Execute() = 0;
        virtual void UnExecute() = 0;
        void         SetRegistry(CommandRegistry* registry);
        std::string  Type() const;

    protected:
        CommandRegistry* m_registry = nullptr;
        std::string      m_type;
    private:
    };
}
