#include <SFML/Graphics.hpp>
#include <iostream>
#include "input.h"
#include <thread>

using namespace std;

atomic<bool> runningInputText = false;

void set_mid(sf::RectangleShape& rect, sf::Text& txt)
{
    // set position mid
    sf::FloatRect textBounds = txt.getLocalBounds();
    txt.setOrigin(textBounds.left + textBounds.width / 2.0f,
        textBounds.top + textBounds.height / 2.0f);
    sf::Vector2f buttonCenter = rect.getPosition() + rect.getSize() / 2.0f;
    txt.setPosition(buttonCenter);
}

void initInputBox(sf::RectangleShape& inputBox, sf::Text& inputText, sf::Text& Label, const size_t& x, const size_t& y)
{
	inputBox.setFillColor(sf::Color::White);
	inputBox.setOutlineColor(sf::Color::Black);
	inputBox.setOutlineThickness(2);
	inputBox.setPosition(x, y);
	Label.setFillColor(sf::Color::Black);
	Label.setPosition(x + 5, y - 25);
	inputText.setFillColor(sf::Color::Black);
	inputText.setPosition(x + 5, y + 5);
}

void initButton(sf::RectangleShape& btn, sf::Text& txt, const size_t& x, const size_t& y)
{
	btn.setFillColor(sf::Color(180, 180, 180));
	btn.setOutlineColor(sf::Color::Black);
	btn.setOutlineThickness(1);
	btn.setPosition(x, y);
	txt.setFillColor(sf::Color::Black);
	set_mid(btn, txt);
}

void renderShape(sf::RenderWindow& window, const vector<sf::Drawable*> drawables)
{
	for (auto drawable : drawables) {
		window.draw(*drawable);
	}
}

void inputEvent(sf::Event& event, string& s, const char& cmin, const char& cmax, const int& length)
{
	if (event.type == sf::Event::TextEntered) {
		if (event.text.unicode == 8) { // �B�zBackspace
			if (!s.empty()) {
				s.pop_back();
			}
		}
		else if (event.text.unicode >= cmin && event.text.unicode <= cmax) { // �B�z�^��r��
			s += static_cast<char>(event.text.unicode);
			if (s.size() > length) {
				s = s.substr(1, length); // �����J����
			}
		}
	}
}

void OpenInputText(string& s)
{
    if (runningInputText.exchange(true)) {
		cout << "Already running input text window." << endl;
		return;
    }
    sf::RenderWindow window(sf::VideoMode(600, 130), "Text Input");
    window.setFramerateLimit(30);

    // ��J��
    sf::RectangleShape inputBox(sf::Vector2f(400, 50));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(2);
    inputBox.setPosition(50, 55);

    sf::Text inputText("", font, 24);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(65, 65);

    sf::RectangleShape submit_btn(sf::Vector2f(80, 50));
    submit_btn.setFillColor(sf::Color::Green);
    submit_btn.setOutlineColor(sf::Color::Black);
    submit_btn.setOutlineThickness(2);
    submit_btn.setPosition(470, 55);

    sf::Text submit_btn_innerText("Submit", font, 24);
    submit_btn_innerText.setFillColor(sf::Color::Black);
    set_mid(submit_btn, submit_btn_innerText);

    bool isTyping = false;    // �O�_���b��J
    std::string userInput = ""; // �x�s��J����r

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
                if (inputBox.getGlobalBounds().contains(mousePos)) {
                    isTyping = true;  // �I���J�ءA�}�l��J
                }
                else if (submit_btn.getGlobalBounds().contains(mousePos)) {
                    isTyping = false; // ������J
                    cout << "��J�F: " << userInput << endl;
                    s = userInput;
                    runningInputText.store(false); // ���� flag
                    return;
                }
                else {
                    isTyping = false;
                }
            }

            // ��L��J��r
            if (isTyping && event.type == sf::Event::TextEntered) {
				inputEvent(event, userInput, ' ', '~', 20); // �����J�d��M����
				inputText.setString(userInput); // ��s�e���W����r
                window.clear(sf::Color(200, 200, 200));

                renderShape(window, { &submit_btn , &submit_btn_innerText, &inputBox , &inputText });

                window.display();
            }
        }

    }
    runningInputText.store(false); // ���� flag
    return;
}