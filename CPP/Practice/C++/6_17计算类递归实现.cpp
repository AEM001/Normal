#include <iostream>
#include <vector>
#include <stdexcept>
//妙：递归，每个运算由上一级的evaluate展开计算，自己又返回evaluate的值进行下一级后续计算
// 抽象表达式基类
class Expression {
public:
    // 纯虚函数：计算表达式的值
    virtual double evaluate() const = 0;

    // 虚析构函数：确保派生类对象能正确释放内存
    virtual ~Expression() {}
};

// 数值表达式类
class NumberExpression : public Expression {
private:
    double value;
    
public:
    NumberExpression(double val) : value(val) {}
    
    // 重写evaluate函数，返回数值
    double evaluate() const override {
        return value;
    }
};

// 二元运算表达式基类
class BinaryExpression : public Expression {
protected:
    Expression* left;
    Expression* right;
    
public:
    BinaryExpression(Expression* l, Expression* r) : left(l), right(r) {}
    
    // 析构函数：递归释放左右子表达式的内存
    virtual ~BinaryExpression() {
        delete left;   // 会递归调用子表达式的析构函数
        delete right;  // 会递归调用子表达式的析构函数
    }
};

// 加法表达式
class AddExpression : public BinaryExpression {
public:
    AddExpression(Expression* l, Expression* r) : BinaryExpression(l, r) {}
    
    // 重写evaluate函数：递归计算左右子表达式并相加
    double evaluate() const override {
        return left->evaluate() + right->evaluate();
    }
};

// 乘法表达式  
class MultiplyExpression : public BinaryExpression {
public:
    MultiplyExpression(Expression* l, Expression* r) : BinaryExpression(l, r) {}
    
    // 重写evaluate函数：递归计算左右子表达式并相乘
    double evaluate() const override {
        return left->evaluate() * right->evaluate();
    }
};

// 除法表达式
class DivideExpression : public BinaryExpression {
public:
    DivideExpression(Expression* l, Expression* r) : BinaryExpression(l, r) {}
    
    // 重写evaluate函数：递归计算左右子表达式并相除，处理除零异常
    double evaluate() const override {
        double leftVal = left->evaluate();
        double rightVal = right->evaluate();
        
        if (rightVal == 0.0) {
            throw std::runtime_error("Division by zero");
        }
        
        return leftVal / rightVal;
    }
};

// 表达式管理器类
class ExpressionManager {
private:
    std::vector<Expression*> expressions;
    
public:
    // 添加表达式到管理器
    void addExpression(Expression* expr) {
        expressions.push_back(expr);
    }
    
    // 计算所有表达式并输出结果
    void evaluateAll() {
        for (size_t i = 0; i < expressions.size(); ++i) {
            try {
                // 多态调用：根据实际对象类型调用对应的evaluate函数
                double result = expressions[i]->evaluate();
                std::cout << "Expression " << i << ": " << result << std::endl;
            } catch (const std::exception& e) {
                std::cout << "Expression " << i << " error: " << e.what() << std::endl;
            }
        }
    }
    
    // 析构函数：释放所有表达式的内存
    ~ExpressionManager() {
        for (Expression* expr : expressions) {
            delete expr;  // 多态析构：会调用实际对象类型的析构函数
        }
    }
    
};

int main() {
    ExpressionManager manager;
    
    // 创建表达式: (3 + 4) * 2
    Expression* expr1 = new MultiplyExpression(
        new AddExpression(new NumberExpression(3), new NumberExpression(4)),
        new NumberExpression(2)
    );
    
    // 创建表达式: 10 / (5 - 3)  
    Expression* expr2 = new DivideExpression(
        new NumberExpression(10),
        new AddExpression(new NumberExpression(5), new NumberExpression(-3))
    );
    
    // 创建表达式: 8 / 0 (测试除零异常)
    Expression* expr3 = new DivideExpression(
        new NumberExpression(8),
        new NumberExpression(0)
    );
    
    manager.addExpression(expr1);
    manager.addExpression(expr2);  
    manager.addExpression(expr3);
    
    manager.evaluateAll();
    
    return 0;
}

/* 
预期输出:
Expression 0: 14
Expression 1: 5
Expression 2 error: Division by zero

关键考点:
1. 虚析构函数的重要性
2. 递归的内存释放
3. 多态函数调用
4. 异常处理
5. 深度嵌套指针的正确管理
*/