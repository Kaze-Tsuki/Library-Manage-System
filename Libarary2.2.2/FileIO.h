#ifndef FileIO_h
#define FileIO_h

#include "Library.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

void Load(Library& library, const string& filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "Failed to open file: " << filename << endl;
		return;
	}
	string line;
	while (getline(file, line)) {
		stringstream ss(line);
		Book book;
		ss >> book.name >> book.author >> book.ISBN >> book.category
			>> book.published.year >> book.published.month >> book.published.day
			>> book.copyAmount >> book.availableCopies;
		library.addBook(new Book(book));
	}
	file.close();
}

void Save(const Library& library, const string& filename) {
	ofstream file(filename);
	if (!file.is_open()) {
		cout << "Failed to open file: " << filename << endl;
		return;
	}
	for (const auto& book : library.getBooks()) {
		file << book->name << " " << book->author << " "
			<< book->ISBN << " " << book->category << " "
			<< book->published.year << " "
			<< book->published.month << " "
			<< book->published.day << " "
			<< book->copyAmount << " "
			<< book->availableCopies << endl;
	}
	file.close();
}

#endif // !FileIO_h

