#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <memory>
#include <fstream>
#include <stdexcept>

// 资源类型枚举
enum class ResourceType {
    CPU,
    GPU
};

// 资源类型枚举转字符串
inline std::string ResourceTypeToString(ResourceType type) {
    switch (type) {
        case ResourceType::CPU:
            return "CPU";
        case ResourceType::GPU:
            return "GPU";
        default:
            return "Unknown";
    }
}

// 资源状态枚举
enum class ResourceStatus {
    IDLE,
    IN_USE
};

// 资源状态枚举转字符串
inline std::string ResourceStatusToString(ResourceStatus status) {
    switch (status) {
        case ResourceStatus::IDLE:
            return "空闲";
        case ResourceStatus::IN_USE:
            return "使用中";
        default:
            return "未知状态";
    }
}

/**
 * @class Resource
 * @brief 系统中所有计算资源的基类。
 *
 * 存储通用资源信息，如ID、类型、名称、状态和具体参数。
 * 提供检查可用性和管理状态的功能。
 */
class Resource {
protected:
    std::string resourceId;
    std::string resourceName;
    ResourceType type;
    ResourceStatus status;
    double storage;
    double hourlyRate;
    
public:
    // 主构造函数
    Resource(std::string id, std::string name, ResourceType t, double rate, 
             ResourceStatus stat = ResourceStatus::IDLE, double stor = 50)
        : resourceId(std::move(id)),
          resourceName(std::move(name)),
          type(t),
          hourlyRate(rate),
          status(stat),
          storage(stor) {}
    
    // 为反序列化添加默认构造函数
    Resource() : resourceId(""), resourceName(""), type(ResourceType::CPU), 
                 status(ResourceStatus::IDLE), storage(0), hourlyRate(0.0) {}
    
    virtual ~Resource() = default;
    
    // 获取器
    std::string getResourceId() const { return resourceId; }
    std::string getResourceName() const { return resourceName; }
    ResourceType getResourceType() const { return type; }
    ResourceStatus getStatus() const { return status; }
    double getStorage() const { return storage; }
    double getHourlyRate() const { return hourlyRate; }

    // 设置器
    void setResourceName(const std::string& newName) { resourceName = newName; }
    void setStatus(ResourceStatus newStatus) { status = newStatus; }
    void setStorage(double newStorage) { storage = newStorage; }
    void setHourlyRate(double newRate) { hourlyRate = newRate; }

    // 显示资源特定详情的虚函数
    virtual void displayDetails() const = 0;

    // 检查资源是否可供租用
    bool isAvailable() const { return status == ResourceStatus::IDLE; }

    // 序列化/反序列化方法（用于数据持久化）
    virtual void serialize(std::ostream& os) const {
        // 写入基本属性
        os.write(resourceId.c_str(), resourceId.size() + 1);
        os.write(resourceName.c_str(), resourceName.size() + 1);
        os.write(reinterpret_cast<const char*>(&type), sizeof(ResourceType));
        os.write(reinterpret_cast<const char*>(&status), sizeof(ResourceStatus));
        os.write(reinterpret_cast<const char*>(&storage), sizeof(double));
        os.write(reinterpret_cast<const char*>(&hourlyRate), sizeof(double));
    }
    
    virtual void deserialize(std::istream& is) {
        // 读取基本属性
        char buffer[256];
        is.getline(buffer, 256, '\0');
        resourceId = buffer;
        
        is.getline(buffer, 256, '\0');
        resourceName = buffer;
        
        is.read(reinterpret_cast<char*>(&type), sizeof(ResourceType));
        is.read(reinterpret_cast<char*>(&status), sizeof(ResourceStatus));
        is.read(reinterpret_cast<char*>(&storage), sizeof(double));
        is.read(reinterpret_cast<char*>(&hourlyRate), sizeof(double));
    }
};

/**
 * @class CPUResource
 * @brief 表示CPU资源。
 *
 * 继承自Resource。具有特定参数，如核心数量、频率。
 */
class CPUResource : public Resource {
private:
    int coreCount;
    double frequency; // 单位：GHz
    
public:
    CPUResource(std::string id, std::string name, double rate, int cores, double freq, 
                ResourceStatus stat = ResourceStatus::IDLE, double stor = 50)
        : Resource(id, name, ResourceType::CPU, rate, stat, stor),
          coreCount(cores), frequency(freq) {}

