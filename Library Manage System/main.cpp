#include <SFML/Graphics.hpp>
#include "input.h"
#include "Library.h"
#include <thread>
#include <iostream>
#include "FileIO.h"

using namespace std;

sf::Font font;

int main()
{
    sf::RenderWindow window(sf::VideoMode(350, 350), "Main Interface");
    window.setFramerateLimit(10);

    if (!font.loadFromFile("arial.ttf")) { // 記得有字體檔
        std::cout << "Failed to load font\n";
        return -1;
    }

    std::string userInput = ""; // 儲存輸入的文字
	Library library;
	struct inputText inputText;

    // 輸入框
	// AddBook
    sf::RectangleShape AddBook(sf::Vector2f(100, 50));
	sf::Text AddBookInnerText("Add Book", font, 20);
	initButton(AddBook, AddBookInnerText, 30, 30);

	// PrintBook
	sf::RectangleShape PrintBook(sf::Vector2f(100, 50));
	sf::Text PrintBookInnerText("Print Book", font, 20);
	initButton(PrintBook, PrintBookInnerText, 30, 100);

	// ListBooks
	sf::RectangleShape ListBooks(sf::Vector2f(100, 50));
	sf::Text ListBooksInnerText("List Books", font, 20);
	initButton(ListBooks, ListBooksInnerText, 30, 170);

	// ListUsers
	sf::RectangleShape ListUsers(sf::Vector2f(100, 50));
	sf::Text ListUsersInnerText("List Users", font, 20);
	initButton(ListUsers, ListUsersInnerText, 30, 240);

    // Save
	sf::RectangleShape Save(sf::Vector2f(100, 50));
	sf::Text SaveInnerText("Save", font, 20);
	initButton(Save, SaveInnerText, 200, 30);

	// Load
	sf::RectangleShape Load(sf::Vector2f(100, 50));
	sf::Text LoadInnerText("Load", font, 20);
	initButton(Load, LoadInnerText, 200, 100);

    window.clear(sf::Color(235, 235, 255));

    renderShape(window, { &AddBook , &AddBookInnerText , &PrintBook , &PrintBookInnerText,
        &ListBooks , &ListBooksInnerText, &ListUsers, &ListUsersInnerText, &Load, &LoadInnerText,
		&Save, &SaveInnerText });

    window.display();

    sf::Event event;
        while (window.waitEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                if (AddBook.getGlobalBounds().contains(mousePos)) {
                    std::thread([&library]() {
                        Book* book = new Book();
						bool create = book->change();
						if (!create) return;
                        library.addBook(book);
                    }).detach();
                }
				else if (PrintBook.getGlobalBounds().contains(mousePos)) {
					// Print book
					thread(&Library::printBooks, &library, 0).detach();
				}
                else if (ListBooks.getGlobalBounds().contains(mousePos)) {
                    // List books
                    thread(&Library::listBooks, &library).detach();
                }
				else if (ListUsers.getGlobalBounds().contains(mousePos)) {
					// List users
					thread(&Library::listUsers, &library).detach();
				}
                else if (Save.getGlobalBounds().contains(mousePos)) {
                    // Save library
					thread([&library, &inputText]() {
						string savePath;
						if (inputText.runningInputText) {
							thread([]() { errorWindow("Already running input text window."); }).detach();
							return;
						}
						inputText.OpenInputText(savePath);
						if (savePath == "") {
							return;
						}
						saveLibrary(library, savePath + ".json");
					}).detach();
                }
				else if (Load.getGlobalBounds().contains(mousePos)) {
					// Load library
					thread([&library, &inputText]() {
						string savePath;
						if (inputText.runningInputText) {
							thread([]() { errorWindow("Already running input text window."); }).detach();
							return;
						}
						inputText.OpenInputText(savePath);
						if (savePath == "") {
							return;
						}
						loadLibrary(library, savePath + ".json");
					}).detach();
				}
            }
        }

    return 0;
}
