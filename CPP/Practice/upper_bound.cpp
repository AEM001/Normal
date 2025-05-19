#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ��������ӡvector�ĸ�������
void printVector(const vector<int>& v) {
    for (int num : v) cout << num << " ";
    cout << endl;
}

int main() {
    vector<int> nums = {10, 20, 30, 30, 40, 50, 60};
    
    // ������ȷ��vector�������
    sort(nums.begin(), nums.end());
    cout << "����������: ";
    printVector(nums);

    vector<int> targets = {5, 20, 30, 35, 60, 65};
    
    // ��ϰ1�������÷�
    cout << "\n��ϰ1������upper_bound����" << endl;
    for (int target : targets) {
        auto it = upper_bound(nums.begin(), nums.end(), target);
        
        cout << "���� " << target << ": ";
        if (it != nums.end()) {
            cout << "�ҵ� " << *it << " (λ�� " << (it - nums.begin()) << ")";
        } else {
            cout << "δ�ҵ�";
        }
        cout << endl;
    }

    // ������ϰ2��ͳ��С�ڵ���target��Ԫ������
    cout << "\n��ϰ2��ͳ�� �� target ��Ԫ������" << endl;
    for (int target : targets) {
        auto it = upper_bound(nums.begin(), nums.end(), target);
        int count = it - nums.begin();
        cout << "�� " << target << " ��Ԫ���� " << count << " ��" << endl;
    }

    // ������ϰ3���Զ���ȽϺ��������ҵ�һ�� > target*2 ��Ԫ�أ�
    cout << "\n��ϰ3��ʹ���Զ���Ƚ�" << endl;
    for (int target : targets) {
        auto it = upper_bound(nums.begin(), nums.end(), target, 
            [](int a, int b) { return a < b; });
        // ... ��������Զ���Ƚ��߼������
        cout<<"search for "<<target<<"*2:"<<*it<<endl;
    }

    return 0;
}

