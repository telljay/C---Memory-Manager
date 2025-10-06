#include <iostream>
#include "MemoryManager.h"

using namespace std;

MemoryManager::MemoryManager(char* memory, int size)
{
	p_memory = memory;

	p_head = (Node*)p_memory;
	p_tail = (Node*)(p_memory + size - sizeof(Node));
	//creates a start and end to the block of data

	p_head->isOpen = true;
	p_tail->isOpen = false;
	//makes the head open and the tail closed

	p_head->p_next = p_tail;
	p_tail->p_next = 0;

	p_head->p_prev = 0;
	p_tail->p_prev = p_head;

	p_head->sizeOfBlock = size - (2*(sizeof(Node)));
	p_tail->sizeOfBlock = 0;
	// marks everything accordingly for the head and tail

	sizeOfMemory = size;
}
//--

MemoryManager::~MemoryManager()
{
}
//--

void MemoryManager::dump()
{
	//announces the data allocated and if it is used
	Node* p_current = p_head;
	//moves the current node to the start
	int blockNumber = 1;
	//begins looping through all of the blocks
	while (p_current->sizeOfBlock != 0)
	{
		cout << "Block" << blockNumber << ": " << p_current->sizeOfBlock;
		if (p_current->isOpen)
		{
			//if the block is open announce that it is and how much is free
			cout << " bytes of free space" << endl;
		}
		else
		{
			//if blocked is closed announce that it is and how much is used
			cout << " bytes of used space" << endl;
		}
		p_current = p_current->p_next;
		blockNumber++;
	}
}
//--

void* MemoryManager::allocate(int numOfBytes)
{
	int oldBytes;
	Node* p_bestFit = (Node*)bestFit(numOfBytes);
	//making current the best fit
	if (p_bestFit == 0)
	{
		return p_bestFit = 0;
	}
	else
	{
		Node* p_newNode = (Node*)((char*)p_bestFit + sizeof(Node) + numOfBytes);
		//finding the location in memory for newNode 

		p_newNode->p_prev = p_bestFit;
		p_bestFit->p_next->p_prev = p_newNode;
		p_newNode->p_next = p_bestFit->p_next;
		p_bestFit->p_next = p_newNode;
		//adjusting the pointers

		p_bestFit->isOpen = false;
		p_newNode->isOpen = true;
		oldBytes = p_bestFit->sizeOfBlock;
		p_bestFit->sizeOfBlock = numOfBytes;
		p_newNode->sizeOfBlock = oldBytes - numOfBytes - sizeof(Node);
	}
	//change sizes

	return (void*)((char*)p_bestFit + sizeof(Node));
}
//--

void* MemoryManager::deallocate(void* locInData)
{
	Node* currentNode = p_head;
	while (currentNode != p_tail)
	{
		if ((char*)currentNode + sizeof(Node) == locInData)
		{
			currentNode->isOpen = true;
			if (currentNode != p_head)
			{
				currentNode = (Node*)CoalessLeft(currentNode);
			}
			CoalessRight(currentNode);
			break;
		}
		else
		{
			currentNode = currentNode->p_next;
		}
	}
	return (void*)currentNode;
}
//--

void* MemoryManager::bestFit(const int& bytes)
{
	Node* p_current = p_head;
	Node* bestFit=0;
	int bestSize = sizeOfMemory;
	while (p_current != p_tail)
	{
		if (p_current->isOpen && p_current->sizeOfBlock >= bytes + sizeof(Node))
		{
			if (bestSize > p_current->sizeOfBlock)
			{
				bestFit = p_current;
				bestSize = p_current->sizeOfBlock;
			}
		}
		p_current = p_current->p_next;
	}
	return (void*)bestFit;
}
//--

void MemoryManager::CoalessRight(Node* p_current)
{
	if (p_current->p_next->isOpen)
	{
		p_current->sizeOfBlock = p_current->sizeOfBlock + p_current->p_next->sizeOfBlock + sizeof(Node);
		p_current->p_next->p_next->p_prev = p_current;
		p_current->p_next = p_current->p_next->p_next;
	}
	if (p_current->p_next->isOpen)
	{
		CoalessRight(p_current);
	}
}
//--
void* MemoryManager::CoalessLeft(Node* p_current)
{
	if (p_current->p_prev->isOpen)
	{
		p_current->p_prev->p_next = p_current->p_next;
		p_current->p_next->p_prev = p_current->p_prev;
		p_current->p_prev->sizeOfBlock = p_current->p_prev->sizeOfBlock + p_current->sizeOfBlock + sizeof(Node);
		p_current = p_current->p_prev;
	}
	if (p_current != p_head && p_current->p_prev->isOpen)
	{
		p_current = (Node*)CoalessLeft(p_current->p_prev);
	}
	return (void*)p_current;
}
//--
