#include "input.h"
#include "Libarary.h"
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <exception>

using namespace std;

Date::Date() :year(0), month(0), day(0) {};

void Date::change_date() {
    sf::RenderWindow window(sf::VideoMode(800, 170), "Text Input");

    // 輸入框
    // Year
    sf::RectangleShape inputYear(sf::Vector2f(200, 50));
    inputYear.setFillColor(sf::Color::White);
    inputYear.setOutlineColor(sf::Color::Black);
    inputYear.setOutlineThickness(2);
    inputYear.setPosition(50, 50);

    sf::Text Yeartxt("Year", font, 20);
    Yeartxt.setFillColor(sf::Color::Black);
    Yeartxt.setPosition(55, 20);

    sf::Text inputYearText("", font, 24);
    inputYearText.setFillColor(sf::Color::Black);
    inputYearText.setPosition(55, 55);

    // Month
    sf::RectangleShape inputMonth(sf::Vector2f(200, 50));
    inputMonth.setFillColor(sf::Color::White);
    inputMonth.setOutlineColor(sf::Color::Black);
    inputMonth.setOutlineThickness(2);
    inputMonth.setPosition(300, 50);

    sf::Text Monthtxt("Month", font, 20);
    Monthtxt.setFillColor(sf::Color::Black);
    Monthtxt.setPosition(305, 20);

    sf::Text inputMonthText("", font, 24);
    inputMonthText.setFillColor(sf::Color::Black);
    inputMonthText.setPosition(305, 55);

    // Day
    sf::RectangleShape inputDay(sf::Vector2f(200, 50));
    inputDay.setFillColor(sf::Color::White);
    inputDay.setOutlineColor(sf::Color::Black);
    inputDay.setOutlineThickness(2);
    inputDay.setPosition(550, 50);

    sf::Text Daytxt("Day", font, 20);
    Daytxt.setFillColor(sf::Color::Black);
    Daytxt.setPosition(555, 20);

    sf::Text inputDayText("", font, 24);
    inputDayText.setFillColor(sf::Color::Black);
    inputDayText.setPosition(555, 55);

    sf::RectangleShape submit_btn(sf::Vector2f(100, 50));
    submit_btn.setFillColor(sf::Color::Green);
    submit_btn.setOutlineColor(sf::Color::Black);
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
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
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
                        year = min(stoi(syear), 3000);
						month = min(stoi(smonth), 12);
						day = min(stoi(sday), 31);
						window.close();
                    }
                    else {
                        inyear = false, inmonth = false, inday = false;
                    }
                }
            }

            // 鍵盤輸入文字
            if (inyear && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8) { // 處理Backspace
                    if (!syear.empty()) {
                        syear.pop_back();
                    }
                }
                else if (event.text.unicode <= '9' && event.text.unicode >= '0') { // 處理英文字元
                    syear += static_cast<char>(event.text.unicode);
                    if (syear.size() > 4) {
                        syear = syear.substr(1, 4); // 限制輸入長度
                    }
                }
                inputYearText.setString(syear); // 更新畫面上的文字
            }

            if (inmonth && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8) { // 處理Backspace
                    if (!smonth.empty()) {
                        smonth.pop_back();
                    }
                }
                else if (event.text.unicode <= '9' && event.text.unicode >= '0') { // 處理英文字元
                    smonth += static_cast<char>(event.text.unicode);
					if (smonth.size() > 2) {
						smonth = smonth.substr(1, 2); // 限制輸入長度
					}
                }
                inputMonthText.setString(smonth); // 更新畫面上的文字
            }

            if (inday && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8) { // 處理Backspace
                    if (!sday.empty()) {
                        sday.pop_back();
                    }
                }
                else if (event.text.unicode <= '9' && event.text.unicode >= '0') { // 處理英文字元
                    sday += static_cast<char>(event.text.unicode);
                    if (sday.size() > 2) {
                        sday = sday.substr(1, 2); // 限制輸入長度
                    }
                }
                inputDayText.setString(sday); // 更新畫面上的文字
            }

        }

        window.clear(sf::Color(200, 200, 200));
		
		renderShape(window, { &inputYear, &inputMonth, &inputDay, &submit_btn });
		renderText(window, { &Yeartxt, &Monthtxt, &Daytxt
			, &submit_btn_innerText, &inputDayText, &inputMonthText, &inputYearText });

        window.display();
    }
    return;
}

void Date::print_date() {
	cout << year << "-" << month << "-" << day << endl;
}

User::User(string& name) : name(name) {}

void User::borrow() {
	// borrow book
}

void User::giveBack() {
	// give back book
}

void User::checkDue() {
	// check due date
}

