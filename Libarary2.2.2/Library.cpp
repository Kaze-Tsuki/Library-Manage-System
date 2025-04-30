#include "input.h"
#include "Library.h"
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <exception>

using namespace std;

void Library::addBook(Book* book) {
    // Check if the book is valid
   if (!book->valid()) {
       cout << "Book is not valid." << endl;
       return;
   }

   // Check if the book already exists
   bool exists = any_of(books.begin(), books.end(), [&](Book* b) {
       return *b == *book;
   });
   if (exists) {
       cout << "Book already exists." << endl;
       return;
   }

   books.push_back(book);
}

void Library::borrowBook(Book& book) {
	// Must exists
	string name;

	sf::RectangleShape nameBox(sf::Vector2f(200, 50));
	sf::Text nameText("Name", font, 20);
	initButton(nameBox, nameText, 50, 50); // onpressed inputBox()

	// ...
}

void Library::giveBackBook(Book& book) {
	// Must exists
	// ...
}

void Library::printBooks(){
	if (books.empty()) {
		cout << "No books in the library." << endl;
		return;
	}
	// using a new window
	sf::RenderWindow window(sf::VideoMode(800, 400), "Books");

	size_t curBook = 0;
	// left side book, right side button(not implented)
	
	// curpage / totalpage
	sf::Text pageText("Page: " + to_string(curBook + 1) + "/" + to_string(books.size()), font, 18);
	pageText.setFillColor(sf::Color::Black);
	pageText.setPosition(35, 5);

	// next
	sf::RectangleShape nextBtn(sf::Vector2f(100, 70));
	sf::Text nextBtnText("Next", font, 20);
	initButton(nextBtn, nextBtnText, 500, 35);

	// prev
	sf::RectangleShape prevBtn(sf::Vector2f(100, 70));
	sf::Text prevBtnText("Prev", font, 20);
	initButton(prevBtn, prevBtnText, 350, 35);

	// borrow
	sf::RectangleShape borrowBtn(sf::Vector2f(100, 70));
	sf::Text borrowBtnText("Borrow", font, 20);
	initButton(borrowBtn, borrowBtnText, 500, 150);

	// give back
	sf::RectangleShape giveBackBtn(sf::Vector2f(100, 70));
	sf::Text giveBackBtnText("Give Back", font, 20);
	initButton(giveBackBtn, giveBackBtnText, 350, 150);

	// Display books
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			// ·Æ¹«ÂIÀ»¿é¤J®Ø
			if (event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == sf::Mouse::Left)
			{

				sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
				if (nextBtn.getGlobalBounds().contains(mousePos)) {
					if (curBook < books.size() - 1) {
						curBook++;
						pageText.setString("Page: " + to_string(curBook + 1) + "/" + to_string(books.size()));
					}
				}
				else if (prevBtn.getGlobalBounds().contains(mousePos)) {
					if (curBook > 0) {
						curBook--;
						pageText.setString("Page: " + to_string(curBook + 1) + "/" + to_string(books.size()));
					}
				}
				else if (borrowBtn.getGlobalBounds().contains(mousePos)) {
					// borrow
					// ...
				}
				else if (giveBackBtn.getGlobalBounds().contains(mousePos)) {
					// give back
					// ...
				}
			}
		}
		window.clear(sf::Color(200, 200, 200));
		// Display book
		books[curBook]->display(window, 35, 35);

		renderShape(window, { &nextBtn, &prevBtn, &borrowBtn, &giveBackBtn,
			&nextBtnText, &prevBtnText, &borrowBtnText, &giveBackBtnText , &pageText });
		window.display();
	}
}

void Library::printUsers() {
	// using a new window
	
	// ...
}

void Library::rearrangeBooks() {
	// using a new window
	// ...
}