#include <unordered_map>
#include <list>
#include <iostream>
#include <cstdlib>

template<typename TK, typename TV>
class LRUCache {
    public:
        LRUCache(size_t size);
        const TV *get(const TK &);
        size_t put(const TK &, const TV &);
        void display() const;
    private:
        size_t _size;
        std::list<TV> _eq;
        std::unordered_map<TK, typename std::list<TV>::iterator> _kv;
};


template<typename TK, typename TV>
LRUCache<TK, TV>::LRUCache(size_t size) {
    this->_size = size;
}

template<typename TK, typename TV>
void LRUCache<TK, TV>::display() const {
    for (auto it : this->_eq) {
        std::cout << it << "\t";
    }
    std::cout << std::endl;
}

template<typename TK, typename TV>
const TV *LRUCache<TK, TV>::get(const TK & k) {
    const auto iter = this->_kv.find(k);
    if (iter == this->_kv.end()) {
        return nullptr;
    }
    const TV v = *(iter->second);
    this->_eq.erase(iter->second);
    this->_eq.push_front(v);
    this->_kv[k] = this->_eq.begin();
    return &(*this->_kv[k]);
}

template<typename TK, typename TV>
size_t LRUCache<TK, TV>::put(const TK & k, const TV & v) {
    const auto iter = this->_kv.find(k);
    if (iter == this->_kv.end()) {
        if (this->_eq.size() >= this->_size) {
            this->_eq.pop_back();
        }
    } else {
        this->_eq.erase(this->_kv[k]);
    }
    // update
    this->_eq.push_front(v);
    this->_kv[k] = this->_eq.begin();
    return 0;
}
