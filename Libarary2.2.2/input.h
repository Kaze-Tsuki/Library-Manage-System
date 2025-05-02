#ifndef inputWindow

#define inputWindow

#include <SFML/Graphics.hpp>
#include <iostream>
#include <atomic>

using namespace std;

extern atomic<bool> runningInputText;

extern sf::Font font;

extern void set_mid(sf::RectangleShape&, sf::Text&);

// occupies [y - 25, y + 5 + inputText height] 
extern void initInputBox(sf::RectangleShape& inputBox, sf::Text& inputText, sf::Text& Label, const size_t& x, const size_t&y);

extern void initButton(sf::RectangleShape& btn, sf::Text& txt, const size_t& x, const size_t& y);

extern void OpenInputText(string& s);

extern void inputEvent(sf::Event& event, string& s, const char& cmin, const char& cmax, const int& length);

extern void renderShape(sf::RenderWindow& window, const vector<sf::Drawable*> drawables);

extern string getClipboardText();

#endif