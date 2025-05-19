
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Network/NetworkManager.h"
#include "Widgets/FadeText.h"

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
 bool draw_cursor = true;
 const float CURSOR_Y = 100.f;
 int cursor_column_index;
 void initCursor(float x_offset);
 void updateCursor(float dt);
 void cursorClicked();
 void drawPointer(sf::RenderWindow& window, int num_columns, float top_y, float triangle_height, sf::Color color);

 // Warning Text
 sf::Font atma_font;
 //FadeText* warning_text = nullptr;
 std::unique_ptr<FadeText> warning_text;
 std::string warning_string = "Space Not Free!";



 // Game State
 int current_go = 1;
 bool game_enabled = true;
 void updateBoard();
 bool checkWin(int win_length = 4);


};


#endif // GAME_H
