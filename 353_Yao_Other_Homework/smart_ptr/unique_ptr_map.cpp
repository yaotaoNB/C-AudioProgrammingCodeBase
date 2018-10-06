#include <iostream>
#include <unordered_map>
#include <memory>

using namespace std;

class Dog{
public:
	Dog(){
		cout<<"Dog is created."<<endl;
	}

	~Dog(){
		cout<<"Dog is destroyed."<<endl;
	}
};

unordered_map<int, unique_ptr<Dog>> ourmap;
unordered_map<int, Dog*> normal_ptr;
//unordered_map<int, int> ourmap;

int main(){
	cout<<"size is: "<<ourmap.size()<<endl;
	ourmap.insert(std::make_pair(1, unique_ptr<Dog>(new Dog())));
	auto it = ourmap.find(1);
	it->second = nullptr;
	cout<<"size is: "<<ourmap.size()<<endl;
	cout<<"middle 1"<<endl;
	ourmap.insert(std::make_pair(2, unique_ptr<Dog>(new Dog())));
	cout<<"size is: "<<ourmap.size()<<endl;
	ourmap.erase(2);
	cout<<"after erase 2, size is: "<<ourmap.size()<<endl;
	cout<<"middle 2"<<endl;

	normal_ptr.insert(std::make_pair(1, new Dog()));
	cout<<"size is: "<<normal_ptr.size()<<endl;
}