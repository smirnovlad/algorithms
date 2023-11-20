/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/347382/problem/G
 */

#include <iostream>
#include <vector>
#include <cmath>

/*
 * Можно реализовать структур с использованием 4 куч (min, max + куча
 * с удалёнными элементами для каждой из них)
 */

class MinMaxHeap{
private:
    std::vector<long long> heap;
    size_t nextIdx;
protected:
    void siftUp(unsigned int);
    void siftUpMin(unsigned int);
    void siftUpMax(unsigned int);
    void siftDown(unsigned int);
    void siftDownMin(unsigned int);
    void siftDownMax(unsigned int);
    static bool isEvenLevel(unsigned int node) {
        unsigned int level = int(log2(node));
        if (level % 2 == 0) return true;
        else return false;
    }
    static unsigned int getMin() { return 1; }
    unsigned int getMax()const;
    unsigned int getIndexOfSmallestChildOrGrandChild(unsigned int)const;
    unsigned int getIndexOfBiggestChildOrGrandChild(unsigned int)const;
public:
    MinMaxHeap() : heap({-1}), nextIdx(1) { }
    explicit MinMaxHeap(std::vector<long long> array) : nextIdx(int(array.size()) + 1) {
        heap.resize(nextIdx);
        std::copy(array.begin(), array.end(), heap.begin() + 1);
        heapify();
    }
    void extractMin();
    void extractMax();
    void insert(long long);
    void heapify();
    void show()const { for (int i = 1; i < nextIdx; ++i) std::cout << heap[i] << ' '; std::cout << std::endl;}
};

void MinMaxHeap::extractMin() {
    std::cout << heap[getMin()] << std::endl;
    heap[1] = heap[nextIdx - 1];
    --nextIdx;
    if (nextIdx != 1) siftDown(1);
}

unsigned int MinMaxHeap::getMax()const {
    if (nextIdx == 2) return 1;
    else if (nextIdx == 3) return 2;
    else return (heap[2] > heap[3]) ? 2 : 3;
}

void MinMaxHeap::extractMax() {
    unsigned int maxIdx = getMax();
    std::cout << heap[maxIdx] << std::endl;
    heap[maxIdx] = heap[nextIdx - 1];
    --nextIdx;
    if (maxIdx < nextIdx) siftDown(maxIdx);
}

unsigned int MinMaxHeap::getIndexOfSmallestChildOrGrandChild(unsigned int node)const {
    unsigned int minIndex = node * 2;
    long long minValue = heap[minIndex];
    std::vector<unsigned int> indicesOfSmallestPossibleChildOrGrandchild ={ (node * 2) * 2, (node * 2) * 2 + 1, node * 2 + 1,(node * 2 + 1) * 2, (node * 2 + 1) * 2 + 1};
    for (int i = 0; i < 5; ++i) {
        unsigned int curNode = indicesOfSmallestPossibleChildOrGrandchild[i];
        if (curNode < nextIdx) {
            if (heap[curNode] < minValue) {
                minIndex = curNode;
                minValue = heap[curNode];
            }
        }
    }
    return minIndex;
}


void MinMaxHeap::siftDownMin(unsigned int node) {
    if (2 * node >= nextIdx) return;
    else {
        unsigned int indexOfSmallest = getIndexOfSmallestChildOrGrandChild(node);
        if (heap[indexOfSmallest] >= heap[node]) return;
        else {
            std::swap(heap[indexOfSmallest] , heap[node]);
            if (((indexOfSmallest / 2) / 2) == node) {
                if (heap[indexOfSmallest] > heap[indexOfSmallest / 2]) {
                    std::swap(heap[indexOfSmallest], heap[indexOfSmallest / 2]);
                }
                siftDownMin(indexOfSmallest);
            }
        }
    }
}

unsigned int MinMaxHeap::getIndexOfBiggestChildOrGrandChild(unsigned int node)const {
    unsigned int maxIndex = node * 2;
    long long maxValue = heap[maxIndex];
    std::vector<unsigned int> indicesOfSmallestPossibleChildOrGrandchild ={ (node * 2) * 2, (node * 2) * 2 + 1, node * 2 + 1,(node * 2 + 1) * 2, (node * 2 + 1) * 2 + 1};
    for (int i = 0; i < 5; ++i) {
        unsigned int curNode = indicesOfSmallestPossibleChildOrGrandchild[i];
        if (curNode < nextIdx) {
            if (heap[curNode] > maxValue) {
                maxIndex = curNode;
                maxValue = heap[curNode];
            }
        }
    }
    return maxIndex;
}

void MinMaxHeap::siftDownMax(unsigned int node) {
    if (2 * node >= nextIdx) return;
    else {
        unsigned int indexOfBiggest = getIndexOfBiggestChildOrGrandChild(node);
        if (heap[indexOfBiggest] <= heap[node]) return;
        else {
            std::swap(heap[indexOfBiggest] , heap[node]);
            if (((indexOfBiggest / 2) / 2) == node) {
                if (heap[indexOfBiggest] < heap[indexOfBiggest / 2]) {
                    std::swap(heap[indexOfBiggest], heap[indexOfBiggest / 2]);
                }
                siftDownMax(indexOfBiggest);
            }
        }
    }
}

void MinMaxHeap::siftDown(unsigned int node) {
    if (isEvenLevel(node)) {
        siftDownMin(node);
    }
    else {
        siftDownMax(node);
    }
}

void MinMaxHeap::heapify() {
    size_t size = int(heap.size());
    for (int i = int(size / 2); i >= 1;--i) {
        siftDown(i);
    }
}

void MinMaxHeap::siftUpMax(unsigned int node) {
    unsigned int grandPar = (node / 2) / 2;
    if (grandPar < 1) return;
    else {
        if (heap[node] > heap[grandPar]) {
            std::swap(heap[node], heap[grandPar]);
            siftUpMax(grandPar);
        }
        else return;
    }
}

void MinMaxHeap::siftUpMin(unsigned int node) {
    unsigned int grandPar = (node / 2) / 2;
    if (grandPar < 1) return;
    else {
        if (heap[node] < heap[grandPar]) {
            std::swap(heap[node], heap[grandPar]);
            siftUpMin(grandPar);
        }
        else return;
    }
}

void MinMaxHeap::siftUp(unsigned int node) {
    if (node != 1) {
        if (isEvenLevel(node)) {
            if (heap[node] < heap[node / 2]) {
                siftUpMin(node);
            }
            else {
                std::swap(heap[node], heap[node / 2]);
                siftUpMax(node / 2);
            }
        }
        else {
            if (heap[node] > heap[node / 2]) {
                siftUpMax(node);
            }
            else {
                std::swap(heap[node], heap[node / 2]);
                siftUpMin(node / 2);
            }
        }
    }
}

void MinMaxHeap::insert(long long val) {
    size_t size = int(heap.size());
    if (nextIdx == size) {
        heap.push_back(val);
    }
    else {
        heap[nextIdx] = val;
    }
    siftUp(nextIdx);
    ++nextIdx;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);
    MinMaxHeap heap;
    int q = 0;
    std::cin >> q;
    while(q-- > 0) {
        std::string s;
        std::cin >> s;
        if (s == "GetMin") heap.extractMin();
        else if (s == "GetMax") heap.extractMax();
        else {
            long long val = 0;
            for (int i = 7; i < int(s.size()) - 1; ++i) {
                val *= 10;
                val += s[i] - '0';
            }
            heap.insert(val);
        }
    }


    return 0;
}