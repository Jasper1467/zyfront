#pragma once

#include <cstdint>

class DisasmView;
class MainWindow;

struct CommandContext
{
    MainWindow* mainWindow = nullptr;
    DisasmView* disasmView = nullptr;
};
