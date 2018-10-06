//g++ -std=c++14 rvalue_ref.cpp main.cpp -o rvalue_ref
#include "rvalue_ref.h"
using namespace std;

/*
//create a function take an GCharacter as argument and return it by value
GCharacter exCopyConstructor(GCharacter tempGC){
	cout << "Copy Constructor called twice. "<< endl;
	cout << "Once for the formal parameter being passed by value" << endl;
	cout << "Once for the return value." <<endl;
	//when move constructor is defined, this return obj will call move constructor instead of copy
	return tempGC;
}

int main(){
	//initialize all argument as default val
	GCharacter gc1;

	gc1.insert("potion");
	gc1.insert("crossbow");
	gc1.insert("candle");
	gc1.insert("cloak");
	gc1.insert("sword");
	gc1.insert("book of spells");//toolHandler full

	cout << gc1 << endl;

	GCharacter gc2("Bob", 5);

	gc2.insert("axe");

//copy constructor is invoked when we passing an obj by value or returning an obj by value from a particular function
	exCopyConstructor(gc2);

//copy constructor is invoked whenever we assigning an obj to another obj and obj on the left doesn't already exist
	GCharacter gc3 = gc2;

//obj have already existen will invoke overloaded move assignment operator
	gc2 = gc1;

	cout <<"gc2: "<<gc2 <<endl;
	cout <<"gc1: "<<gc1 <<endl;
	cout <<"gc3: "<<gc3 <<endl;

//Finally Destructor taking care of what we allocated on the heap using new

}
*/

/* Move Constructor / Assigment operator demonstrate: Move will empty the original obj when you pass it as argument!
Rule of 5: Copy Constructor, Copy Assignment Operator, Move Constructor, Move Assignment Operator, Destructor */

/* this function will:
	1. A copy constructor is called since we pass the argument by value GCharacter gc_move(gc2)
	2. because the function it self is a temporary rvalue obj, the return will invoke move constructor */
GCharacter exMoveConstructor(GCharacter gc_move){
	return gc_move;
}

void exRefConstructor(GCharacter& gc_move){
	//this will call no constructor since passing by lvalue, faster than both move/copy constructor
}

int main(){
/*
	GCharacter gc1;//constructor is called
	gc1.insert("potion");
	gc1.insert("crossbow");
	gc1.insert("candle");
	gc1.insert("cloak");
	gc1.insert("sword");

	cout << "gc1: (before move)"<< gc1 << endl;
//std::move will not only cast original gc1 to from a rvalue to a lvalue, but also setgc1.toolHandler = null, so only use it when I'm sure I'll not use gc1 anymore
	GCharacter gc2(std::move(gc1));//move constructor is called to construct the temporary object

	cout << "gc1: (after move)"<< gc1 << endl;
	cout << "gc2: " << gc2 << endl;

//move constructor is invoked when we returning an rvalue / temporary obj from a particular function (the return obj is function it self)
	exMoveConstructor(gc2);//std::move(gc2) will invoke move constructor twice 

//move constructor is invoked when we passing an rvalue / temporary obj as argument to a function
	exMoveConstructor(GCharacter("Wo Cao",10));

//move constructor is invoked whenever we assigning an rvalue / temp obj to another obj and obj on the left doesn't already exist
	GCharacter gc3 = exMoveConstructor(GCharacter("Move CONSTRUCTOR instead of assignment operator",11));
	cout<<"gc3: " << gc3 << endl;


//Move assignment operator will be invoked if object on the left have already existen, and right side is rvalue, and it's defined
	GCharacter gc5("Test Move Assignment",15);
	cout<<"g5 before move assignment: "<<gc5<<endl;
	gc5 = GCharacter("Now invoke Move Assignment", 2);//rvalue need to invoke the constructor to construct the temporary object first!
	cout<<"g5 after move assignment: "<<gc5<<endl;//so that's the reason why Bo created a non-member constructor function outside the class


	//copy and swap idiom
	GCharacter gc6("Test CAS Idiom.",12);
	cout<<"g6 before move assignment: "<<gc6<<endl;
	GCharacter gc7("Now invoke CAS Idiom!", 13);
//When we construct a new rvalue object on the right will call default constructor instead of copy and move constructor
	//because we are calling the default constructor here
	//gc6 = GCharacter ("Now invoke CAS Idiom!", 13);
	gc6 = std::move(gc7);//so in this case we have to use std::move()  
	cout<<"g6 after move assignment: "<<gc6<<endl;
*/

//Perfect Forwarding
	relay(GCharacter());
	GCharacter gc8;
	relay(gc8);
	cout<<gc8<<endl;
	relay(std::move(gc8));
	cout<<gc8<<endl;
}

/*Copy and Swap Idiom: both copy and move assignment operator will be handled by one member function!
Rule of 4: Copy Constructor, Move Constructor, Copy and swap idiom (this function provide both copy and move assignment operator), Destructor
	A Pass by value copy is more efficient than creating a temporary obj in the body via function since compiler can provide optimization on it.
*/

/*
	Most of time copy / move constructor is used for existing objects on the right hand side, pass or assign a temporary rvalue object will need to
	call constructor to construct it first! So we need to use std::move() a lot for our existing xrvalue objects.
*/