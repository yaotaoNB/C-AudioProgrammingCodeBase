Harmonix standard programming screening test

Tao Yao

3 cpp files in the folder stands for each questions below, use -std=c++14 or -std=c++11 to change compiler version sicne some syntax might not support 98 GNU compiler version of C++. I'm using a MacOS so it might not be able to run on windows, please let me know if it's not working due to operating system difference.

Q1. Write a function to destructively remove all the even numbers from a singly-linked list of integers, without using any existing container class libraries.  Write a set of unit test cases to test your code.

file_name: rm_evenNum.cpp

I created a custom singly-linked list class containing some basic functions that you can add a value in the end, insert a value to a specific position, erase from a specific position and so on. The function that removes all even numbers is a function outside of the List class, it removes even numbers from the list by passing a reference of the List.

Finally for unit testing I used the traditional assert(), I built a set of functions that first check to make sure each functions in the class works, and then test the rm_evenNum function by giving it different possible combination of ints and check if every single elements in the list are odd number after passing it to the rm_evenNum function.


Q2. Implement a thread-safe circular queue of integers based on an array, with an Enqueue and a Dequeue method.  Enqueuing to a full queue and Dequeuing from an empty queue should produce an error or wait rather than dropping entries or resizing.

file_name: circular_queue.cpp

To implement a simple I thread-safe circular queue by creating a custom class called Circular_Queue and mutex to lock internal functions of the class. you can compile and run it in the shell, and follow the instruction by typing a number to make difference choices, choices include all functionality of requirements of the thread-safe circular queue demonstrated above.


Q3. If you were asked to write a function to remove duplicates from an array of ints, what questions would you ask before you started coding?  Describe in general terms the solutions you would give depending on the answers to those questions, and analyze the performance characteristics of your solutions in time, heap, and stack.

file_name: rm_dupint.cpp

For convenience I used vector in C++ as array.

There are couple questions I'm concerning before I started coding. The first question is how many different algorithms are there that can remove duplicates from an array of ints? If comparing these algorithms, how much difference would it impact the performance in terms of executing time, stack and heap usage? Is the array already sorted or not? Will an unsorted array impact the performance? What's the relationship between size of the array and performance on these different algorithms? Will different amount of duplicates impact the performance? What if all duplicates concentrate on one side (the first half) of the array compare to another side (second half)?

To test the performance I created a set of functions, and implemented 3 different algorithms in my program (of course I know there are more), brute force removing, sort removing and remove duplicates through a set data structure. I also tried fill the duplicates in the array near the front and near the 2nd half with size from 10 to 10000, and duplicates amount from 2 to 30000. 


Solutions:
The answer is the biggest 2 elements that impact the performance is the algorithm and size of the array. 

first let's talk about brute force algorithm:
I implemented this by using a nested loop, time complexity is increasing with the array size more than any other algorithm, but the amount of duplicates and on which side won't impact the performance that much.

relation between time and the performance would be:
t = o (n²)

Comparing to brute force, time complexity is reduced. Sorting algorithm will still increase the execution time and usage of stack/heap along with array complexity but it's increasing logarithmically

t = o (nlogn) + o (n)

Finally the Set algorithm, when the array size is small, it's slower than both brute force and sorting algorithm. By the size increases, it performs better and faster than brute force but still slower than sorting no matter what size it is, it might caused by the space complexity of the set

t = o(n) but space complexity o (n)



