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
	void input_date();
	void change_date();
}Date;

typedef struct User {
	string name;
	vector<Book> history;
	vector<Book> lending;
	User();
	void borrow();
	void giveBack();
	void checkDue();
}User;

class Book {
private:
	string name;
	string author;
	Date published, borrow;
	string ISBN;
	string category;
	size_t copyId;
	bool is_available;
public:
	Book();
	Book init();
	Book change();
};

#endif