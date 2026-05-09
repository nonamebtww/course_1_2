#include "services/ConsoleMenu.h"
#include "services/FileDataStorage.h"
#include "services/TaxiService.h"
#include "tests/SelfTests.h"

#include <clocale>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char* argv[])
{
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (output != INVALID_HANDLE_VALUE && GetConsoleMode(output, &mode))
    {
        SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
    std::setlocale(LC_ALL, ".UTF8");

    if (argc > 1 && std::string(argv[1]) == "--self-test")
    {
        return runSelfTests();
    }

    TaxiService service;
    FileDataStorage storage("data");

    try
    {
        storage.load(service);
    }
    catch (const std::exception& exception)
    {
        std::cerr << "Не удалось загрузить данные: " << exception.what() << '\n';
    }

    ConsoleMenu menu(service, storage);
    menu.run();
    return 0;
}
