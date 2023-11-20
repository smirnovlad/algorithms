/*
 * https://codeforces.com/group/PVbQ8eK2T4/contest/347382/problem/I
 */

#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;

void Partition(vector<int>& curArr, const int& val, vector <int>& leftArr, vector <int>& rightArr) {
    size_t size = int(curArr.size());
    for (int i = 0; i < size; i++) {
        if (curArr[i] < val) leftArr.push_back(curArr[i]);
        else if (curArr[i] > val) rightArr.push_back(curArr[i]);
    }
    for (int i = 0; i < int(leftArr.size()); i++) {
        curArr[i] = leftArr[i];
    }
    for (int i = 0; i < size - int(leftArr.size()) - int(rightArr.size()); i++) {
        curArr[int(leftArr.size()) + i] = val;
    }
    for (int i = 0; i < int(rightArr.size()); i++) {
        curArr[size - int(rightArr.size()) + i] = rightArr[i];
    }
}

int getMedian(vector<int>& curArr, const int& l, const int& r) {
    std::sort(curArr.begin() + l, curArr.begin() + r + 1);
    return curArr[(l + r) / 2];
}

int kth(vector<int> curArr, unsigned int pos) {
    size_t size = int(curArr.size());
    if (size == 1) return curArr[0];
    vector <int> medians;
    for (int i = 0; i < size; i += 5) {
        if ((i + 4) < size) medians.push_back(getMedian(curArr, i, i + 4));
        else medians.push_back(getMedian(curArr, i, int(size) - 1));
    }
    int m = kth(medians, size / 10);
    vector <int> leftArr, rightArr;
    Partition(curArr, m, leftArr, rightArr);
    if (pos < int(leftArr.size())) return kth(leftArr, pos);
    else if (pos < size - int(rightArr.size())) return curArr[pos];
    else return kth(rightArr, pos - size + int(rightArr.size()));
}

void determQuickSort(vector <int>& curArr) {
    size_t size = int(curArr.size());
    if (size == 1) return;
    int m = kth(curArr, size / 2);
    vector <int> leftArr, rightArr;
    Partition(curArr, m, leftArr, rightArr);
    if (int(leftArr.size()) != 0) determQuickSort(leftArr);
    if (int(rightArr.size()) != 0) determQuickSort(rightArr);
    for (int i = 0; i < int(leftArr.size()); ++i) {
        curArr[i] = leftArr[i];
    }
    for (int i = 0; i < int(rightArr.size()); ++i) {
        curArr[size - int(rightArr.size()) + i] = rightArr[i];
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int n = 0;
    std::cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) std::cin >> a[i];
    determQuickSort(a);
    for (int el: a) std::cout << el << '\n';

    return 0;
}