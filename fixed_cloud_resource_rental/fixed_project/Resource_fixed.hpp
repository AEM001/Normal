#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <stdexcept>

// 资源类型枚举
enum class ResourceType {
    CPU,
    GPU
};

// 资源状态枚举
enum class ResourceStatus {
    IDLE,       // 空闲，可被租用
    IN_USE,     // 正在使用中
    MAINTENANCE // 维护中，不可租用
};

// 枚举转字符串函数
inline const char* ResourceTypeToString(ResourceType type) {
    switch(type) {
        case ResourceType::CPU: return "CPU";
        case ResourceType::GPU: return "GPU";
        default: return "未知类型";
    }
}

inline const char* ResourceStatusToString(ResourceStatus status) {
    switch(status) {
        case ResourceStatus::IDLE: return "空闲";
        case ResourceStatus::IN_USE: return "使用中";
        case ResourceStatus::MAINTENANCE: return "维护中";
        default: return "未知状态";
    }
}

/**
 * @class Resource
 * @brief 表示系统中的计算资源。
 *
 * 这是一个抽象基类，定义了所有资源共有的属性和方法。
 * 具体的资源类型（如CPU、GPU）继承自此类。
 */
class Resource {
protected:
    std::string resourceId;
    std::string resourceName;
    double hourlyRate;
    ResourceStatus status;
    ResourceType resourceType;

public:
    // 构造函数
    Resource(std::string id, std::string name, double rate, ResourceType type, ResourceStatus stat = ResourceStatus::IDLE)
        : resourceId(id), resourceName(name), hourlyRate(rate), resourceType(type), status(stat) {}
    
    // 为反序列化添加默认构造函数
    Resource() : resourceId(""), resourceName(""), hourlyRate(0.0), 
                 resourceType(ResourceType::CPU), status(ResourceStatus::IDLE) {}
    
    // 虚析构函数
    virtual ~Resource() = default;

    // 获取器
    std::string getResourceId() const { return resourceId; }
    std::string getResourceName() const { return resourceName; }
    double getHourlyRate() const { return hourlyRate; }
    ResourceStatus getStatus() const { return status; }
    ResourceType getResourceType() const { return resourceType; }

    // 设置器
    void setResourceName(const std::string& name) { resourceName = name; }
    void setHourlyRate(double rate) { hourlyRate = rate; }
    void setStatus(ResourceStatus stat) { status = stat; }

    // 检查资源是否可用
    bool isAvailable() const { return status == ResourceStatus::IDLE; }

    // 显示资源详情的纯虚函数
    virtual void displayDetails() const = 0;

    // 序列化/反序列化方法（用于数据持久化）
    virtual void serialize(std::ostream& os) const {
        // 写入基本属性
        os.write(resourceId.c_str(), resourceId.size() + 1);
        os.write(resourceName.c_str(), resourceName.size() + 1);
        os.write(reinterpret_cast<const char*>(&hourlyRate), sizeof(double));
        os.write(reinterpret_cast<const char*>(&status), sizeof(ResourceStatus));
        os.write(reinterpret_cast<const char*>(&resourceType), sizeof(ResourceType));
    }
    
    virtual void deserialize(std::istream& is) {
        // 读取基本属性
        char buffer[256];
        is.getline(buffer, 256, '\0');
        resourceId = buffer;
        
        is.getline(buffer, 256, '\0');
        resourceName = buffer;
        
        is.read(reinterpret_cast<char*>(&hourlyRate), sizeof(double));
        is.read(reinterpret_cast<char*>(&status), sizeof(ResourceStatus));
        is.read(reinterpret_cast<char*>(&resourceType), sizeof(ResourceType));
    }
};

/**
 * @class CPUResource
 * @brief 表示CPU计算资源。
 *
 * 继承自Resource。包含CPU特有的属性，如核心数和频率。
 */
class CPUResource : public Resource {
private:
    int cores;
    double frequency; // GHz
    double storage; // GB

public:
    CPUResource(std::string id, std::string name, double rate, int numCores, double freq, 
                ResourceStatus stat = ResourceStatus::IDLE, double storageGB = 100.0)
        : Resource(id, name, rate, ResourceType::CPU, stat), cores(numCores), frequency(freq), storage(storageGB) {}
    
