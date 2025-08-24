#include <iostream>
#include <string>
#include "List.h"
#include "Stack.h"
#include "Array.h"
#include "HashTable.h"
#include "Tree.h"
#include "Graph.h"

using namespace std;

int main() 
{
    GraphAsList<int, int> graph;

    int dep[] = { 1, 2, 1, 4 };

    int* res = graph.DefineOrder(dep, 4);

    for (int i = 0; i < 4; i++) {
        cout << res[i];
    }

    return 0;
}