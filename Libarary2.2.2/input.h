#ifndef inputWindow

#define inputWindow

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

extern bool runningInputText;

extern sf::Font font;

extern void set_mid(sf::RectangleShape&, sf::Text&);

void OpenInputText(string& s);

#endif