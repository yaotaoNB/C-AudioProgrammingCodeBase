#include <iostream>
#include <unordered_map>
#include <memory>

using namespace std;

class Parent {
public:
	Parent(){
		cout<<"Parent constructor is called."<<endl;
	}
	virtual ~Parent(){
		cout<<"Parent destructor is called."<<endl;
	}
	virtual void function(){
		cout<<"Parent function that take no argument is called."<<endl;
	}
};

class Child1 : public Parent {
public:
	Child1(){
		cout<<"Child1 constructor is called."<<endl;
	}
	~Child1() override {
		cout<<"Child1 destructor is called."<<endl;
	}
	void function() override {
		cout<<"Child1 no argument function is called"<<endl;
	}
};

class Child2 : public Parent {
public:
	Child2(){
		cout<<"Child2 constructor is called."<<endl;
	}
	~Child2() override {
		cout<<"Child2 destructor is called."<<endl;
	}
	void function() override{
		cout<<"Child2 function is called, the numebr is: "<<endl;
	}
};


//virtual function is 
int main(){
	Parent* ptr;
	ptr = (Child1*)ptr;
	ptr = new Child1();
	ptr->function();
	delete ptr;
}