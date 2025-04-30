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
    sf::RectangleShape input_text_btn(sf::Vector2f(60, 30));
    sf::Text input_text_innerText("Input", font, 20);
	initButton(input_text_btn, input_text_innerText, 20, 20);

    sf::RectangleShape AddBook(sf::Vector2f(60, 30));
	sf::Text AddBookInnerText("Add Book", font, 20);
	initButton(AddBook, AddBookInnerText, 100, 20);

    std::string userInput = ""; // 儲存輸入的文字

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    if (input_text_btn.getGlobalBounds().contains(mousePos)) {
                        thread(OpenInputText, ref(userInput)).detach();
                    }
					else if (AddBook.getGlobalBounds().contains(mousePos)) {
						Book book;
						book.change();
						book.print_book();
					}
                }
            }
            

        }

        window.clear(sf::Color(70, 70, 70));

		renderShape(window, { &input_text_btn , &input_text_innerText , &AddBook , &AddBookInnerText });

        window.display();
    }

    return 0;
}
