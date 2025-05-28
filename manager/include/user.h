#ifndef USER_H
#define USER_H

#include "entity.h"

// 用户基类
class User : public Entity {
protected:
    std::string username;
    std::string password;
    std::string name;
    double balance;
    UserStatus status;
    UserType type;

public:
    User() : Entity(), balance(DEFAULT_BALANCE), status(ACTIVE) {}
    
    User(const std::string& username, const std::string& password, const std::string& name, 
         UserType type, double balance = DEFAULT_BALANCE, UserStatus status = ACTIVE)
        : Entity(), username(username), password(password), name(name), 
          balance(balance), status(status), type(type) {}
    
    User(const std::string& id, const std::string& createTime,
         const std::string& username, const std::string& password, const std::string& name,
         UserType type, double balance, UserStatus status)
        : Entity(id, createTime), username(username), password(password), name(name),
          balance(balance), status(status), type(type) {}
    
    virtual ~User() {}
    
    // Getters
    std::string getUsername() const { return username; }
    std::string getName() const { return name; }
    double getBalance() const { return balance; }
    UserStatus getStatus() const { return status; }
    UserType getType() const { return type; }
    
    // Setters
    void setName(const std::string& name) { this->name = name; }
    void setPassword(const std::string& password) { this->password = password; }
    void setBalance(double balance) { this->balance = balance; }
    void setStatus(UserStatus status) { this->status = status; }
    
    // 业务方法
    bool checkPassword(const std::string& password) const { return this->password == password; }
    bool isActive() const { return status == ACTIVE; }
    void addBalance(double amount) { balance += amount; }
    bool deductBalance(double amount) {
        if (balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }
    
    // 序列化和反序列化
    virtual void serialize(std::ofstream& out) const override {
        Entity::serialize(out);
        out.write(username.c_str(), username.size() + 1);
        out.write(password.c_str(), password.size() + 1);
        out.write(name.c_str(), name.size() + 1);
        out.write(reinterpret_cast<const char*>(&balance), sizeof(balance));
        out.write(reinterpret_cast<const char*>(&status), sizeof(status));
        out.write(reinterpret_cast<const char*>(&type), sizeof(type));
    }
    
    virtual void deserialize(std::ifstream& in) override {
        Entity::deserialize(in);
        char buffer[256];
        
        in.getline(buffer, 256, '\0');
        username = buffer;
        
        in.getline(buffer, 256, '\0');
        password = buffer;
        
        in.getline(buffer, 256, '\0');
        name = buffer;
        
        in.read(reinterpret_cast<char*>(&balance), sizeof(balance));
        in.read(reinterpret_cast<char*>(&status), sizeof(status));
        in.read(reinterpret_cast<char*>(&type), sizeof(type));
    }
    
    // 显示用户信息
    virtual void displayInfo() const {
        std::cout << "ID: " << id << std::endl;
        std::cout << "用户名: " << username << std::endl;
        std::cout << "姓名: " << name << std::endl;
        std::cout << "余额: " << std::fixed << std::setprecision(2) << balance << std::endl;
        std::cout << "状态: " << (status == ACTIVE ? "激活" : "暂停") << std::endl;
        std::cout << "创建时间: " << createTime << std::endl;
    }
};

// 学生类
class Student : public User {
private:
    std::string studentId;
    std::string major;
    
public:
    Student() : User() { type = STUDENT; }
    
    Student(const std::string& username, const std::string& password, const std::string& name,
            const std::string& studentId, const std::string& major)
        : User(username, password, name, STUDENT), studentId(studentId), major(major) {}
    
    Student(const std::string& id, const std::string& createTime,
            const std::string& username, const std::string& password, const std::string& name,
            double balance, UserStatus status,
            const std::string& studentId, const std::string& major)
        : User(id, createTime, username, password, name, STUDENT, balance, status),
          studentId(studentId), major(major) {}
    
    // Getters
    std::string getStudentId() const { return studentId; }
    std::string getMajor() const { return major; }
    
