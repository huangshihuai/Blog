#include <iostream>
#include "lru.h"

int main()
{
    LRUCache<int, int> cc(10);
    cc.put(1, 10);
    cc.put(2, 20);
    cc.put(3, 30);
    cc.put(4, 40);
    std::cout << *cc.get(1) << std::endl;
    cc.put(5, 50);
    cc.put(4, 44);
    std::cout << *cc.get(3) << std::endl;
    std::cout << *cc.get(2) << std::endl;
    std::cout << std::endl;
    cc.display();
    return 0;
}
