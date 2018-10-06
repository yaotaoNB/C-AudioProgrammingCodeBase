/*
	Created by Tao Yao
	 If you were asked to write a function to remove duplicates from an array of ints, what questions would you ask before you started coding?  
	 Describe in general terms the solutions you would give depending on the answers to those questions, and analyze the performance characteristics 
	 of your solutions in time, heap, and stack.

	 g++ -std=c++14 -o rm_dupint rm_dupint.cpp
*/

#include <iostream>
#include <vector>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace std::chrono;

//passing an array and its length n
//1. Brute force solution
void rmDupBruteForce(vector<int>& myVector){
	for(int i = 0; i <myVector.size(); i++){
		//compare the rest members one by one with myVector[i]
		if((i+1)<myVector.size()){
			for(int j = i + 1; j < myVector.size(); j++){
				if(myVector[i] == myVector[j]){
					//cout<<"duplicate happens at element "<<j<<"and its value is: "<<myVector[j]<<endl;
					//remove myVector[j]
					myVector.erase(myVector.begin() + j);
					--j;
				}
			}
		}
	}
}

//2. Sorting solution (too keep the program simple I'll just use the default sort algorithm)
void rmDupSorting(vector<int>& myVector){
	sort(myVector.begin(),myVector.end());
	for(int i = 0; i < myVector.size(); i++){
		if((i+1)<myVector.size()){
			if(myVector[i] == myVector[i+1]){
			//cout<<"duplicate happens at element "<<i<<"and its value is: "<<myVector[i]<<endl;
			//remove myVector[i]
			myVector.erase(myVector.begin() + (i+1));
			--i;
			}
		}
	}
}

//3. Use Set
void rmDup_Set(vector<int>& myVector){
	unordered_set<int> mySet;
	for(int i = 0; i < myVector.size(); i++){
		if(mySet.insert(myVector[i]).second == false){
			//cout<<"duplicate happens at element "<<i<<"and its value is: "<<myVector[i]<<endl;
			myVector.erase(myVector.begin() + (i));
			--i;
		}
	}
}

void vectorDisplay(vector<int>& myVector){
	for(int i = 0; i < myVector.size(); i++){
		cout<<myVector.at(i)<<endl;
	}
}

int random(int min, int max){
	//change the rand algorithm everytime
	srand(time(0));
	int random = min + (rand()%(max+1));
	return random;
}

//function to fill an vector with ints, when half = 1 all duplicates will fill in the first half of the array, when half = 2 will fill the 2nd half
//to keep the program simple I won't check if duplicateNum is bigger than total, and the total and duplicateNum will be always even
void fillVector(vector<int> &myVector,int total, int duplicateNum, int half){
  int totalTemp;
  //first fill the array without duplicates
	for(int i = 0; i < (total - duplicateNum); i++){
		myVector.insert(myVector.begin() + i, i);
	}
  //then fill the 1st or 2nd half with duplicates in random order
	switch (half)
	{
		case 1://1st half, 0 - total/2, index and the value random from 0 - total/2
			for(int i = 0; i < duplicateNum; i++){
				totalTemp = myVector.size();
				if(totalTemp % 2 != 0){//if not even
					totalTemp--;
				}
				myVector.insert(myVector.begin() + random(0,totalTemp/2), random(0,total/2));
			}
			break;
		case 2://2nd half, range from total/2 to total
			for(int i = 0; i < duplicateNum; i++){
				totalTemp = myVector.size();
				myVector.insert(myVector.begin() + (i + myVector.size()/2), random(total/2,total));
			}
			break;
		default:
			cout<<"half should be either 1 or 2."<<endl;
			exit(1);
	}
}

void perf_measure(vector<int> &myVector){
	high_resolution_clock::time_point t1;
	high_resolution_clock::time_point t2;
	auto duration = 0;
	vector<int> holdVector = myVector;

	//brute force algorithm
	cout<<"\n execution of brute force removing duplicates begins."<<endl;
	t1 = high_resolution_clock::now();
	rmDupBruteForce(myVector);
	t2 = high_resolution_clock::now();
	duration = duration_cast<microseconds>( t2 - t1 ).count();
	cout<<"\n brute force removing duplicates finished, the time is: "<<duration<<" μs."<<endl;
	//vectorDisplay(myVector);
	myVector.clear();

	//sorting algorithm
	myVector = holdVector;
	cout<<"\n execution of sorting algorithm removing duplicates begins."<<endl;
	t1 = high_resolution_clock::now();
	rmDupSorting(myVector);
	t2 = high_resolution_clock::now();
	duration = duration_cast<microseconds>( t2 - t1 ).count();
	cout<<"\n sorting algorithm removing duplicates finished, the time is: "<<duration<<" μs."<<endl;
	//vectorDisplay(myVector);
	myVector.clear();

	//Set algorithm
	myVector = holdVector;
	cout<<"\n execution of set algorithm removing duplicates begins."<<endl;
	t1 = high_resolution_clock::now();
	rmDup_Set(myVector);
	t2 = high_resolution_clock::now();
	duration = duration_cast<microseconds>( t2 - t1 ).count();
	cout<<"\n Set removing duplicates finished, the time is: "<<duration<<" μs. \n"<<endl;
	//vectorDisplay(myVector);
	myVector.clear();
}

