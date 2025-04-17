#include <iostream>
#include<string>
using namespace std;

template <typename T>
T sumArray(T arr[], int size) {
    T sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}
template<>
string sumArray(string arr[],int size){
    string sum="";
    for(int i=0;i<size;i++){
        sum+=arr[i];
    }
    return sum;
}

int main() {
    int intArray[] = {1, 2, 3, 4, 5};
    cout << "Sum of int array: " << sumArray(intArray, 5) << endl;

    string strArray[] = {"Hello", " ", "World"};
    cout << "Sum of string array: " << sumArray(strArray, 3) << endl;

    return 0;
}