Book::Book() {
    name = "";
	author = "";
	ISBN = "";
	category = "";
	copyAmount = 0;
	is_available = true;
	copyAmount = 0;
};

Book Book::change() {
	sf::RenderWindow window(sf::VideoMode(1000, 600), "ADD Book");

    // Name
    sf::RectangleShape inputName(sf::Vector2f(200, 50));
    inputName.setFillColor(sf::Color::White);
    inputName.setOutlineColor(sf::Color::Black);
    inputName.setOutlineThickness(2);
    inputName.setPosition(50, 50);
    sf::Text Nametxt("Name", font, 20);
    Nametxt.setFillColor(sf::Color::Black);
    Nametxt.setPosition(55, 20);
    sf::Text inputNameText("", font, 24);
    inputNameText.setFillColor(sf::Color::Black);
    inputNameText.setPosition(55, 55);

    // Author
    sf::RectangleShape inputAuthor(sf::Vector2f(200, 50));
    inputAuthor.setFillColor(sf::Color::White);
    inputAuthor.setOutlineColor(sf::Color::Black);
    inputAuthor.setOutlineThickness(2);
    inputAuthor.setPosition(50, 120); // Adjusted position
    sf::Text Authortxt("Author", font, 20);
    Authortxt.setFillColor(sf::Color::Black);
    Authortxt.setPosition(55, 90); // Adjusted position
    sf::Text inputAuthorText("", font, 24);
    inputAuthorText.setFillColor(sf::Color::Black);
    inputAuthorText.setPosition(55, 125); // Adjusted position

    // ISBN
    sf::RectangleShape inputISBN(sf::Vector2f(200, 50));
    inputISBN.setFillColor(sf::Color::White);
    inputISBN.setOutlineColor(sf::Color::Black);
    inputISBN.setOutlineThickness(2);
    inputISBN.setPosition(50, 190); // Adjusted position
    sf::Text ISBNtxt("ISBN", font, 20);
    ISBNtxt.setFillColor(sf::Color::Black);
    ISBNtxt.setPosition(55, 160); // Adjusted position
    sf::Text inputISBNText("", font, 24);
    inputISBNText.setFillColor(sf::Color::Black);
    inputISBNText.setPosition(55, 195); // Adjusted position

    // Category
    sf::RectangleShape inputCategory(sf::Vector2f(200, 50));
    inputCategory.setFillColor(sf::Color::White);
    inputCategory.setOutlineColor(sf::Color::Black);
    inputCategory.setOutlineThickness(2);
    inputCategory.setPosition(300, 50); // Adjusted position
    sf::Text Categorytxt("Category", font, 20);
    Categorytxt.setFillColor(sf::Color::Black);
    Categorytxt.setPosition(305, 20); // Adjusted position
    sf::Text inputCategoryText("", font, 24);
    inputCategoryText.setFillColor(sf::Color::Black);
    inputCategoryText.setPosition(305, 55); // Adjusted position

    // copyAmount
    sf::RectangleShape inputcopyAmount(sf::Vector2f(200, 50));
    inputcopyAmount.setFillColor(sf::Color::White);
    inputcopyAmount.setOutlineColor(sf::Color::Black);
    inputcopyAmount.setOutlineThickness(2);
    inputcopyAmount.setPosition(300, 120); // Adjusted position
    sf::Text copyAmounttxt("copyAmount", font, 20);
    copyAmounttxt.setFillColor(sf::Color::Black);
    copyAmounttxt.setPosition(305, 90); // Adjusted position
    sf::Text inputcopyAmountText("", font, 24);
    inputcopyAmountText.setFillColor(sf::Color::Black);
    inputcopyAmountText.setPosition(305, 125); // Adjusted position

    // Published a btn with a text, onpress, open a new window(Date::change_date)
    sf::RectangleShape Published_btn(sf::Vector2f(80, 80));
    Published_btn.setFillColor(sf::Color(180, 180, 180));
    Published_btn.setOutlineColor(sf::Color::Black);
    Published_btn.setOutlineThickness(1);
    Published_btn.setPosition(550, 50); // Adjusted position
    sf::Text PublishedText("Published", font, 20);
    PublishedText.setFillColor(sf::Color::Black);
    set_mid(Published_btn, PublishedText);

    // Submit
    sf::RectangleShape submit_btn(sf::Vector2f(100, 50));
    submit_btn.setFillColor(sf::Color::Green);
    submit_btn.setOutlineColor(sf::Color::Black);
    submit_btn.setOutlineThickness(2);
    submit_btn.setPosition(550, 150); // Adjusted position
    sf::Text submit_btn_innerText("Submit", font, 24);
    submit_btn_innerText.setFillColor(sf::Color::Black);
    set_mid(submit_btn, submit_btn_innerText);

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
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
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
						published.change_date();
					}
					else if (inputcopyAmount.getGlobalBounds().contains(mousePos)) {
						inname = false, inauthor = false, inISBN = false, incategory = false, incopyAmount = true;
					}
					else if (submit_btn.getGlobalBounds().contains(mousePos)) {
						inname = false, inauthor = false, inISBN = false, incategory = false, incopyAmount = false;
						name = sname;
						author = sauthor;
						ISBN = sISBN;
						category = scategory;
						copyAmount = min(stoi(scopyAmount), 1000000);
						window.close();
					}
					else {
						inname = false, inauthor = false, inISBN = false, incategory = false, incopyAmount = false;
					}
				}
			}

			// 鍵盤輸入文字
			if (inname && event.type == sf::Event::TextEntered) {
				if (event.text.unicode == 8) { // 處理
					if (!sname.empty()) {
						sname.pop_back();
					}
				}
				else if (event.text.unicode <= 126 && event.text.unicode >= 32) { // 處理英文字元
					sname += static_cast<char>(event.text.unicode);
					if (sname.size() > 20) {
						sname = sname.substr(1, 20); // 限制輸入長度
					}
				}
				inputNameText.setString(sname); // 更新畫面上的文字
			}
			if (inauthor && event.type == sf::Event::TextEntered) {
				if (event.text.unicode == 8) { // 處理Backspace
					if (!sauthor.empty()) {
						sauthor.pop_back();
					}
				}
				else if (event.text.unicode <= 126 && event.text.unicode >= 32) { // 處理英文字元
					sauthor += static_cast<char>(event.text.unicode);
					if (sauthor.size() > 20) {
						sauthor = sauthor.substr(1, 20); // 限制輸入長度
					}
				}
				inputAuthorText.setString(sauthor); // 更新畫面上的文字
			}
			if (inISBN && event.type == sf::Event::TextEntered) {
				if (event.text.unicode == 8) { // 處理Backspace
					if (!sISBN.empty()) {
						sISBN.pop_back();
					}
				}
				else if (event.text.unicode <= 126 && event.text.unicode >= 32) { // 處理英文字元
					sISBN += static_cast<char>(event.text.unicode);
					if (sISBN.size() > 20) {
						sISBN = sISBN.substr(1, 20); // 限制輸入長度
					}
				}
				inputISBNText.setString(sISBN); // 更新畫面上的文字
			}
			if (incategory && event.type == sf::Event::TextEntered) {
				if (event.text.unicode == 8) { // 處理Backspace
					if (!scategory.empty()) {
						scategory.pop_back();
					}
				}
				else if (event.text.unicode <= 126 && event.text.unicode >= 32) { // 處理英文字元
					scategory += static_cast<char>(event.text.unicode);
					if (scategory.size() > 20) {
						scategory = scategory.substr(1, 20); // 限制輸入長度
					}
				}
				inputCategoryText.setString(scategory); // 更新畫面上的文字
			}
			if (incopyAmount && event.type == sf::Event::TextEntered) {
				if (event.text.unicode == 8) { // 處理Backspace
					if (!scopyAmount.empty()) {
						scopyAmount.pop_back();
					}
				}
				else if (event.text.unicode <= '9' && event.text.unicode >= '0') { // 處理英文字元
					scopyAmount += static_cast<char>(event.text.unicode);
					if (scopyAmount.size() > 6) {
						scopyAmount = scopyAmount.substr(1, 6); // 限制輸入長度
					}
				}
				inputcopyAmountText.setString(scopyAmount); // 更新畫面上的文字
			}
		}
		window.clear(sf::Color(200, 200, 200));
		
		renderShape(window, { &inputName, &inputAuthor, &inputISBN, &inputCategory
            , &inputcopyAmount, &Published_btn, &submit_btn });
		renderText(window, { &Nametxt, &Authortxt, &ISBNtxt, &Categorytxt, &copyAmounttxt
			, &submit_btn_innerText, &inputNameText, &inputAuthorText, &inputISBNText
			, &inputCategoryText, &inputcopyAmountText, &PublishedText });

		window.display();
		}
	return *this;
}

void Book::print_book() {
	cout << "Name: " << name << endl;
	cout << "Author: " << author << endl;
	cout << "Published: ";
	published.print_date();
	cout << "Due: ";
	due.print_date();
	cout << "ISBN: " << ISBN << endl;
	cout << "Category: " << category << endl;
	cout << "Copy Amount: " << copyAmount << endl;
}