/*
	Created by Tao Yao on 09/29/2017

	Write a function to destructively remove all the even numbers from a singly-linked list of integers, 
	without using any existing container class libraries.  Write a set of unit test cases to test your code.

	g++ -std=c++14 -o rm_evennum rm_evenNum.cpp
*/

#include <iostream>
#include <cassert>

using namespace std;

//Create a Node class that only store int
class Node{
public:
	int data;
	Node* next;

//constructor take no argument
	Node(){
		this->next = nullptr;
	}
//constructor initialize with a value
	Node(int dataTemp){
		this->data = dataTemp;
		this->next = nullptr;
	}
};

//Singly-linked list class node starts from 1 that only store int
class List{

public:

//constructor method takes total number of node as its argument, avoiding segmentation fault by initializing all ptr to be null 
 List(int totalTemp) : head(nullptr), cur(nullptr), pre(nullptr), end(nullptr) {

 	if(totalTemp <= 0){
 		total = 0;
 		return;
 	}else{
 		total = totalTemp;
 	}

 	for(int i = 1; i <= total; ++i){
      if(i == 1){
 		cur = new Node();
 		pre = cur;
 		head = cur;
 		end = cur;
 	   }else{
 		cur = new Node();
 		pre->next = cur;
 		pre = pre->next;
 			}
 		end = cur;	
 		}
 }

 //constructor method to create the list, 1st is how many members, 2nd is set all nodes as value
 List(int totalTemp, int val) : head(nullptr), cur(nullptr), pre(nullptr), end(nullptr) {
 	if(totalTemp < 0){
 		total = 0;
 		return;
 	}else{
 		total = totalTemp;
 	}
	//create the first node start from 1
	for(int i = 1; i <= total; ++i){
      if(i == 1){
 		cur = new Node(val);
 		pre = cur;
 		head = cur;
 		end = cur;
 	   }else{
 		cur = new Node(val);
 		pre->next = cur;
 		pre = pre->next;
 			}
 		end = cur;	
 		}
	}

  ~List(){
  	  this->clean();
  }

//get the data of the specified position, pass an int variable as 2nd argument to receive the value
  void getPosVal(int pos, int &val){
  	if(head == nullptr || pos <= 0 || pos > total){
  	  	return;
  	  }else{
  	  	cur = head;
	  	for(int i = 1;i < pos;++i){
	  		pre = cur;
	  		cur = cur->next;
	  	}
  		val = cur->data;
  	  }
  }

//add a node in the end of list
  void add(int val){
  	//if total is 0
  	  if(head == nullptr) {
  	  	head = new Node(val);
  	  	end = head;
  	  	cur = head;
  	  	pre = head;
  	  	total++;
  	  	return;
  	  }else{
	  	Node *temp = new Node(val);
	  	end->next = temp;
	  	end = end->next;
	  	total++;
  	  }
  }
 
 //insert a single node to position
  void insertPos(int pos, int val){
  	  if(head == nullptr || pos <= 0 || pos > total){
  	  	return;
  	  }
  		Node *temp = new Node(val);
  		cur = head;
  		for(int i = 1;i < pos;++i){
  			pre = cur;
  			cur = cur->next;
  		}
  		pre->next = temp;
  		temp->next = cur;

  		total++;
  }
 
 //remove the node at position
  void erasePos(int pos){
  	if(head == nullptr || pos <= 0 || pos > total){
  	  	return;
  	  }
  	cur = head;
  	for(int i = 1;i<pos;++i){
  		pre = cur;
  		cur = cur->next;
  	}
  	if(cur == head){
  		pre = cur;
  		cur = cur->next;
  		head = cur;
  		delete pre;
	  }else{
	  pre->next = cur->next;
	  delete cur;
	  }
	  total--;
  }

//display all members of this list and print it to the console
  void display(){
  	int val;
  	for(int i = 1; i <= this->get_total(); i++){
			this->getPosVal(i,val);
			cout<<val<<endl;
		}
  }

