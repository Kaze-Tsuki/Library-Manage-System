#include <SFML/Graphics.hpp>
#include "input.h"
#include "Libarary.h"
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
    input_text_btn.setFillColor(sf::Color(180, 180, 180));
    input_text_btn.setOutlineColor(sf::Color::Black);
    input_text_btn.setOutlineThickness(1);
    input_text_btn.setPosition(20, 20);

    sf::Text input_text_innerText("Input", font, 20);
    input_text_innerText.setFillColor(sf::Color::Black);
    // set position mid
    set_mid(input_text_btn, input_text_innerText);

    sf::RectangleShape AddBook(sf::Vector2f(60, 30));
    AddBook.setFillColor(sf::Color(180, 180, 180));
    AddBook.setOutlineColor(sf::Color::Black);
    AddBook.setOutlineThickness(1);
    AddBook.setPosition(20, 70);

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
                        if (runningInputText)
                            cout << "This window is already open!\n";
                        else {
                            runningInputText = true;
                            thread(OpenInputText, ref(userInput)).detach();
                        }
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
        window.draw(input_text_btn);
        window.draw(input_text_innerText);
		window.draw(AddBook);

        window.display();
    }

    return 0;
}
