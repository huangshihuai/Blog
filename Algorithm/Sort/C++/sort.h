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
        // heap
        void Heap();
    private:
        void Quick(size_t low, size_t high);
        void AdjustMaxHeap(const size_t _index);
    private:
        T *m_array;
        size_t m_size;
};

template<typename T>
void Sort<T>::Heap() {
    // 大顶堆：每个节点的值都大于或等于其子节点的值，在堆排序算法中用于升序排列；
    // 小顶堆：每个节点的值都小于或等于其子节点的值，在堆排序算法中用于降序排列；
    /*
    算法步骤
        将待排序序列构建成一个堆 H[0……n-1]，根据（升序降序需求）选择大顶堆或小顶堆；
        把堆首（最大值）和堆尾互换；
        把堆的尺寸缩小 1，并调用 shift_down(0)，目的是把新的数组顶端数据调整到相应位置；
        重复步骤 2，直到堆的尺寸为 1。
    */
    // 堆排序的平均时间复杂度为 Ο(nlogn)。
    // 叶子节点: size / 2 -1
    for (auto lastChildNode = m_size / 2 - 1; lastChildNode > 0; --lastChildNode) {
        AdjustMaxHeap(lastChildNode);
    }
    AdjustMaxHeap(0);
    // sort
}

template<typename T>
void Sort<T>::AdjustMaxHeap(const size_t _index) {
    // left child node index: 2 * i + 1;
    // right child node index: 2 * i + 2;
    size_t index = _index;
    auto value = m_array[index];
    for (auto lni = 2 * index + 1; lni < m_size; lni = 2 * index + 1) {
        if (lni + 1 < m_size && m_array[lni + 1] > m_array[lni]) {
            ++lni;
        }
        if (m_array[lni] > value) {
            m_array[index] = m_array[lni];
            index = lni;
        } else {
            break;
        }
    }
    m_array[index] = value;
}

template<typename T>
void Sort<T>::Quick() {
    Quick(0, m_size - 1);
}

template<typename T>
void Sort<T>::Quick(size_t low, size_t high) {
    size_t index = low;
    size_t tempHigh = high;
    T val = m_array[low];
    while (low < high) {
        while (low < high && m_array[high] >= val) {
            --high;
        }
        if (low >= high) {
            break;
        }
        m_array[low] = m_array[high];
        while (low < high && m_array[low] <= val) {
            ++low;
        }
        if (low >= high) {
            break;
        }
        m_array[high] = m_array[low];
    }
    m_array[low] = val;
    if (index + 1 < low) {
        Quick(index, low - 1);
    }
    if (low + 1 < tempHigh) {
        Quick(low + 1, tempHigh);
    }
}

template<typename T>
void Sort<T>::Straight() {
    for (size_t index = 0; index < this->m_size; ++index) {
        size_t keep = index;
        for (size_t lindex = index + 1; lindex < this->m_size; ++lindex) {
            if (this->m_array[keep] > this->m_array[lindex]) {
                keep = lindex;
            }
        }
        if (keep != index) {
            T t = this->m_array[index];
            this->m_array[index] = this->m_array[keep];
            this->m_array[keep] = t;
        }
    }
}

template<typename T>
void Sort<T>::Bublle() {
    for (size_t index = 0; index < this->m_size; ++index) {
        for (size_t lindex = index + 1; lindex < this->m_size; ++lindex) {
            if (this->m_array[index] > this->m_array[lindex]) {
                T t = this->m_array[index];
                this->m_array[index] = this->m_array[lindex];
                this->m_array[lindex] = t;
            }
        }
    }
}

template<typename T>
void Sort<T>::UpSet() {
    std::random_device rd;
    std::default_random_engine re(rd());
    std::uniform_int_distribution<size_t> uniform_dist(0, this->m_size - 1);
    for (size_t index = 0; index < this->m_size; ++index) {
        size_t nindex = uniform_dist(re);
        T t = this->m_array[index];
        this->m_array[index] = this->m_array[nindex];
        this->m_array[nindex] = t;
    }
}

template<typename T>
size_t Sort<T>::Set(size_t index, T &t) {
    if (index > this->m_size) {
        return 1;
    }
    this->m_array[index] = t;
    return 0;
}

template<typename T>
void Sort<T>::Display() const {
    for (size_t index = 0; index < m_size; ++index) {
        std::cout << this->m_array[index] << "\t";
    }
    std::cout << std::endl;
}

template<typename T>
Sort<T>::Sort(size_t size) {
    this->m_size = size;
    this->m_array = new (std::nothrow) T[size];
}

template<typename T>
Sort<T>::~Sort() {
    this->m_size = 0;
    delete [] this->m_array;
}

#endif // ALGORITHM_LRU_SORT_C_PLUS_PLUS___H
