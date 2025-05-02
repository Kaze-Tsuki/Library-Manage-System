#ifndef Library_h
#define Library_h

#include <SFML/Graphics.hpp>
#include "input.h"
#include <thread>
#include <iostream>
#include <vector>
#include <atomic>
#include <Windows.h>

using namespace std;

typedef struct Date {
	int year, month, day;
	atomic<bool> changing;
	Date();
	Date(int y, int m, int d);
	void change_date();
	void print_date() const;
	string getString() const;
	void operator=(const Date& other);
	bool operator<(const Date& other);
	bool operator>(const Date& other);
}Date;

class Book {
public:
	string name;
	string author;
	Date published, due; // due is a attribute of a borrowed book
	string ISBN;
	string category;
	size_t copyAmount, availableCopies;
	atomic<bool> changing;
	Book();
	void change();
	void print_book();
	void display(sf::RenderWindow& window, int x, int y);
	void displayBrief(sf::RenderWindow& window, int x, int y);
	bool valid() const;
	bool operator==(const Book& other) const;
};


typedef struct User {
	string name;
	vector<Book*> history;
	vector<Book*> borrowing;
	User(string& name);
	void borrow();
	void giveBack();
	void checkDue();
}User;

class Library {
private:
	vector<Book*> books;
	vector<User> users;
public:
	void addBook(Book* book);
	void borrowBook(Book* book);// use a new window
	void giveBackBook(Book* book); // use a new window
	void printBooks(const size_t& start); // use a new window
	void printUsers(); // use a new window
	void rearrangeBooks(vector<Book*>&); // use a new window
	void listBooks(); // use a new window
	~Library();
};

#endif