#pragma once

template<typename T>
struct Element
{
	Element * next;
	Element * previous;

	T * value;
};

template <typename T>
struct Linked_List
{
	int count;
	Element * head;
	Element * tail;

	Element * insert(T * t);
	T * remove(Element * element);
};

template <typename T>
Linked_List<T> create_linked_list();