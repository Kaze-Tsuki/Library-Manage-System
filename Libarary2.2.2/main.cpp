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
    window.setFramerateLimit(10);

    if (!font.loadFromFile("arial.ttf")) { // �O�o���r����
        std::cout << "Failed to load font\n";
        return -1;
    }

    // ��J��
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



    std::string userInput = ""; // �x�s��J����r
	Library library;

    window.clear(sf::Color(70, 70, 70));

    renderShape(window, { &AddBook , &AddBookInnerText , &PrintBook , &PrintBookInnerText,
        &ListBooks , &ListBooksInnerText, &ListUsers, &ListUsersInnerText });

    window.display();

    while (window.isOpen())
    {
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
                        book->change();  // �l��������i���ܧ�

                        library.addBook(book);
                    }).detach();
                }
				else if (PrintBook.getGlobalBounds().contains(mousePos)) {
					// Print book
					cout << "Print Book" << endl;
					thread(&Library::printBooks, &library, 0).detach();
				}
                else if (ListBooks.getGlobalBounds().contains(mousePos)) {
                    // List books
                    cout << "List Books" << endl;
                    thread(&Library::listBooks, &library).detach();
                }
				else if (ListUsers.getGlobalBounds().contains(mousePos)) {
					// List users
					cout << "List Users" << endl;
					thread(&Library::listUsers, &library).detach();
				}
            }
        }

        window.clear(sf::Color(70, 70, 70));

		renderShape(window, { &AddBook , &AddBookInnerText , &PrintBook , &PrintBookInnerText,
            &ListBooks , &ListBooksInnerText });

        window.display();
    }

    return 0;
}
