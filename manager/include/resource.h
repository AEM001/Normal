#ifndef RESOURCE_H
#define RESOURCE_H

#include "entity.h"

// 资源基类
class Resource : public Entity {
protected:
    std::string name;
    std::string description;
    ResourceStatus status;
    ResourceType type;
    double pricePerHour;

public:
    Resource() : Entity(), status(IDLE), pricePerHour(0.0) {}
    
    Resource(const std::string& name, const std::string& description, 
             ResourceType type, double pricePerHour)
        : Entity(), name(name), description(description), 
          status(IDLE), type(type), pricePerHour(pricePerHour) {}
    
    Resource(const std::string& id, const std::string& createTime,
             const std::string& name, const std::string& description,
             ResourceStatus status, ResourceType type, double pricePerHour)
        : Entity(id, createTime), name(name), description(description),
          status(status), type(type), pricePerHour(pricePerHour) {}
    
    virtual ~Resource() {}
    
    // Getters
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    ResourceStatus getStatus() const { return status; }
    ResourceType getType() const { return type; }
    double getPricePerHour() const { return pricePerHour; }
    
    // Setters
    void setName(const std::string& name) { this->name = name; }
    void setDescription(const std::string& description) { this->description = description; }
    void setStatus(ResourceStatus status) { this->status = status; }
    void setPricePerHour(double price) { this->pricePerHour = price; }
    
    // 业务方法
    bool isIdle() const { return status == IDLE; }
    
    // 序列化和反序列化
    virtual void serialize(std::ofstream& out) const override {
        Entity::serialize(out);
        out.write(name.c_str(), name.size() + 1);
        out.write(description.c_str(), description.size() + 1);
        out.write(reinterpret_cast<const char*>(&status), sizeof(status));
        out.write(reinterpret_cast<const char*>(&type), sizeof(type));
        out.write(reinterpret_cast<const char*>(&pricePerHour), sizeof(pricePerHour));
    }
    
    virtual void deserialize(std::ifstream& in) override {
        Entity::deserialize(in);
        char buffer[256];
        
        in.getline(buffer, 256, '\0');
        name = buffer;
        
        in.getline(buffer, 256, '\0');
        description = buffer;
        
        in.read(reinterpret_cast<char*>(&status), sizeof(status));
        in.read(reinterpret_cast<char*>(&type), sizeof(type));
        in.read(reinterpret_cast<char*>(&pricePerHour), sizeof(pricePerHour));
    }
    
    // 显示资源信息
    virtual void displayInfo() const {
        std::cout << "ID: " << id << std::endl;
        std::cout << "名称: " << name << std::endl;
        std::cout << "描述: " << description << std::endl;
        std::cout << "状态: " << (status == IDLE ? "空闲" : "使用中") << std::endl;
        std::cout << "类型: ";
        switch (type) {
            case CPU: std::cout << "CPU"; break;
            case GPU: std::cout << "GPU"; break;
            case STORAGE: std::cout << "存储"; break;
        }
        std::cout << std::endl;
        std::cout << "每小时价格: " << std::fixed << std::setprecision(2) << pricePerHour << std::endl;
        std::cout << "创建时间: " << createTime << std::endl;
    }
};

// CPU资源类
class CPUResource : public Resource {
private:
    int cores;
    double frequency;
    
public:
    CPUResource() : Resource() { type = CPU; }
    
    CPUResource(const std::string& name, const std::string& description,
                int cores, double frequency, double pricePerHour)
        : Resource(name, description, CPU, pricePerHour), cores(cores), frequency(frequency) {}
    
    CPUResource(const std::string& id, const std::string& createTime,
                const std::string& name, const std::string& description,
                ResourceStatus status, double pricePerHour,
                int cores, double frequency)
        : Resource(id, createTime, name, description, status, CPU, pricePerHour),
          cores(cores), frequency(frequency) {}
    
    // Getters
    int getCores() const { return cores; }
    double getFrequency() const { return frequency; }
    
    // Setters
    void setCores(int cores) { this->cores = cores; }
    void setFrequency(double frequency) { this->frequency = frequency; }
    
    // 序列化和反序列化
    void serialize(std::ofstream& out) const override {
        Resource::serialize(out);
        out.write(reinterpret_cast<const char*>(&cores), sizeof(cores));
        out.write(reinterpret_cast<const char*>(&frequency), sizeof(frequency));
    }
    
    void deserialize(std::ifstream& in) override {
        Resource::deserialize(in);
        in.read(reinterpret_cast<char*>(&cores), sizeof(cores));
        in.read(reinterpret_cast<char*>(&frequency), sizeof(frequency));
    }
    
    // 显示资源信息
    void displayInfo() const override {
        Resource::displayInfo();
        std::cout << "核心数: " << cores << std::endl;
        std::cout << "频率: " << frequency << " GHz" << std::endl;
    }
};

