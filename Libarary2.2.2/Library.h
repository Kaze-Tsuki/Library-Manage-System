#ifndef Libarary_h
#define Libarary_h

#include <SFML/Graphics.hpp>
#include "input.h"
#include <thread>
#include <iostream>
#include <vector>
#include <atomic>

using namespace std;

typedef struct Date {
	int year, month, day;
	atomic<bool> changing;
	Date();
	void change_date();
	void print_date();
	string getString();
}Date;

class Book {
private:
	string name;
	string author;
	Date published, due;
	string ISBN;
	string category;
	size_t copyAmount;
	bool is_available;
	atomic<bool> changing;
public:
	Book();
	void change();
	void print_book();
};


typedef struct User {
	string name;
	vector<Book> history;
	vector<Book> lending;
	User(string& name);
	void borrow();
	void giveBack();
	void checkDue();
}User;

#endif