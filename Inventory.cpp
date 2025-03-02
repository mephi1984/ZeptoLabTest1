#include "Inventory.h"

namespace ZL
{
    std::unordered_map<int, InventoryItem> gInventoryMap;

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

        gInventoryMap[slot_index] = item;
        std::cout << "Added item to slot " << slot_index << std::endl;
    }

    void RemoveItemFromInventory(int slot_index)
    {
        gInventoryMap.erase(slot_index);
    }

    InventoryItem* GetItemByIndex(int slot_index)
    {
        auto it = gInventoryMap.find(slot_index);
        if (it != gInventoryMap.end()) {
            return &it->second;
        }
        return nullptr;
    }

    InventoryItem* GetItemByHotkey(int hotkey)
    {
        return GetItemByIndex(hotkey); // Now we can just use the index directly
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

    const std::unordered_map<int, InventoryItem>& ReturnInventory()
    {
        return gInventoryMap;
    }

}
