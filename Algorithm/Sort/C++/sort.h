#ifndef ALGORITHM_LRU_SORT_C_PLUS_PLUS___H
#define ALGORITHM_LRU_SORT_C_PLUS_PLUS___H
#include <array>
#include <iostream>
#include <random>

// TODO overload operator(> <)
template<typename T>
class Sort {
    public:
        Sort(size_t size);
        ~Sort();
        size_t Set(size_t index, T &t);
        void UpSet();
        void Display() const;
        // TODO 冒泡 n * n
        void Bublle();
        // TODO 直接排序 n * n
        void Straight();
        // TODO 快速排序 n * log(n) - n * n
        void Quick();
        // 希尔排序

        // 
    private:
        void Quick(size_t low, size_t high);
    private:
        T *_t;
        size_t _size;
};


template<typename T>
void Sort<T>::Quick() {
    Quick(0, _size - 1);
}

template<typename T>
void Sort<T>::Quick(size_t low, size_t high) {
    size_t index = low;
    size_t tempHigh = high;
    T val = _t[low];
    while (low < high) {
        while (low < high && _t[high] >= val) {
            --high;
        }
        if (low >= high) {
            break;
        }
        _t[low] = _t[high];
        while (low < high && _t[low] <= val) {
            ++low;
        }
        if (low >= high) {
            break;
        }
        _t[high] = _t[low];
    }
    _t[low] = val;
    if (index + 1 < low) {
        Quick(index, low - 1);
    }
    if (low + 1 < tempHigh) {
        Quick(low + 1, tempHigh);
    }
}

template<typename T>
void Sort<T>::Straight() {
    for (size_t index = 0; index < this->_size; ++index) {
        size_t keep = index;
        for (size_t lindex = index + 1; lindex < this->_size; ++lindex) {
            if (this->_t[keep] > this->_t[lindex]) {
                keep = lindex;
            }
        }
        if (keep != index) {
            T t = this->_t[index];
            this->_t[index] = this->_t[keep];
            this->_t[keep] = t;
        }
    }
}

template<typename T>
void Sort<T>::Bublle() {
    for (size_t index = 0; index < this->_size; ++index) {
        for (size_t lindex = index + 1; lindex < this->_size; ++lindex) {
            if (this->_t[index] > this->_t[lindex]) {
                T t = this->_t[index];
                this->_t[index] = this->_t[lindex];
                this->_t[lindex] = t;
            }
        }
    }
}

template<typename T>
void Sort<T>::UpSet() {
    std::random_device rd;
    std::default_random_engine re(rd());
    std::uniform_int_distribution<size_t> uniform_dist(0, this->_size - 1);
    for (size_t index = 0; index < this->_size; ++index) {
        size_t nindex = uniform_dist(re);
        T t = this->_t[index];
        this->_t[index] = this->_t[nindex];
        this->_t[nindex] = t;
    }
}

template<typename T>
size_t Sort<T>::Set(size_t index, T &t) {
    if (index > this->_size) {
        return 1;
    }
    this->_t[index] = t;
    return 0;
}

template<typename T>
void Sort<T>::Display() const {
    for (size_t index = 0; index < _size; ++index) {
        std::cout << this->_t[index] << "\t";
    }
    std::cout << std::endl;
}

template<typename T>
Sort<T>::Sort(size_t size) {
    this->_size = size;
    this->_t = new (std::nothrow) T[size];
}

template<typename T>
Sort<T>::~Sort() {
    this->_size = 0;
    delete [] this->_t;
}

#endif // ALGORITHM_LRU_SORT_C_PLUS_PLUS___H
