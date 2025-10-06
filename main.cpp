#include <iostream>
#include "MemoryManager.h"
#include <vector>

using namespace std;

void easyInsertsDeletes();
void edgeCasesTests();
void randomInsertsDeletes();
void numberGuesser();

//structure used later in a dynamic memory allocation
struct Point
{
	double x;
	double y;
	double z;
};


int main()
{

	numberGuesser();
	easyInsertsDeletes();
	edgeCasesTests();
	randomInsertsDeletes();

	return 0;
}
//Node pointers need to be of size 24
//--
void easyInsertsDeletes()
{
	cout << "****************************" << endl;
	cout << "* Easy inserts and deletes *" << endl;
	cout << "****************************" << endl;

	//size of the managed heap
	const int heapSize = 10000;

	//create 10000 bytes of heap space (memory is actually a char pointer)
	char memory[heapSize];

	//let the memory manager know which chunk of memory it is managing
	MemoryManager m(memory, heapSize);

	cout << "Initial look at memory" << endl;
	//show memory
	m.dump();

	cout << "Allocating 1000 ints to p_int (size required " << (sizeof(int) * 1000) << " bytes)" << endl;
	//allocate room for 1000 ints. Allocate returns a void* that must be cast to the
	//correct type. The program needs to know the type in order to do pointer
	//arithmetic (array notation)
	int* p_int = (int*)(m.allocate(sizeof(int) * 1000));
	//turns the void pointer into an int pointer


	//if there was room on the heap the pointer will not be 0
	
	if (p_int != 0)
	{
		//use the memory somehow
		for (int i = 0; i < 1000; i++)
		{
			//store all 0's in the block of memory
			p_int[i] = 0;
		}
	}
	
	//show memory
	m.dump();

	cout << "Allocating 100 doubles to p_double (size required " << (sizeof(double) * 100) << " bytes)" << endl;
	//allocate room for 100 doubles
	double* p_double = (double*)(m.allocate(sizeof(double) * 100));

	//if there was room on the heap the pointer will not be 0
	
	if (p_double != 0)
	{
		for (int i = 0; i < 100; i++)
		{
			//use the memory by storing all 0.0's
			p_double[i] = 0.0;
		}
	}
	
	//show memory
	m.dump();

	cout << "Allocating 10 Point structs to p_point (size required " << (sizeof(Point) * 10) << " bytes)" << endl;
	//allocate room for 10 Points
	Point* p_point = (Point*)(m.allocate(sizeof(Point) * 10));

	//to be used to copy over
	Point p = { 2.0, 4.0, 2.4 };

	//if there was room on the heap the pointer will not be 0
	if (p_point != 0)
	{
		for (int i = 0; i < 10; i++)
		{
			//use the memory by copying p multiple times
			p_point[i] = p;
		}
	}

	//show memory
	m.dump();

	cout << "Allocating 1000 more ints to p_int2 (size required " << (sizeof(int) * 1000) << " bytes)" << endl;
	//allocate room for 1000 ints
	int* p_int2 = (int*)(m.allocate(sizeof(int) * 1000));

	//if there was room on the heap the pointer will not be 0
	if (p_int2 != 0)
	{
		//use the memory
		for (int i = 0; i < 1000; i++)
		{
			p_int2[i] = 0;
		}
	}

	//show memory
	m.dump();

	cout << "Allocating 1000 more ints to p_int3 (size required " << (sizeof(int) * 1000) << " bytes- shouldn't work!)" << endl;
	//shouldn't work
	int* p_int3 = (int*)(m.allocate(sizeof(int) * 1000));

	//if there was room on the heap the pointer will not be 0
	
	if (p_int3 != 0)
	{
		//use the memory
		for (int i = 0; i < 1000; i++)
		{
			p_int3[i] = 0;
		}
	}
	
	//show memory
	m.dump();

	//now start giving some of the memory back
	cout << "Deallocating p_int" << endl;
	m.deallocate(p_int);
	m.dump();
	cout << "Deallocating p_double" << endl;
	m.deallocate(p_double);
	m.dump();
	cout << "Deallocating p_int2" << endl;
	m.deallocate(p_int2);
	m.dump();
	cout << "Deallocating p_point" << endl;
	m.deallocate(p_point);
	m.dump();
	cout << "Deallocating p_int3- shouldn't change memory" << endl;
	m.deallocate(p_int3);
	m.dump();

	cout << "Allocating 1000 more ints to p_int3 (should work this time)" << endl;
	//should work
	p_int3 = (int*)(m.allocate(sizeof(int) * 1000));

	//if there was room on the heap the pointer will not be 0
	if (p_int3 != 0)
	{
		//use the memory
		for (int i = 0; i < 1000; i++)
		{
			p_int3[i] = 0;
		}
	}
	//show memory
	m.dump();

	cout << "Deallocating p_int3" << endl;
	//get rid of the memory
	m.deallocate(p_int3);

	//show memory
	m.dump();
}
//--
void edgeCasesTests()
{
	cout << "******************************" << endl;
	cout << "* Edge cases and other tests *" << endl;
	cout << "******************************" << endl;

	//size of the managed heap
	const int heapSize = 10000;

	//create 10000 bytes of heap space (memory is actually a char pointer)
	char memory[heapSize];

	//let the memory manager know which chunk of memory it is managing
	MemoryManager m(memory, heapSize);

	cout << "Allocating 4 equal blocks (with an extra empty block at the end)" << endl;
	char* p_1 = (char*)m.allocate(2464);
	char* p_2 = (char*)m.allocate(2464);
	char* p_3 = (char*)m.allocate(2464);
	char* p_4 = (char*)m.allocate(2464);
	//p_1 -> Node: Used 2464
	//p_2 -> Node: Used 2464
	//p_3 -> Node: Used 2464
	//p_4 -> Node: Used 2464
	//Node: Free 0
	m.dump();


	cout << "Deallocating the first and third block" << endl;
	m.deallocate(p_1);
	m.deallocate(p_3);
	//p_1 -> Node: Free 2464
	//p_2 -> Node: Used 2464
	//p_3 -> Node: Free 2464
	//p_4 -> Node: Used 2464
	//Node: Free 0
	m.dump();


	cout << "Allocating 1000 bytes, should be allocated in the first slot" << endl;
	p_1 = (char*)m.allocate(1000);
	//p_1 -> Node: Used 1000
	//Node: Free 1440
	//p_2 -> Node: Used 2464
	//p_3 -> Node: Free 2464
	//p_4 -> Node: Used 2464
	//Node: Free 0
	m.dump();

	cout << "Allocating 200 bytes, should be allocated in the secon slot (it is the best fit slot)" << endl;
	p_3 = (char*)m.allocate(200);
	//p_1 -> Node: Used 1000
	//p_3 -> Node: Used 200
	//Node: Free 1216
	//p_2 -> Node: Used 2464
	//Node: Free 2464
	//p_4 -> Node: Used 2464
	//Node: Free 0
	m.dump();

	cout << "Coalescing right" << endl;
	//attempt coalesce right
	m.deallocate(p_3);
	//p_1 -> 1000
	//Node: Free 1440
	//p_2 -> Node: Used 2464
	//Node: Free 2464
	//p_4 -> Node: Used 2464
	//Node: Free 0
	m.dump();

	cout << "Coalescing left" << endl;
	//attempt to coalesce left
	m.deallocate(p_4);
	//p_1 -> 1000
	//Node: Free 1440
	//p_2 -> Node: Used 2464
	//Node: Free 4976
	m.dump();

	cout << "Coalescing right and left" << endl;
	//attempt to coalesce right and left
	m.deallocate(p_2);
	//p_1 -> 1000
	//Node: Free 8928
	m.dump();

	cout << "Clean up the last block" << endl;
	m.deallocate(p_1);
	//Node: Free 9952
	m.dump();

	//reallocate the four equal sized blocks
	p_1 = (char*)m.allocate(2464);
	p_2 = (char*)m.allocate(2464);
	p_3 = (char*)m.allocate(2464);
	p_4 = (char*)m.allocate(2464);

	cout << "Attempting to deallocate a very bad address" << endl;
	//attempt to delete something way outside the bounds of the array (should have no effect)
	m.deallocate((void*)2);
	m.dump();

	cout << "Attempting to deallocate an address that is between nodes" << endl;
	//attempt to delete a bad address within the array of bytes (should have no effect)
	m.deallocate(p_1 + 100);
	m.dump();

	cout << "Attempting to deallocate the same pointer twice" << endl;
	//deallocate the same thing twice
	m.deallocate(p_1);
	m.deallocate(p_1);
	m.dump();

	cout << "Attempt to allocate where there is room for the data but not the data and the node" << endl;
	if (m.allocate(2464) != 0)
	{
		cout << "Error:: not enough room for this allocation" << endl;
	}
	m.dump();

	cout << "2440 is the smallest chunk that can be allocated in 2464 bytes" << endl;
	m.allocate(2440);
	m.dump();
}
//--
void randomInsertsDeletes()
{
	cout << "******************************" << endl;
	cout << "* Random inserts and deletes *" << endl;
	cout << "******************************" << endl;

	//size of the managed heap
	const int heapSize = 10000;

	//create 10000 bytes of heap space (memory is actually a char pointer)
	char memory[heapSize];

	//let the memory manager know which chunk of memory it is managing
	MemoryManager m(memory, heapSize);

	//holds a series of allocated pointers
	vector < char* > randomPointers;

	//repeatedly exercise the memory manager
	for (int i = 0; i < 100; i++)
	{
		//choose a random number
		int randomSize = rand() % 1000;

		cout << "Attempting to Allocate " << randomSize << " bytes" << endl;

		//attempt to allocate the random size
		char* p_random = (char*)m.allocate(randomSize);

		//if the allocate was successful
		if (p_random != 0)
		{
			randomPointers.push_back(p_random);

			//zero out the allocated memory
			for (int j = 0; j < randomSize; j++)
			{
				p_random[j] = 0;
			}

			cout << "Successful Allocate" << endl;

		}
		else //not enough room for allocate
		{
			cout << "Unsuccessful Allocate" << endl;

			if (!randomPointers.empty())
			{
				//choose a random element in the array of pointers and remove it
				int randPos = rand() % randomPointers.size();
				char* p_delete = randomPointers[randPos];
				randomPointers.erase(randomPointers.begin() + randPos);

				cout << "Attempting to Deallocate to make some room" << endl;

				//deallocate to make room
				m.deallocate(p_delete);

				cout << "Successful Deallocate" << endl;
			}
		}

		m.dump();
	}
}

