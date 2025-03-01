#include "Inventory.h"
#include <algorithm> // Для std::remove_if

namespace ZL
{
    // Определяем глобальный инвентарь
    std::vector<InventoryItem> gInventory;

    void AddItemToInventory(const std::string& name, std::shared_ptr<Texture> tex)
    {
        gInventory.push_back({ name, tex });
    }

    void RemoveItemFromInventory(const std::string name)
    {
        gInventory.erase(
            std::remove_if(gInventory.begin(), gInventory.end(),
                           [&name](const InventoryItem& item) {
                               return item.name == name;
                           }),
            gInventory.end());
    }

    void PrintInventory()
    {
        for (const auto& item : gInventory)
        {
            std::cout << "Item: " << item.name
                      << ", texture ID = "
                      << (item.texture ? item.texture->getTexID() : 0)
                      << std::endl;
        }
    }

    bool HasObject(const std::string& name){
            for (const auto& item : gInventory) {
                if (item.name == name) {
                    return true;
                }
            }
            return false;
    }

    const std::vector<InventoryItem>& ReturnInventory()
    {
        return gInventory;
    }
}
