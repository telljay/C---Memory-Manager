#pragma once
class MemoryManager
{
public:
	MemoryManager(char*, int);
	~MemoryManager();
	void dump();
	void* allocate(int numOfBytes);
	void* deallocate(void* locInData);
private:
	struct Node
	{
		int sizeOfBlock;
		bool isOpen;
		Node* p_prev;
		Node* p_next;
	};
	//=to 24bites
	void* bestFit(const int& bytes);
	void CoalessRight(Node*);
	void* CoalessLeft(Node*);
	char* p_memory;
	Node* p_head;
	Node* p_tail;
	int sizeOfMemory;
};

