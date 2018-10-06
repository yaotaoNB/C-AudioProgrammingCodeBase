/*
	Created by Tao Yao on 09/29/2017
	
	 Implement a thread-safe circular queue of integers based on an array, with an Enqueue and a Dequeue method.  
	 Enqueuing to a full queue and Dequeuing from an empty queue should produce an error or wait rather than 
	 dropping entries or resizing.

*/

/*
	g++ -std=c++14 -o circular_queue circular_queue.cpp

    Use unique ptr instead of traditional ptr
 */
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

//my custom circular queue class
class Circular_Queue
{
    private:
        mutex m_mutex;
        int *cqueue_arr;
        int front, rear, total;
    public:
        Circular_Queue(int totalTemp)
        {
        	this->total = totalTemp;
            cqueue_arr = new int [total];
            rear = front = -1;
        }

        ~Circular_Queue()
        {
            delete cqueue_arr;
        }
        
        void Enqueue(int item)
        {
            lock_guard<mutex> locker(m_mutex);

            if ((front == 0 && rear == total-1) || (front == rear+1))
            {
                cout<<"The Queue is full!\n";
                return;
            }
            if (front == -1)
            {
                front = 0;
                rear = 0;
            }
            else
            {
                if (rear == total - 1)
                    rear = 0;
                else
                    rear = rear + 1;
            }
            cqueue_arr[rear] = item;
        }
        
        void Dequeue()
        {
            lock_guard<mutex> locker(m_mutex);

            if (front == -1)
            {
                cout<<"Can't Dequeuing, the Queue is empty!\n";
                return ;
            }
            cout<<"Element deleted from queue is : "<<cqueue_arr[front]<<endl;
            if (front == rear)
            {
                front = -1;
                rear = -1;
            }
            else
            {
                if (front == total - 1)
                    front = 0;
                else
                    front = front + 1;
            }
        }
        
        void display()
        {
            lock_guard<mutex> locker(m_mutex);
            
            int front_pos = front, rear_pos = rear;
            if (front == -1)
            {
                cout<<"The Queue is empty!\n";
                return;
            }
            cout<<"Queue elements :\n";
            if (front_pos <= rear_pos)
            {
                while (front_pos <= rear_pos)
                {
                    cout<<cqueue_arr[front_pos]<<"  ";
                    front_pos++;
                }
            }
            else
            {
                while (front_pos <= total - 1)
                {
                    cout<<cqueue_arr[front_pos]<<"  ";
                    front_pos++;
                }
                front_pos = 0;
                while (front_pos <= rear_pos)
                {
                    cout<<cqueue_arr[front_pos]<<"  ";
                    front_pos++;
                }
            }
            cout<<endl;
        }
};

void cinClear()
{
	cin.clear();
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main()
{
    int choice, item, total;
    cout<<"How many items do you want in your circular queue? (type a number)"<<endl;
    cin>>total;
    if(cin.fail()){
    cout<<"You typed an invalid value, please type a positive integer!"<<endl;
    exit(1);
	}else{
	cout<<"the max size of your circular queue is: "<<total<<endl;
	cinClear();
	}
    Circular_Queue circular_queue(total);

    do
    {
    choose:
        cout<<"1.Insert\n";
        cout<<"2.Delete\n";
        cout<<"3.Display\n";
        cout<<"4.Quit\n";
        cout<<"Enter your choice : ";
        cin>>choice;
        if(cin.fail()){
        	cout<<"you typed an invalid value! make a choice from 1 to 4."<<endl;
        	cinClear();
        	goto choose;
        }
        switch(choice)
        {
        case 1:
            cout<<"Input the element for Enqueuing the queue : ";
            cin>>item;	
            circular_queue.Enqueue(item);
            cinClear();
	    break;
		case 2:
            circular_queue.Dequeue();
	    break;
        case 3:
            circular_queue.display();
	    break;
		case 4:
	    break;
		default:
	    cout<<"Wrong choice\n";
		}
    }
    while(choice != 4);
    cout<<"Bye!"<<endl;
    return 0;
}