    // 空构造函数用于反序列化
    CPUResource() : Resource(), cores(0), frequency(0.0), storage(0.0) {
        resourceType = ResourceType::CPU;
    }
    
    // 获取器
    int getCores() const { return cores; }
    double getFrequency() const { return frequency; }
    double getStorage() const { return storage; }
    
    // 设置器
    void setCores(int numCores) { cores = numCores; }
    void setFrequency(double freq) { frequency = freq; }
    void setStorage(double storageGB) { storage = storageGB; }
    
    void displayDetails() const override {
        std::cout << "===== CPU资源详情 =====\n";
        std::cout << "ID: " << resourceId << "\n";
        std::cout << "名称: " << resourceName << "\n";
        std::cout << "状态: " << ResourceStatusToString(status) << "\n";
        std::cout << "小时费率: " << hourlyRate << " 元/小时\n";
        std::cout << "核心数: " << cores << "\n";
        std::cout << "频率: " << frequency << " GHz\n";
        std::cout << "存储容量: " << storage << " GB\n";
    }

    // 序列化/反序列化方法
    void serialize(std::ostream& os) const override {
        Resource::serialize(os);
        os.write(reinterpret_cast<const char*>(&cores), sizeof(int));
        os.write(reinterpret_cast<const char*>(&frequency), sizeof(double));
        os.write(reinterpret_cast<const char*>(&storage), sizeof(double));
    }
    
    void deserialize(std::istream& is) override {
        Resource::deserialize(is);
        is.read(reinterpret_cast<char*>(&cores), sizeof(int));
        is.read(reinterpret_cast<char*>(&frequency), sizeof(double));
        is.read(reinterpret_cast<char*>(&storage), sizeof(double));
    }
};

/**
 * @class GPUResource
 * @brief 表示GPU计算资源。
 *
 * 继承自Resource。包含GPU特有的属性，如CUDA核心数和显存容量。
 */
class GPUResource : public Resource {
private:
    int cudaCores;
    int vram; // GB
    double storage; // GB

public:
    GPUResource(std::string id, std::string name, double rate, int numCudaCores, int vramGB, 
                ResourceStatus stat = ResourceStatus::IDLE, double storageGB = 100.0)
        : Resource(id, name, rate, ResourceType::GPU, stat), cudaCores(numCudaCores), vram(vramGB), storage(storageGB) {}
    
    // 空构造函数用于反序列化
    GPUResource() : Resource(), cudaCores(0), vram(0), storage(0.0) {
        resourceType = ResourceType::GPU;
    }
    
    // 获取器
    int getCudaCores() const { return cudaCores; }
    int getVRAM() const { return vram; }
    double getStorage() const { return storage; }
    
    // 设置器
    void setCudaCores(int numCudaCores) { cudaCores = numCudaCores; }
    void setVRAM(int vramGB) { vram = vramGB; }
    void setStorage(double storageGB) { storage = storageGB; }
    
    void displayDetails() const override {
        std::cout << "===== GPU资源详情 =====\n";
        std::cout << "ID: " << resourceId << "\n";
        std::cout << "名称: " << resourceName << "\n";
        std::cout << "状态: " << ResourceStatusToString(status) << "\n";
        std::cout << "小时费率: " << hourlyRate << " 元/小时\n";
        std::cout << "CUDA核心数: " << cudaCores << "\n";
        std::cout << "显存容量: " << vram << " GB\n";
        std::cout << "存储容量: " << storage << " GB\n";
    }

    // 序列化/反序列化方法
    void serialize(std::ostream& os) const override {
        Resource::serialize(os);
        os.write(reinterpret_cast<const char*>(&cudaCores), sizeof(int));
        os.write(reinterpret_cast<const char*>(&vram), sizeof(int));
        os.write(reinterpret_cast<const char*>(&storage), sizeof(double));
    }
    
    void deserialize(std::istream& is) override {
        Resource::deserialize(is);
        is.read(reinterpret_cast<char*>(&cudaCores), sizeof(int));
        is.read(reinterpret_cast<char*>(&vram), sizeof(int));
        is.read(reinterpret_cast<char*>(&storage), sizeof(double));
    }
};

/**
 * @class ResourceCollection
 * @brief 管理系统中所有资源的集合。
 *
 * 提供添加、查找、列出资源的功能。
 * 可以按类型筛选资源。
 */
