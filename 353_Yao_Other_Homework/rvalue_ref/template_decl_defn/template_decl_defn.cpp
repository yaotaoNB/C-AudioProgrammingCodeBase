#include "template_decl_defn.h"

template<typename T>
void foo(T t){
	cout<<"foo is called: "<< t <<endl;
}

template void foo<int>(int t);

/*template argument <> can be placeholder for a type, or for a value just like function argument in the () 
	But note that template variable <> is resulted in compile time. whatever in int main() are all run time
	because main is called during run time.
*/