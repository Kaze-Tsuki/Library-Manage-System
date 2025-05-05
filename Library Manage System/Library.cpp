#include "input.h"
#include "Library.h"
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <exception>
#include "sort.h"

using namespace std;

void Library::addBook(Book* book) {
    // Check if the book is valid
   if (!book->valid()) {
	   thread([]() { errorWindow("Invalid book information."); }).detach();
	   delete book; // Free the memory allocated for the book
       return;
   }

   // Check if the book already exists
   bool exists = any_of(books.begin(), books.end(), [&](Book* b) {
       return *b == *book;
   });
   if (exists) {
	   thread([]() { errorWindow("Book already exists."); }).detach();
       return;
   }

   books.push_back(book);
}

// for loading files
void Library::addUser(const User& user) {
	for (const auto& u : users) {
		if (u.name == user.name) {
			thread([]() { errorWindow("User already exists."); }).detach();
			return;
		}
	}
	users.push_back(user);
}

void Library::borrowBook(Book* book) {
	if (book->availableCopies == 0) {
		thread([]() { errorWindow("No copies available."); }).detach();
		return;
	}
	if (book->changing.exchange(true)) {
		thread([]() { errorWindow("Already running input text window."); }).detach();
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
	submit_btn.setFillColor(sf::Color(168, 255, 255));
	submit_btn.setOutlineColor(sf::Color(167, 167, 211));
	submit_btn.setOutlineThickness(2);
	submit_btn.setPosition(50, 250);
	sf::Text submit_btn_innerText("Submit", font, 24);
	submit_btn_innerText.setFillColor(sf::Color::Black);
	set_mid(submit_btn, submit_btn_innerText);

	bool inname = false;
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
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
				inputEvent(event, username, ' ', 'z', 20);
				inputNameText.setString(username); // 更新畫面上的文字
			}
			this_thread::sleep_for(std::chrono::milliseconds(5));
		}
		window.clear(sf::Color(210, 233, 233));

		renderShape(window, { &inputName, &submit_btn , &NameLabel, &submit_btn_innerText,
			&inputNameText , &DueDate_btn , &DueDateText });

		window.display();
	}

	// Find user
	User user(username);
	auto it = my_find_if(users.begin(), users.end(), [&](User& u) {
		return u.name == user.name;
		});
	if (it != users.end()) {
		// User exists
		it->borrowing.emplace_back(*book);
		it->borrowing.back().due = due;
	}
	else {
		// User does not exist
		user.borrowing.emplace_back(*book);
		users.push_back(user);
	}
	book->changing.store(false);
	return;
}

void Library::giveBackBook(Book* book) {
	// Input username
	struct inputText input;
	string username; input.OpenInputText(username);
	// Find user
	auto it = my_find_if(users.begin(), users.end(), [&](User& u) {
		return u.name == username;
		});
	if (it != users.end()) {
		// User exists
		auto bookIt = my_find(it->borrowing.begin(), it->borrowing.end(), *book);
		if (bookIt != it->borrowing.end()) {
			it->borrowing.erase(bookIt);
			book->availableCopies++;
		}
		else {
			thread([]() { errorWindow("Book not borrowed by user."); }).detach();
		}
	}
	else {
		thread([]() { errorWindow("User not found."); }).detach();
	}
}

void Library::printBooks(const size_t& start){
	if (books.empty()) {
		thread([]() { errorWindow("No books available."); }).detach();
		return;
	}
	// using a new window
	sf::RenderWindow window(sf::VideoMode(650, 280), "Books");
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

	// change properties
	sf::RectangleShape changeBtn(sf::Vector2f(60, 30));
	sf::Text changeBtnText("Change", font, 16);
	initButton(changeBtn, changeBtnText, 35, 215);

	// Display books
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
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
					thread(&Library::borrowBook, this, books[curBook]).detach();
				}
				else if (giveBackBtn.getGlobalBounds().contains(mousePos)) {
					// give back
					thread(&Library::giveBackBook, this, books[curBook]).detach();
				}
				else if (changeBtn.getGlobalBounds().contains(mousePos)) {
					// change book properties
					thread(&Library::changeAvailableCopies, this, books[curBook]).detach();
				}
			}
			this_thread::sleep_for(std::chrono::milliseconds(30));
		}
		window.clear(sf::Color(210, 233, 233));
		books[curBook]->display(window, 35, 35);
		renderShape(window, { &nextBtn, &prevBtn, &borrowBtn, &giveBackBtn,
			&nextBtnText, &prevBtnText, &borrowBtnText, &giveBackBtnText , &pageText,
			&changeBtn, &changeBtnText });
		window.display();
	}
}

