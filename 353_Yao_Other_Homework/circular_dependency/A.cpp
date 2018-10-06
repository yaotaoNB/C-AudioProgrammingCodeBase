#include "A.h"
#include "B.h"

A::A(string _string){
	A_string = _string;
}

void A::GetNumber(B* temp_b){
	if(temp_b != nullptr){
		this->ptr_b = temp_b;
		cout<<"the int in class B is: "<<ptr_b->B_int<<endl;
	}
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

	A::A(string _string){
	A_string = _string;
	}

	void A::GetNumber(B* temp_b){
		if(temp_b != nullptr){
			this->ptr_b = temp_b; 
			//Since B members has already been declared above in B.h, the declaration of ptr_b->B_int can be found by compiler
			cout<<"the int in class B is: "<<ptr_b->B_int<<endl;
		}
	}
*/