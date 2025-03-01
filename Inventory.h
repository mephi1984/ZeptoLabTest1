#pragma once

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include "TextureManager.h"

namespace ZL
{
    // Структура, описывающая элемент инвентаря
    struct InventoryItem
    {
        std::string name;
        std::shared_ptr<Texture> texture;
    };

    // Глобальный список инвентаря
    extern std::vector<InventoryItem> gInventory;

    // Добавить предмет в инвентарь
    void AddItemToInventory(const std::string& name, std::shared_ptr<Texture> tex);

    // Удалить предмет из инвентаря
    void RemoveItemFromInventory(const std::string& name);

    // Вывести все предметы в инвентаре
    void PrintInventory();

    // Получить список предметов инвентаря
    const std::vector<InventoryItem>& ReturnInventory();
}
