#ifndef Libarary_h
#define Libarary_h

#include <SFML/Graphics.hpp>
#include "input.h"
#include <thread>
#include <iostream>
#include <vector>

using namespace std;

typedef struct Date {
	int year, month, day;
	Date();
	void change_date();
	void print_date();
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
public:
	Book();
	Book change();
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