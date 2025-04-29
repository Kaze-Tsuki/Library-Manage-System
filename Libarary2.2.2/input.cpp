#include <SFML/Graphics.hpp>
#include <iostream>
#include "input.h"

using namespace std;

bool runningInputText = false;

void set_mid(sf::RectangleShape& rect, sf::Text& txt)
{
    // set position mid
    sf::FloatRect textBounds = txt.getLocalBounds();
    txt.setOrigin(textBounds.left + textBounds.width / 2.0f,
        textBounds.top + textBounds.height / 2.0f);
    sf::Vector2f buttonCenter = rect.getPosition() + rect.getSize() / 2.0f;
    txt.setPosition(buttonCenter);
}

void OpenInputText(string& s)
{
    sf::RenderWindow window(sf::VideoMode(600, 130), "Text Input");

    // 輸入框
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

    bool isTyping = false;    // 是否正在輸入
    std::string userInput = ""; // 儲存輸入的文字

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
                    if (inputBox.getGlobalBounds().contains(mousePos)) {
                        isTyping = true;  // 點到輸入框，開始輸入
                    }
                    else if (submit_btn.getGlobalBounds().contains(mousePos)) {
                        isTyping = false; // 結束輸入
                        cout << "輸入了: " << userInput << endl;
                        s = userInput;
                        runningInputText = false;
                        return;
                    }
                    else {
                        isTyping = false;
                    }
                }
            }

            // 鍵盤輸入文字
            if (isTyping && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8) { // 處理Backspace
                    if (!userInput.empty()) {
                        userInput.pop_back();
                    }
                }
                else if (event.text.unicode < 128) { // 處理英文字元
                    userInput += static_cast<char>(event.text.unicode);
                }
                inputText.setString(userInput); // 更新畫面上的文字
            }

        }

        window.clear(sf::Color(200, 200, 200));

        window.draw(inputBox);
        window.draw(inputText);
        window.draw(submit_btn);
        window.draw(submit_btn_innerText);

        window.display();
    }
    runningInputText = false;
    return;
}