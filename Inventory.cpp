#include "Inventory.h"

namespace ZL
{
    // Определяем глобальную переменную
    std::unordered_map<std::string, InventoryItem> gInventoryMap;

    void AddItemToInventory(const std::string& name, std::shared_ptr<Texture> tex)
    {
        InventoryItem item;
        item.name = name;
        item.texture = tex;

        // Вставляем или перезаписываем (operator[] так сделает).
        gInventoryMap[name] = item;
    }

    void RemoveItemFromInventory(const std::string& name)
    {
        // erase вернёт количество удалённых элементов, если нужно проверить
        gInventoryMap.erase(name);
    }

    InventoryItem* GetItemByName(const std::string& name)
    {
        // Пытаемся найти элемент по ключу
        auto it = gInventoryMap.find(name);
        if (it != gInventoryMap.end())
        {
            // Возвращаем адрес найденного InventoryItem
            return &it->second;
        }
        // Если не нашли – nullptr
        return nullptr;
    }

    void PrintInventory()
    {
        std::cout << "Inventory contents:\n";
        for (auto& [itemName, item] : gInventoryMap)
        {
            std::cout << "  - " << itemName << "\n";
        }
    }

    const std::unordered_map<std::string, InventoryItem>& ReturnInventory()
    {
        return gInventoryMap;
    }

}
