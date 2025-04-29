#include "input.h"
#include "Libarary.h"
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <exception>

using namespace std;

Date::Date(){
	sf::RenderWindow window(sf::VideoMode(600, 170), "Text Input");

    // 輸入框
    // Year
    sf::RectangleShape inputYear(sf::Vector2f(400, 50));
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
    sf::RectangleShape inputMonth(sf::Vector2f(400, 50));
    inputMonth.setFillColor(sf::Color::White);
    inputMonth.setOutlineColor(sf::Color::Black);
    inputMonth.setOutlineThickness(2);
    inputMonth.setPosition(50, 50);

    sf::Text Monthtxt("Year", font, 20);
    Monthtxt.setFillColor(sf::Color::Black);
    Monthtxt.setPosition(55, 20);

    sf::Text inputMonthText("", font, 24);
    inputMonthText.setFillColor(sf::Color::Black);
    inputMonthText.setPosition(55, 55);

    // Day
    sf::RectangleShape inputDay(sf::Vector2f(400, 50));
    inputDay.setFillColor(sf::Color::White);
    inputDay.setOutlineColor(sf::Color::Black);
    inputDay.setOutlineThickness(2);
    inputDay.setPosition(50, 50);

    sf::Text Daytxt("Year", font, 20);
    Daytxt.setFillColor(sf::Color::Black);
    Daytxt.setPosition(55, 20);

    sf::Text inputDayText("", font, 24);
    inputDayText.setFillColor(sf::Color::Black);
    inputDayText.setPosition(55, 55);

    sf::RectangleShape submit_btn(sf::Vector2f(100, 50));
    submit_btn.setFillColor(sf::Color::Green);
    submit_btn.setOutlineColor(sf::Color::Black);
    submit_btn.setOutlineThickness(2);
    submit_btn.setPosition(250, 100);

    sf::Text submit_btn_innerText("Submit", font, 24);
    submit_btn_innerText.setFillColor(sf::Color::Black);
    set_mid(submit_btn, submit_btn_innerText);

    bool inyear = false, inmonth = false, inday = false;// 是否正在輸入
    string syear, smonth, sday;

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
                    else if (submit_btn.getGlobalBounds().contains(mousePos)) {
                        inyear = false, inmonth = false, inday = false;
                        /*try{year = stoi(syear);}
                        catch(const err& e)*/

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
                else if (event.text.unicode < 128) { // 處理英文字元
                    syear += static_cast<char>(event.text.unicode);
                }
                inputYearText.setString(syear); // 更新畫面上的文字
            }

        }

        window.clear(sf::Color(200, 200, 200));

        window.draw(inputYear);
        window.draw(inputMonth);
        window.draw(inputDay);
        window.draw(Yeartxt);
        window.draw(Monthtxt);
        window.draw(Daytxt);
        window.draw(inputYearText);
        window.draw(inputMonthText);
        window.draw(inputDayText);
        window.draw(submit_btn);
        window.draw(submit_btn_innerText);

        window.display();
    }
    return;
}