#include <iostream>

using namespace std;

#define print_resource(v) \
    do { \
        std::cout << "Resource form " << __func__ << ": " << hex << (v) << endl; \
    } while(false)

#define print_address(v) \
    do { \
        std::cout << "this resource from " << __func__ << " address: " << hex << (v) << endl; \
    } while(false)

class HasPtrMem {
#define print_has(v) \
    do { \
        std::cout << __func__ << ": " << (v) << endl; \
    } while(false)
    public:
        HasPtrMem() : data(new int(3)) {
            std::cout << "constructor\n";
            print_has(this);
        }
        HasPtrMem(const HasPtrMem& h) : data(new int(*h.data)) {
            std::cout << "copy constructor\n";
            print_has(this);
        }

        HasPtrMem(HasPtrMem&& h) : data(h.data) {
            std::cout << "move constructor\n";
            h.data = nullptr;
            print_has(this);
        }

        HasPtrMem& operator=(const HasPtrMem& h) {
            std::cout << "assignment operator\n";
            if (this == &h) {
                return *this;
            }
            this->data = new int(*h.data);
            return *this;
        }

        HasPtrMem& operator=(HasPtrMem&& h) {
            std::cout << "move operator\n";
            if (this == &h) {
                return *this;
            }
            this->data = h.data;
            h.data = nullptr;
            return *this;
        }
        ~HasPtrMem() {
            if (data) {
                delete data;
            }
            print_has(this);
        }
        int *data;
};


HasPtrMem getTemp() {
    HasPtrMem h;
    HasPtrMem h1;
    HasPtrMem h2;
    print_resource(&h1);
    return h2;
}

void testRvalue(int data) {
    cout << "not Rvalue\n";
    print_resource(&data);
}

void testRvalue(double &&data) {
    cout << "testRvalue: " << data << endl;
}

int main() {
    std::cout << std::is_trivial<HasPtrMem>::value << std::endl;
    testRvalue(10.00);
    int b = 10;
    testRvalue(b);
    HasPtrMem a = getTemp();
    print_resource(&a);
    return b;
}