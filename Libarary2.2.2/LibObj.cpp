#include "input.h"
#include "Library.h"
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <exception>

using namespace std;

Date::Date() :year(2020), month(1), day(1) {};

void Date::change_date() {
    if (changing.exchange(true)) {
        cout << "Already running input text window." << endl;
        return;
    }
    sf::RenderWindow window(sf::VideoMode(800, 170), "Text Input");

    // ��J��
    // Year
    sf::RectangleShape inputYear(sf::Vector2f(200, 50));
    sf::Text Yeartxt("Year", font, 20);
    sf::Text inputYearText("2020", font, 24);
    initInputBox(inputYear, inputYearText, Yeartxt, 50, 50);

    // Month
    sf::RectangleShape inputMonth(sf::Vector2f(200, 50));
    sf::Text Monthtxt("Month", font, 20);
    sf::Text inputMonthText("1", font, 24);
    initInputBox(inputMonth, inputMonthText, Monthtxt, 300, 50);

    // Day
    sf::RectangleShape inputDay(sf::Vector2f(200, 50));
    sf::Text Daytxt("Day", font, 20);
    sf::Text inputDayText("1", font, 24);
    initInputBox(inputDay, inputDayText, Daytxt, 550, 50);

    sf::RectangleShape submit_btn(sf::Vector2f(100, 50));
    submit_btn.setFillColor(sf::Color::Green);
    submit_btn.setOutlineColor(sf::Color::Black);
    submit_btn.setOutlineThickness(2);
    submit_btn.setPosition(350, 110);

    sf::Text submit_btn_innerText("Submit", font, 24);
    submit_btn_innerText.setFillColor(sf::Color::Black);
    set_mid(submit_btn, submit_btn_innerText);

    bool inyear = false, inmonth = false, inday = false;// �O�_���b��J
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

            // �ƹ��I����J��
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
            }

            // ��L��J��r
            if (inyear && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8) { // �B�zBackspace
                    if (!syear.empty()) {
                        syear.pop_back();
                    }
                }
                else if (event.text.unicode <= '9' && event.text.unicode >= '0') { // �B�z�^��r��
                    syear += static_cast<char>(event.text.unicode);
                    if (syear.size() > 4) {
                        syear = syear.substr(1, 4); // �����J����
                    }
                }
                inputYearText.setString(syear); // ��s�e���W����r
            }

            if (inmonth && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8) { // �B�zBackspace
                    if (!smonth.empty()) {
                        smonth.pop_back();
                    }
                }
                else if (event.text.unicode <= '9' && event.text.unicode >= '0') { // �B�z�^��r��
                    smonth += static_cast<char>(event.text.unicode);
                    if (smonth.size() > 2) {
                        smonth = smonth.substr(1, 2); // �����J����
                    }
                }
                inputMonthText.setString(smonth); // ��s�e���W����r
            }

            if (inday && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8) { // �B�zBackspace
                    if (!sday.empty()) {
                        sday.pop_back();
                    }
                }
                else if (event.text.unicode <= '9' && event.text.unicode >= '0') { // �B�z�^��r��
                    sday += static_cast<char>(event.text.unicode);
                    if (sday.size() > 2) {
                        sday = sday.substr(1, 2); // �����J����
                    }
                }
                inputDayText.setString(sday); // ��s�e���W����r
            }

        }

        window.clear(sf::Color(200, 200, 200));

        renderShape(window, { &inputYear, &inputMonth, &inputDay, &submit_btn , &Yeartxt, &Monthtxt, &Daytxt
            , &submit_btn_innerText, &inputDayText, &inputMonthText, &inputYearText });

        window.display();
    }
    changing.store(false); // ���� flag
    return;
}

string Date::getString() {
    string s = to_string(year) + "-" + to_string(month) + "-" + to_string(day);
    return s;
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
    copyAmount = 1;
    availableCopies = 1;
    changing.store(false);
};

void Book::change() {
    if (changing.exchange(true)) {
        cout << "Already running input text window." << endl;
        return;
    }
    sf::RenderWindow window(sf::VideoMode(530, 540), "ADD Book");

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
    submit_btn.setFillColor(sf::Color::Green);
    submit_btn.setOutlineColor(sf::Color::Black);
    submit_btn.setOutlineThickness(2);
    submit_btn.setPosition(380, 50);
    sf::Text submit_btn_innerText("Submit", font, 24);
    submit_btn_innerText.setFillColor(sf::Color::Black);
    set_mid(submit_btn, submit_btn_innerText);

    bool inname = false, inauthor = false, inISBN = false, incategory = false, incopyAmount = false;// �O�_���b��J
    string sname(name), sauthor(author), sISBN(ISBN), scategory(category), scopyAmount(to_string(copyAmount));

    if (scopyAmount == "0") scopyAmount = "";

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            // �ƹ��I����J��
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
                        cout << "Please wait for the date to be set." << endl;
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
                else {
                    inname = false, inauthor = false, inISBN = false, incategory = false, incopyAmount = false;
                }
            }

            if (!published.changing.load()) {
                PublichedContent.setString(published.getString());
            }

            // ��L��J��r
            if (inname) {
                inputEvent(event, sname, 'A', 'z', 20);
                inputNameText.setString(sname); // ��s�e���W����r
            }
            if (inauthor) {
                inputEvent(event, sauthor, 'A', 'z', 20);
                inputAuthorText.setString(sauthor); // ��s�e���W����r
            }
            if (inISBN) {
                inputEvent(event, sISBN, '0', '9', 10);
                inputISBNText.setString(sISBN); // ��s�e���W����r
            }
            if (incategory) {
                inputEvent(event, scategory, 'A', 'z', 20);
                inputCategoryText.setString(scategory); // ��s�e���W����r
            }
            if (incopyAmount) {
                inputEvent(event, scopyAmount, '0', '9', 3);
                inputcopyAmountText.setString(scopyAmount); // ��s�e���W����r
            }
        }
        window.clear(sf::Color(200, 200, 200));

        renderShape(window, { &inputName, &inputAuthor, &inputISBN, &inputCategory
            , &inputcopyAmount, &Published_btn, &submit_btn , &Nametxt, &Authortxt, &ISBNtxt, &Categorytxt, &copyAmounttxt
            , &submit_btn_innerText, &inputNameText, &inputAuthorText, &inputISBNText
            , &inputCategoryText, &inputcopyAmountText, &PublishedText , &PublichedContent });

        window.display();
    }
    changing.store(false);
    return;
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