void numberGuesser()
{
	char memory[500];
	MemoryManager m(memory, 500);
	int* p_min = (int*)m.allocate(sizeof(int));
	int* p_guess = (int*)m.allocate(sizeof(int));
	int* p_max = (int*)m.allocate(sizeof(int));
	int* p_range = (int*)m.allocate(sizeof(int));
	char* p_ynVar = (char*)m.allocate(sizeof(char) * 2);
	p_min[0] = 1;
	p_max[0] = 1000;
	p_range[0] = p_max[0] - p_min[0];
	p_guess[0] = rand() % p_range[0];
	p_ynVar[0] = 'n';
	p_ynVar[1] = 'l';
	cout << "Is your number: " << p_guess[0] << endl;
	cout << "Y or N" << endl;
	cin >> p_ynVar[0];
	while (p_ynVar[0] == 'n' || p_ynVar[0] == 'N')
	{
		cout << "Is your number higher (H) or lower (L)?" << endl;
		cin >> p_ynVar[1];
		if(p_ynVar[1] == 'H'||p_ynVar[1] == 'h')
		{ 
			p_min[0] = p_guess[0];
		}
		else if (p_ynVar[1]=='L'||p_ynVar[1]=='l')
		{
			p_max[0] = p_guess[0];
		}
		p_range[0] = (p_max[0] - p_min[0]);
		p_guess[0] = (rand() % p_range[0]) + p_min[0];

		cout << "Is your number: " << p_guess[0] << endl;
		cout << "Y or N" << endl;
		cin >> p_ynVar[0];
	}
	cout << "Your number is: " << p_guess[0] << endl;


	m.deallocate(p_min);
	m.deallocate(p_max);
	m.deallocate(p_guess);
	m.deallocate(p_ynVar);

}
