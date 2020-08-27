#include "EditorCommand.hpp"

namespace Engine5
{
    EditorCommand::EditorCommand()
    {
    }

    EditorCommand::~EditorCommand()
    {
    }

    void EditorCommand::SetRegistry(CommandRegistry* registry)
    {
        m_registry = registry;
    }

    std::string EditorCommand::Type() const
    {
        return m_type;
    }
}
