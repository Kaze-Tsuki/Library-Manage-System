#include "input.h"
#include "Library.h"
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <exception>

using namespace std;

Date::Date() :year(2020), month(1), day(1) {};
Date::Date(int y, int m, int d) : year(y), month(m), day(d) {
	changing.store(false);
}

void Date::change_date() {
    if (changing.exchange(true)) {
		thread([]() { errorWindow("Already running input text window."); }).detach();
        return;
    }
    sf::RenderWindow window(sf::VideoMode(800, 170), "Text Input");
    window.setFramerateLimit(30);

    // 輸入框
    // Year
    sf::RectangleShape inputYear(sf::Vector2f(200, 50));
    sf::Text Yeartxt("Year", font, 20);
    sf::Text inputYearText(to_string(year), font, 24);
    initInputBox(inputYear, inputYearText, Yeartxt, 50, 50);

    // Month
    sf::RectangleShape inputMonth(sf::Vector2f(200, 50));
    sf::Text Monthtxt("Month", font, 20);
    sf::Text inputMonthText(to_string(month), font, 24);
    initInputBox(inputMonth, inputMonthText, Monthtxt, 300, 50);

    // Day
    sf::RectangleShape inputDay(sf::Vector2f(200, 50));
    sf::Text Daytxt("Day", font, 20);
    sf::Text inputDayText(to_string(day), font, 24);
    initInputBox(inputDay, inputDayText, Daytxt, 550, 50);

    sf::RectangleShape submit_btn(sf::Vector2f(100, 50));
    submit_btn.setFillColor(sf::Color(168, 255, 255));
    submit_btn.setOutlineColor(sf::Color(167, 167, 211));
    submit_btn.setOutlineThickness(2);
    submit_btn.setPosition(350, 110);

    sf::Text submit_btn_innerText("Submit", font, 24);
    submit_btn_innerText.setFillColor(sf::Color::Black);
    set_mid(submit_btn, submit_btn_innerText);

    bool inyear = false, inmonth = false, inday = false;// 是否正在輸入
    string syear(to_string(year)), smonth(to_string(month)), sday(to_string(day));
    if (syear == "0") syear = "";
    if (smonth == "0") smonth = "";
    if (sday == "0") sday = "";

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
                if (inputYear.getGlobalBounds().contains(mousePos)) {
                    inyear = true, inmonth = false, inday = false;
                }
                else if (inputMonth.getGlobalBounds().contains(mousePos)) {
                    inyear = false, inmonth = true, inday = false;
                }
                else if (inputDay.getGlobalBounds().contains(mousePos)) {
                    inyear = false, inmonth = false, inday = true;
                }
                else if (submit_btn.getGlobalBounds().contains(mousePos)) {
                    inyear = false, inmonth = false, inday = false;
                    if (syear == "") syear = "2020";
                    if (smonth == "") smonth = "1";
                    if (sday == "") sday = "1";
                    year = max(min(stoi(syear), 3000), 1);
                    month = max(min(stoi(smonth), 12), 1);
                    day = max(min(stoi(sday), 31), 1);
                    window.close();
                }
                else {
                    inyear = false, inmonth = false, inday = false;
                }
            }

            // 鍵盤輸入文字
            if (inyear) {
				inputEvent(event, syear, '0', '9', 4);
                inputYearText.setString(syear); // 更新畫面上的文字
            }

            if (inmonth) {
				inputEvent(event, smonth, '0', '9', 2);
                inputMonthText.setString(smonth); // 更新畫面上的文字
            }

            if (inday) {
				inputEvent(event, sday, '0', '9', 2);
                inputDayText.setString(sday); // 更新畫面上的文字
            }
            this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        window.clear(sf::Color(210, 233, 233));

        renderShape(window, { &inputYear, &inputMonth, &inputDay, &submit_btn , &Yeartxt, &Monthtxt, &Daytxt
            , &submit_btn_innerText, &inputDayText, &inputMonthText, &inputYearText });

        window.display();
    }
    changing.store(false); // 關閉 flag
    return;
}

string Date::getString() const{
    string s = to_string(year) + "-" + to_string(month) + "-" + to_string(day);
    return s;
}

void Date::operator=(const Date& other) {
    year = other.year;
    month = other.month;
    day = other.day;
}