  void clean(){
  	for(int i = 1; i <= this->get_total(); i++){
  		erasePos(i);
  		--i;
  	}
  	cur = pre = head = end = nullptr;
  }

  int get_total(){
  	return total;
  }

  Node* get_head_node(){
  	return head;
  }

  Node* get_end_node(){
  	return end;
  }

private:
 //keep track of total number of nodes
 	int total;
 //head, current and previous
	Node *head, *cur, *pre, *end;

};

//pass the list as reference to modify it and remove all even number from this list, 0 should be removed
void rm_even_num(List &list){
	int val;
	for(int i = 1; i <=list.get_total(); ++i){
		list.getPosVal(i,val);
		if(val % 2 == 0){
			list.erasePos(i);
			--i;
		}
	}
}

//check if all numbers in the list are even, later use for assert() to test rm_even_num()
bool checkEven(List &mylist){
	int val;
	rm_even_num(mylist);
	for(int i = 1; i <= mylist.get_total(); i++){
		mylist.getPosVal(i,val);
		if(val % 2 == 0){//if val is even number
			return false;
		}
	}
	return true;
}

//Unit test cases
bool Test_add_insert_erase(){
	Node *test_nodeHead;
	Node *test_cur;
	Node *test_nodeEnd;
	List test_list(0);
//Add testing
	test_list.add(10);
	test_nodeHead = test_list.get_head_node();
	test_nodeEnd = test_list.get_end_node();
	//add one value to the list
	if(test_list.get_total() != 1 || test_nodeHead->data != 10 || test_nodeEnd->data != 10) return false;
	//the values in the list should be 10, -20
	test_list.add(-20);
	test_nodeHead = test_list.get_head_node();
	test_nodeEnd = test_list.get_end_node();
	if(test_list.get_total() != 2 || test_nodeHead->data != 10 || test_nodeEnd->data != -20) return false;

//insert testing
	//insert a number to the 2nd element in the list, the values in the list now should be 10, 5, -20
	test_list.insertPos(2,5);
	test_nodeHead = test_list.get_head_node();
	test_nodeEnd = test_list.get_end_node();
	test_cur = test_nodeHead->next;
	if(test_list.get_total() != 3 || test_nodeHead->data != 10 || test_cur->data !=5 ||test_nodeEnd->data != -20) return false;

//erase testing, not it should be back to 10, -20
	test_list.erasePos(2);
	test_nodeHead = test_list.get_head_node();
	test_nodeEnd = test_list.get_end_node();
	if(test_list.get_total() != 2 || test_nodeHead->data != 10 || test_nodeEnd->data != -20) return false;

	test_nodeHead = test_cur = test_nodeEnd = nullptr;
	return true;
}

void Test_MyList(){
	assert(Test_add_insert_erase());
	cout<<"NO ERROR, all functions in List class is ready to be used!"<<endl;
}

//test start from value -100 include 0
void Test_rmEvenNum(){
	int val;
	List *test_list = new List(0);

	//1. test from value -100 to 100
	for(int i = -100; i <=100; i++){
		test_list->add(i);
	}

	auto has_even_numbers[]
	{
		bool a = 1;
		a = false;
		return 1;
	};

	remove_even_numbers(test_list);
	bool result = has_even_numbers(test_list);


	assert(checkEven(*test_list));
	test_list->clean();

	//2. test value that are all odd number
	for(int i = 2; i <=20; i = i + 2){
		test_list->add(i);
	}
	assert(checkEven(*test_list));
	test_list->clean();
	//3. test value that are all even number
	for(int i = 1; i <=23; i = i + 2){
		test_list->add(i);
	}
	assert(checkEven(*test_list));
	delete test_list;

	cout<<"NO ERROR, rm_even_num() is qualified and ready to be used!"<<endl;
}

int main(){
	Test_MyList();
	Test_rmEvenNum();
	return 0;
}

/*
bool areEqual(int one, int two){
	if(one == two) return true;
	else return false;
}
*/