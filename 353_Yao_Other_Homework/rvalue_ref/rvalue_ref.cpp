#include "rvalue_ref.h"

using namespace std;

//constructor
GCharacter::GCharacter(string n, int cap){
	cout<<"Constructor is called!"<<endl;
	name = n;
	capacity = cap;
	used = 0;
	toolHolder = new string[cap];
}

void GCharacter::swap(GCharacter& other){
	cout<<"swap!"<<endl;
	std::swap(name, other.name);
	std::swap(capacity, other.capacity);
	std::swap(used, other.used);
	std::swap(toolHolder, other.toolHolder);
}

//copy constructor: pass by ref can avoid one additional step of constructing an obj
GCharacter::GCharacter(const GCharacter& source){
	cout<<"copy constructor is called."<<endl;
	this->name = source.name;
	this->capacity = source.capacity;
	used = source.used;
	toolHolder = new string[source.capacity];
//1st way to copy our array from another obj: 1st argument is ptr to the 1st array element, 2nd argument is the last element, 
	//3rd is the actual obj
	copy(source.toolHolder, source.toolHolder + used, this->toolHolder);
//2nd way to copy array is using a for loop
	for(int i = 0; i < used; i++){
		toolHolder[i] = source.toolHolder[i];
	}
}

//Copy assignment operator
GCharacter& GCharacter::operator=(const GCharacter& source){
	cout<<"overloaded assignment called. "<<endl;

	//check for self assgiment
	//gc1 = gc1;
	if(this == &source){
		return *this;
	}
	cout<<"copy constructor is called."<<endl;
	this->name = source.name;
	this->capacity = source.capacity;
	used = source.used;
	toolHolder = new string[source.capacity];
	copy(source.toolHolder, source.toolHolder + used, this->toolHolder);

	return *this;
}

//1st way: Move Constructor (shallow copy)
/*GCharacter::GCharacter(GCharacter&& rhs) _NOEXCEPT {
	std::cout<<"move constructor is called."<<std::endl;
	//Is that necessary to use std::move() for these primitive types???
	name = std::move(rhs.name);
	capacity = std::move(rhs.capacity);
	used = std::move(rhs.used);
	toolHolder = rhs.toolHolder;
	//so that rhs.toolHolder will delete a nullptr instead of the actually object we just moved when this rvalue destructor is called
	rhs.toolHolder = nullptr;
}*/

//2nd way: Move Constructor (better way)
GCharacter::GCharacter(GCharacter&& rhs) : name(""), capacity(0), used(0), toolHolder(nullptr) {//rhs this will swap and get a nullptr 
	std::cout<<"move constructor is called."<<std::endl;
	swap(rhs);
}

//Move Assignment Operator
GCharacter& GCharacter::operator=(GCharacter&& rhs) {
	cout<<"Move Assignment operator is called."<<endl;

	if(this != &rhs){
		//we can also use std::move() for each member here, but we need to set toolHandler to null if so
		this->swap(rhs);
	}
	return *this;
}

//Copy and swap idiom: When we pass lvalue to this function, other will be constructed with copy constructor, and When we pass rvalue to this function, 
//other will be constructed with move constructor, other is constructed as a temporary object in both cases so swap with it is totally fine.
/*GCharacter& GCharacter::operator=(GCharacter other){
	cout<<"Copy and swap is called."<<endl;
	swap(other);
	return *this;
}*/

//Destructor
GCharacter::~GCharacter(){
	cout<<"Destructor called for "<<this->name<<"@ this memory location "<<this<<endl;
	delete[] toolHolder;
}

//Inserting a new tool to toolHolder
void GCharacter::insert(const string& toolName){
	if(used == capacity){
		cout<<"Tool Holder is full, cannot add any aditional tools."<<endl;
	}else{
		toolHolder[used] = toolName;
		++used;
	}
}

std::ostream& operator<<(std::ostream& os, const GCharacter& gc){
	os<<"Game Character "<<gc.name<<"\n has the following tools: "<<std::endl<<" | ";
	for(int i = 0; i < gc.capacity; ++i){
		os << gc.toolHolder[i] + " | ";
	}

	return os << std::endl;
}

//Perfect Forwarding: Make sure when argument is forwarded to foo() have the same semantic type so that it won't call an extra constructor for foo() 
template<typename T>
void foo(T gc){
	cout<<"foo is called."<<endl;
}

template<typename T>
void relay(T&& arg){
	foo(std::forward<T>(arg));//this become foo(T&& arg); so that relay() and foo() will be passed with the same type
}

/*!!!	when seperate declaration and definition in a .h and .cpp file, you need to do this, coz compiler need to see both template definition, 
	declaration and SPECIFIC TYPE used to FILL IN the template in order to compile (it need to know that you want to use that specific type)
	check out: https://isocpp.org/wiki/faq/templates#separate-template-fn-defn-from-decl and my own template_defn_decl
	1st solution is this, 2nd solution is all define template function in the header file */
template void foo<GCharacter>(GCharacter gc);
//template void relay<GCharacter>(GCharacter gc); //We don't need this!!! because universal reference only take reference (either l or r) but NO value!
template void relay<GCharacter&>(GCharacter& gc);//but in here lvalue we need to add & in <> ! (<GCharacter&> is correct)
template void relay<GCharacter>(GCharacter&& gc);//for some reason for rvalue it can't be a & inside <GCharacter> (<GCharacter&> or <GCharacter&&> is wrong)

//Implementation of std::forward()
//this function cast arg to type T&&
template<class T>
T&& forward(typename remove_reference<T>::type& arg){//first remove
	return static_cast<T&&>(arg);//equals to T&& arg (only if arg is &&)
}

/*
	std::move<T>(arg);	Turn arg into rvalue type
	std::forward<T>(arg);	Turn arg into type of T&&
*/