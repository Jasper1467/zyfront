#include "GotoCommand.hpp"

#include "../CommandContext.hpp"

#include "../../disasm/DisasmView.hpp"

bool GotoCommand::execute(const QStringList& args, const CommandContext& ctx,
                          QString& error)
{
    if (!ctx.disasmView)
    {
        error = "No active disassembly view";
        return false;
    }

    if (args.isEmpty())
    {
        error = "Usage: goto <address> [--nearest]";
        return false;
    }

    bool ok = false;
    uint64_t addr = args[0].toULongLong(&ok, 16);
    if (!ok)
    {
        error = "Invalid address";
        return false;
    }

    bool nearest = args.contains("--nearest");

    if (!ctx.disasmView->gotoAddress(addr, nearest))
    {
        error = QString("Address 0x%1 not found").arg(addr, 0, 16);
        return false;
    }

    return true;
}
