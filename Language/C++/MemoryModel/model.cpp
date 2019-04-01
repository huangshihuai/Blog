/**
 * 多重继承 & 虚表
 * 虚为了解决继承覆盖问题
 */
#include <iostream>

// TODO: 多重继承虚表问题
class BaseOne {
public:
    virtual void a() { std::cout << "Base One func:" << __func__ << std::endl; }
    virtual void b() { std::cout << "Base One func:" << __func__ << std::endl; }
    virtual void c() { std::cout << "Base One func:" << __func__ << std::endl; }
private:
};

class BaseTwo {
public:
    virtual void a() { std::cout << "Base Two func:" << __func__ << std::endl; }
    virtual void b() { std::cout << "Base Two func:" << __func__ << std::endl; }
    virtual void c() { std::cout << "Base Two func:" << __func__ << std::endl; }
private:
};

class BaseThree {
public:
    virtual void a() { std::cout << "Base Three func:" << __func__ << std::endl; }
    virtual void b() { std::cout << "Base Three func:" << __func__ << std::endl; }
    virtual void c() { std::cout << "Base Three func:" << __func__ << std::endl; }
private:
};

// TODO: 虚继承问题
class BaseFour {
public:
    void a() { std::cout << "Base Four func:" << __func__ << std::endl; }
    void b() { std::cout << "Base Four func:" << __func__ << std::endl; }
    void c() { std::cout << "Base Four func:" << __func__ << std::endl; }
private:
};

class Device : public BaseOne, public BaseTwo, public BaseThree, public virtual BaseFour {
public:
    void a() { std::cout << "Device func:" << __func__ << std::endl; }
    void b() { std::cout << "Device func:" << __func__ << std::endl; }
    void c1() { std::cout << "Device func:" << __func__ << std::endl; }
private:
};

typedef void(*FunV)(void);

int main(void) {
    FunV fv = nullptr;
    Device obj;
    std::cout << "Device size: " << sizeof(obj) << std::endl;
    // TODO: 虚表
    std::cout << "Device address: " << (void*)(&obj) << std::endl;
    std::cout << "Device nu: " << (void*)(&obj) << std::endl;
    // TODO: 'class v_tb' -> '&v_tb'
    void *v_base_one = (int *)(*(int *)(&obj));
    fv = (FunV)(int *)(*(int *)(*(int *)(&obj)));
    fv();
    int **pvt = (int **)(&obj);
    fv = (FunV)pvt[0];
    fv();

    std::cout << "int size: " << sizeof(int) << std::endl;
    std::cout << "pointer size: " << sizeof(&obj) << std::endl;
    std::cout << "Base one size: " << sizeof(BaseOne) << std::endl;
    std::cout << "Base two size: " << sizeof(BaseTwo) << std::endl;
    std::cout << "Base three size: " << sizeof(BaseThree) << std::endl;
    std::cout << "Base four size: " << sizeof(BaseFour) << std::endl;
    return 0;
}