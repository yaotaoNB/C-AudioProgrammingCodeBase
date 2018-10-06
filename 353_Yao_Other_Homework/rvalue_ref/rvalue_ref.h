/*
	Move semantic is mainly used for resource managing class
	whenever we have a class that has members that can reference heap space, we should implement our custom rule of three:
	overloaded copy constructor, assignment operator and destructor

	2 types of rvalue:
		1. pure rvalue: e.g. 53; or a returning function
		2. expiring rvalue (temporary object): e.g. {Foo F2; }, the var itself is an lvalue but going to expired, 
			for this need move(F2); to tell compiler treat it as an rvalue

	move semantic is used to optimize our program through move content (directly assign pointers or swap) in the temporary object
	instead of doing deep copying

	classes like unique_ptr is move only so we should disable copy constructor / assignment operator
	
*/
#pragma once

#include <iostream>
#include <string>

//#define _NOEXCEPT noexcept

class GCharacter
{
	//1st argument is cout, 2nd argument is instance of our class, so that we can cout<<GCharacter_instance<<endl;
	friend std::ostream& operator<<(std::ostream& os, const GCharacter& gc);

public:
	static const int DEFAULT_CAPACITY = 5;

	//Constructor
	GCharacter(std::string name = "John", int capacity = DEFAULT_CAPACITY);

	//swap function uses move instead of copy
	void swap(GCharacter&);

	//Copy Constructor
	GCharacter(const GCharacter& source);

	//Copy Assignment
	GCharacter& operator=(const GCharacter& source);

	//Move Constructor: since argument being passed is a temporary variable, we don't mind to change the content in that var
	GCharacter(GCharacter&& rhs);

	//Move Assignment Operator
	GCharacter& operator=(GCharacter&& rhs);

	//Copy and swap idiom: using copy and move constructor provide move/copy assignment operator in one implementation
	/*GCharacter& operator=(GCharacter other);*///when using this, mute other 2 assignment operator

	//Destructor
	~GCharacter();

	void insert(const std::string& toolName);

private:

	//data members
	std::string name;
	int capacity;
	int used;
	std::string* toolHolder;
};

//Perfect forwarding: 
template<typename T> 
void foo(T);
template<typename T> 
void relay(T&&);

/* Reference Collapsing Rules (only when both T and arg are double && can it be an rvalue ref, all other 3 cases are lvalue ref)
	1. T&	& (arg)  ==>	T&	(copy constructor)
	2. T&	&& (arg) ==>	T&	(copy constructor)
	3. T&&	& (arg)  ==>	T&	(copy constructor)
	4. T&&	&& (arg) ==>	T&&	(move constructor)

	T&& is universal reference: type T&& depending on how arg is initialized to decide wheather passing argument as an lvalue or rvalue ref
	Conditions:
	1. T is template type
	2. Type deduction (reference collapsing) happens to T.
		-T is a function template, not class template type.
	If match above, it's universal ref, other wise it's a rvalue ref.
*/