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

void Library::borrowBook(Book* book) {
	if (book->changing.exchange(true)) {
		cout << "System Busy...\n";
		return;
	}
	// Must exists
	string username;
	Date due;

	sf::RenderWindow window(sf::VideoMode(800, 400), "Borrow Book");

	// Input username
	sf::RectangleShape inputName(sf::Vector2f(200, 50));
	sf::Text NameLabel("Name", font, 20);
	sf::Text inputNameText("Name", font, 24);
	initInputBox(inputName, inputNameText, NameLabel, 50, 50);

	// Due date button
	sf::RectangleShape DueDate_btn(sf::Vector2f(170, 70));
	sf::Text DueDateText("Due Date\n   Year", font, 20);
	initButton(DueDate_btn, DueDateText, 300, 50);


	// Submit
	sf::RectangleShape submit_btn(sf::Vector2f(100, 50));
	submit_btn.setFillColor(sf::Color::Green);
	submit_btn.setOutlineColor(sf::Color::Black);
	submit_btn.setOutlineThickness(2);
	submit_btn.setPosition(50, 250);
	sf::Text submit_btn_innerText("Submit", font, 24);
	submit_btn_innerText.setFillColor(sf::Color::Black);
	set_mid(submit_btn, submit_btn_innerText);

	bool inname = false;// 是否正在輸入
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			// 滑鼠點擊輸入框
			if (event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
				if (inputName.getGlobalBounds().contains(mousePos)) {
					inname = true;
				}
				else if (submit_btn.getGlobalBounds().contains(mousePos)) {
					book->availableCopies--;
					window.close();
				}
				else if (DueDate_btn.getGlobalBounds().contains(mousePos)) {
					inname = false;
					thread(&Date::change_date, &due).detach();
				}
			}

			if (inname) {
				inputEvent(event, username, 'A', 'z', 20);
				inputNameText.setString(username); // 更新畫面上的文字
			}
			this_thread::sleep_for(std::chrono::milliseconds(5));
		}
		window.clear(sf::Color(200, 200, 200));

		renderShape(window, { &inputName, &submit_btn , &NameLabel, &submit_btn_innerText,
			&inputNameText , &DueDate_btn , &DueDateText });

		window.display();
	}

	// Find user
	User user(username);
	auto it = find_if(users.begin(), users.end(), [&](User& u) {
		return u.name == user.name;
		});
	if (it != users.end()) {
		// User exists
		it->borrowing.push_back(book);
		it->borrowing.back()->due = due;
		it->borrowing.back()->changing.store(false);
	}
	else {
		// User does not exist
		user.borrowing.push_back(book);
		users.push_back(user);
	}
	book->changing.store(false);
	return;
}

void Library::giveBackBook(Book* book) {
	// Must exists
	// ...
}

void Library::printBooks(const size_t& start){
	if (books.empty()) {
		cout << "No books in the library." << endl;
		return;
	}
	// using a new window
	sf::RenderWindow window(sf::VideoMode(800, 400), "Books");
	window.setFramerateLimit(20);

	size_t curBook = start;
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
			// 滑鼠點擊輸入框
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
					borrowBook(books[curBook]);
				}
				else if (giveBackBtn.getGlobalBounds().contains(mousePos)) {
					// give back
					// ...
				}
			}
			this_thread::sleep_for(std::chrono::milliseconds(30));
		}
		window.clear(sf::Color(200, 200, 200));
		books[curBook]->display(window, 35, 35);
		renderShape(window, { &nextBtn, &prevBtn, &borrowBtn, &giveBackBtn,
			&nextBtnText, &prevBtnText, &borrowBtnText, &giveBackBtnText , &pageText });
		window.display();
	}
}

void Library::listBooks() {
	// One page with ten lines of brief book info
	// each line of info is a button (a button with lower layer), occupying the whole line
	// bottom side two buttons to go to next page and prev page

	sf::RenderWindow window(sf::VideoMode(950, 820), "Books");
	window.setFramerateLimit(30);

	// button[10]
	sf::RectangleShape bookBtn[10];
	for (int i = 0; i < 10; i++) {
		bookBtn[i].setSize(sf::Vector2f(850, 60));
		bookBtn[i].setFillColor(sf::Color(200, 200, 200));
		bookBtn[i].setOutlineColor(sf::Color::Black);
		bookBtn[i].setOutlineThickness(1);
		bookBtn[i].setPosition(50, 50 + i * 60);
	}
	// text will be rendered by book
	// next page
	sf::RectangleShape nextBtn(sf::Vector2f(100, 70));
	sf::Text nextBtnText("Next", font, 20);
	initButton(nextBtn, nextBtnText, 700, 730);
	// prev
	sf::RectangleShape prevBtn(sf::Vector2f(100, 70));
	sf::Text prevBtnText("Prev", font, 20);
	initButton(prevBtn, prevBtnText, 200, 730);
	// Display page
	sf::Text pageText(to_string(1) + "/" + to_string((books.size() + 9) / 10), font, 18);
	pageText.setFillColor(sf::Color::Black);
	sf::FloatRect textBounds = pageText.getLocalBounds();
	pageText.setOrigin(textBounds.left + textBounds.width / 2.0f,
		textBounds.top + textBounds.height / 2.0f);
	pageText.setPosition(500, 750);


	// Display books
	size_t curPage = 0;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			// 滑鼠點擊輸入框
			if (event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
				if (nextBtn.getGlobalBounds().contains(mousePos)) {
					if ((curPage+1) * 10 < books.size()) {
						curPage++;
						pageText.setString(to_string(curPage + 1) + "/" + to_string((books.size() + 9) / 10));
					}
				}
				else if (prevBtn.getGlobalBounds().contains(mousePos)) {
					if (curPage > 0) {
						curPage--;
						pageText.setString(to_string(curPage + 1) + "/" + to_string((books.size() + 9) / 10));
					}
				}
				else {
					for (int i = 0; i < 10; i++) {
						if (bookBtn[i].getGlobalBounds().contains(mousePos)) {
							thread(&Library::printBooks, this, i + curPage * 10).detach();
						}
					}
				}
			}
			this_thread::sleep_for(std::chrono::milliseconds(30));
		}
		// Render
		window.clear(sf::Color(200, 200, 200));

		for (int i = 0; i < 10; i++) {
			window.draw(bookBtn[i]);
			if (i + curPage * 10 < books.size())
			try {
				books.at(i + curPage * 10)->displayBrief(window, 50, 50 + i * 60);
			}
			catch (const std::out_of_range& e) {
				// 如果越界可以跳過或記錄錯誤
				std::cerr << "Conflict of multi tasking: " << e.what() << std::endl;
			}
		}
		renderShape(window, { &nextBtn, &prevBtn,
			&nextBtnText, &prevBtnText, &pageText });

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

Library::~Library() {
	for (auto book : books) {
		delete book;
	}
}