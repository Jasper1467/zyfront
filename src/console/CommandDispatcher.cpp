#include "CommandDispatcher.hpp"

void CommandDispatcher::registerCommand(std::unique_ptr<Command> cmd)
{
    m_commands.emplace(cmd->name(), std::move(cmd));
}

bool CommandDispatcher::dispatch(const QString& line, const CommandContext& ctx,
                                 QString& error)
{
    QStringList tokens = line.split(' ', Qt::SkipEmptyParts);
    if (tokens.isEmpty())
        return true;

    const QString cmdName = tokens.takeFirst();

    auto it = m_commands.find(cmdName);
    if (it == m_commands.end())
    {
        error = QString("Unknown command: %1").arg(cmdName);
        return false;
    }

    return it->second->execute(tokens, ctx, error);
}
