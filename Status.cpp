#include "Status.h"

Status::Status(StatusType type, int duration) : type(type), duration(duration) {}

StatusType Status::getType() const {
    return type;
}

int Status::getDuration() const {
    return duration;
}

void Status::reduceDuration() {
    if (duration > 0) {
        duration--;
    }
}

std::string Status::getStatusName() const {
    switch (type) {
        case StatusType::POISON:
            return "Poison";
        case StatusType::FROZEN:
            return "Frozen";
        case StatusType::PARALYZED:
            return "Paralyzed";
        case StatusType::STUNNED:
            return "Stunned";
        case StatusType::CURSED:
            return "Cursed";
        default:
            return "None";
    }
}

