#include <iostream>
#include <memory>

int i = 5;

int main(){
	char i = 'b';
	auto r = [=](double i) {
		i = 5.6666666666;
		return i;
	};

//use variable returned from a lambda need a () after the var identifier just like a function, var with the same name CAN'T be in the same scope!!!
	std::cout<<"the result of lambda is: "<<r()<<std::endl;

//shared_ptr custom deleter eexample:
	//when deleter function is called, p1 is passed as an argument to our lambda function (where *p is at)
	shared_ptr<int> p1 = shared_ptr<int>(new int[3],
		[](int *p){delete[] p;});//or you can use auto var = [](int *p) {...}; to placehold the deleter

	return 0;
}

/*	
	Conclusion: The piority of captured variables in lambda function is inner scope first, outer scope second 
*/