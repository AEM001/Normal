#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "common.h"

// 异常基类
class Exception {
protected:
    std::string message;
    int code;
    std::string time;

public:
    Exception(const std::string& message, int code) 
        : message(message), code(code), time(getCurrentTime()) {}

    virtual ~Exception() {}

    std::string getMessage() const { return message; }
    int getCode() const { return code; }
    std::string getTime() const { return time; }

    virtual std::string what() const {
        std::stringstream ss;
        ss << "[Error " << code << "] " << message << " (at " << time << ")";
        return ss.str();
    }
};

// 系统异常基类
class SystemException : public Exception {
public:
    SystemException(const std::string& message, int code = 1000) 
        : Exception(message, code) {}
};

// 文件操作异常
class FileException : public SystemException {
public:
    FileException(const std::string& message, int code = 1100) 
        : SystemException(message, code) {}
};

// 数据处理异常
class DataException : public SystemException {
public:
    DataException(const std::string& message, int code = 1200) 
        : SystemException(message, code) {}
};

// 业务异常基类
class BusinessException : public Exception {
public:
    BusinessException(const std::string& message, int code = 2000) 
        : Exception(message, code) {}
};

// 用户相关异常
class UserException : public BusinessException {
public:
    UserException(const std::string& message, int code = 2100) 
        : BusinessException(message, code) {}
};

// 资源相关异常
class ResourceException : public BusinessException {
public:
    ResourceException(const std::string& message, int code = 2200) 
        : BusinessException(message, code) {}
};

// 租用相关异常
class RentalException : public BusinessException {
public:
    RentalException(const std::string& message, int code = 2300) 
        : BusinessException(message, code) {}
};

// 计费相关异常
class BillingException : public BusinessException {
public:
    BillingException(const std::string& message, int code = 2400) 
        : BusinessException(message, code) {}
};

#endif // EXCEPTION_H
