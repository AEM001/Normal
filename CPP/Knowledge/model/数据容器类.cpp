#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <type_traits>
 
// 主模板（处理数值类型）
template <typename T>
class DataContainer {
private:
    std::vector<T> data;
 
public:
    void add(const T& element) {
        data.push_back(element); 
    }
 
    void statistics() const {
        if constexpr (std::is_arithmetic_v<T>) {
            if (data.empty())  {
                std::cout << "[数值统计] 无有效数据\n";
                return;
            }
 
            T max_val = *std::max_element(data.begin(),  data.end()); 
            double sum = 0;
            for (const auto& num : data) sum += num;
            
            std::cout << "[数值统计] 平均值: " << (sum / data.size()) 
                      << " | 最大值: " << max_val << "\n";
        } else {
            std::cout << "[警告] 该类型不支持数值统计\n";
        }
    }
};
 
// 字符串全特化 
template <>
class DataContainer<std::string> {
private:
    std::vector<std::string> data;
 
public:
    void add(const std::string& element) {
        data.push_back(element); 
    }
 
    void statistics() const {
        if (data.empty())  {
            std::cout << "[字符串统计] 无有效数据\n";
            return;
        }
 
        // 最长字符串统计 
        size_t max_len = 0;
        for (const auto& str : data) {
            max_len = std::max(max_len, str.length()); 
        }
        
        // 字符频率统计 
        std::unordered_map<char, int> char_freq;
        for (const auto& str : data) {
            for (char c : str) char_freq[c]++;
        }
 
        auto max_pair = *std::max_element(
            char_freq.begin(),  char_freq.end(), 
            [](auto& a, auto& b) { return a.second  < b.second;  });
 
        std::cout << "[字符串统计] 最长长度: " << max_len 
                  << " | 高频字符: '" << max_pair.first  
                  << "' (出现次数: " << max_pair.second  << ")\n";
    }
};
 
// 指针偏特化 
template <typename T>
class DataContainer<T*> {
private:
    std::vector<T*> data;
 
public:
    void add(T* element) {
        data.push_back(element); 
    }
 
    void statistics() const {
        int valid_count = std::count_if(
            data.begin(),  data.end(), 
            [](T* ptr) { return ptr != nullptr; });
        
        std::cout << "[指针统计] 有效指针数量: " 
                  << valid_count << "/" << data.size()  << "\n";
    }
};


// 测试函数 
void test() {
    // 测试整型容器 
    DataContainer<int> int_container;
    int_container.add(85); 
    int_container.add(92); 
    int_container.add(78); 
    std::cout << "\n=== 整型测试 ===" << std::endl;
    int_container.statistics(); 
 
    // 测试浮点型容器 
    DataContainer<double> double_container;
    double_container.add(3.14); 
    double_container.add(2.718); 
    double_container.add(1.618); 
    std::cout << "\n=== 浮点型测试 ===" << std::endl;
    double_container.statistics(); 
 
    // 测试字符串容器 
    DataContainer<std::string> str_container;
    str_container.add("template"); 
    str_container.add("programming"); 
    str_container.add("C++"); 
    std::cout << "\n=== 字符串测试 ===" << std::endl;
    str_container.statistics(); 
 
    // 测试指针容器 
    int x = 10, y = 20;
    DataContainer<int*> ptr_container;
    ptr_container.add(&x); 
    ptr_container.add(nullptr); 
    ptr_container.add(&y); 
    std::cout << "\n=== 指针测试 ===" << std::endl;
    ptr_container.statistics(); 
}
 
int main() {
    test();
    return 0;
}