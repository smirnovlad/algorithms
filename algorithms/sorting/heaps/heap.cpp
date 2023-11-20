/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/347382/problem/D
 */

#include <iostream>
#include <vector>

class Heap{
private:
    std::vector<long long> heap;
    // идентефикатор вершины и указатель на вершину соответственно
    std::vector<unsigned int> identArr, pointer;
    size_t nextIdx;
protected:
    void siftUp(unsigned int);
    void siftDown(unsigned int);
    void exchange(unsigned int, unsigned int);
public:
    Heap() : heap({-1}), identArr({0}), pointer({0}), nextIdx(1) { }
    long long getMin()const { return heap[1]; }
    void extractMin();
    void insert(int, unsigned int);
    void decreaseKey(unsigned int, unsigned int);
    //void show()const { for (int i = 1; i < nextIdx; ++i) std::cout << heap[i] << ' '; std::cout << '\n';}
};

void Heap::insert(int val, unsigned int query){
    size_t size = int(heap.size());
    if (nextIdx == size) {
        heap.push_back(val);
        /*
         * identArr.push_back(nextIdx); // identArr[nextIdx] = nextIdx - номер идентефикатор
         * pointer.push_back(nextIdx); // pointer[nextIdx] = nextIdx - номер вершины
         */
        identArr.push_back(query);
    }
    else {
        heap[nextIdx] = val;
        identArr[nextIdx] = query;
    }
    pointer.resize(query + 1);
    pointer[query] = nextIdx;
    siftUp(nextIdx);
    ++nextIdx;
}

/*
 * Ошибка предыдущей реализации была в том, что элемент с идентефикатором nextIdx может
 * не быть минимум и остаться в куче после extractMin, тогда
 * при следующем insert'е он перезатрётся.
*/
void Heap::extractMin(){
    exchange(1, int(nextIdx) - 1);
    --nextIdx;
    siftDown(1);
}

void Heap::siftDown(unsigned int par    ) {
    if (2 * par >= nextIdx) return;
    size_t leftSon = 2 * par, rightSon = 2 * par + 1;
    size_t curSon = leftSon;
    if (rightSon < nextIdx && heap[leftSon] > heap[rightSon]) curSon = rightSon;
    if (heap[par] > heap[curSon]) {
        exchange(par, curSon);
        siftDown(curSon);
    }
}

void Heap::siftUp(unsigned int child) {
    if (child == 1) return;
    unsigned int par = child / 2;
    if (heap[par] > heap[child]) {
        exchange(par, child);
        siftUp(par);
    }
}

void Heap::exchange(unsigned int firstNode, unsigned int secondNode) {
    // определим, какими по счёту были добавлены рассматриваемые вершины
    unsigned int firstIdent = identArr[firstNode], secondIdent = identArr[secondNode];
    std::swap(identArr[firstNode], identArr[secondNode]);
    std::swap(pointer[firstIdent], pointer[secondIdent]);
    std::swap(heap[firstNode], heap[secondNode]);
}

void Heap::decreaseKey(unsigned int ptr, unsigned int delta) {
    unsigned int node = pointer[ptr];
    heap[node] -= int(delta);
    siftUp(node);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    Heap heap;

    int q = 0;
    std::cin >> q;
    for (int i = 0; i < q; i++) {
        std::string s;
        std::cin >> s;
        if (s == "insert") {
            int x = 0;
            std::cin >> x;
            heap.insert(x, i + 1);
        }
        else if (s == "getMin") {
            std::cout << heap.getMin() << '\n';
        }
        else if (s == "extractMin") {
            heap.extractMin();
        }
        else if (s == "decreaseKey") {
            int query = -1, delta = -1;
            std::cin >> query >> delta;
            heap.decreaseKey(query, delta);
        }
    }

    return 0;
}
