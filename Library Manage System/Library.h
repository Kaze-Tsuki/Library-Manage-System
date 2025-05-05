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
	string getString() const;
	void operator=(const Date& other);
	bool operator<(const Date& other);
	bool operator>(const Date& other);
}Date;

extern Date today; // Global variable for today's date

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
	Book(const Book& other);
	bool change();
	void print_book();
	void display(sf::RenderWindow& window, int x, int y);
	void displayBrief(sf::RenderWindow& window, int x, int y);
	void displayFUser(sf::RenderWindow& window, int x, int y);
	bool valid() const;
	bool operator==(const Book& other) const;
	void operator=(const Book& other);
};


typedef struct User {
	string name;
	vector<Book> borrowing;
	User(string& name);
	User();
	void displayBrief(sf::RenderWindow& window, int x, int y);
	void borrow();
	void giveBack();
	void checkDue();
}User;

class Library {
private:
	vector<Book*> books;
	vector<User> users;
public:
	vector<Book*> getBooks() { return books; }
	vector<User> getUsers() { return users; }
	void addBook(Book* book);
	void addUser(const User& user);
	void borrowBook(Book* book);// use a new window
	void giveBackBook(Book* book); // use a new window
	void printBooks(const size_t& start); // use a new window
	void printUser(User& user); // use a new window
	void rearrangeBooks(vector<Book*>&); // use a new window
	void listBooks(); // use a new window
	void listUsers(); // use a new window
	void userBorrowedBook(User& user); // use a new window
	~Library();
};

#endif