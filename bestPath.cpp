#include <iostream>
#include <vector>

using namespace std;

int n = 5;
vector<int> cost = {3, 2, 1, 4, 5};
vector<int> prevIndex = {-1, -1, -1, -1, -1};
vector<vector<int>> nextList = {{1, 2}, {3}, {4}, {4}, {}};
vector<int> totalCost(n, -1);

void findBestPath(int index, int currentCost, int previousIndex)
{
    if (index == n)
        return;
    currentCost += cost[index];
    if (currentCost >= totalCost[index] && totalCost[index] != -1)
        return;
    totalCost[index] = currentCost;
    prevIndex[index] = previousIndex;
    if (nextList[index].size() == 0)
    {
        return;
    }
    for (int nextNode : nextList[index])
    {
        findBestPath(nextNode, currentCost, index);
    }
}

int main()
{
    findBestPath(0, 0, -1);
    for (int i = 0; i < n; i++)
    {
        cout << "index: " << i << ", cost: " << totalCost[i] << ", prev: " << prevIndex[i] << endl;
    }
    int index = n - 1;
    while (true)
    {
        cout << index << " << ";
        if (prevIndex[index] == -1)
            break;
        index = prevIndex[index];
    }
}
