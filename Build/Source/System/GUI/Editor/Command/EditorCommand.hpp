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

    template <typename T>
    class EditData final : public EditorCommand
    {
    public:
        EditData(T& data, const T& changed)
            : m_data(data), m_prev(data), m_next(changed)
        {
            m_type = "Edit Data";
        }

        ~EditData()
        {
        }

        void Execute() override
        {
            m_data = m_next;
        }

        void UnExecute() override
        {
            m_data = m_prev;
        }

    private:
        T& m_data;
        T  m_prev;
        T  m_next;
    };
}