bool Date::operator<(const Date& other) {
	if (year != other.year) return year < other.year;
	if (month != other.month) return month < other.month;
	return day < other.day;
}

bool Date::operator>(const Date& other) {
	if (year != other.year) return year > other.year;
	if (month != other.month) return month > other.month;
	return day > other.day;
}

User::User(string& name) : name(name) {}
User::User() {
	name = "";
}

void User::displayBrief(sf::RenderWindow& window, int x, int y) {
	sf::Text nameText(name, font, 20);
	nameText.setFillColor(sf::Color::Black);
	nameText.setPosition(x+10, y+10);
	sf::Text borrowText("Borrowed: " + to_string(borrowing.size()), font, 20);
	borrowText.setFillColor(sf::Color::Black);
	borrowText.setPosition(x+300, y + 10);
	renderShape(window, { &nameText, &borrowText });
}

void User::borrow() {}

void User::giveBack() {}

void User::checkDue() {}

Book::Book() {
    name = "";
    author = "";
    ISBN = "";
    category = "";
    copyAmount = 1;
    availableCopies = 1;
    changing.store(false);
};

Book::Book(const Book& other) {
	name = other.name;
	author = other.author;
	ISBN = other.ISBN;
	category = other.category;
	published = other.published;
	due = other.due;
	copyAmount = other.copyAmount;
	availableCopies = other.availableCopies;
	changing.store(false);
}

void Book::operator=(const Book& other) {
	name = other.name;
	author = other.author;
	ISBN = other.ISBN;
	category = other.category;
	published = other.published;
	due = other.due;
	copyAmount = other.copyAmount;
	availableCopies = other.availableCopies;
	changing.store(false);
}

