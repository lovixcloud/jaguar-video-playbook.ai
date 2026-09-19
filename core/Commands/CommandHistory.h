#pragma once

#include <memory>
#include <vector>
#include <string>

namespace jaguar {

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void Execute() = 0;
    virtual void Undo() = 0;
    virtual std::string GetName() const = 0;
};

class CommandHistory {
public:
    static CommandHistory& Instance();

    void ExecuteCommand(std::unique_ptr<ICommand> command);
    bool CanUndo() const;
    bool CanRedo() const;
    void Undo();
    void Redo();
    void Clear();

private:
    CommandHistory() = default;

    std::vector<std::unique_ptr<ICommand>> m_history;
    size_t m_currentIndex{ 0 };
};

} // namespace jaguar
