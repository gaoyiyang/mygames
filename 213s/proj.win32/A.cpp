#include "A.h"
#include <iostream>

A::A()
{
}


A::~A()
{
}

int A::rt(){
	return 1;
}

int B::rt(){
	return 2;
}

void testPrint(A* a){
	std::cout << a->rt();
}

void main(){
	B* b = new B();
	testPrint(b);
	delete b;
	system("pause");
}
