# 编译与运行说明

## 项目概述

这是一个云算力资源租用系统，使用C++实现，采用传统C++特性，避免使用现代C++特性（如智能指针、filesystem等）。系统实现了用户管理、资源管理、租赁管理、计费管理和通知管理等功能。

## 文件结构

项目包含以下主要文件：

- `User.hpp` - 用户类定义
- `Resource.hpp` - 资源类定义
- `Rental.hpp` - 租赁类定义
- `Notification.hpp` - 通知类定义
- `Notification.cpp` - 通知类实现
- `Billing.hpp` - 计费类定义
- `DataManager.hpp` - 数据管理类定义
- `DataManager.cpp` - 数据管理类实现
- `main.cpp` - 主程序入口

## 编译环境要求

- C++11或更高版本的编译器（如GCC、MSVC、Clang等）
- 支持标准库的基本功能（如iostream、fstream、vector等）

## 编译步骤

### Windows环境（使用Visual Studio）

1. 创建一个新的空白C++项目
2. 将所有源文件添加到项目中
3. 在项目属性中设置C++11或更高版本的支持
4. 编译项目

```bash
# 使用Visual Studio命令行编译
cl /EHsc /std:c++11 main.cpp Notification.cpp DataManager.cpp /Fe:CloudResourceRental.exe
```

### Windows环境（使用MinGW）

```bash
# 使用g++编译
g++ -std=c++11 main.cpp Notification.cpp DataManager.cpp -o CloudResourceRental.exe
```

### Linux/macOS环境

```bash
# 使用g++编译
g++ -std=c++11 main.cpp Notification.cpp DataManager.cpp -o CloudResourceRental
```

## 运行前准备

1. 在程序运行目录下创建`data`文件夹，用于存储系统数据
   ```bash
   # Windows
   mkdir data
   
   # Linux/macOS
   mkdir -p data
   ```

2. 注意事项：
   - 程序使用了`conio.h`头文件和`_getch()`函数，这些在Windows环境下可用，但在Linux/macOS环境下需要替换
   - 如果在Linux/macOS环境下编译，请将`main.cpp`中的密码输入函数修改为：
     ```cpp
     // Linux/macOS版本的密码输入函数
     std::string getPasswordInput() {
         std::string password;
         char ch;
         system("stty -echo"); // 关闭回显
         while ((ch = getchar()) != '\n') {
             password.push_back(ch);
         }
         system("stty echo"); // 恢复回显
         std::cout << std::endl;
         return password;
     }
     ```

## 运行程序

### Windows环境

```bash
CloudResourceRental.exe
```

### Linux/macOS环境

```bash
./CloudResourceRental
```

## 系统初始化

首次运行时，系统会自动创建必要的数据文件，并初始化默认资源。

## 默认管理员账户

为了方便测试，您可以在首次运行后注册一个管理员账户：

1. 选择"注册"选项
2. 输入用户名和密码
3. 选择角色（学生或教师）
4. 登录后，可以使用系统功能

注意：系统默认不会创建管理员账户，需要手动注册。如需创建管理员账户，可以修改代码或通过数据文件直接添加。

## 常见问题

1. **编译错误：找不到conio.h**
   - 这是Windows特有的头文件，在Linux/macOS下不可用
   - 解决方案：修改密码输入函数，使用上述Linux/macOS版本

2. **运行时错误：无法创建或访问数据文件**
   - 确保已创建`data`目录
   - 确保程序有权限读写该目录

3. **密码输入不隐藏**
   - Windows环境下使用`conio.h`的`_getch()`函数实现密码隐藏
   - Linux/macOS环境下使用`system("stty -echo")`和`system("stty echo")`实现

## 代码修改说明

如果需要在不同环境下运行，可能需要对代码进行以下修改：

1. 替换密码输入函数（如上所述）
2. 修改目录创建方式（DataManager.cpp中的createDataDirectory函数）
3. 根据需要调整文件路径格式（Windows使用反斜杠，Linux/macOS使用正斜杠）

## 数据文件说明

系统使用二进制文件存储数据，所有数据文件位于`data`目录下：

- `users.dat` - 用户数据
- `resources.dat` - 资源数据
- `rentals.dat` - 租赁数据
- `notifications.dat` - 通知数据
- `bills.dat` - 账单数据
- `billing_rules.dat` - 计费规则数据
- `system.log` - 系统日志