    // 为反序列化添加默认构造函数
    CPUResource() : Resource("", "", ResourceType::CPU, 0.0, ResourceStatus::IDLE, 0.0), 
                    coreCount(0), frequency(0.0) {}

    void displayDetails() const override {
        std::cout << "===== CPU资源详情 =====\n";
        std::cout << "ID: " << resourceId << "\n";
        std::cout << "名称: " << resourceName << "\n";
        std::cout << "状态: " << ResourceStatusToString(status) << "\n";
        std::cout << "核心数: " << coreCount << "\n";
        std::cout << "频率: " << frequency << " GHz\n";
        std::cout << "存储容量: " << storage << " GB\n";
        std::cout << "小时费率: " << hourlyRate << " 元/小时\n";
    }
    
    // CPU特定方法或参数访问器
    int getCoreCount() const { return coreCount; }
    double getFrequency() const { return frequency; }
    
    void serialize(std::ostream& os) const override {
        // 先序列化基类部分
        Resource::serialize(os);
        // 再序列化子类特有属性
        os.write(reinterpret_cast<const char*>(&coreCount), sizeof(int));
        os.write(reinterpret_cast<const char*>(&frequency), sizeof(double));
    }
    
    void deserialize(std::istream& is) override {
        // 先反序列化基类部分
        Resource::deserialize(is);
        // 再反序列化子类特有属性
        is.read(reinterpret_cast<char*>(&coreCount), sizeof(int));
        is.read(reinterpret_cast<char*>(&frequency), sizeof(double));
    }
};

/**
 * @class GPUResource
 * @brief 表示GPU资源。
 *
 * 继承自Resource。可能具有特定参数，如显存、CUDA核心数。
 */
class GPUResource : public Resource {
private:
    int cudaCores;
    int vramGB; // 显存容量，单位：GB
    
public:
    GPUResource(std::string id, std::string name, double rate, int cores, int vram,
                ResourceStatus stat = ResourceStatus::IDLE, double stor = 50)
        : Resource(id, name, ResourceType::GPU, rate, stat, stor),
          cudaCores(cores), vramGB(vram) {}

    // 为反序列化添加默认构造函数
    GPUResource() : Resource("", "", ResourceType::GPU, 0.0, ResourceStatus::IDLE, 0.0), 
                    cudaCores(0), vramGB(0) {}
    
    void displayDetails() const override {
        std::cout << "===== GPU资源详情 =====\n";
        std::cout << "ID: " << resourceId << "\n";
        std::cout << "名称: " << resourceName << "\n";
        std::cout << "状态: " << ResourceStatusToString(status) << "\n";
        std::cout << "CUDA核心数: " << cudaCores << "\n";
        std::cout << "显存: " << vramGB << " GB\n";
        std::cout << "存储容量: " << storage << " GB\n";
        std::cout << "小时费率: " << hourlyRate << " 元/小时\n";
    }
    
    // GPU特定方法或参数访问器
    int getCudaCores() const { return cudaCores; }
    int getVRAM() const { return vramGB; }
    
    void serialize(std::ostream& os) const override {
        // 先序列化基类部分
        Resource::serialize(os);
        // 再序列化子类特有属性
        os.write(reinterpret_cast<const char*>(&cudaCores), sizeof(int));
        os.write(reinterpret_cast<const char*>(&vramGB), sizeof(int));
    }
    
    void deserialize(std::istream& is) override {
        // 先反序列化基类部分
        Resource::deserialize(is);
        // 再反序列化子类特有属性
        is.read(reinterpret_cast<char*>(&cudaCores), sizeof(int));
        is.read(reinterpret_cast<char*>(&vramGB), sizeof(int));
    }
};

/**
 * @class ResourceCollection
 * @brief 管理系统中所有计算资源的集合。
 *
 * 提供添加、查找、列出资源的功能。
 * 可以按类型筛选资源。
 */
class ResourceCollection {
private:
    std::vector<std::shared_ptr<Resource>> resources;

public:
    // 添加资源到集合
    void addResource(std::shared_ptr<Resource> resource) {
        resources.push_back(resource);
    }

    // 根据ID查找资源
    std::shared_ptr<Resource> findResourceById(const std::string& id) const {
        for (const auto& resource : resources) {
            if (resource->getResourceId() == id) {
                return resource;
            }
        }
        return nullptr; // 未找到资源
    }

