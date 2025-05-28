#ifndef ENTITY_H
#define ENTITY_H

#include "common.h"

// 实体基类，所有实体类的基类
class Entity {
protected:
    std::string id;
    std::string createTime;

public:
    Entity() {
        id = generateId();
        createTime = getCurrentTime();
    }

    Entity(const std::string& id, const std::string& createTime) 
        : id(id), createTime(createTime) {}

    virtual ~Entity() {}

    // Getters
    std::string getId() const { return id; }
    std::string getCreateTime() const { return createTime; }

    // 序列化和反序列化接口
    virtual void serialize(std::ofstream& out) const {
        out.write(id.c_str(), id.size() + 1);
        out.write(createTime.c_str(), createTime.size() + 1);
    }

    virtual void deserialize(std::ifstream& in) {
        char buffer[256];
        
        in.getline(buffer, 256, '\0');
        id = buffer;
        
        in.getline(buffer, 256, '\0');
        createTime = buffer;
    }
};

#endif // ENTITY_H
