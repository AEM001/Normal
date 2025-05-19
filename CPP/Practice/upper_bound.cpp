#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 新增：打印vector的辅助函数
void printVector(const vector<int>& v) {
    for (int num : v) cout << num << " ";
    cout << endl;
}

int main() {
    vector<int> nums = {10, 20, 30, 30, 40, 50, 60};
    
    // 新增：确保vector是有序的
    sort(nums.begin(), nums.end());
    cout << "排序后的数组: ";
    printVector(nums);

    vector<int> targets = {5, 20, 30, 35, 60, 65};
    
    // 练习1：基础用法
    cout << "\n练习1：基本upper_bound查找" << endl;
    for (int target : targets) {
        auto it = upper_bound(nums.begin(), nums.end(), target);
        
        cout << "查找 " << target << ": ";
        if (it != nums.end()) {
            cout << "找到 " << *it << " (位置 " << (it - nums.begin()) << ")";
        } else {
            cout << "未找到";
        }
        cout << endl;
    }

    // 新增练习2：统计小于等于target的元素数量
    cout << "\n练习2：统计 ≤ target 的元素数量" << endl;
    for (int target : targets) {
        auto it = upper_bound(nums.begin(), nums.end(), target);
        int count = it - nums.begin();
        cout << "≤ " << target << " 的元素有 " << count << " 个" << endl;
    }

    // 新增练习3：自定义比较函数（查找第一个 > target*2 的元素）
    cout << "\n练习3：使用自定义比较" << endl;
    for (int target : targets) {
        auto it = upper_bound(nums.begin(), nums.end(), target, 
            [](int a, int b) { return a < b; });
        // ... 可以添加自定义比较逻辑的输出
        cout<<"search for "<<target<<"*2:"<<*it<<endl;
    }

    return 0;
}

