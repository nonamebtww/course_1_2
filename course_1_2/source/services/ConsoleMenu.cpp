#include "services/ConsoleMenu.h"

#include "services/ConsoleInput.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace
{
namespace ui
{
constexpr const char* Reset = "\033[0m";
constexpr const char* Title = "\033[1;38;2;255;201;92m";
constexpr const char* Section = "\033[1;38;2;111;183;255m";
constexpr const char* Muted = "\033[38;2;145;150;160m";
constexpr const char* Success = "\033[1;38;2;86;211;100m";
constexpr const char* Warning = "\033[1;38;2;255;177;66m";
constexpr const char* Error = "\033[1;38;2;255;94;94m";
constexpr const char* Prompt = "\033[1;38;2;255;255;255m";
constexpr const char* Command = "\033[1m";

void printRule()
{
    std::cout << Muted << "+------------------------------------------------------------+" << Reset << '\n';
}

void printSection(const std::string& title)
{
    std::cout << '\n' << Section << title << Reset << '\n'
              << Muted << "--------------------------------------------------------------" << Reset << '\n';
}

void printCommand(int number, const std::string& label)
{
    std::cout << "  " << Command;
    if (number < 10)
    {
        std::cout << ' ';
    }
    std::cout << number << Reset << "  " << label << '\n';
}

void printSuccess(const std::string& text)
{
    std::cout << Success << "[OK] " << Reset << text << '\n';
}

void printWarning(const std::string& text)
{
    std::cout << Warning << "[!] " << Reset << text << '\n';
}

void printError(const std::string& text)
{
    std::cout << Error << "[ERROR] " << Reset << text << '\n';
}

void printEmpty(const std::string& text)
{
    std::cout << Muted << "[empty] " << Reset << text << '\n';
}

std::string formatMoney(double value)
{
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << value;
    return stream.str();
}
}
}

ConsoleMenu::ConsoleMenu(TaxiService& service, IDataStorage& storage)
    : service(service), storage(storage), running(true)
{
}

void ConsoleMenu::run()
{
    while (running)
    {
        printMenu();
        const int choice = readInt("Команда: ");

        try
        {
            switch (choice)
            {
            case 1:
                addClient();
                break;
            case 2:
                listClients();
                break;
            case 3:
                editClient();
                break;
            case 4:
                deleteClient();
                break;
            case 5:
                addVehicle();
                break;
            case 6:
                listVehicles();
                break;
            case 7:
                editVehicle();
                break;
            case 8:
                deleteVehicle();
                break;
            case 9:
                addDriver();
                break;
            case 10:
                listDrivers();
                break;
            case 11:
                editDriver();
                break;
            case 12:
                deleteDriver();
                break;
            case 13:
                createOrder();
                break;
            case 14:
                listOrders();
                break;
            case 15:
                changeOrderStatus();
                break;
            case 16:
                saveData();
                break;
            case 0:
                exitMenu();
                break;
            default:
                ui::printWarning("Такого пункта меню нет.");
                break;
            }
        }
        catch (const std::exception& exception)
        {
            ui::printError(exception.what());
        }
    }
}

void ConsoleMenu::printMenu() const
{
    std::cout << '\n';
    ui::printRule();
    std::cout << ui::Title << "  СЛУЖБА ТАКСИ" << ui::Reset
              << ui::Muted << "  |  консоль диспетчера" << ui::Reset << '\n';
    std::cout << ui::Muted << "  Клиенты: " << service.getClients().size()
              << "  Авто: " << service.getVehicles().size()
              << "  Водители: " << service.getDrivers().size()
              << "  Заказы: " << service.getOrders().size() << ui::Reset << '\n';
    ui::printRule();

    ui::printSection("Клиенты");
    ui::printCommand(1, "Добавить клиента");
    ui::printCommand(2, "Показать клиентов");
    ui::printCommand(3, "Изменить клиента");
    ui::printCommand(4, "Удалить клиента");

    ui::printSection("Автопарк");
    ui::printCommand(5, "Добавить автомобиль");
    ui::printCommand(6, "Показать автомобили");
    ui::printCommand(7, "Изменить автомобиль");
    ui::printCommand(8, "Удалить автомобиль");

    ui::printSection("Водители");
    ui::printCommand(9, "Добавить водителя");
    ui::printCommand(10, "Показать водителей");
    ui::printCommand(11, "Изменить водителя");
    ui::printCommand(12, "Удалить водителя");

    ui::printSection("Заказы и данные");
    ui::printCommand(13, "Создать заказ");
    ui::printCommand(14, "Показать заказы");
    ui::printCommand(15, "Изменить статус заказа");
    ui::printCommand(16, "Сохранить данные");
    ui::printCommand(0, "Выход");
}

