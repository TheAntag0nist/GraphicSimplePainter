#pragma once

template <class T>
struct node {
	T data;

	struct node* next;

	node() {
		next = NULL;
	}
};

template <class T>
class Stack {
private:
	node<T>* up_node;
	int stack_size;

	// create node functions
	node<T>* create_node() {
		// allocate mem
		node<T>* tmp = new node<T>;

		return tmp;
	}
	node<T>* create_node(node<T> data) {
		// allocate mem
		node<T>* tmp = new node<T>;

		// save data
		*tmp = data;

		return tmp;
	}
	node<T>* create_node(T& data) {
		// allocate mem
		node<T>* tmp = new node<T>;

		// save data
		tmp->data = data;

		return tmp;
	}
public:
	// constructor
	Stack() {
		stack_size = 0;
		up_node = nullptr;
	}
	
	// push node
	void push_node(T data) {
		node<T>* tmp = up_node;

		up_node = create_node(data);
		up_node->next = tmp;

		stack_size++;
	}

	T pop_node() {
		node<T>* tmp_pnt = up_node;
		T tmp = up_node->data;

		up_node = up_node->next;

		delete tmp_pnt;

		stack_size--;

		return tmp;
	}

	bool isEmpty() {
		return (stack_size <= 0) ? 1 : 0;
	}
};