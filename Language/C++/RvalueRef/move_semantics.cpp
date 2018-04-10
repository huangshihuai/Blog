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
            print_has(++this->n_cstr);
        }
        HasPtrMem(const HasPtrMem &h) : data(new int(*h.data)) {
            print_has(++this->n_cptr);
        }
        HasPtrMem(HasPtrMem &&h) : data(h.data) {
            h.data = nullptr;
            print_has(++this->n_mvtr);
        }
        ~HasPtrMem() {
            delete data;
            print_has(++this->n_dstr);
        }
        int *data;
        static int n_cstr;
        static int n_dstr;
        static int n_cptr;
        static int n_mvtr;
};

int HasPtrMem::n_cstr = 0;
int HasPtrMem::n_dstr = 0;
int HasPtrMem::n_cptr = 0;
int HasPtrMem::n_mvtr = 0;


HasPtrMem getTemp() {
    HasPtrMem h;
    print_resource(h.data);
    return h;
}

void testRvalue(int data) {
    cout << "not Rvalue\n";
    print_resource(&data);
}

void testRvalue(double &&data) {
    cout << "testRvalue: " << data << endl;
}

int main() {
    testRvalue(10.00);
    int b = 10;
    testRvalue(b);
    HasPtrMem a = getTemp();
    print_resource(a.data);
}