int main(){
	
	vector<int> myVector;
	//when I fill my array with only 8 elements with 2 pairs of duplicates that concentrate on the 1st half of the array
	fillVector(myVector,10,2,1);
	cout<<"\n When there's "<<myVector.size()<<" elements with 2 duplicate constrate in the 1st half of the array."<<endl;
	perf_measure(myVector);

	//when I fill my array with only 8 elements with 2 pairs of duplicates that concentrate on the 2nd half of the array
	fillVector(myVector,10,2,2);
	cout<<"\n When there's "<<myVector.size()<<" elements with 2 duplicate constrate in the 2nd half of the array."<<endl;
	perf_measure(myVector);

	//when I fill my array with only 20 elements with 6 pairs of duplicates that concentrate on the 1st half of the array
	fillVector(myVector,20,6,1);
	cout<<"\n When there's "<<myVector.size()<<" elements with 6 duplicate constrate in the 1st half of the array."<<endl;
	perf_measure(myVector);

	//when I fill my array with only 20 elements with 6 pairs of duplicates that concentrate on the 2nd half of the array
	fillVector(myVector,20,6,2);
	cout<<"\n When there's "<<myVector.size()<<" elements with 6 duplicate constrate in the 2nd half of the array."<<endl;
	perf_measure(myVector);

	//when I fill my array with 100 elements with 20 pairs of duplicates that concentrate on the 1st half of the array
	fillVector(myVector,100,20,1);
	cout<<"\n When there's "<<myVector.size()<<" elements with 20 duplicate constrate in the 1st half of the array."<<endl;
	perf_measure(myVector);

	//when I fill my array with 100 elements with 20 pairs of duplicates that concentrate on the 2nd half of the array
	fillVector(myVector,100,20,2);
	cout<<"\n When there's "<<myVector.size()<<" elements with 20 duplicate constrate in the 2nd half of the array."<<endl;
	perf_measure(myVector);


	//when I fill my array with 1000 elements with 300 pairs of duplicates that concentrate on the 1st half of the array
	fillVector(myVector,1000,300,1);
	cout<<"\n When there's "<<myVector.size()<<" elements with 300 duplicate constrate in the 1st half of the array."<<endl;
	perf_measure(myVector);

	//when I fill my array with 1000 elements with 300 pairs of duplicates that concentrate on the 2nd half of the array
	fillVector(myVector,1000,300,2);
	cout<<"\n When there's "<<myVector.size()<<" elements with 300 duplicate constrate in the 2nd half of the array."<<endl;
	perf_measure(myVector);

	//when I fill my array with 10000 elements with 3500 pairs of duplicates that concentrate on the 1st half of the array
	fillVector(myVector,10000,3500,1);
	cout<<"\n When there's "<<myVector.size()<<" elements with 3500 duplicate constrate in the 1st half of the array."<<endl;
	perf_measure(myVector);

	//when I fill my array with 10000 elements with 3500 pairs of duplicates that concentrate on the 2nd half of the array
	fillVector(myVector,10000,3500,1);
	cout<<"\n When there's "<<myVector.size()<<" elements with 3500 duplicate constrate in the 2nd half of the array."<<endl;
	perf_measure(myVector);

	//when I fill my array with 100000 elements with 30000 pairs of duplicates that concentrate on the 1st half of the array
	fillVector(myVector,100000,30000,1);
	cout<<"\n When there's "<<myVector.size()<<" elements with 30000 duplicate constrate in the 1st half of the array."<<endl;
	perf_measure(myVector);

	//when I fill my array with 100000 elements with 30000 pairs of duplicates that concentrate on the 2nd half of the array
	fillVector(myVector,100000,30000,1);
	cout<<"\n When there's "<<myVector.size()<<" elements with 30000 duplicate constrate in the 2nd half of the array."<<endl;
	perf_measure(myVector);

	return 0;
}