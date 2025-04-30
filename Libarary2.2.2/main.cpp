#include <SFML/Graphics.hpp>
#include "input.h"
#include "Library.h"
#include <thread>
#include <iostream>

using namespace std;

sf::Font font;

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 400), "Main Interface");

    if (!font.loadFromFile("arial.ttf")) { // 記得有字體檔
        std::cout << "Failed to load font\n";
        return -1;
    }

    // 輸入框
	// AddBook
    sf::RectangleShape AddBook(sf::Vector2f(100, 50));
	sf::Text AddBookInnerText("Add Book", font, 20);
	initButton(AddBook, AddBookInnerText, 30, 30);

	// PrintBook
	sf::RectangleShape PrintBook(sf::Vector2f(100, 50));
	sf::Text PrintBookInnerText("Print Book", font, 20);
	initButton(PrintBook, PrintBookInnerText, 30, 100);


    std::string userInput = ""; // 儲存輸入的文字
	Library library;
    Book* book;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                if (AddBook.getGlobalBounds().contains(mousePos)) {
					book = new Book();
                    book->change();
					library.addBook(book);
                }
				else if (PrintBook.getGlobalBounds().contains(mousePos)) {
					// Print book
					cout << "Print Book" << endl;
					thread(&Library::printBooks, &library).detach();
				}
				
            }
            

        }

        window.clear(sf::Color(70, 70, 70));

		renderShape(window, { &AddBook , &AddBookInnerText , &PrintBook , &PrintBookInnerText });

        window.display();
    }

    return 0;
}
