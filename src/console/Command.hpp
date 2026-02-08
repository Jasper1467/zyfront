#pragma once

#include <QString>
#include <QStringList>

struct CommandContext;

class Command
{
public:
    virtual ~Command() = default;

    virtual QString name() const = 0;
    virtual QString help() const = 0;

    virtual bool execute(const QStringList& args, const CommandContext& ctx,
                         QString& error) = 0;
};
