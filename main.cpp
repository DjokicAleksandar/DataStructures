#include <iostream>
#include <string>
#include "List.h"
#include "Stack.h"
#include "Array.h"
#include "Tree.h"

using namespace std;

int main() 
{
    StackArray<char> stack(20);
    stack.AddLong("29", "93");

    return 0;
}