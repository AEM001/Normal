#pragma once
#include <iostream>
#include <cmath>  
using namespace std;

template <typename T> class List;
template <typename T> class Node
{
public:
	T info;                        //节点数据，为便于操作方便，节点数据定义为公有
	Node<T>* link;                 //节点指针

	Node() { link = 0; }           //创建空节点
	Node(T data)                   //创建节点并初始化
	{
		info = data; link = 0;
	}

	void insertAfter(Node<T>* p)   //将参数节点接在本节点后
	{
		p->link = link;
		link = p;
	}

	Node<T>* removeAfter()        //删除本节点的后节点，返回删除节点地址，若为空，则表示无法删除
	{
		Node<T>* after = link;    //指向本节点的后节点
		if (after)
			link = after->link;
		return after;
	}

	friend class List<T>;
};

template <typename T>
class List
{
	Node<T>* head, * tail;
public:
	List()                        //构造空链表，无链头空节点
	{
		//补充代码
		head = tail = 0;
	}

	~List()                       //析构链表 
	{
		Node<T>* p;
		while (head)
		{
			p = head;
			head = head->link;
			delete p;
		}
	}
	    
	bool isEmpty() { return head == 0 && tail == 0; }                //判断是否空链表
	
	friend ostream& operator << (ostream& os, const List<T>& list)   //重载运算符用于输出链表
	{
		if (list.head == 0 && list.tail == 0) os << "空链表！" << endl;
		else
		{
			//补充代码
			Node<T>* p = list.head;
			while (p)
			{
				os << p->info;
				p = p->link;
			}
		}
		return os;
	}

	//添加声明：将数据节点插在链尾
	void insertRear(const T& data);

	//添加声明：数据节点插入升序链表合适位置，保持链表升序
	void insertSort(const T& data);

	//添加声明: 在链表中查找数据节点，返回节点指针
	Node<T>* find(const T& data);

	//添加声明：删除链表中给定数据节点（给出待删除数据），返回被删除节点指针
	Node<T>* del(const T& data);
	
	//添加声明：获取给定节点的前节点	
	Node<T>* findFront(Node<T>* p);
};

//添加代码：补充以上函数声明的定义
template <typename T>
void List<T>::insertRear(const T& data)
{
	Node<T>* newNode = new Node<T>(data);
	if (isEmpty())
	{
		head = tail = newNode;
	}
	else
	{
		tail->link = newNode;
		tail = newNode;
	}
}

template <typename T>
void List<T>::insertSort(const T& data)
{
	Node<T>* newNode = new Node<T>(data);
	
	// 如果链表为空或新节点应该在头部
	if (isEmpty() || !(head->info < data))
	{
		newNode->link = head;
		head = newNode;
		if (tail == 0) tail = newNode;
		return;
	}
	
	// 查找插入位置
	Node<T>* p = head;
	while (p->link && p->link->info < data)
	{
		p = p->link;
	}
	
	// 插入新节点
	newNode->link = p->link;
	p->link = newNode;
	
	// 如果插入在尾部，更新tail
	if (newNode->link == 0)
		tail = newNode;
}

template <typename T>
Node<T>* List<T>::find(const T& data)
{
	Node<T>* p = head;
	while (p)
	{
		if (p->info == data)
			return p;
		p = p->link;
	}
	return 0;
}

template <typename T>
Node<T>* List<T>::del(const T& data)
{
	if (isEmpty())
		return 0;
	
	Node<T>* p = head;
	Node<T>* prev = 0;
	
	// 如果要删除的是头节点
	if (head->info == data)
	{
		head = head->link;
		if (head == 0) tail = 0; // 如果删除后链表为空
		return p;
	}
	
	// 查找要删除的节点
	while (p && !(p->info == data))
	{
		prev = p;
		p = p->link;
	}
	
	if (p == 0) return 0; // 未找到节点
	
	// 删除节点
	prev->link = p->link;
	
	// 如果删除的是尾节点，更新tail
	if (p == tail)
		tail = prev;
	
	return p;
}

template <typename T>
Node<T>* List<T>::findFront(Node<T>* p)
{
	if (p == 0 || p == head)
		return 0;
	
	Node<T>* q = head;
	while (q && q->link != p)
		q = q->link;
	
	return q;
}


