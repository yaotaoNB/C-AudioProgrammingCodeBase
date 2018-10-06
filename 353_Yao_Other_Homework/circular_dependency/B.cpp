#include "B.h"	//B.h includeded A.h

B::B(int temp){
	B_int = temp;
}

void B::GetString(A a){
	cout<<"The A_string is: "<<a.A_string<<endl;
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

	B::B(int temp){
	B_int = temp;
	}

	void B::GetString(A a){
		cout<<"The A_string is: "<<a.A_string<<endl;
	}

*/