bool Book::change() {
    if (changing.exchange(true)) {
		thread([]() { errorWindow("Already running input text window."); }).detach();
        return false;
    }
    sf::RenderWindow window(sf::VideoMode(530, 540), "ADD Book");
    window.setFramerateLimit(20);

    // Name
    sf::RectangleShape inputName(sf::Vector2f(300, 50));
    sf::Text Nametxt("Name", font, 20);
    sf::Text inputNameText("", font, 24);
    initInputBox(inputName, inputNameText, Nametxt, 50, 50);


    // Author
    sf::RectangleShape inputAuthor(sf::Vector2f(300, 50));
    sf::Text Authortxt("Author", font, 20);
    sf::Text inputAuthorText("", font, 24);
    initInputBox(inputAuthor, inputAuthorText, Authortxt, 50, 130);

    // ISBN
    sf::RectangleShape inputISBN(sf::Vector2f(300, 50));
    sf::Text ISBNtxt("ISBN", font, 20);
    sf::Text inputISBNText("", font, 24);
    initInputBox(inputISBN, inputISBNText, ISBNtxt, 50, 210);

    // Category
    sf::RectangleShape inputCategory(sf::Vector2f(300, 50));
    sf::Text Categorytxt("Category", font, 20);
    sf::Text inputCategoryText("", font, 24);
    initInputBox(inputCategory, inputCategoryText, Categorytxt, 50, 290);

    // copyAmount
    sf::RectangleShape inputcopyAmount(sf::Vector2f(150, 50));
    sf::Text copyAmounttxt("Copies", font, 20);
    sf::Text inputcopyAmountText("1", font, 24);
    initInputBox(inputcopyAmount, inputcopyAmountText, copyAmounttxt, 50, 370);

    // Published a btn with a text, onpress, open a new window(Date::change_date)
    sf::RectangleShape Published_btn(sf::Vector2f(170, 70));
    sf::Text PublishedText("Published\n   Year", font, 20);
    initButton(Published_btn, PublishedText, 50, 440);
    sf::Text PublichedContent(published.getString(), font, 20);
    PublichedContent.setFillColor(sf::Color::Black);
    PublichedContent.setPosition(240, 460);

    // Submit
    sf::RectangleShape submit_btn(sf::Vector2f(100, 50));
    submit_btn.setFillColor(sf::Color(168, 255, 255));
    submit_btn.setOutlineColor(sf::Color(167, 167, 211));
    submit_btn.setOutlineThickness(2);
    submit_btn.setPosition(380, 50);
    sf::Text submit_btn_innerText("Submit", font, 24);
    submit_btn_innerText.setFillColor(sf::Color::Black);
    set_mid(submit_btn, submit_btn_innerText);

	// Abandon
	sf::RectangleShape abandon_btn(sf::Vector2f(100, 50));
	abandon_btn.setFillColor(sf::Color(168, 255, 255));
	abandon_btn.setOutlineColor(sf::Color(167, 167, 211));
	abandon_btn.setOutlineThickness(2);
	abandon_btn.setPosition(380, 130);
	sf::Text abandon_btn_innerText("Abandon", font, 24);
	abandon_btn_innerText.setFillColor(sf::Color::Black);
	set_mid(abandon_btn, abandon_btn_innerText);

    bool inname = false, inauthor = false, inISBN = false, incategory = false, incopyAmount = false;// 是否正在輸入
    string sname(name), sauthor(author), sISBN(ISBN), scategory(category), scopyAmount(to_string(copyAmount));

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
                if (inputName.getGlobalBounds().contains(mousePos)) {
                    inname = true, inauthor = false, inISBN = false, incategory = false, incopyAmount = false;
                }
                else if (inputAuthor.getGlobalBounds().contains(mousePos)) {
                    inname = false, inauthor = true, inISBN = false, incategory = false, incopyAmount = false;
                }
                else if (inputISBN.getGlobalBounds().contains(mousePos)) {
                    inname = false, inauthor = false, inISBN = true, incategory = false, incopyAmount = false;
                }
                else if (inputCategory.getGlobalBounds().contains(mousePos)) {
                    inname = false, inauthor = false, inISBN = false, incategory = true, incopyAmount = false;
                }
                else if (Published_btn.getGlobalBounds().contains(mousePos)) {
                    inname = false, inauthor = false, inISBN = false, incategory = false, incopyAmount = false;
                    thread(&Date::change_date, &published).detach();
                }
                else if (inputcopyAmount.getGlobalBounds().contains(mousePos)) {
                    inname = false, inauthor = false, inISBN = false, incategory = false, incopyAmount = true;
                }
                else if (submit_btn.getGlobalBounds().contains(mousePos)) {
                    inname = false, inauthor = false, inISBN = false, incategory = false, incopyAmount = false;
                    if (published.changing.load()) {
						thread([]() { errorWindow("Please set the date first."); }).detach();
                        continue;
                    }
                    name = sname;
                    author = sauthor;
                    ISBN = sISBN;
                    category = scategory;
                    if (scopyAmount == "")scopyAmount = "1";
                    copyAmount = min(stoi(scopyAmount), 1000000);
                    availableCopies = copyAmount;
                    window.close();
                }
                else if (abandon_btn.getGlobalBounds().contains(mousePos)) {
					window.close();
					changing.store(false);
                    return false;
                }
                else {
                    inname = false, inauthor = false, inISBN = false, incategory = false, incopyAmount = false;
                }
            }

            if (!published.changing.load()) {
                PublichedContent.setString(published.getString());
            }

            // 鍵盤輸入文字
            if (inname) {
                inputEvent(event, sname, ' ', 'z', 19);
                inputNameText.setString(sname); // 更新畫面上的文字
            }
            else if (inauthor) {
                inputEvent(event, sauthor, ' ', 'z', 19);
                inputAuthorText.setString(sauthor); // 更新畫面上的文字
            }
            else if (inISBN) {
                inputEvent(event, sISBN, '0', '9', 13);
                inputISBNText.setString(sISBN); // 更新畫面上的文字
            }
            else if (incategory) {
                inputEvent(event, scategory, ' ', 'z', 19);
                inputCategoryText.setString(scategory); // 更新畫面上的文字
            }
            else if (incopyAmount) {
                inputEvent(event, scopyAmount, '0', '9', 4);
                inputcopyAmountText.setString(scopyAmount); // 更新畫面上的文字
            }
            //this_thread::sleep_for(std::chrono::milliseconds(7));
        }
        window.clear(sf::Color(210, 233, 233));

        renderShape(window, { &inputName, &inputAuthor, &inputISBN, &inputCategory, &abandon_btn
            , &inputcopyAmount, &Published_btn, &submit_btn , &Nametxt, &Authortxt, &ISBNtxt, &Categorytxt, &copyAmounttxt
            , &submit_btn_innerText, &inputNameText, &inputAuthorText, &inputISBNText, &abandon_btn_innerText
            , &inputCategoryText, &inputcopyAmountText, &PublishedText , &PublichedContent });

        window.display();
    }
    changing.store(false);
    return true;
}

