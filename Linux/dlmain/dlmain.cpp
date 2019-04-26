#include <iostream>
#include <string>
#include <dlfcn.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        return 1;
    }
    int (*dlmain)(int, const char **);
    void *handle = dlopen(argv[1], RTLD_LAZY);
    if (!handle) {
        std::cout << "dlopen error: " << dlerror() << std::endl;
        return 1;
    }
    do {
        dlmain = (int (*)(int, const char **))dlsym(handle, "main");
        if (!dlmain) {
            std::cout << "not found function main\n";
            break;
        }
        const char *ar[3] = {"main", "10", "20"};
        std::cout << "result: " << dlmain(3, ar) << std::endl;
    } while(false);
    dlclose(handle);
    return 0;
}
