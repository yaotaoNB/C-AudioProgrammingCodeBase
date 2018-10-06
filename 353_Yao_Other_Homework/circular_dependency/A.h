#pragma once
#include <iostream>
#include <string>

using namespace std;

class B;

class A{
public:
	A(string);
	string A_string;
	B* ptr_b = nullptr;
	void GetNumber(B*);
};