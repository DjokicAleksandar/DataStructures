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
    List<int> lista;
    lista.AddToHead(1);
    lista.AddToTail(2);
    lista.AddToTail(3);
    lista.AddToTail(4);
    lista.AddToTail(5);
    lista.AddToTail(6);
    lista.AddToTail(7);
    lista.AddToTail(8);
    lista.AddToTail(9);
    lista.AddToTail(10);

    lista.Print();

    lista.MoveSectionToHead(4, 8);

    lista.Print();

    //cout << strlen("samokomplementirajuci") << " " << strlen("otorinolaringologija");

    return 0;
}