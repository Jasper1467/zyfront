#pragma once

#include "../Command.hpp"

class GotoCommand : public Command
{
public:
    QString name() const override { return "goto"; }
    QString help() const override { return "goto <address> [--nearest]"; }

    bool execute(const QStringList& args, const CommandContext& ctx,
                 QString& error) override;
};
