#ifndef _DATA_CLASS_H_
#define _DATA_CLASS_H_

class Base
{
public:
	virtual void Click()
	{
		::MessageBoxA(0, "����Base::Click()��ʵ�ֱ�����", "����", 0);
	}
	virtual ~Base(){}
};

class Derived1 : public Base
{
public:
	virtual void Click()
	{
		::MessageBoxA(0, "����Derived1�Ի��෽��Click()��ʵ�ֱ�����", "����", 0);
	}
	void Click1()
	{
		::MessageBoxA(0, "����Derived1�ķ���Click1()��ʵ�ֱ�����", "����", 0);
	}
};

class Derived2 : public Base
{
public:
	virtual void Click()
	{
		::MessageBoxA(0, "����Derived1�Ի��෽��Click()��ʵ�ֱ�����", "����", 0);
	}
	void Click2()
	{
		::MessageBoxA(0, "����Derived2�ķ���Click2()��ʵ�ֱ�����", "����", 0);
	}
};

#endif