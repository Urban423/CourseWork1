#pragma once
#include "Prerequisites.h"

template<class T>
class list;

template<class T>
class listElement
{
public:
	 listElement(T* element);
	~listElement();
	
	T* get();
private:
	listElement<T>* next = nullptr;
	T* element;
	
	friend class list<T>;
};


template<class T>
class list
{
public:
	 list();
	~list();
	
	void pop(uint index);
	void sort(bool (*comarator)(T&, T&));
	void begin();
	uint size();
	T* add(const T& element);
	T* get();
public:
	list<T> operator++(int);
private:
	listElement<T>* listmerge(listElement<T>* a, listElement<T>* b, bool (*comarator)(T&, T&));
	void merge(listElement<T>** head, bool (*comarator)(T&, T&));
private:
	listElement<T>* head = nullptr;
	uint _size = 0;
	
	listElement<T>* pointer = head;
};


#include "List.imp"