void ConsoleMenu::addClient()
{
    ui::printSection("Новый клиент");
    const std::string name = readLine("ФИО клиента: ");
    const std::string phone = readLine("Телефон клиента: ");
    const Client& client = service.addClient(name, phone);
    ui::printSuccess("Клиент добавлен с ID " + std::to_string(client.getId()) + ".");
}

void ConsoleMenu::listClients() const
{
    ui::printSection("Клиенты");
    if (service.getClients().empty())
    {
        ui::printEmpty("Клиенты не добавлены.");
        return;
    }

    for (const Client& client : service.getClients())
    {
        client.print(std::cout);
        std::cout << '\n';
    }
}

void ConsoleMenu::editClient()
{
    listClients();
    const int id = readInt("ID клиента для изменения: ");
    const std::string name = readLine("Новое ФИО: ");
    const std::string phone = readLine("Новый телефон: ");

    if (service.updateClient(id, name, phone))
    {
        ui::printSuccess("Клиент изменен.");
    }
    else
    {
        ui::printWarning("Клиент не найден.");
    }
}

void ConsoleMenu::deleteClient()
{
    listClients();
    const int id = readInt("ID клиента для удаления: ");
    if (service.removeClient(id))
    {
        ui::printSuccess("Клиент удален.");
    }
    else
    {
        ui::printWarning("Клиента нельзя удалить: он не найден или уже связан с заказами.");
    }
}

void ConsoleMenu::addVehicle()
{
    ui::printSection("Новый автомобиль");
    const std::string category = readVehicleCategory();
    const std::string brand = readLine("Марка автомобиля: ");
    const std::string model = readLine("Модель автомобиля: ");
    const std::string plateNumber = readLine("Госномер: ");
    const Vehicle& vehicle = service.addVehicle(category, brand, model, plateNumber);
    ui::printSuccess("Автомобиль добавлен с ID " + std::to_string(vehicle.getId()) + ".");
}

void ConsoleMenu::listVehicles() const
{
    ui::printSection("Автопарк");
    if (service.getVehicles().empty())
    {
        ui::printEmpty("Автомобили не добавлены.");
        return;
    }

    for (const auto& vehicle : service.getVehicles())
    {
        vehicle->print(std::cout);
        std::cout << '\n';
    }
}

void ConsoleMenu::editVehicle()
{
    listVehicles();
    const int id = readInt("ID автомобиля для изменения: ");
    const std::string brand = readLine("Новая марка: ");
    const std::string model = readLine("Новая модель: ");
    const std::string plateNumber = readLine("Новый госномер: ");

    if (service.updateVehicle(id, brand, model, plateNumber))
    {
        ui::printSuccess("Автомобиль изменен.");
    }
    else
    {
        ui::printWarning("Автомобиль не найден.");
    }
}

void ConsoleMenu::deleteVehicle()
{
    listVehicles();
    const int id = readInt("ID автомобиля для удаления: ");
    if (service.removeVehicle(id))
    {
        ui::printSuccess("Автомобиль удален.");
    }
    else
    {
        ui::printWarning("Автомобиль нельзя удалить: он не найден, закреплен за водителем или участвует в активном заказе.");
    }
}

void ConsoleMenu::addDriver()
{
    ui::printSection("Новый водитель");
    listVehicles();
    const std::string name = readLine("ФИО водителя: ");
    const std::string phone = readLine("Телефон водителя: ");
    const int vehicleId = readInt("ID закрепленного автомобиля: ");
    const int experienceYears = readInt("Стаж в годах: ");
    const Driver& driver = service.addDriver(name, phone, vehicleId, experienceYears);
    ui::printSuccess("Водитель добавлен с ID " + std::to_string(driver.getId()) + ".");
}

void ConsoleMenu::listDrivers() const
{
    ui::printSection("Водители");
    if (service.getDrivers().empty())
    {
        ui::printEmpty("Водители не добавлены.");
        return;
    }

    for (const Driver& driver : service.getDrivers())
    {
        driver.print(std::cout);
        std::cout << '\n';
    }
}

void ConsoleMenu::editDriver()
{
    listDrivers();
    listVehicles();
    const int id = readInt("ID водителя для изменения: ");
    const std::string name = readLine("Новое ФИО: ");
    const std::string phone = readLine("Новый телефон: ");
    const int vehicleId = readInt("Новый ID автомобиля: ");
    const int experienceYears = readInt("Новый стаж: ");

    if (service.updateDriver(id, name, phone, vehicleId, experienceYears))
    {
        ui::printSuccess("Водитель изменен.");
    }
    else
    {
        ui::printWarning("Водителя нельзя изменить: проверьте ID водителя, автомобиль и стаж.");
    }
}

void ConsoleMenu::deleteDriver()
{
    listDrivers();
    const int id = readInt("ID водителя для удаления: ");
    if (service.removeDriver(id))
    {
        ui::printSuccess("Водитель удален.");
    }
    else
    {
        ui::printWarning("Водителя нельзя удалить: он не найден или выполняет активный заказ.");
    }
}

