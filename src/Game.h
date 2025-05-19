
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Network/NetworkManager.h"

class Game
{
public:
 Game(sf::RenderWindow& window);
 ~Game();
 bool init();
 void update(float dt);
 void render();


 void keyPressed(sf::Event::KeyPressed key_press);
 void keyReleased(sf::Event::KeyReleased key_released);

 void mousePressed(sf::Event::MouseButtonPressed mouse_pressed);
 void mouseReleased(sf::Event::MouseButtonReleased mouse_released);
 void mouseScrolled(sf::Event::MouseWheelScrolled mouse_scroll);
 void mouseMoved(sf::Event::MouseMoved mouse_moved);


private:
 sf::RenderWindow& window;

 static constexpr int GRID_X = 7;
 static constexpr int GRID_Y = 6;

 int grid_array[GRID_X][GRID_Y];

 // Circles
 void initCircles();
 void updateCircles();

 sf::CircleShape circle_grid[GRID_X][GRID_Y];

 const int CIRCLE_RADIUS = 40;
 const float GRID_GAP = 18;
 const sf::Vector2f GRID_START = { GRID_GAP, 200 };

 sf::Color player_one_colour = sf::Color::Red;
 sf::Color player_two_colour = sf::Color::Yellow;
 sf::Color empty_colour = sf::Color(186, 227, 255, 255);

 // Background
 sf::RectangleShape grid_background;
 sf::Color background_colour = sf::Color(51, 91, 187, 255);

 // Game Cursor
 sf::CircleShape cursor_counter;
 void initCursor();
 void updateCursor(float dt);



 // Game State
 int current_go = 1;


};


#endif // GAME_H
