#include "sort.h"
#include <random>

#define DEF_SIZE 10

int main() {
    Sort<int> sort(DEF_SIZE);
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(1, 60000);
    for (int index = 0; index < DEF_SIZE; ++index) {
        int mean = uniform_dist(e1);
        sort.Set(index, mean);
    }
    sort.Display();
    sort.Bublle();
    sort.Display();
    sort.UpSet();
    sort.Straight();
    sort.Display();
    sort.Display();
    sort.Quick();
    sort.Display();
    return 0;
}
