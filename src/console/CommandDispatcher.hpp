#pragma once

#include "Command.hpp"

#include <memory>
#include <unordered_map>

class CommandDispatcher
{
public:
    void registerCommand(std::unique_ptr<Command> cmd);
    bool dispatch(const QString& line, const CommandContext& ctx,
                  QString& error);

private:
    std::unordered_map<QString, std::unique_ptr<Command>> m_commands;
};
