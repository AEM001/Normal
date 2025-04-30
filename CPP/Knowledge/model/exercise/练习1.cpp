#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>
#include <iomanip>

using namespace std;

template<typename T, typename Container>
class DataAnalyzer {
private:
    Container& data;  // 使用模板参数Container代替vector<T>
    
public:
    DataAnalyzer(Container& data): data(data) {}

    T getMax() const {
        if(data.empty()) throw logic_error("Empty container");
        auto it = data.begin();
        T max = *it++;
        for(; it != data.end(); ++it) {
            if(*it > max) max = *it;
        }
        return max;
    }
    
    double getAverage() const {
        if(data.empty()) throw logic_error("Empty container");
        double sum = 0;
        for(auto it = data.begin(); it != data.end(); ++it) {
            sum += *it;
        }
        return sum / data.size();
    }
    
    int countGreaterThan(const T& threshold) const {
        int count = 0;
        for(auto it = data.begin(); it != data.end(); ++it) {
            if(*it > threshold) count++;
        }
        return count;
    }
    
    void mergeData(const Container& newData) {
        for(auto it = newData.begin(); it != newData.end(); ++it) {
            data.push_back(*it);
        }
    }
    
    void printStatistics() const {
        cout << fixed << setprecision(2);
        cout << "Max: " << getMax() << endl;
        cout << "Average: " << getAverage() << endl;
        cout << "Count greater than average: " << countGreaterThan(getAverage()) << endl;
    }
};

int main() {
    // 测试用例1：整数vector
    vector<int> intVec{5, 2, 8, 3};
    DataAnalyzer<int, vector<int>> analyzer1(intVec);
    
    cout << "Integer Vector Test:" << endl;
    analyzer1.printStatistics();
    
    vector<int> newIntVec{10, 4};
    analyzer1.mergeData(newIntVec);
    cout << "\nAfter merging new data:" << endl;
    analyzer1.printStatistics();
    
    // 测试用例2：浮点数list
    list<double> doubleList{1.5, 2.5, 3.5};
    DataAnalyzer<double, list<double>> analyzer2(doubleList);
    
    cout << "\nDouble List Test:" << endl;
    analyzer2.printStatistics();
    
    list<double> newDoubleList{4.5, 1.0};
    analyzer2.mergeData(newDoubleList);
    cout << "\nAfter merging new data:" << endl;
    analyzer2.printStatistics();
    
    return 0;
}