    // 获取所有资源
    const std::vector<std::shared_ptr<Resource>>& getAllResources() const {
        return resources;
    }

    // 获取特定类型的资源
    std::vector<std::shared_ptr<Resource>> getResourcesByType(ResourceType type) const {
        std::vector<std::shared_ptr<Resource>> result;
        for (const auto& resource : resources) {
            if (resource->getResourceType() == type) {
                result.push_back(resource);
            }
        }
        return result;
    }

    // 获取所有可用资源
    std::vector<std::shared_ptr<Resource>> getAvailableResources() const {
        std::vector<std::shared_ptr<Resource>> result;
        for (const auto& resource : resources) {
            if (resource->isAvailable()) {
                result.push_back(resource);
            }
        }
        return result;
    }

    // 获取特定类型的可用资源
    std::vector<std::shared_ptr<Resource>> getAvailableResourcesByType(ResourceType type) const {
        std::vector<std::shared_ptr<Resource>> result;
        for (const auto& resource : resources) {
            if (resource->getResourceType() == type && resource->isAvailable()) {
                result.push_back(resource);
            }
        }
        return result;
    }

    // 显示所有资源
    void displayAllResources() const {
        std::cout << "===== 所有资源列表 =====\n";
        if (resources.empty()) {
            std::cout << "当前没有资源\n";
            return;
        }
        
        for (const auto& resource : resources) {
            resource->displayDetails();
            std::cout << "------------------------\n";
        }
    }

    // 显示特定类型的资源
    void displayResourcesByType(ResourceType type) const {
        std::cout << "===== " << ResourceTypeToString(type) << " 资源列表 =====\n";
        bool found = false;
        
        for (const auto& resource : resources) {
            if (resource->getResourceType() == type) {
                resource->displayDetails();
                std::cout << "------------------------\n";
                found = true;
            }
        }
        
        if (!found) {
            std::cout << "当前没有 " << ResourceTypeToString(type) << " 类型的资源\n";
        }
    }
    
    // 返回可用CPU资源型号列表
    std::vector<std::string> getAvailableCPUModels() const {
        std::vector<std::string> models;
        for (const auto& resource : resources) {
            if (resource->getResourceType() == ResourceType::CPU && resource->isAvailable()) {
                models.push_back(resource->getResourceName());
            }
        }
        return models;
    }
    
    // 返回可用GPU资源型号列表
    std::vector<std::string> getAvailableGPUModels() const {
        std::vector<std::string> models;
        for (const auto& resource : resources) {
            if (resource->getResourceType() == ResourceType::GPU && resource->isAvailable()) {
                models.push_back(resource->getResourceName());
            }
        }
        return models;
    }
    
    // 打印可用CPU资源型号
    void displayAvailableCPUModels() const {
        std::cout << "===== 可用CPU型号列表 =====\n";
        auto models = getAvailableCPUModels();
        if (models.empty()) {
            std::cout << "当前没有可用的CPU资源\n";
        } else {
            for (const auto& model : models) {
                std::cout << "- " << model << "\n";
            }
        }
    }
    
    // 打印可用GPU资源型号
    void displayAvailableGPUModels() const {
        std::cout << "===== 可用GPU型号列表 =====\n";
        auto models = getAvailableGPUModels();
        if (models.empty()) {
            std::cout << "当前没有可用的GPU资源\n";
        } else {
            for (const auto& model : models) {
                std::cout << "- " << model << "\n";
            }
        }
    }

    // 持久化方法
    void saveToFile(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("无法打开文件进行写入: " + filename);
        }
        
        // 写入资源数量
        size_t count = resources.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));
        
        // 逐个写入资源
        for (const auto& resource : resources) {
            // 写入资源类型标识
            ResourceType type = resource->getResourceType();
            file.write(reinterpret_cast<const char*>(&type), sizeof(ResourceType));
            
            // 根据类型序列化资源
            resource->serialize(file);
        }
        
        file.close();
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("无法打开文件进行读取: " + filename);
        }
        
        // 清空当前资源
        resources.clear();
        
        // 读取资源数量
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(size_t));
        
        // 逐个读取资源
        for (size_t i = 0; i < count; ++i) {
            // 读取资源类型标识
            ResourceType type;
            file.read(reinterpret_cast<char*>(&type), sizeof(ResourceType));
            
            // 根据类型创建相应的资源对象
            std::shared_ptr<Resource> resource;
            if (type == ResourceType::CPU) {
                resource = std::make_shared<CPUResource>();
            } else if (type == ResourceType::GPU) {
                resource = std::make_shared<GPUResource>();
            } else {
                throw std::runtime_error("未知的资源类型");
            }
            
            // 反序列化资源
            resource->deserialize(file);
            
            // 添加到集合
            resources.push_back(resource);
        }
        
        file.close();
    }
};

