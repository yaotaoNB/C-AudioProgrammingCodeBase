//g++ -std=c++14 function_ptr.cpp -o function_ptr
//Function pointer points to instructions in RAM. It's useful to be passed as an argument in another function for callback.
//function signature (return type, argument type and number) of both function ptr and function it's pointing to must be the same!

#include <iostream>

using namespace std;

int *Func(int a, int b);//this is a function return a int*
int (*_Func)(int a, int b);//this is a function pointer

void PrintHello(){
	cout<<"Hello"<<endl;
}

int Add(int a, int b){
	return a+b;
}

//callback example 1
void A(){
	cout<<"CallBack example 1."<<endl;
}

void B(void (*ptr)()){
	ptr();
}

//callback example 2
void C (int interger){
	cout<<"CallBack exmaple 2: "<<interger<<endl;
}

void D(void (*ptr)(int)){
	ptr(12);
}

int main(){
	int c;
	int (*p)(int, int);//pranthesis around *p means it's a function pointer
	p = &Add; //or p = Add; assigning function to a function ptr don't need () behind
	c = (*p)(2,3);//or p(2,3); just like using a function
	cout<<c<<endl;

	//pointer take no argument
	void (*ptr)();
	ptr = PrintHello;
	ptr();

//callback example 1
	B(A);//pass function A to B's 1st argument (function ptr)

//callback example 2
	D(C);

}