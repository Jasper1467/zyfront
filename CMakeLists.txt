cmake_minimum_required(VERSION 3.24)
project(zyfront LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Zydis
set(ZYDIS_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(ZYDIS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ZYDIS_BUILD_DOXYGEN OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/Zydis)

add_executable(zyfront
    src/main.cpp
    
    src/app/MainWindow.hpp src/app/MainWindow.cpp
    src/app/StartupDialog.hpp src/app/StartupDialog.cpp

    src/disasm/DisasmView.hpp src/disasm/DisasmView.cpp
    src/disasm/DisasmModel.hpp src/disasm/DisasmModel.cpp

    src/dialogs/GotoDialog.hpp src/dialogs/GotoDialog.cpp

    src/widgets/InstructionDetailsWidget.hpp src/widgets/InstructionDetailsWidget.cpp
    src/widgets/ConsoleWidget.hpp src/widgets/ConsoleWidget.cpp

    src/console/Command.hpp
    src/console/CommandContext.hpp
    src/console/CommandDispatcher.hpp src/console/CommandDispatcher.cpp

    src/console/commands/GotoCommand.hpp src/console/commands/GotoCommand.cpp

)

target_link_libraries(zyfront
    PRIVATE
        Qt6::Widgets
        Zydis
)

target_include_directories(zyfront
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Nice warnings
if (MSVC)
    target_compile_options(zyfront PRIVATE /W4)
else()
    target_compile_options(zyfront PRIVATE -Wall -Wextra -Werror=overloaded-virtual)
endif()

