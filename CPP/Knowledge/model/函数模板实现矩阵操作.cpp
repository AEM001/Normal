#include<iostream>
using namespace std;

template<typename T>
T getmatrix(T **matrix, int row, int col, int i, int j) {
    return matrix[i][j];
}

template<typename T>
T **creatematrix(int row, int col) {
    T **matrix = new T *[row];
    for (int i = 0; i < row; i++) {
        matrix[i] = new T[col];
    }
    return matrix;
}
template<typename T>
void deletematrix(T **matrix, int row, int col) {
    for (int i = 0; i < row; i++) {
        delete[] matrix[i]; 
    }  
}
template<typename T>
void inputmatrix(T **matrix, int row, int col) {
    cout << "请输入矩阵元素 (" << row << "x" << col << "):" << endl;
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            cin >> matrix[i][j];
        }
    }
}

template<typename T>
void printmatrix(T **matrix, int row, int col) {
    cout << "矩阵内容 (" << row << "x" << col << "):" << endl;
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

template<typename T>
T **transmatrix(T **matrix, int row, int col) {
    T **matrix2 = creatematrix<T>(col, row);
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            matrix2[j][i] = matrix[i][j];
        }
    }
    return matrix2;  // 添加返回值
}

// 示例用法
int main() {
    int row = 3, col = 3;
    
    // 创建矩阵
    int **matrix = creatematrix<int>(row, col);
    
    // 输入矩阵
    inputmatrix(matrix, row, col);
    
    // 输出原始矩阵
    printmatrix(matrix, row, col);
    
    // 转置矩阵
    int **transposed = transmatrix(matrix, row, col);
    
    // 输出转置后的矩阵
    printmatrix(transposed, col, row);
    
    // 释放内存
    deletematrix(matrix, row, col);
    deletematrix(transposed, col, row);
    
    return 0;
}