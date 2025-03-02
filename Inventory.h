#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <iostream>
#include "TextureManager.h"

namespace ZL
{
    static const int MAX_INVENTORY_SLOTS = 9;
    static const float SELECTED_ITEM_SCALE = 1.2f; // Scale factor for selected items

    struct InventoryItem
    {
        std::string name;
        std::shared_ptr<Texture> texture;
        bool isSelected = false;
        int hot_key;
        float scale = 1.0f; // Add scale property
    };

    // Глобальное хранилище предметов
    extern std::unordered_map<std::string, InventoryItem> gInventoryMap;  // Changed key type from string to int

    // Добавить предмет в инвентарь
    void AddItemToInventory(const std::string& name, std::shared_ptr<Texture> tex, int slot_index);

    // Удалить предмет из инвентаря
    void RemoveItemFromInventory(int slot_index);

    // Поиск предмета по индексу (возвращает указатель или nullptr)
    InventoryItem* GetItemByHotkey(int hot_key);
    InventoryItem* GetItemSelected(bool isSelected);
    InventoryItem* GetItemByName(std::string name);

    // Вывести весь инвентарь в консоль
    void PrintInventory();

    const std::unordered_map<std::string, InventoryItem>& ReturnInventory();

    // Add these new functions
    void UnselectAllItems();
}
