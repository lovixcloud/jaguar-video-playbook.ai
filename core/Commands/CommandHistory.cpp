#include "CommandHistory.h"

namespace jaguar {

CommandHistory& CommandHistory::Instance() {
    static CommandHistory instance;
    return instance;
}

void CommandHistory::ExecuteCommand(std::unique_ptr<ICommand> command) {
    if (!command) return;

    command->Execute();

    if (m_currentIndex < m_history.size()) {
        m_history.erase(m_history.begin() + m_currentIndex, m_history.end());
    }

    m_history.push_back(std::move(command));
    m_currentIndex = m_history.size();
}

bool CommandHistory::CanUndo() const {
    return m_currentIndex > 0;
}

bool CommandHistory::CanRedo() const {
    return m_currentIndex < m_history.size();
}

void CommandHistory::Undo() {
    if (CanUndo()) {
        --m_currentIndex;
        m_history[m_currentIndex]->Undo();
    }
}

void CommandHistory::Redo() {
    if (CanRedo()) {
        m_history[m_currentIndex]->Execute();
        ++m_currentIndex;
    }
}

void CommandHistory::Clear() {
    m_history.clear();
    m_currentIndex = 0;
}

} // namespace jaguar
