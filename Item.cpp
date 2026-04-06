#include "Item.h"

Item::Item(std::string name, int damage, StatusType sType, int sDuration, int chance)
    : name(name), damage(damage), statusEffect(sType), statusDuration(sDuration), applyChance(chance) {}

std::string Item::getName() const {
    return name;
}
int Item::getDamage() const {
    return damage;
}
StatusType Item::getStatusEffect() const {
    return statusEffect;
}
int Item::getStatusDuration() const {
    return statusDuration;
}
int Item::getChance() const {
    return applyChance;
}

