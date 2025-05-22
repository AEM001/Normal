#include <cstring>    // for strlen, memcpy, strcmp
#include <iostream>

// 自定义字符串类
class MyString {
private:
    char* data;        // 指向堆上分配的字符数组
    size_t length;     // 字符串长度（不含\0）
    size_t capacity;   // 分配的内存容量

public:
    // 默认构造函数，初始化为空字符串
    MyString() : data(new char[1]), length(0), capacity(1) {
        data[0] = '\0';
    }

    // 从C风格字符串构造
    explicit MyString(const char* str) {
        if (str) {
            length = std::strlen(str);
            capacity = length + 1;
            data = new char[capacity];
            std::memcpy(data, str, length + 1); // 包含\0
        } else {
            length = 0;
            capacity = 1;
            data = new char[1];
            data[0] = '\0';
        }
    }

    // 创建n个字符c的字符串
    MyString(size_t n, char c) {
        length = n;
        capacity = length + 1;
        data = new char[capacity];
        for (size_t i = 0; i < length; ++i) {
            data[i] = c;
        }
        data[length] = '\0';
    }

    // 析构函数，释放内存
    ~MyString() {
        delete[] data;
    }

    // 拷贝构造函数，深拷贝
    MyString(const MyString& other) {
        length = other.length;
        capacity = other.capacity;
        data = new char[capacity];
        std::memcpy(data, other.data, length + 1);
    }

    // 拷贝赋值运算符，处理自赋值，深拷贝
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data;
            length = other.length;
            capacity = other.capacity;
            data = new char[capacity];
            std::memcpy(data, other.data, length + 1);
        }
        return *this;
    }

    // 移动构造函数，高效转移所有权
    MyString(MyString&& other) noexcept
        : data(other.data), length(other.length), capacity(other.capacity) {
        other.data = new char[1];
        other.data[0] = '\0';
        other.length = 0;
        other.capacity = 1;
    }

    // 移动赋值运算符
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            length = other.length;
            capacity = other.capacity;
            other.data = new char[1];
            other.data[0] = '\0';
            other.length = 0;
            other.capacity = 1;
        }
        return *this;
    }

    // 追加另一个MyString
    MyString& append(const MyString& str) {
        if (str.length == 0) return *this;
        if (length + str.length + 1 > capacity) {
            reserve((length + str.length + 1) * 2);
        }
        std::memcpy(data + length, str.data, str.length + 1); // 包含\0
        length += str.length;
        return *this;
    }

    // 追加C风格字符串
    MyString& append(const char* str) {
        if (!str) return *this;
        size_t str_len = std::strlen(str);
        if (str_len == 0) return *this;
        if (length + str_len + 1 > capacity) {
            reserve((length + str_len + 1) * 2);
        }
        std::memcpy(data + length, str, str_len + 1);
        length += str_len;
        return *this;
    }

    // 子字符串
    MyString substr(size_t pos, size_t len) const {
        if (pos >= length) return MyString();
        size_t n = (pos + len > length) ? (length - pos) : len;
        MyString sub(n, '\0');
        std::memcpy(sub.data, data + pos, n);
        sub.data[n] = '\0';
        sub.length = n;
        return sub;
    }

    // 重载[]运算符，访问字符
    char& operator[](size_t index) {
        return data[index];
    }
    const char& operator[](size_t index) const {
        return data[index];
    }

    // 字符串连接
    MyString operator+(const MyString& rhs) const {
        MyString result(*this);
        result.append(rhs);
        return result;
    }

    // 比较相等
    bool operator==(const MyString& rhs) const {
        return (length == rhs.length) && (std::strcmp(data, rhs.data) == 0);
    }

    // 小于比较
    bool operator<(const MyString& rhs) const {
        return std::strcmp(data, rhs.data) < 0;
    }

    // 获取长度
    size_t size() const { return length; }

    // 获取容量
    size_t getCapacity() const { return capacity; }

    // 判空
    bool empty() const { return length == 0; }

    // 返回C风格字符串
    const char* c_str() const { return data; }

    // 预留空间
    void reserve(size_t new_capacity) {
        if (new_capacity > capacity) {
            char* new_data = new char[new_capacity];
            std::memcpy(new_data, data, length + 1);
            delete[] data;
            data = new_data;
            capacity = new_capacity;
        }
    }

    // 迭代器
    char* begin() { return data; }
    char* end() { return data + length; }
    const char* begin() const { return data; }
    const char* end() const { return data + length; }
};

// 测试
int main() {
    // 构造函数测试
    MyString s1;
    MyString s2("Hello");
    MyString s3(5, 'a');

    // 拷贝测试
    MyString s4 = s2;
    MyString s5;
    s5 = s3;

    // 移动测试
    MyString s6 = std::move(s4);
    std::cout << "s6: " << s6.c_str() << std::endl;
    std::cout << "s4 after move: " << s4.c_str() << std::endl;

    // 运算符测试
    s1 = s2 + s3;
    std::cout << "s1 (s2+s3): " << s1.c_str() << std::endl;

    if (s2 == MyString("Hello")) {
        std::cout << "s2 equals 'Hello'" << std::endl;
    }

    // 方法测试
    s1.append(" World");
    std::cout << "s1 after append: " << s1.c_str() << std::endl;

    MyString sub = s1.substr(5, 6);
    std::cout << "Substring: " << sub.c_str() << std::endl;

    return 0;
}
