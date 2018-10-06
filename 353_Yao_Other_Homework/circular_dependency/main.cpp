#include "B.h" //B.h includeded A.h, A.h included all std lib

int main(){

	int temp_int_b = 66;
	string temp_string_a = "This is the string of A.";
	B my_b(temp_int_b);
	A my_a(temp_string_a);
	my_b.GetString(my_a);
	my_a.GetNumber(&my_b);

	return 0;
}

/*
	This file equals to:

	class B;

	class A{
	public:
		A(string);
		string A_string;
		B* ptr_b = nullptr;
		void GetNumber(B*);
	};

	class B{
	public: 
		B(int);
		int B_int;
		void GetString(A a);
	};

	int main(){

		int temp_int_b = 66;
		string temp_string_a = "This is the string of A.";
		B my_b(temp_int_b);
		A my_a(temp_string_a);
		my_b.GetString(my_a);
		my_a.GetNumber(&my_b);

		return 0;
	}

	//And finally linker will link the implementation in A.cpp, B.cpp and main.cpp
	g++ -std=c++14 A.cpp B.cpp main.cpp -o circular_dependency
	
	conclusion: To deal with forward decleration for circular dependency in 2 separate header files with 2 separate classes, since 2 header files can't 
	include each other, forward delecration another class in the file (A.h) which doesn't include another header file (B.h), and in the cpp file of first
	class (A.cpp), include BOTH .h file! Another cpp file only need to include one it own header file.
*/




