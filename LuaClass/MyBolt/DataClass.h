#ifndef _DATA_CLASS_H_
#define _DATA_CLASS_H_

class Base
{
public:
	virtual void Click()
	{
		::MessageBoxA(0, "基类Base::Click()的实现被调用", "警告", 0);
	}
	virtual ~Base(){}
};

class Derived1 : public Base
{
public:
	virtual void Click()
	{
		::MessageBoxA(0, "子类Derived1对基类方法Click()的实现被调用", "警告", 0);
	}
	void Click1()
	{
		::MessageBoxA(0, "子类Derived1的方法Click1()的实现被调用", "警告", 0);
	}
};

class Derived2 : public Base
{
public:
	virtual void Click()
	{
		::MessageBoxA(0, "子类Derived1对基类方法Click()的实现被调用", "警告", 0);
	}
	void Click2()
	{
		::MessageBoxA(0, "子类Derived2的方法Click2()的实现被调用", "警告", 0);
	}
};

#endif