class ResourceCollection {
private:
    std::vector<Resource*> resources;

public:
    // 析构函数释放所有资源对象
    ~ResourceCollection() {
        for (auto resource : resources) {
            delete resource;
        }
        resources.clear();
    }

    // 添加资源到集合
    bool addResource(Resource* resource) {
        // 检查资源ID是否已存在
        for (auto res : resources) {
            if (res->getResourceId() == resource->getResourceId()) {
                return false; // 资源ID已存在
            }
        }
        resources.push_back(resource);
        return true;
    }

    // 根据ID查找资源
    Resource* findResourceById(const std::string& id) const {
        for (auto resource : resources) {
            if (resource->getResourceId() == id) {
                return resource;
            }
        }
        return nullptr; // 未找到资源
    }

    // 获取所有资源
    const std::vector<Resource*>& getAllResources() const {
        return resources;
    }

    // 获取特定类型的资源
    std::vector<Resource*> getResourcesByType(ResourceType type) const {
        std::vector<Resource*> result;
        for (auto resource : resources) {
            if (resource->getResourceType() == type) {
                result.push_back(resource);
            }
        }
        return result;
    }

    // 获取可用资源
    std::vector<Resource*> getAvailableResources() const {
        std::vector<Resource*> result;
        for (auto resource : resources) {
            if (resource->isAvailable()) {
                result.push_back(resource);
            }
        }
        return result;
    }

