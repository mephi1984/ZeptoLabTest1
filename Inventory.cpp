#include "Inventory.h"

namespace ZL
{
    std::unordered_map<std::string, InventoryItem> gInventoryMap;

    void AddItemToInventory(const std::string& name, std::shared_ptr<Texture> tex, int slot_index)
    {
        if (slot_index > MAX_INVENTORY_SLOTS || slot_index < 1) {
            return;
        }

        InventoryItem item;
        item.name = name;
        item.texture = tex;
        item.hot_key = slot_index;
        item.scale = 1.0f;

        gInventoryMap[name] = item;
        std::cout << "Added item to slot " << slot_index << std::endl;
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

    InventoryItem* GetItemByHotkey(int hotkey)
    {
        for (auto& [_, item] : gInventoryMap) {
            if (item.hot_key == hotkey) {
                return &item;
            }
        }
        return nullptr;
    }

    InventoryItem* GetItemSelected(bool selected)
    {
        for (auto& [_, item] : gInventoryMap) {
            if (item.isSelected == selected) {
                return &item;
            }
        }
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

    void UnselectAllItems()
    {
        for (auto& [_, item] : gInventoryMap) {
            item.isSelected = false;
            item.scale = 1.0f; // Reset scale when unselected
        }
    }

    const std::unordered_map<std::string, InventoryItem>& ReturnInventory()
    {
        return gInventoryMap;
    }

}