void Library::changeAvailableCopies(Book* book)
{
	if (book->changing.exchange(true)) {
		thread([]() { errorWindow("Already running input text window."); }).detach();
		return;
	}
	sf::RenderWindow window(sf::VideoMode(400, 150), "Change Book Amount");
	window.setFramerateLimit(20);
	// copyAmount
	sf::RectangleShape inputcopyAmount(sf::Vector2f(150, 50));
	sf::Text copyAmounttxt("Copies", font, 20);
	sf::Text inputcopyAmountText(to_string(book->copyAmount), font, 24);
	initInputBox(inputcopyAmount, inputcopyAmountText, copyAmounttxt, 50, 50);
	// Submit
	sf::RectangleShape submit_btn(sf::Vector2f(100, 50));
	submit_btn.setFillColor(sf::Color(168, 255, 255));
	submit_btn.setOutlineColor(sf::Color(167, 167, 211));
	submit_btn.setOutlineThickness(2);
	submit_btn.setPosition(230, 50);
	sf::Text submit_btn_innerText("Submit", font, 24);
	submit_btn_innerText.setFillColor(sf::Color::Black);
	set_mid(submit_btn, submit_btn_innerText);

	bool incopyAmount = false;// 是否正在輸入
	string scopyAmount(to_string(book->copyAmount));
	if (scopyAmount == "0") scopyAmount = "";

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			// 滑鼠點擊輸入框
			if (event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
				if (inputcopyAmount.getGlobalBounds().contains(mousePos)) {
					incopyAmount = true;
				}
				else if (submit_btn.getGlobalBounds().contains(mousePos)) {
					incopyAmount = false;
					if (scopyAmount == "") scopyAmount = "1";
					int newamount = max(min(stoi(scopyAmount), 1000000), 0);
					if (book->changeAmount(newamount))
						window.close();
				}
				else {
					incopyAmount = false;
				}
			}
			// input copyAmount
			if (incopyAmount) {
				inputEvent(event, scopyAmount, '0', '9', 4);
				inputcopyAmountText.setString(scopyAmount); // 更新畫面上的文字
			}
		}
		window.clear(sf::Color(210, 233, 233));
		renderShape(window, { &inputcopyAmount, &submit_btn , &copyAmounttxt, &submit_btn_innerText, &inputcopyAmountText
			});
		window.display();
	}
	book->changing.store(false);
}