    // 显示所有资源
    void displayAllResources() const {
        std::cout << "===== 所有资源列表 =====\n";
        for (auto resource : resources) {
            resource->displayDetails();
            std::cout << "------------------------\n";
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
        for (auto resource : resources) {
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
        for (auto resource : resources) {
            delete resource;
        }
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
            Resource* resource = nullptr;
            switch (type) {
                case ResourceType::CPU:
                    resource = new CPUResource();
                    break;
                case ResourceType::GPU:
                    resource = new GPUResource();
                    break;
                default:
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
    
    // 添加CPU资源
    collection.addResource(new CPUResource("CPU001", "Intel Xeon E5-2680", 10.0, 8, 2.7));
    collection.addResource(new CPUResource("CPU002", "AMD EPYC 7742", 15.0, 64, 2.25));
    collection.addResource(new CPUResource("CPU003", "Intel Core i9-10980XE", 12.0, 18, 3.0));
    collection.addResource(new CPUResource("CPU004", "AMD Ryzen Threadripper 3990X", 20.0, 64, 2.9));
    collection.addResource(new CPUResource("CPU005", "Intel Xeon Platinum 8280", 25.0, 28, 2.7));
    
    // 添加GPU资源
    collection.addResource(new GPUResource("GPU001", "NVIDIA Tesla V100", 30.0, 5120, 32));
    collection.addResource(new GPUResource("GPU002", "NVIDIA Tesla A100", 50.0, 6912, 40));
    collection.addResource(new GPUResource("GPU003", "NVIDIA GeForce RTX 3090", 25.0, 10496, 24));
    collection.addResource(new GPUResource("GPU004", "AMD Radeon Instinct MI100", 40.0, 7680, 32));
    collection.addResource(new GPUResource("GPU005", "NVIDIA Tesla T4", 20.0, 2560, 16));
    
    return collection;
}

/**
 * @class ResourceManager
 * @brief 管理系统中的所有资源。
 *
 * 负责资源的添加、查找、修改和数据持久化。
 */
class ResourceManager {
private:
    std::vector<Resource*> resources;
    std::string dataFilePath; // 资源数据文件路径
    int nextCpuId;
    int nextGpuId;

public:
    ResourceManager(const std::string& filePath) : dataFilePath(filePath), nextCpuId(6), nextGpuId(6) {}
    
    ~ResourceManager() { 
        saveResources(); 
        for (auto resource : resources) {
            delete resource;
        }
        resources.clear();
    }

    // 加载资源数据
    void loadResources() {
        try {
            std::ifstream file(dataFilePath, std::ios::binary);
            if (!file) {
                std::cerr << "警告: 无法打开资源数据文件，将创建新文件。\n";
                // 创建默认资源
                resources = createDefaultResourceCollection().getAllResources();
                saveResources(); // 立即保存默认资源
                return;
            }

            // 清空当前资源
            for (auto resource : resources) {
                delete resource;
            }
            resources.clear();

            // 读取下一个可用的资源ID
            file.read(reinterpret_cast<char*>(&nextCpuId), sizeof(int));
            file.read(reinterpret_cast<char*>(&nextGpuId), sizeof(int));

            // 读取资源数量
            size_t count;
            file.read(reinterpret_cast<char*>(&count), sizeof(size_t));

            // 逐个读取资源
            for (size_t i = 0; i < count; ++i) {
                // 读取资源类型标识
                ResourceType type;
                file.read(reinterpret_cast<char*>(&type), sizeof(ResourceType));

                // 根据类型创建相应的资源对象
                Resource* resource = nullptr;
                switch (type) {
                    case ResourceType::CPU:
                        resource = new CPUResource();
                        break;
                    case ResourceType::GPU:
                        resource = new GPUResource();
                        break;
                    default:
                        throw std::runtime_error("未知的资源类型");
                }

                // 反序列化资源
                resource->deserialize(file);

                // 添加到集合
                resources.push_back(resource);
            }

            file.close();
        } catch (const std::exception& e) {
            std::cerr << "加载资源数据时发生错误: " << e.what() << "\n";
            // 创建默认资源
            resources = createDefaultResourceCollection().getAllResources();
            saveResources(); // 立即保存默认资源
        }
    }

    // 保存资源数据
    void saveResources() const {
        try {
            // 确保data目录存在
            #ifdef _WIN32
                _mkdir("data");
            #else
                mkdir("data", 0777);
            #endif
            
            std::ofstream file(dataFilePath, std::ios::binary);
            if (!file) {
                throw std::runtime_error("无法打开文件进行写入: " + dataFilePath);
            }

            // 写入下一个可用的资源ID
            file.write(reinterpret_cast<const char*>(&nextCpuId), sizeof(int));
            file.write(reinterpret_cast<const char*>(&nextGpuId), sizeof(int));

            // 写入资源数量
            size_t count = resources.size();
            file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));

            // 逐个写入资源
            for (auto resource : resources) {
                // 写入资源类型标识
                ResourceType type = resource->getResourceType();
                file.write(reinterpret_cast<const char*>(&type), sizeof(ResourceType));

                // 序列化资源
                resource->serialize(file);
            }

            file.close();
        } catch (const std::exception& e) {
            std::cerr << "保存资源数据时发生错误: " << e.what() << "\n";
        }
    }

    // 添加资源
    bool addResource(Resource* resource) {
        // 检查资源ID是否已存在
        for (auto res : resources) {
            if (res->getResourceId() == resource->getResourceId()) {
                return false; // 资源ID已存在
            }
        }
        resources.push_back(resource);
        return true;
    }

    // 删除资源
    bool deleteResource(const std::string& resourceId) {
        for (auto it = resources.begin(); it != resources.end(); ++it) {
            if ((*it)->getResourceId() == resourceId) {
                delete *it;
                resources.erase(it);
                return true;
            }
        }
        return false; // 未找到资源
    }

    // 根据ID查找资源
    Resource* findResourceById(const std::string& id) const {
        for (auto resource : resources) {
            if (resource->getResourceId() == id) {
                return resource;
            }
        }
        return nullptr; // 未找到资源
    }

    // 设置资源状态
    bool setResourceStatus(const std::string& resourceId, ResourceStatus status) {
        Resource* resource = findResourceById(resourceId);
        if (resource) {
            resource->setStatus(status);
            return true;
        }
        return false; // 未找到资源
    }

    // 获取所有资源
    const std::vector<Resource*>& getAllResources() const {
        return resources;
    }

    // 生成唯一的资源ID
    std::string generateUniqueResourceId(ResourceType type) {
        std::string prefix;
        int id;
        
        if (type == ResourceType::CPU) {
            prefix = "CPU";
            id = nextCpuId++;
        } else {
            prefix = "GPU";
            id = nextGpuId++;
        }
        
        // 格式化ID，确保至少3位数字
        char idStr[4];
        sprintf(idStr, "%03d", id);
        
        return prefix + idStr;
    }
};

#endif // RESOURCE_HPP