    // Setters
    void setStudentId(const std::string& studentId) { this->studentId = studentId; }
    void setMajor(const std::string& major) { this->major = major; }
    
    // 序列化和反序列化
    void serialize(std::ofstream& out) const override {
        User::serialize(out);
        out.write(studentId.c_str(), studentId.size() + 1);
        out.write(major.c_str(), major.size() + 1);
    }
    
    void deserialize(std::ifstream& in) override {
        User::deserialize(in);
        char buffer[256];
        
        in.getline(buffer, 256, '\0');
        studentId = buffer;
        
        in.getline(buffer, 256, '\0');
        major = buffer;
    }
    
    // 显示用户信息
    void displayInfo() const override {
        User::displayInfo();
        std::cout << "学号: " << studentId << std::endl;
        std::cout << "专业: " << major << std::endl;
    }
};

// 教师类
class Teacher : public User {
private:
    std::string teacherId;
    std::string title;
    
public:
    Teacher() : User() { type = TEACHER; }
    
    Teacher(const std::string& username, const std::string& password, const std::string& name,
            const std::string& teacherId, const std::string& title)
        : User(username, password, name, TEACHER), teacherId(teacherId), title(title) {}
    
    Teacher(const std::string& id, const std::string& createTime,
            const std::string& username, const std::string& password, const std::string& name,
            double balance, UserStatus status,
            const std::string& teacherId, const std::string& title)
        : User(id, createTime, username, password, name, TEACHER, balance, status),
          teacherId(teacherId), title(title) {}
    
    // Getters
    std::string getTeacherId() const { return teacherId; }
    std::string getTitle() const { return title; }
    
    // Setters
    void setTeacherId(const std::string& teacherId) { this->teacherId = teacherId; }
    void setTitle(const std::string& title) { this->title = title; }
    
    // 序列化和反序列化
    void serialize(std::ofstream& out) const override {
        User::serialize(out);
        out.write(teacherId.c_str(), teacherId.size() + 1);
        out.write(title.c_str(), title.size() + 1);
    }
    
    void deserialize(std::ifstream& in) override {
        User::deserialize(in);
        char buffer[256];
        
        in.getline(buffer, 256, '\0');
        teacherId = buffer;
        
        in.getline(buffer, 256, '\0');
        title = buffer;
    }
    
    // 显示用户信息
    void displayInfo() const override {
        User::displayInfo();
        std::cout << "工号: " << teacherId << std::endl;
        std::cout << "职称: " << title << std::endl;
    }
};

// 管理员类
class Admin : public User {
private:
    int permissionLevel;
    
public:
    Admin() : User(), permissionLevel(1) { type = ADMIN; }
    
    Admin(const std::string& username, const std::string& password, const std::string& name,
          int permissionLevel = 1)
        : User(username, password, name, ADMIN), permissionLevel(permissionLevel) {}
    
    Admin(const std::string& id, const std::string& createTime,
          const std::string& username, const std::string& password, const std::string& name,
          double balance, UserStatus status, int permissionLevel)
        : User(id, createTime, username, password, name, ADMIN, balance, status),
          permissionLevel(permissionLevel) {}
    
    // Getters
    int getPermissionLevel() const { return permissionLevel; }
    
    // Setters
    void setPermissionLevel(int level) { permissionLevel = level; }
    
    // 序列化和反序列化
    void serialize(std::ofstream& out) const override {
        User::serialize(out);
        out.write(reinterpret_cast<const char*>(&permissionLevel), sizeof(permissionLevel));
    }
    
    void deserialize(std::ifstream& in) override {
        User::deserialize(in);
        in.read(reinterpret_cast<char*>(&permissionLevel), sizeof(permissionLevel));
    }
    
    // 显示用户信息
    void displayInfo() const override {
        User::displayInfo();
        std::cout << "权限级别: " << permissionLevel << std::endl;
    }
};

#endif // USER_H
