#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/window.hpp>
#include <SFML/system.hpp>

#include "Grid.h"

int main()
{
    std::cout << "Hello World!\n";
    //sf::RenderWindow window(sf::VideoMode(1200, 1000), "Orbiting", sf::Style::Close | sf::Style::Titlebar); //sf::Style::Fullscreen
    sf::RenderWindow window(sf::VideoMode(1200, 1000), "GoL", sf::Style::Fullscreen);
    sf::Event event;

    sf::err().rdbuf(NULL);
    sf::Texture grid_texture;
    grid_texture.loadFromFile("cells_texture.png");
    Grid<80, 60> grid(grid_texture);
    grid.setPosition(50, 65);
    grid.rescale(0.5f);

    sf::Font font; font.loadFromFile("arial.ttf");
    sf::Text hint("spacebar = next step", font); hint.setFillColor(sf::Color::Green);
    sf::Text ac_text("alive cells: ", font); ac_text.setFillColor(sf::Color::Green);
    ac_text.setPosition(0, 30);
    sf::Text amount_text("0", font); amount_text.setFillColor(sf::Color::Green);
    amount_text.setPosition(150, 30);

    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
        if (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) window.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                amount_text.setString(std::to_string( grid.click(event.mouseButton.x, event.mouseButton.y) ));
                grid.updateGrid();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    amount_text.setString(std::to_string(grid.step_up()));
                    grid.updateGrid();
                }
                if (event.key.code == sf::Keyboard::R) {
                    grid.reset();
                    amount_text.setString("0");
                    grid.updateGrid();
                }
            }
        }

        //sf::CircleShape circle(200, 9);
        //circle.setPosition(((sf::Vector2f)window.getSize()) / 2.f);

        //window.draw(circle);
        window.draw(grid);
        window.draw(hint);
        window.draw(ac_text);
        window.draw(amount_text);
        window.display();
    }
}