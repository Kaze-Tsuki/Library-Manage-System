#include <SFML/Graphics.hpp>
#include <iostream>
#include "input.h"
#include <thread>

using namespace std;

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
	inputBox.setOutlineColor(sf::Color(167, 167, 211));
	inputBox.setOutlineThickness(2);
	inputBox.setPosition(x, y);
	Label.setFillColor(sf::Color::Black);
	Label.setPosition(x + 5, y - 25);
	inputText.setFillColor(sf::Color::Black);
	inputText.setPosition(x + 5, y + 5);
}

void initButton(sf::RectangleShape& btn, sf::Text& txt, const size_t& x, const size_t& y)
{
	btn.setFillColor(sf::Color(255, 229, 204));
	btn.setOutlineColor(sf::Color(79, 79, 79));
	btn.setOutlineThickness(1);
	btn.setPosition(x, y);
	txt.setFillColor(sf::Color(40, 0, 77));
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
		if (event.text.unicode == 8) { // 處理Backspace
			if (!s.empty()) {
				s.pop_back();
			}
		}
		else if (event.text.unicode == 22) { // Ctrl + V
			s += sf::Clipboard::getString();
			if (s.size() > length) {
				s = s.substr(0, length); // 限制輸入長度
			}
		}
		else if (event.text.unicode >= cmin && event.text.unicode <= cmax) { // 處理英文字元
			s += static_cast<char>(event.text.unicode);
			if (s.size() > length) {
				s = s.substr(1, length); // 限制輸入長度
			}
		}
	}
}

void inputText::OpenInputText(string& s)
{
    if (runningInputText.exchange(true)) {
		thread([]() { errorWindow("Already running input text window."); }).detach();
		return;
    }
    sf::RenderWindow window(sf::VideoMode(600, 130), "Text Input");
    window.setFramerateLimit(20);

    // 輸入框
    sf::RectangleShape inputBox(sf::Vector2f(400, 50));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color(167, 167, 211));
    inputBox.setOutlineThickness(2);
    inputBox.setPosition(50, 55);

    sf::Text inputText("", font, 24);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(65, 65);

    sf::RectangleShape submit_btn(sf::Vector2f(80, 50));
    submit_btn.setFillColor(sf::Color(168, 255, 255));
    submit_btn.setOutlineColor(sf::Color(167, 167, 211));
    submit_btn.setOutlineThickness(2);
    submit_btn.setPosition(470, 55);

    sf::Text submit_btn_innerText("Submit", font, 24);
    submit_btn_innerText.setFillColor(sf::Color::Black);
    set_mid(submit_btn, submit_btn_innerText);

    bool isTyping = false;    // 是否正在輸入
    std::string userInput = ""; // 儲存輸入的文字
    window.clear(sf::Color(244, 255, 229));
    renderShape(window, { &submit_btn , &submit_btn_innerText, &inputBox , &inputText });

    window.display();

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
                if (inputBox.getGlobalBounds().contains(mousePos)) {
                    isTyping = true;  // 點到輸入框，開始輸入
                }
                else if (submit_btn.getGlobalBounds().contains(mousePos)) {
                    isTyping = false; // 結束輸入
                    s = userInput;
                    runningInputText.store(false); // 關閉 flag
                    return;
                }
                else {
                    isTyping = false;
                }
            }

            // 鍵盤輸入文字
            if (isTyping && event.type == sf::Event::TextEntered) {
				inputEvent(event, userInput, ' ', '~', 20); // 限制輸入範圍和長度
				inputText.setString(userInput); // 更新畫面上的文字
                window.clear(sf::Color(244, 255, 229));

                renderShape(window, { &submit_btn , &submit_btn_innerText, &inputBox , &inputText });

                window.display();
            }
        }

    }
    runningInputText.store(false); // 關閉 flag
    return;
}

void errorWindow(string s)
{
	sf::RenderWindow window(sf::VideoMode(500, 70), "Error");
	window.setFramerateLimit(0);
	sf::Text errorText(s, font, 30);
	errorText.setFillColor(sf::Color::Red);
	errorText.setPosition(20, 5);
	window.clear(sf::Color(244, 255, 229));
	window.draw(errorText);
	window.display();
	sf::Event event;
	while (window.waitEvent(event)) {
		window.close();
	}
}