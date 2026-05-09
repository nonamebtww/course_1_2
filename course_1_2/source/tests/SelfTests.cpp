#include "tests/SelfTests.h"

#include "services/FileDataStorage.h"
#include "services/ConsoleInput.h"
#include "services/TaxiService.h"

#include <cmath>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

namespace
{
void require(bool condition, const std::string& message)
{
    if (!condition)
    {
        throw std::runtime_error(message);
    }
}

void testOrderLifecycle()
{
    TaxiService service;

    auto& client = service.addClient("Иван Петров", "+79990000001");
    auto& car = service.addVehicle("Economy", "Lada", "Vesta", "A111AA");
    service.addDriver("Петр Сидоров", "+79990000002", car.getId(), 5);

    auto& order = service.createOrder(client.getId(), "Эконом", "ул. Ленина, 1", "пр. Мира, 10", 12.5);

    require(order.getClientId() == client.getId(), "Заказ должен ссылаться на выбранного клиента.");
    require(order.getDriverId() > 0, "Для заказа должен быть подобран водитель.");
    require(std::fabs(order.getPrice() - 350.0) < 0.01, "Стоимость эконом-заказа рассчитана неверно.");
    require(!service.findDriver(order.getDriverId())->isAvailable(), "Водитель должен быть занят после создания заказа.");
    require(!service.findVehicle(order.getVehicleId())->isAvailable(), "Автомобиль должен быть занят после создания заказа.");

    service.changeOrderStatus(order.getId(), OrderStatus::Completed);

    require(service.findDriver(order.getDriverId())->isAvailable(), "Водитель должен освободиться после завершения заказа.");
    require(service.findVehicle(order.getVehicleId())->isAvailable(), "Автомобиль должен освободиться после завершения заказа.");
}

void testNoFreeDriver()
{
    TaxiService service;

    service.addClient("Анна Смирнова", "+79990000003");

    bool failed = false;
    try
    {
        service.createOrder(1, "Комфорт", "Аэропорт", "Центр", 20.0);
    }
    catch (const std::runtime_error&)
    {
        failed = true;
    }

    require(failed, "Создание заказа без свободного водителя должно завершаться ошибкой.");
}

void testConsoleInputUtf16ToUtf8()
{
    const std::wstring input = L"\x0418\x0432\x0430\x043D \x041F\x0435\x0442\x0440\x043E\x0432";
    const std::string expected = "\xD0\x98\xD0\xB2\xD0\xB0\xD0\xBD \xD0\x9F\xD0\xB5\xD1\x82\xD1\x80\xD0\xBE\xD0\xB2";

    require(consoleInput::utf16ToUtf8(input) == expected, "Консольный ввод UTF-16 должен преобразовываться в UTF-8.");
}

void testFileStorageRoundTrip()
{
    const std::filesystem::path folder = "selftest_data";
    std::filesystem::remove_all(folder);

    TaxiService source;
    auto& client = source.addClient("Олег Иванов", "+79990000004");
    auto& car = source.addVehicle("Comfort", "Toyota", "Camry", "B222BB");
    source.addDriver("Мария Орлова", "+79990000005", car.getId(), 7);
    source.createOrder(client.getId(), "Комфорт", "Вокзал", "Офис", 8.0);

    FileDataStorage storage(folder.string());
    storage.save(source);

    TaxiService loaded;
    storage.load(loaded);

    require(loaded.getClients().size() == 1, "После загрузки должен быть один клиент.");
    require(loaded.getVehicles().size() == 1, "После загрузки должен быть один автомобиль.");
    require(loaded.getDrivers().size() == 1, "После загрузки должен быть один водитель.");
    require(loaded.getOrders().size() == 1, "После загрузки должен быть один заказ.");
    require(loaded.getClients().front().getName() == source.getClients().front().getName(), "Имя клиента должно сохраниться без потери русских символов.");
    require(loaded.getDrivers().front().getName() == source.getDrivers().front().getName(), "Имя водителя должно сохраниться без потери русских символов.");
    require(loaded.getOrders().front().getTariffName() == source.getOrders().front().getTariffName(), "Название тарифа должно сохраниться без потери русских символов.");
    require(loaded.getOrders().front().getStartAddress() == source.getOrders().front().getStartAddress(), "Адрес отправления должен сохраниться без потери русских символов.");
    require(loaded.getOrders().front().getDestinationAddress() == source.getOrders().front().getDestinationAddress(), "Адрес назначения должен сохраниться без потери русских символов.");

    std::filesystem::remove_all(folder);
}
}

int runSelfTests()
{
    try
    {
        testOrderLifecycle();
        testNoFreeDriver();
        testConsoleInputUtf16ToUtf8();
        testFileStorageRoundTrip();
        std::cout << "Самотесты успешно пройдены.\n";
        return 0;
    }
    catch (const std::exception& exception)
    {
        std::cerr << "Ошибка самотеста: " << exception.what() << '\n';
        return 1;
    }
}
