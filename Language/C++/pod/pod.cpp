#include <iostream>

// pod: 没有使用面向对象方法设计class/struct, 它完全兼容C struct
// 可以使用: memcpy, memset

/*
默认构造和析构
默认拷贝构造和默认移动构造
默认赋值操作和移动赋值操作

所有非静态成员拥有相同的访问级别，(访问级别就是public，private，protected)
在类和结构体继承时需要满足以下两个情况之一
    派生类中有非静态的数据那么它的基类只能是只有静态的，而且基类只能有一个。
    如果基类有非静态的， 那么派生类就不能有非静态的。有种跷跷板的感觉，非静态的对面坐着的是静态，父子类就 是坐在跷跷板的两端这种对应关系。)
没有虚类和虚基类（与trival中重复）
所有非静态数据成员都符合标准布局的要求，这其实就是一个递归的定义。
*/
class pod
{
    pod() = default;                            // constructor
    pod(const pod& t) = default;            // copy constructor
    pod(pod&& t) = default;                 // move constructor
    ~pod() = default;                           // destructor
    pod& operator=(const pod& t) = default; // assignment operator
    pod& operator=(pod&& t) = default;      // move operator
public:
    int m_data;
    static int s_data;
};
int pod::s_data = 0;

class nonPodOfTrivial
{
public:
    nonPodOfTrivial() {}
};

class access : public pod
{
public:
    int m_dataPublicH1;
private:
    int m_dataH1;
};

class podBase
{
    podBase() = default;                            // constructor
    podBase(const podBase& t) = default;            // copy constructor
    podBase(podBase&& t) = default;                 // move constructor
    ~podBase() = default;                           // destructor
    podBase& operator=(const podBase& t) = default; // assignment operator
    podBase& operator=(podBase&& t) = default;      // move operator
public:
    int m_data;
};

class podAccess : podBase
{
public:
    static int m_dataAccess;
};

int podAccess::m_dataAccess = 0;


int main(void)
{
    std::cout << "pod, trivial: " << std::is_trivial<pod>::value << ",standard layout: " << std::is_standard_layout<pod>::value << std::endl;

    std::cout << "access, trivial: " << std::is_trivial<access>::value << ",standard layout: " << std::is_standard_layout<access>::value << std::endl;

    std::cout << "nonPodOfTrivial, trivial: " << std::is_trivial<nonPodOfTrivial>::value << ",standard layout: " << std::is_standard_layout<nonPodOfTrivial>::value << std::endl;

    std::cout << "podAccess, trivial: " << std::is_trivial<podAccess>::value << ",standard layout: " << std::is_standard_layout<podAccess>::value << std::endl;

    std::cout << "podAccess, pod: " << std::is_pod<podAccess>::value << std::endl;
    return 0;
}