//g++ -std=c++14 experiment.cpp -o experiment
#include <iostream>

using namespace std;

void printInt(int& i){
	cout<<"L value func is called."<<endl;
	i++;
	cout<<i<<endl;
}

void printInt(int&& i){
	cout<<"R value func is called."<<endl;
	i++;
	cout<<i<<endl;
}

//this function pass both r and l value by value, construct an int argument called i and assign the value (either r and l) to it
//this can't co-exist with printInt(int& i) and printInt(int&& i)
/*void printInt(int i){
	cout<<"pass by value func is called."<<endl;
	i++;
	cout<<i<<endl;
}
*/

foo(){
	return 0;
}

int main(){

	printInt(99);
	int x = 0;
	int& rx = x;//this is purely an lvalue ref

// this will bind both lvalue and rvalue ref, if it's an rvalue it will extend the life time of that rvalue to the lifetime of current scope {} 
	//(normally rvalue ends after the semicolon), but a const ref doesn't provide modification to the right variable
	const int& crx = foo();

	return 0;
}