// 创建预设资源集合的辅助函数
inline ResourceCollection createDefaultResourceCollection() {
    ResourceCollection collection;
    
    // 添加CPU资源 - 高性能服务器计算型号
    collection.addResource(std::make_shared<CPUResource>("CPU001", "Intel Xeon Platinum 8380", 4.0, 40, 2.3, ResourceStatus::IDLE, 512));
    collection.addResource(std::make_shared<CPUResource>("CPU002", "Intel Xeon Platinum 8380", 4.0, 40, 2.3, ResourceStatus::IDLE, 512));
    collection.addResource(std::make_shared<CPUResource>("CPU003", "Intel Xeon Gold 6348", 3.5, 28, 2.6, ResourceStatus::IDLE, 256));
    collection.addResource(std::make_shared<CPUResource>("CPU004", "Intel Xeon Gold 6348", 3.5, 28, 2.6, ResourceStatus::IDLE, 256));
    collection.addResource(std::make_shared<CPUResource>("CPU005", "AMD EPYC 7763", 4.2, 64, 2.45, ResourceStatus::IDLE, 512));
    collection.addResource(std::make_shared<CPUResource>("CPU006", "AMD EPYC 7763", 4.2, 64, 2.45, ResourceStatus::IDLE, 512));
    collection.addResource(std::make_shared<CPUResource>("CPU007", "Intel Xeon Platinum 8480+", 4.5, 56, 2.0, ResourceStatus::IDLE, 1024));
    collection.addResource(std::make_shared<CPUResource>("CPU008", "Intel Xeon Platinum 8480+", 4.5, 56, 2.0, ResourceStatus::IDLE, 1024));
    collection.addResource(std::make_shared<CPUResource>("CPU009", "AMD EPYC 7713", 3.8, 64, 2.0, ResourceStatus::IDLE, 256));
    collection.addResource(std::make_shared<CPUResource>("CPU010", "AMD EPYC 7713", 3.8, 64, 2.0, ResourceStatus::IDLE, 256));
    
    // 添加GPU资源
    collection.addResource(std::make_shared<GPUResource>("GPU001", "NVIDIA H100 80GB", 10.0, 16896, 80, ResourceStatus::IDLE, 4096));
    collection.addResource(std::make_shared<GPUResource>("GPU002", "NVIDIA H100 80GB", 10.0, 16896, 80, ResourceStatus::IDLE, 4096));
    collection.addResource(std::make_shared<GPUResource>("GPU003", "NVIDIA A100 80GB", 8.0, 6912, 80, ResourceStatus::IDLE, 2048));
    collection.addResource(std::make_shared<GPUResource>("GPU004", "NVIDIA A100 80GB", 8.0, 6912, 80, ResourceStatus::IDLE, 2048));
    collection.addResource(std::make_shared<GPUResource>("GPU005", "NVIDIA A100 40GB", 7.0, 6912, 40, ResourceStatus::IDLE, 1024));
    collection.addResource(std::make_shared<GPUResource>("GPU006", "NVIDIA A100 40GB", 7.0, 6912, 40, ResourceStatus::IDLE, 1024));
    collection.addResource(std::make_shared<GPUResource>("GPU007", "NVIDIA Tesla V100", 7.0, 5120, 32, ResourceStatus::IDLE, 512));
    collection.addResource(std::make_shared<GPUResource>("GPU008", "NVIDIA Tesla V100", 7.0, 5120, 32, ResourceStatus::IDLE, 512));
    collection.addResource(std::make_shared<GPUResource>("GPU009", "NVIDIA RTX 4090", 6.5, 16384, 24, ResourceStatus::IDLE, 512));
    collection.addResource(std::make_shared<GPUResource>("GPU010", "NVIDIA RTX 4090", 6.5, 16384, 24, ResourceStatus::IDLE, 512));
    
    return collection;
}