void Library::listBooks() {
	// One page with ten lines of brief book info
	// each line of info is a button (a button with lower layer), occupying the whole line
	// bottom side two buttons to go to next page and prev page

	sf::RenderWindow window(sf::VideoMode(950, 820), "Books");
	window.setFramerateLimit(15);

	// button[10]
	sf::RectangleShape bookBtn[10];
	for (int i = 0; i < 10; i++) {
		bookBtn[i].setSize(sf::Vector2f(850, 60));
		bookBtn[i].setFillColor(sf::Color(209, 233, 255));
		bookBtn[i].setOutlineColor(sf::Color(167, 167, 211));
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
	// change filter condition
	sf::RectangleShape filterBtn(sf::Vector2f(100, 70));
	sf::Text filterBtnText("Filter", font, 20);
	initButton(filterBtn, filterBtnText, 50, 730);

	// Display page
	sf::Text pageText(to_string(1) + "/" + to_string((books.size() + 9) / 10), font, 18);
	pageText.setFillColor(sf::Color::Black);
	sf::FloatRect textBounds = pageText.getLocalBounds();
	pageText.setOrigin(textBounds.left + textBounds.width / 2.0f,
		textBounds.top + textBounds.height / 2.0f);
	pageText.setPosition(500, 750);


	// Display books
	atomic<size_t> curPage = 0;
	vector<Book*> present = books;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			// 
			if (event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
				if (nextBtn.getGlobalBounds().contains(mousePos)) {
					if ((curPage+1) * 10 < present.size()) {
						curPage++;
						pageText.setString(to_string(curPage + 1) + "/" + to_string((present.size() + 9) / 10));
					}
				}
				else if (prevBtn.getGlobalBounds().contains(mousePos)) {
					if (curPage > 0) {
						curPage--;
						pageText.setString(to_string(curPage + 1) + "/" + to_string((present.size() + 9) / 10));
					}
				}
				else if (filterBtn.getGlobalBounds().contains(mousePos)) {
					// filter
					thread([&]() {
						// filter books
						present.clear();
						present = books;
						rearrangeBooks(present);
						curPage = 0;
						// update page text
						pageText.setString(to_string(curPage + 1) + "/" + to_string((present.size() + 9) / 10));
					}).detach();
					
				}
				else {
					for (int i = 0; i < 10; i++) {
						// on click on button[i] div
						if (i + curPage * 10 < present.size() && bookBtn[i].getGlobalBounds().contains(mousePos)) {
							// find the book index in the books vector
							size_t bookIndex = my_find_if(books.begin(), books.end(), [&](Book* b) {
								return b == present.at(i + curPage * 10);
								}) - books.begin();
							thread(&Library::printBooks, this, bookIndex).detach();
						}
					}
				}
			}
			this_thread::sleep_for(std::chrono::milliseconds(20));
		}
		// Render
		window.clear(sf::Color(210, 233, 233));

		for (int i = 0; i < 10; i++) {
			if (i + curPage * 10 < present.size())
			try {
				window.draw(bookBtn[i]);
				present.at(i + curPage * 10)->displayBrief(window, 50, 50 + i * 60);
			}
			catch (const std::out_of_range& e) {
				// Handle the out of range exception
				std::cerr << "Conflict of multi tasking: " << e.what() << std::endl;
			}
			else break;
		}
		renderShape(window, { &nextBtn, &prevBtn, &filterBtn, &filterBtnText,
			&nextBtnText, &prevBtnText, &pageText });

		window.display();
	}
}

void Library::listUsers() {
	// using a new window
	sf::RenderWindow window(sf::VideoMode(550, 600), "Users");
	window.setFramerateLimit(20);
	// A list of users with their borrowed books amount
	// 10 users one page, next page and prev page, user div is a btn
	sf::RectangleShape userBtn[10];
	for (int i = 0; i < 10; i++) {
		userBtn[i].setSize(sf::Vector2f(450, 40));
		userBtn[i].setFillColor(sf::Color(209, 233, 255));
		userBtn[i].setOutlineColor(sf::Color(167, 167, 211));
		userBtn[i].setOutlineThickness(1);
		userBtn[i].setPosition(50, 50 + i * 40);
	}
	// next page
	sf::RectangleShape nextBtn(sf::Vector2f(100, 70));
	sf::Text nextBtnText("Next", font, 20);
	initButton(nextBtn, nextBtnText, 350, 480);
	// prev
	sf::RectangleShape prevBtn(sf::Vector2f(100, 70));
	sf::Text prevBtnText("Prev", font, 20);
	initButton(prevBtn, prevBtnText, 100, 480);

	// Display page
	sf::Text pageText(to_string(1) + "/" + to_string((users.size() + 9) / 10), font, 18);
	pageText.setFillColor(sf::Color::Black);
	sf::FloatRect textBounds = pageText.getLocalBounds();
	pageText.setOrigin(textBounds.left + textBounds.width / 2.0f,
		textBounds.top + textBounds.height / 2.0f);
	pageText.setPosition(275, 500);
	// Display users
	size_t curPage = 0;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			// left click
			if (event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
				if (nextBtn.getGlobalBounds().contains(mousePos)) {
					if ((curPage + 1) * 10 < users.size()) {
						curPage++;
						pageText.setString(to_string(curPage + 1) + "/" + to_string((users.size() + 9) / 10));
					}
				}
				else if (prevBtn.getGlobalBounds().contains(mousePos)) {
					if (curPage > 0) {
						curPage--;
						pageText.setString(to_string(curPage + 1) + "/" + to_string((users.size() + 9) / 10));
					}
				}
				for (int i = 0; i < 10; i++) {
					if (i + curPage * 10 < users.size() && userBtn[i].getGlobalBounds().contains(mousePos)) {
						// printuser
						thread(&Library::printUser, this, ref(users.at(i + curPage * 10))).detach();
					}
				}
			}
			this_thread::sleep_for(std::chrono::milliseconds(30));
		}
		// Render
		window.clear(sf::Color(210, 233, 233));
		renderShape(window, { &nextBtn, &prevBtn, &nextBtnText,
			&prevBtnText, &pageText });
		for (int i = 0; i < 10; i++) {
			if (i + curPage * 10 < users.size()) {
				window.draw(userBtn[i]);
				users.at(i + curPage * 10).displayBrief(window, 50, 50 + i * 40);
			}
			else break;
		}
		window.display();
	}
}

