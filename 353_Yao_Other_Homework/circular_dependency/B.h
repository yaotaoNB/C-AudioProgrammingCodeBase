#pragma once
#include "A.h"

class B{
public: 
	B(int);
	int B_int;
	void GetString(A a);
};

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

*/