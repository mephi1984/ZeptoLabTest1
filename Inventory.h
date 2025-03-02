#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <iostream>
#include "TextureManager.h"

namespace ZL
{
    struct InventoryItem
    {
        std::string name;
        std::shared_ptr<Texture> texture;
    };

    // Глобальное хранилище предметов
    extern std::unordered_map<std::string, InventoryItem> gInventory;

    // Добавить предмет в инвентарь
    void AddItemToInventory(const std::string& name, std::shared_ptr<Texture> tex);

    // Удалить предмет из инвентаря
    void RemoveItemFromInventory(const std::string& name);

    // Поиск предмета по имени (возвращает указатель или nullptr)
    InventoryItem* GetItemByName(const std::string& name);

    // Вывести весь инвентарь в консоль
    void PrintInventory();

    const std::unordered_map<std::string, InventoryItem>& ReturnInventory();
}
