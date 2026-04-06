#ifndef Status_H
#define Status_H
#include <string>

enum class StatusType { POISON, FROZEN, PARALYZED, STUNNED, CURSED, NONE };

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