void ConsoleMenu::createOrder()
{
    ui::printSection("Новый заказ");
    listClients();
    const int clientId = readInt("ID клиента: ");
    const std::string tariffName = readTariffName();
    const std::string startAddress = readLine("Адрес отправления: ");
    const std::string destinationAddress = readLine("Адрес назначения: ");
    const double distanceKm = readDouble("Расстояние, км: ");

    const Order& order = service.createOrder(clientId, tariffName, startAddress, destinationAddress, distanceKm);
    ui::printSuccess("Заказ создан с ID " + std::to_string(order.getId()) + ". Стоимость: " + ui::formatMoney(order.getPrice()) + ".");
}

void ConsoleMenu::listOrders() const
{
    ui::printSection("Заказы");
    if (service.getOrders().empty())
    {
        ui::printEmpty("Заказы не созданы.");
        return;
    }

    for (const Order& order : service.getOrders())
    {
        order.print(std::cout);
        std::cout << '\n';
    }
}

void ConsoleMenu::changeOrderStatus()
{
    listOrders();
    const int id = readInt("ID заказа: ");
    const OrderStatus status = readOrderStatus();
    if (service.changeOrderStatus(id, status))
    {
        ui::printSuccess("Статус заказа изменен.");
    }
    else
    {
        ui::printWarning("Статус заказа нельзя изменить.");
    }
}

void ConsoleMenu::saveData() const
{
    storage.save(service);
    ui::printSuccess("Данные сохранены.");
}

void ConsoleMenu::exitMenu()
{
    if (readYesNo("Сохранить данные перед выходом?"))
    {
        saveData();
    }
    running = false;
}

std::string ConsoleMenu::readLine(const std::string& prompt, bool allowEmpty) const
{
    while (true)
    {
        std::cout << ui::Prompt << "> " << ui::Reset << prompt;
        std::cout.flush();
        const std::string value = consoleInput::readUtf8Line(std::cin);

        if (allowEmpty || !value.empty())
        {
            return value;
        }
        ui::printWarning("Значение не должно быть пустым.");
    }
}

int ConsoleMenu::readInt(const std::string& prompt) const
{
    while (true)
    {
        const std::string value = readLine(prompt);
        std::stringstream stream(value);
        int result = 0;
        char extra = '\0';
        if (stream >> result && !(stream >> extra))
        {
            return result;
        }
        ui::printWarning("Введите целое число.");
    }
}

double ConsoleMenu::readDouble(const std::string& prompt) const
{
    while (true)
    {
        const std::string value = readLine(prompt);
        std::stringstream stream(value);
        double result = 0.0;
        char extra = '\0';
        if (stream >> result && !(stream >> extra))
        {
            return result;
        }
        ui::printWarning("Введите число.");
    }
}

bool ConsoleMenu::readYesNo(const std::string& prompt) const
{
    while (true)
    {
        const std::string value = readLine(prompt + " (y/n): ");
        if (value == "y" || value == "Y")
        {
            return true;
        }
        if (value == "n" || value == "N")
        {
            return false;
        }
        ui::printWarning("Введите y или n.");
    }
}

std::string ConsoleMenu::readVehicleCategory() const
{
    while (true)
    {
        ui::printSection("Класс автомобиля");
        ui::printCommand(1, "Economy");
        ui::printCommand(2, "Comfort");
        const int choice = readInt("Выберите класс: ");
        if (choice == 1)
        {
            return "Economy";
        }
        if (choice == 2)
        {
            return "Comfort";
        }
        ui::printWarning("Выберите 1 или 2.");
    }
}

std::string ConsoleMenu::readTariffName() const
{
    const auto names = service.getTariffNames();
    while (true)
    {
        ui::printSection("Тарифы");
        for (std::size_t i = 0; i < names.size(); ++i)
        {
            ui::printCommand(static_cast<int>(i + 1), names[i]);
        }
        const int choice = readInt("Выберите тариф: ");
        if (choice >= 1 && choice <= static_cast<int>(names.size()))
        {
            return names[choice - 1];
        }
        ui::printWarning("Такого тарифа нет.");
    }
}

OrderStatus ConsoleMenu::readOrderStatus() const
{
    while (true)
    {
        ui::printSection("Статус заказа");
        ui::printCommand(1, "Создан");
        ui::printCommand(2, "В пути");
        ui::printCommand(3, "Завершен");
        ui::printCommand(4, "Отменен");
        const int choice = readInt("Выберите статус: ");
        switch (choice)
        {
        case 1:
            return OrderStatus::Created;
        case 2:
            return OrderStatus::OnWay;
        case 3:
            return OrderStatus::Completed;
        case 4:
            return OrderStatus::Cancelled;
        default:
            ui::printWarning("Выберите пункт от 1 до 4.");
            break;
        }
    }
}