void Library::rearrangeBooks(vector<Book*>& sorted) {
	// using a new window
	// left filter attributes , right toggle button for all sorting attributes & asc/desc
	sf::RenderWindow window(sf::VideoMode(800, 450), "Rearrange Books");
	window.setFramerateLimit(20);
	// filter
	sf::RectangleShape filterNameBox(sf::Vector2f(200, 50));
	sf::Text filterNameLabel("Name", font, 20);
	sf::Text filterNameText("Name", font, 24);
	initInputBox(filterNameBox, filterNameText, filterNameLabel, 50, 50);
	// filter author
	sf::RectangleShape filterAuthorBox(sf::Vector2f(200, 50));
	sf::Text filterAuthorLabel("Author", font, 20);
	sf::Text filterAuthorText("Author", font, 24);
	initInputBox(filterAuthorBox, filterAuthorText, filterAuthorLabel, 50, 150);
	// filter category
	sf::RectangleShape filterCategoryBox(sf::Vector2f(200, 50));
	sf::Text filterCategoryLabel("Category", font, 20);
	sf::Text filterCategoryText("Category", font, 24);
	initInputBox(filterCategoryBox, filterCategoryText, filterCategoryLabel, 50, 250);
	// filter published (button)
	sf::RectangleShape filterPublishedBox(sf::Vector2f(200, 50));
	sf::Text filterPublishedLabel("Published", font, 20);
	initButton(filterPublishedBox, filterPublishedLabel, 50, 350);
	sf::Text filterPublishedDisplay("2020-1-1", font, 24);
	filterPublishedDisplay.setFillColor(sf::Color::Black);
	filterPublishedDisplay.setPosition(50, 400);
	// toggle published
	sf::RectangleShape GoFilterPublished(sf::Vector2f(80, 50));
	sf::Text GoFilterPublishedLabel("Nah", font, 24);
	initButton(GoFilterPublished, GoFilterPublishedLabel, 300, 350);
	// sort by(button)
	sf::RectangleShape sortBox(sf::Vector2f(200, 50));
	sf::Text sortLabel("Sort By", font, 20);
	initButton(sortBox, sortLabel, 300, 50);
	// asc/desc
	sf::RectangleShape ascBox(sf::Vector2f(200, 50));
	sf::Text ascLabel("Asc", font, 20);
	initButton(ascBox, ascLabel, 300, 150);
	// submit
	sf::RectangleShape submitBox(sf::Vector2f(200, 50));
	sf::Text submitLabel("Submit", font, 24);
	initButton(submitBox, submitLabel, 300, 250);
	submitBox.setFillColor(sf::Color(168, 255, 255));

	// switch for current input box
	bool inName = false, inAuthor = false, inCategory = false, isAsc = true;
	Date filterPublished;
	size_t sortIndex = 0, togglePusblished = 0;
	const vector<string> sortBy = { "NYA!", "Name", "Author", "Published", "Category" };
	string filterName, filterAuthor, filterCategory;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
				return;
			}
			else if (event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
				if (filterNameBox.getGlobalBounds().contains(mousePos)) {
					inName = true;
					inAuthor = false;
					inCategory = false;
				}
				else if (filterAuthorBox.getGlobalBounds().contains(mousePos)) {
					inName = false;
					inAuthor = true;
					inCategory = false;
				}
				else if (filterCategoryBox.getGlobalBounds().contains(mousePos)) {
					inName = false;
					inAuthor = false;
					inCategory = true;
				}
				else if (submitBox.getGlobalBounds().contains(mousePos)) {
					if (filterPublished.changing.load()) {
						thread([]() { errorWindow("Window is not closed can't exit!"); }).detach();
						continue;
					}
					window.close();
				}
				else if (filterPublishedBox.getGlobalBounds().contains(mousePos)) {
					inName = false;
					inAuthor = false;
					inCategory = false;
					thread([&]() {
						filterPublished.change_date();
						filterPublishedDisplay.setString(filterPublished.getString());
					}).detach();
				}
				else if (sortBox.getGlobalBounds().contains(mousePos)) {
					// sort
					sortIndex++;
					if (sortIndex >= sortBy.size()) {
						sortIndex = 0;
					}
					sortLabel.setString(sortBy[sortIndex]);
				}
				else if (ascBox.getGlobalBounds().contains(mousePos)) {
					// asc/desc
					if (isAsc) {
						isAsc = false;
						ascLabel.setString("Desc");
					}
					else {
						isAsc = true;
						ascLabel.setString("Asc");
					}
				}
				else if (GoFilterPublished.getGlobalBounds().contains(mousePos)) {
					// filter published
					togglePusblished++;
					togglePusblished %= 3;
					switch (togglePusblished)
					{
					case 0:
						GoFilterPublishedLabel.setString("Nah");
						break;
					case 1:
						GoFilterPublishedLabel.setString(">=");
						break;
					case 2:
						GoFilterPublishedLabel.setString("<=");
						break;
					default:
						break;
					}
				}
				else {
					inName = false;
					inAuthor = false;
					inCategory = false;
				}
			}
			if (inName) {
				inputEvent(event, filterName, 'A', 'z', 20);
				filterNameText.setString(filterName);
			}
			else if (inAuthor) {
				inputEvent(event, filterAuthor, 'A', 'z', 20);
				filterAuthorText.setString(filterAuthor);
			}
			else if (inCategory) {
				inputEvent(event, filterCategory, 'A', 'z', 20);
				filterCategoryText.setString(filterCategory);
			}
		}
		// Render
		window.clear(sf::Color(210, 233, 233));
		
		renderShape(window, { &filterNameBox, &filterNameLabel, &filterNameText,
			&filterAuthorBox, &filterAuthorLabel, &filterAuthorText,
			&filterCategoryBox, &filterCategoryLabel, &filterCategoryText,
			&filterPublishedBox, &filterPublishedLabel, &filterPublishedDisplay,
			&sortBox, &sortLabel,
			&ascBox, &ascLabel,
			&submitBox, &submitLabel, & GoFilterPublished, &GoFilterPublishedLabel });
		window.display();
	}
	// start filtering and sorting
	if (filterCategory != "")
		sorted.erase(my_remove_if(sorted.begin(), sorted.end(),
			[&](Book* b) { return b->category != filterCategory; }),
			sorted.end());
	if (filterAuthor != "")
		sorted.erase(my_remove_if(sorted.begin(), sorted.end(),
			[&](Book* b) { return b->author != filterAuthor; }),
			sorted.end());
	if (filterName != "")
		sorted.erase(my_remove_if(sorted.begin(), sorted.end(),
			[&](Book* b) { return b->name != filterName; }),
			sorted.end());
	if (togglePusblished == 1) // find larger
		sorted.erase(my_remove_if(sorted.begin(), sorted.end(),
			[&](Book* b) { return b->published < filterPublished; }),
			sorted.end());
	else if (togglePusblished == 2) // find smaller
		sorted.erase(my_remove_if(sorted.begin(), sorted.end(),
			[&](Book* b) { return b->published > filterPublished; }),
			sorted.end());
	if (sortIndex == 0) return; // no sort
	if (sortIndex == 1) {
		msort(sorted.begin(), sorted.end(), [&](Book* a, Book* b) {
			return isAsc ? a->name < b->name : a->name > b->name;
			});
	}
	else if (sortIndex == 2) {
		msort(sorted.begin(), sorted.end(), [&](Book* a, Book* b) {
			return isAsc ? a->author < b->author : a->author > b->author;
			});
	}
	else if (sortIndex == 3) {
		msort(sorted.begin(), sorted.end(), [&](Book* a, Book* b) {
			return isAsc ? a->published < b->published : a->published > b->published;
			});
	}
	else if (sortIndex == 4) {
		msort(sorted.begin(), sorted.end(), [&](Book* a, Book* b) {
			return isAsc ? a->category < b->category : a->category > b->category;
			});
	}
	return;
}

