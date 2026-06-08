#ifndef Status_H
#define Status_H
#include <string>

enum class StatusType { POISON, FROZEN, CONFUSED, BLEEDING, CURSED, NONE, STRENGTH_BUFF, DEFENSE_BUFF, SPEED_BUFF };

class Status {
public:
    Status(StatusType type, int duration);
    
    StatusType getType() const;
    int getDuration() const;
    void reduceDuration(); 
    std::string getStatusName() const;

private:
    StatusType type;
    int duration;
};

#endif 