/**
 * @class ResourceManager
 * @brief 管理系统中的所有资源。
 *
 * 负责资源的添加、修改、删除、查找和数据持久化。
 */
class ResourceManager {
private:
    std::vector<std::unique_ptr<Resource>> resources;
    std::string dataFilePath; // 资源数据文件路径
    int nextResourceId;

public:
    ResourceManager(const std::string& filePath) : dataFilePath(filePath), nextResourceId(1) {}
    ~ResourceManager() { saveResources(); } // 析构时保存数据

    // 加载资源数据
    void loadResources() {
        std::ifstream file(dataFilePath, std::ios::binary);
        if (!file) {
            std::cerr << "警告: 无法打开资源数据文件，将创建新文件。\n";
            return;
        }

        // 清空当前资源
        resources.clear();

        // 读取下一个可用的资源ID
        file.read(reinterpret_cast<char*>(&nextResourceId), sizeof(int));

        // 读取资源数量
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(size_t));

        // 逐个读取资源
        for (size_t i = 0; i < count; ++i) {
            // 读取资源类型标识
            ResourceType type;
            file.read(reinterpret_cast<char*>(&type), sizeof(ResourceType));

            // 根据类型创建相应的资源对象
            std::unique_ptr<Resource> resource;
            if (type == ResourceType::CPU) {
                resource = std::make_unique<CPUResource>();
            } else if (type == ResourceType::GPU) {
                resource = std::make_unique<GPUResource>();
            } else {
                throw std::runtime_error("未知的资源类型");
            }

            // 反序列化资源
            resource->deserialize(file);

            // 添加到集合
            resources.push_back(std::move(resource));
        }

        file.close();
    }

    // 保存资源数据
    void saveResources() const {
        std::ofstream file(dataFilePath, std::ios::binary);
        if (!file) {
            throw std::runtime_error("无法打开文件进行写入: " + dataFilePath);
        }

        // 写入下一个可用的资源ID
        file.write(reinterpret_cast<const char*>(&nextResourceId), sizeof(int));

        // 写入资源数量
        size_t count = resources.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));

        // 逐个写入资源
        for (const auto& resource : resources) {
            // 写入资源类型标识
            ResourceType type = resource->getResourceType();
            file.write(reinterpret_cast<const char*>(&type), sizeof(ResourceType));

            // 序列化资源
            resource->serialize(file);
        }

        file.close();
    }

    // 添加资源
    bool addResource(std::unique_ptr<Resource> resource) {
        // 检查资源ID是否已存在
        for (const auto& res : resources) {
            if (res->getResourceId() == resource->getResourceId()) {
                return false; // 资源ID已存在
            }
        }

        resources.push_back(std::move(resource));
        return true;
    }

    // 根据ID查找资源
    Resource* findResourceById(const std::string& resourceId) {
        for (auto& resource : resources) {
            if (resource->getResourceId() == resourceId) {
                return resource.get();
            }
        }
        return nullptr; // 未找到资源
    }

    const Resource* findResourceById(const std::string& resourceId) const {
        for (const auto& resource : resources) {
            if (resource->getResourceId() == resourceId) {
                return resource.get();
            }
        }
        return nullptr; // 未找到资源
    }

    // 获取所有资源（用于用户浏览和管理员管理）
    const std::vector<std::unique_ptr<Resource>>& getAllResources() const { return resources; }

    // 管理员操作：删除资源
    bool deleteResource(const std::string& resourceId) {
        for (auto it = resources.begin(); it != resources.end(); ++it) {
            if ((*it)->getResourceId() == resourceId) {
                resources.erase(it);
                return true;
            }
        }
        return false;
    }

    // 更新资源状态
    bool setResourceStatus(const std::string& resourceId, ResourceStatus status) {
        Resource* resource = findResourceById(resourceId);
        if (resource) {
            resource->setStatus(status);
            return true;
        }
        return false;
    }

    // 生成唯一的资源ID
    std::string generateUniqueResourceId(ResourceType type) {
        std::string prefix = (type == ResourceType::CPU) ? "CPU" : "GPU";
        return prefix + std::to_string(nextResourceId++);
    }
};

#endif // RESOURCE_HPP