void Book::print_book() {
    cout << "Name: " << name << endl;
    cout << "Author: " << author << endl;
    cout << "Published: ";
	cout << published.getString() << endl;
    cout << "Due: ";
	cout << due.getString() << endl;
    cout << "ISBN: " << ISBN << endl;
    cout << "Category: " << category << endl;
    cout << "Copy Amount: " << copyAmount << endl;
}

bool Book::valid() const {
    if (name.empty() || author.empty() || ISBN.empty() || category.empty()) {
        return false;
    }
    if (copyAmount <= 0 || availableCopies < 0) {
        return false;
    }
    return true;
}

bool Book::operator==(const Book& other) const {
    return name == other.name && author == other.author && ISBN == other.ISBN;
}

void Book::display(sf::RenderWindow& window, int x, int y) {
	sf::Text nameText("Name: " + name, font, 20);
	nameText.setFillColor(sf::Color::Black);
	nameText.setPosition(x, y);
	window.draw(nameText);
	sf::Text authorText("Author: " + author, font, 20);
	authorText.setFillColor(sf::Color::Black);
	authorText.setPosition(x, y + 30);
	window.draw(authorText);
	sf::Text ISBNText("ISBN: " + ISBN, font, 20);
	ISBNText.setFillColor(sf::Color::Black);
	ISBNText.setPosition(x, y + 60);
	window.draw(ISBNText);
	sf::Text categoryText("Category: " + category, font, 20);
	categoryText.setFillColor(sf::Color::Black);
	categoryText.setPosition(x, y + 90);
	window.draw(categoryText);
	sf::Text copyAmountText("Copies: " + to_string(availableCopies)
        + '/' + to_string(copyAmount), font, 20);
	copyAmountText.setFillColor(sf::Color::Black);
	copyAmountText.setPosition(x, y + 120);
	window.draw(copyAmountText);
	sf::Text publishedText("Published: " + published.getString(), font, 20);
	publishedText.setFillColor(sf::Color::Black);
	publishedText.setPosition(x, y + 150);
	window.draw(publishedText);
}

void Book::displayBrief(sf::RenderWindow& window, int x, int y) {
	sf::Text nameText("Name: " + name, font, 20);
	nameText.setFillColor(sf::Color::Black);
	nameText.setPosition(x, y+5);
	sf::Text authorText("Author: " + author, font, 20);
	authorText.setFillColor(sf::Color::Black);
	authorText.setPosition(x, y+30);
	sf::Text CategoryText("Category: " + category, font, 20);
	CategoryText.setFillColor(sf::Color::Black);
	CategoryText.setPosition(x + 370, y+5);
	sf::Text dateText("Published: " + published.getString(), font, 20);
	dateText.setFillColor(sf::Color::Black);
	dateText.setPosition(x + 370, y + 30);
	sf::Text copyAmountText("Available: " + to_string(availableCopies)
		+ '/' + to_string(copyAmount), font, 20);
	copyAmountText.setFillColor(sf::Color::Black);
	copyAmountText.setPosition(x + 650, y+30);
	renderShape(window, { &nameText, &authorText, &CategoryText, &copyAmountText, &dateText });
	
}

void Book::displayFUser(sf::RenderWindow& window, int x, int y) {
	sf::Text nameText("Name: " + name.substr(0, 10), font, 20);
	nameText.setFillColor(sf::Color::Black);
	nameText.setPosition(x, y + 5);
	sf::Text authorText("Author: " + author.substr(0, 10), font, 20);
	authorText.setFillColor(sf::Color::Black);
	authorText.setPosition(x, y + 30);
	sf::Text CategoryText("Category: " + category.substr(0, 10), font, 20);
	CategoryText.setFillColor(sf::Color::Black);
	CategoryText.setPosition(x + 250, y + 5);
	sf::Text dueText("Due: " + due.getString(), font, 20);
    if (due > today) dueText.setFillColor(sf::Color::Red);
    else dueText.setFillColor(sf::Color::Black);   
	dueText.setPosition(x + 250, y + 30);
	renderShape(window, { &nameText, &authorText, &CategoryText, &dueText });
}
