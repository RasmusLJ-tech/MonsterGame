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
        case StatusType::CONFUSED:
            return "Confused";
        case StatusType::BLEEDING:
            return "Bleeding";
        case StatusType::CURSED:
            return "Cursed";
        case StatusType::STRENGTH_BUFF:
            return "Strength Buff";
        case StatusType::DEFENSE_BUFF:
            return "Defense Buff";
        case StatusType::SPEED_BUFF:
            return "Speed Buff";

        default:
            return "None";
    }
}

