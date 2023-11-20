/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/347382/problem/A
 */

#include <iostream>
#include <vector>
#include <fstream>

using std::string;

class Heap{
private:
    std::vector <string> heap;
    size_t nextIdx;
public:
    Heap() : heap({"rubbish"}), nextIdx(1) { }
    string getMin()const { return heap[1]; }
    void extractMin();
    void siftUp(size_t);
    void siftDown(size_t);
    void insert(string);
    //void showHeap()const {for (int i = 1; i < nextIdx; i++) std::cout << heap[i] << ' '; std::cout << std::endl;}
    //void decreaseKey(pointer, key);
};

void Heap::insert(string val){
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

void Heap::extractMin(){
    heap[1] = heap[int(nextIdx) - 1];
    --nextIdx;
    siftDown(1);
}

void Heap::siftDown(size_t par) {
    if (2 * par >= nextIdx) return;
    size_t leftSon = 2 * par, rightSon = 2 * par + 1;
    size_t curSon = leftSon;
    if (rightSon < nextIdx && (heap[leftSon] + heap[rightSon] > heap[rightSon] + heap[leftSon])) curSon = rightSon;
    if ((heap[par] + heap[curSon]) > (heap[curSon] + heap[par])) {
        std::swap(heap[par], heap[curSon]);
        siftDown(curSon);
    }
}

void Heap::siftUp(size_t child) {
    if (child == 1) return;
    size_t par = child / 2;
    if ((heap[par] + heap[child]) > (heap[child] + heap[par])) {
        std::swap(heap[par], heap[child]);
        siftUp(par);
    }
}

void heapSort() {
    Heap heap;
    std::ifstream Fin;
    std::ofstream Fout;
    Fin.open("number.in");
    std::vector <string> nums;
    while (!Fin.eof()) {
        string num = "";
        Fin >> num;
        nums.push_back(num);
    }
    Fin.close();
    size_t size = nums.size();
    std::vector <string> ans(size);
    for (int i = 0; i < size; i++) {
        heap.insert(nums[i]);
    }
    for (int i = int(size) - 1; i >= 0; --i) {
        ans[i] = heap.getMin();
        heap.extractMin();
    }
    Fout.close();
    Fout.open("number.out");
    for (string el: ans) Fout << el;
    Fout.close();
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    heapSort();

    return 0;
}
