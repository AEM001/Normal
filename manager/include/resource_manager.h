#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "resource.h"
#include "exception.h"
#include "data_manager.h"
#include <vector>
#include <memory>
#include <algorithm>

// 资源管理器类
class ResourceManager {
private:
    std::vector<std::shared_ptr<Resource>> resources;
    std::shared_ptr<DataManager> dataManager;
    
    // 查找资源
    std::shared_ptr<Resource> findResourceById(const std::string& id) {
        for (const auto& resource : resources) {
            if (resource->getId() == id) {
                return resource;
            }
        }
        return nullptr;
    }
    
public:
    ResourceManager(std::shared_ptr<DataManager> dataManager) 
        : dataManager(dataManager) {
        // 加载资源数据
        dataManager->loadResources<Resource>(resources);
    }
    
    ~ResourceManager() {
        saveResources();
    }
    
    // 保存资源数据
    bool saveResources() {
        return dataManager->saveResources<Resource>(resources);
    }
    
    // 添加CPU资源
    bool addCPUResource(const std::string& name, const std::string& description,
                        int cores, double frequency, double pricePerHour) {
        try {
            auto resource = std::make_shared<CPUResource>(name, description, cores, frequency, pricePerHour);
            resources.push_back(resource);
            saveResources();
            return true;
        } catch (const Exception& e) {
            std::cerr << "添加CPU资源失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 添加GPU资源
    bool addGPUResource(const std::string& name, const std::string& description,
                        const std::string& model, int memory, double computePower, double pricePerHour) {
        try {
            auto resource = std::make_shared<GPUResource>(name, description, model, memory, computePower, pricePerHour);
            resources.push_back(resource);
            saveResources();
            return true;
        } catch (const Exception& e) {
            std::cerr << "添加GPU资源失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 添加存储资源
    bool addStorageResource(const std::string& name, const std::string& description,
                            int capacity, double readSpeed, double writeSpeed, 
                            const std::string& storageType, double pricePerHour) {
        try {
            auto resource = std::make_shared<StorageResource>(name, description, capacity, 
                                                             readSpeed, writeSpeed, storageType, pricePerHour);
            resources.push_back(resource);
            saveResources();
            return true;
        } catch (const Exception& e) {
            std::cerr << "添加存储资源失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 删除资源
    bool deleteResource(const std::string& id) {
        try {
            auto it = std::find_if(resources.begin(), resources.end(), 
                                  [&id](const std::shared_ptr<Resource>& resource) {
                                      return resource->getId() == id;
                                  });
            
            if (it == resources.end()) {
                throw ResourceException("资源不存在");
            }
            
            if ((*it)->getStatus() == IN_USE) {
                throw ResourceException("资源正在使用中，无法删除");
            }
            
            resources.erase(it);
            saveResources();
            return true;
        } catch (const Exception& e) {
            std::cerr << "删除资源失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 更新资源状态
    bool updateResourceStatus(const std::string& id, ResourceStatus status) {
        try {
            auto resource = findResourceById(id);
            if (resource == nullptr) {
                throw ResourceException("资源不存在");
            }
            
            resource->setStatus(status);
            saveResources();
            return true;
        } catch (const Exception& e) {
            std::cerr << "更新资源状态失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 更新资源价格
    bool updateResourcePrice(const std::string& id, double pricePerHour) {
        try {
            auto resource = findResourceById(id);
            if (resource == nullptr) {
                throw ResourceException("资源不存在");
            }
            
            resource->setPricePerHour(pricePerHour);
            saveResources();
            return true;
        } catch (const Exception& e) {
            std::cerr << "更新资源价格失败: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 获取所有资源
    std::vector<std::shared_ptr<Resource>> getAllResources() const {
        return resources;
    }
    
    // 获取指定类型的资源
    std::vector<std::shared_ptr<Resource>> getResourcesByType(ResourceType type) const {
        std::vector<std::shared_ptr<Resource>> result;
        for (const auto& resource : resources) {
            if (resource->getType() == type) {
                result.push_back(resource);
            }
        }
        return result;
    }
    
    // 获取空闲资源
    std::vector<std::shared_ptr<Resource>> getIdleResources() const {
        std::vector<std::shared_ptr<Resource>> result;
        for (const auto& resource : resources) {
            if (resource->isIdle()) {
                result.push_back(resource);
            }
        }
        return result;
    }
    
    // 获取资源数量
    size_t getResourceCount() const {
        return resources.size();
    }
    
    // 获取资源
    std::shared_ptr<Resource> getResource(const std::string& id) {
        return findResourceById(id);
    }
};

#endif // RESOURCE_MANAGER_H
