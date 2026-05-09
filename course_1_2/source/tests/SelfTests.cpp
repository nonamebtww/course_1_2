#include "tests/SelfTests.h"

#include "services/FileDataStorage.h"
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

    std::filesystem::remove_all(folder);
}
}

int runSelfTests()
{
    try
    {
        testOrderLifecycle();
        testNoFreeDriver();
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