void Library::printUser(User& user) {
	sf::RenderWindow window(sf::VideoMode(700, 800), "Users");
	window.setFramerateLimit(20);
	// A list of books borrowed by a user
	// 10 books one page, next page and prev page, book div is a btn
	sf::RectangleShape bookBtn[10];
	for (int i = 0; i < 10; i++) {
		bookBtn[i].setSize(sf::Vector2f(600, 55));
		bookBtn[i].setFillColor(sf::Color(209, 233, 255));
		bookBtn[i].setOutlineColor(sf::Color(167, 167, 211));
		bookBtn[i].setOutlineThickness(1);
		bookBtn[i].setPosition(25, 50 + i * 55);
	}
	// Return book btn(box + "return") for each book div
	sf::RectangleShape returnBtn[10];
	sf::Text returnBtnText[10];
	for (int i = 0; i < 10; i++) {
		returnBtn[i].setSize(sf::Vector2f(100, 55));
		returnBtnText[i].setString("Return");
		returnBtnText[i].setFont(font);
		returnBtnText[i].setCharacterSize(20);
		initButton(returnBtn[i], returnBtnText[i], 550, (size_t)50 + (size_t)i * 55);
		returnBtn[i].setFillColor(sf::Color(197, 226, 226));
	}
	// next page
	sf::RectangleShape nextBtn(sf::Vector2f(100, 70));
	sf::Text nextBtnText("Next", font, 20);
	initButton(nextBtn, nextBtnText, 450, 630);
	// prev
	sf::RectangleShape prevBtn(sf::Vector2f(100, 70));
	sf::Text prevBtnText("Prev", font, 20);
	initButton(prevBtn, prevBtnText, 150, 630);
	// Display page
	sf::Text pageText(to_string(1) + "/" + to_string((user.borrowing.size() + 9) / 10), font, 18);
	pageText.setFillColor(sf::Color::Black);
	sf::FloatRect textBounds = pageText.getLocalBounds();
	pageText.setOrigin(textBounds.left + textBounds.width / 2.0f,
		textBounds.top + textBounds.height / 2.0f);
	pageText.setPosition(350, 650);
	// Instruction: "click to return"
	sf::Text instructionText("Click to return", font, 18);
	instructionText.setFillColor(sf::Color(50, 50, 50));
	instructionText.setPosition(500, 5);

	// Display books
	size_t curPage = 0;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
				if (nextBtn.getGlobalBounds().contains(mousePos)) {
					if ((curPage + 1) * 10 < user.borrowing.size()) {
						curPage++;
						pageText.setString(to_string(curPage + 1) + "/" + to_string((user.borrowing.size() + 9) / 10));
					}
				}
				else if (prevBtn.getGlobalBounds().contains(mousePos)) {
					if (curPage > 0) {
						curPage--;
						pageText.setString(to_string(curPage + 1) + "/" + to_string((user.borrowing.size() + 9) / 10));
					}
				}
				else {
					for (int i = 0; i < 10; i++) {
						// on click on some book div
						if (i + curPage * 10 < user.borrowing.size() && returnBtn[i].getGlobalBounds().contains(mousePos)) {
							// find the book index in the books vector and return the book
							size_t bookIndex = my_find_if(books.begin(), books.end(), [&](Book* b) {
								return *b == user.borrowing.at(i + curPage * 10);
								}) - books.begin();
							books[bookIndex]->availableCopies++;
							user.borrowing.erase(user.borrowing.begin() + i + curPage * 10);
							break;
						}
					}
				}
			}
			this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		// Render
		window.clear(sf::Color(210, 233, 233));
		renderShape(window, { &nextBtn, &prevBtn, &nextBtnText,
			&prevBtnText, &pageText, &instructionText });
		for (int i = 0; i < 10; i++) {
			if (i + curPage * 10 < user.borrowing.size()) {
				window.draw(bookBtn[i]);
				user.borrowing.at(i + curPage * 10).displayFUser(window, 50, 50 + i * 55);
				window.draw(returnBtn[i]);
				window.draw(returnBtnText[i]);
			}
			else break;
		}
		window.display();
	}
}

void Library::userBorrowedBook(User& user){}

Library::~Library() {
	for (auto book : books) {
		delete book;
	}
}