// GPU资源类
class GPUResource : public Resource {
private:
    std::string model;
    int memory;
    double computePower;
    
public:
    GPUResource() : Resource() { type = GPU; }
    
    GPUResource(const std::string& name, const std::string& description,
                const std::string& model, int memory, double computePower, double pricePerHour)
        : Resource(name, description, GPU, pricePerHour), model(model), 
          memory(memory), computePower(computePower) {}
    
    GPUResource(const std::string& id, const std::string& createTime,
                const std::string& name, const std::string& description,
                ResourceStatus status, double pricePerHour,
                const std::string& model, int memory, double computePower)
        : Resource(id, createTime, name, description, status, GPU, pricePerHour),
          model(model), memory(memory), computePower(computePower) {}
    
    // Getters
    std::string getModel() const { return model; }
    int getMemory() const { return memory; }
    double getComputePower() const { return computePower; }
    
    // Setters
    void setModel(const std::string& model) { this->model = model; }
    void setMemory(int memory) { this->memory = memory; }
    void setComputePower(double computePower) { this->computePower = computePower; }
    
    // 序列化和反序列化
    void serialize(std::ofstream& out) const override {
        Resource::serialize(out);
        out.write(model.c_str(), model.size() + 1);
        out.write(reinterpret_cast<const char*>(&memory), sizeof(memory));
        out.write(reinterpret_cast<const char*>(&computePower), sizeof(computePower));
    }
    
    void deserialize(std::ifstream& in) override {
        Resource::deserialize(in);
        char buffer[256];
        
        in.getline(buffer, 256, '\0');
        model = buffer;
        
        in.read(reinterpret_cast<char*>(&memory), sizeof(memory));
        in.read(reinterpret_cast<char*>(&computePower), sizeof(computePower));
    }
    
    // 显示资源信息
    void displayInfo() const override {
        Resource::displayInfo();
        std::cout << "型号: " << model << std::endl;
        std::cout << "显存: " << memory << " GB" << std::endl;
        std::cout << "计算能力: " << computePower << " TFLOPS" << std::endl;
    }
};

// 存储资源类
class StorageResource : public Resource {
private:
    int capacity;
    double readSpeed;
    double writeSpeed;
    std::string storageType; // SSD或HDD
    
public:
    StorageResource() : Resource() { type = STORAGE; }
    
    StorageResource(const std::string& name, const std::string& description,
                    int capacity, double readSpeed, double writeSpeed, 
                    const std::string& storageType, double pricePerHour)
        : Resource(name, description, STORAGE, pricePerHour), capacity(capacity),
          readSpeed(readSpeed), writeSpeed(writeSpeed), storageType(storageType) {}
    
    StorageResource(const std::string& id, const std::string& createTime,
                    const std::string& name, const std::string& description,
                    ResourceStatus status, double pricePerHour,
                    int capacity, double readSpeed, double writeSpeed, 
                    const std::string& storageType)
        : Resource(id, createTime, name, description, status, STORAGE, pricePerHour),
          capacity(capacity), readSpeed(readSpeed), writeSpeed(writeSpeed), storageType(storageType) {}
    
    // Getters
    int getCapacity() const { return capacity; }
    double getReadSpeed() const { return readSpeed; }
    double getWriteSpeed() const { return writeSpeed; }
    std::string getStorageType() const { return storageType; }
    
    // Setters
    void setCapacity(int capacity) { this->capacity = capacity; }
    void setReadSpeed(double readSpeed) { this->readSpeed = readSpeed; }
    void setWriteSpeed(double writeSpeed) { this->writeSpeed = writeSpeed; }
    void setStorageType(const std::string& storageType) { this->storageType = storageType; }
    
    // 序列化和反序列化
    void serialize(std::ofstream& out) const override {
        Resource::serialize(out);
        out.write(reinterpret_cast<const char*>(&capacity), sizeof(capacity));
        out.write(reinterpret_cast<const char*>(&readSpeed), sizeof(readSpeed));
        out.write(reinterpret_cast<const char*>(&writeSpeed), sizeof(writeSpeed));
        out.write(storageType.c_str(), storageType.size() + 1);
    }
    
    void deserialize(std::ifstream& in) override {
        Resource::deserialize(in);
        in.read(reinterpret_cast<char*>(&capacity), sizeof(capacity));
        in.read(reinterpret_cast<char*>(&readSpeed), sizeof(readSpeed));
        in.read(reinterpret_cast<char*>(&writeSpeed), sizeof(writeSpeed));
        
        char buffer[256];
        in.getline(buffer, 256, '\0');
        storageType = buffer;
    }
    
    // 显示资源信息
    void displayInfo() const override {
        Resource::displayInfo();
        std::cout << "容量: " << capacity << " GB" << std::endl;
        std::cout << "读取速度: " << readSpeed << " MB/s" << std::endl;
        std::cout << "写入速度: " << writeSpeed << " MB/s" << std::endl;
        std::cout << "存储类型: " << storageType << std::endl;
    }
};

#endif // RESOURCE_H
