/*
g++ -std=c++14 functor.cpp -o functor
	Functor (function type) is operator() overload in a class so that the instance of the class can be called and behave like a function. It's really 
	useful when we need to define a custom sort algorithm/function for stl containers, stl functions such as transform(), and when we created the stl 
	container the 2nd template argument accept a functor for custom default sort algorithm if the container is auto sort container such as set.
	Benefits:
		1. functor can remember state (through a member varaible) in the class.
		2. It can have it own type.
*/
#include <iostream>
#include <vector>
#include <set>
#include <deque>
#include <string>
#include <cmath>

using namespace std;

//1. Basic usage of functor
class AddValue {
	int val;
public:
	AddValue(int j) : val(j) {	}
	void operator()(int i)	{
		cout<< i+val <<endl;
	}
};

//2. Build-in functors
//such as less_equal, not_equal_to ...

//3. Parameter binding
set<int> myset = {2, 3, 4, 5};
vector<int> vec_bind;

//4. Convert a regular function to a functor
double Pow(double x, double y) {
	return pow(x, y);
}

//5. Binding two functor into one functor
set<int> functor_bind = {3, 1, 25, 7, 12};
//when (x > 20) || (x < 5), copy from meset to d_functor_bind
deque<int> d_functor_bind;

//6. Lambda function

//7. Functor for sorting stl container
/* 2nd template argument of set is the sorting functor*/
set<int> stl_set = {3, 1, 25, 7, 12 };
set<int, less<int>> stl_set_2 = {3, 1, 25, 7, 12 }; //is equvilant with stl_set above
//the functor takes 2 operand this time, instead of one, we can write our own sort functor like:
class lsb_less {
public:
	bool operator()(int x, int y){
		return (x%10)<(y%10);
	}
};
/*and pass it as 2nd tempalte argument of set, for 2nd tempate argument don't need constructor: lsb_less() bacuase it specifies a type/class and 
  will look for operator() overload inside that class instead of construct in the template, the lsb_less instance will be constructed while it construct
   stl_set_3 during compile time */
set<int, lsb_less> stl_set_3 = {3, 1, 25, 7, 12 };

//8. call functor except using it in stl
/* since stl function above will auto pass parameter to functor so we don't need to specify argument of the functor, except using it in stl
	we still need to pass it argument or empty () after the constructor just like a function */
class X {
public:
	void operator()(string str){
		cout<<"Functor called, string we passed is: "<<str<<endl;
	}
};

//9. Predicate
/* Predicate is a function or functor that:
	1. return a bool
	2. Does not modify data*/

/*
	10. functor is really useful when you are using stl containers and functions for custom class, you can write a seperate functor class to sort that
	custom class, tempalte functor class is also acceptable.
*/

int main(){
//1. 
	std::vector<int> vec = {2, 3, 4, 5};
	int x = 2;
/* third argument of for_each behave like a function pointer, but only take functor, when we pass functor to for_each each element in vec is auto passed
	as argument to the functor so we only put the name of functor in here without passing argument, and here for_each 3rd argument functor requiring 
	one operand (unary), AddValue(x) is actually constructor of the class! extended look like: AddValue(x)(vec.element) */
	for_each(vec.begin(),vec.end(), AddValue(x));

//2. Build-in functor multiplies: still first () constructor, second (3, 4) function arguments.
	int _x = multiplies<int>()(3, 4); // x = 3 * 4

//3. 
/* In here we multiply all element in myset by 10 and insert it to vec_bind, because 3rd argument of transform only accept unary operand functor, with 
	bind everytime we call multiplies it will replace its 2nd argument with 10 and pass each element of myset into its 1st argumet (placeholder). so now
	multiplies becomes one operand functor */
	transform(myset.begin(), myset.end(),//source
			 back_inserter(vec_bind),//destination
			 bind(multiplies<int>(), placeholders::_1, 10));//functor

//4. Convert a regular function to a functor in order to use it in stl
	deque<int> d;
	auto f = function<double (double, double)>(Pow);
	transform(myset.begin(), myset.end(),//source
			 back_inserter(d),//destination
			 bind(f, placeholders::_1, 2));//converted functor

//5. Binding two functor into one functor
	transform(myset.begin(), myset.end(),//source
			 back_inserter(d_functor_bind),//destination
			 bind(logical_or<bool>(),
			 	bind(greater<int>(), placeholders::_1, 20),//return true if placeholder greater than 20
			 	bind(less<int>(), placeholders::_1, 5)) //return true if placeholder less than 5
			 );

//6. Lambda function
	transform(myset.begin(), myset.end(),//source
			 back_inserter(d_functor_bind),//destination
			 [](int x) { return (x>20) || (x<5); } //so that we can create function in one place
			 );

//8. to call our functor:
	X foo;
	foo("Hi");
//or construct a rvalue type object
	X()("Haha");
}