
#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window) {
  srand(time(NULL));
}

Game::~Game()
{
}

bool Game::init() {
  // Load Font
  if (!atma_font.openFromFile("Data/Fonts/Atma/Atma-Bold.ttf")) {
    std::cerr << "Failed to load font" << std::endl;
    return false;
  }


  // Init grid
  for (int i = 0; i < GRID_X; i++) {
    for (int j = 0; j < GRID_Y; j++) {
      grid_array[i][j] = 0;
    }
  }

  // Init Background
  grid_background.setSize({
    static_cast<float>(window.getSize().x),
     static_cast<float>(window.getSize().y) * 0.8f
  });
  grid_background.setFillColor(background_colour);
  grid_background.setPosition({ 0, 200 - (GRID_GAP*1.5f) });

  initCircles();
  initCursor(55.f);

  // Init Text
  warning_text = std::make_unique<FadeText>(atma_font, warning_string, .3f, 1.2f, background_colour);
  warning_text->setFont(atma_font);
  warning_text->setString(warning_string);
  warning_text->setCharacterSize(50);
  warning_text->setOrigin(warning_text->getLocalBounds().getCenter());
  warning_text->setPosition({
   static_cast<float>(window.getSize().x) / 2,
   100 });




  return true;
}

void Game::update(float dt) {
  updateCircles();
  updateCursor(dt);
  updateBoard();

  warning_text->update();

  if (checkWin()) {
    game_enabled = false;
    std::cerr << "Game Over" << std::endl;
  }
}

void Game::render() {

  window.draw(grid_background);

  for (int i = 0; i < GRID_X; i++) {
    for (int j = 0; j < GRID_Y; j++) {
      window.draw(circle_grid[i][j]);
    }
  }

  if (draw_cursor) {
    window.draw(cursor_counter);
  }

  window.draw(*warning_text);

  drawPointer(window, GRID_X, 10.f, 40.f, sf::Color(128, 128, 128, 64));


}

// *** Keyboard Events ***
void Game::keyPressed(sf::Event::KeyPressed key_press) {

}

void Game::keyReleased(sf::Event::KeyReleased key_released) {

}

// *** Mouse Events ***
void Game::mousePressed(sf::Event::MouseButtonPressed mouse_pressed) {

  if (game_enabled) {
    cursorClicked();
  }

}

void Game::mouseReleased(sf::Event::MouseButtonReleased mouse_released) {

}

void Game::mouseScrolled(sf::Event::MouseWheelScrolled mouse_scroll) {

}

void Game::mouseMoved(sf::Event::MouseMoved mouse_moved) {

}

void Game::updateBoard() {
  for (int x = 0; x < GRID_X; ++x) {
    // from bottom-1 row up to the top row
    for (int y = GRID_Y - 2; y >= 0; --y) {
      if (grid_array[x][y] != 0) {
        if (grid_array[x][y + 1] == 0) {
          grid_array[x][y + 1] = grid_array[x][y];
          grid_array[x][y] = 0;
        }
      }
    }
  }
}

bool Game::checkWin(int win_length) {
  // Loop through every cell in the grid
  for (int x = 0; x < GRID_X; ++x) {
    for (int y = 0; y < GRID_Y; ++y) {
      int player = grid_array[x][y];
      if (player == 0) continue;  // empty cell, skip
            
      // Check horizontal
      if (x + win_length <= GRID_X) {
        bool win = true;
        for (int i = 1; i < win_length; ++i) {
          if (grid_array[x + i][y] != player) {
            win = false;
            break;
          }
        }
        if (win) return true;
      }
            
      // Check vertical
      if (y + win_length <= GRID_Y) {
        bool win = true;
        for (int i = 1; i < win_length; ++i) {
          if (grid_array[x][y + i] != player) {
            win = false;
            break;
          }
        }
        if (win) return true;
      }
            
      // Check diagonal down-right
      if (x + win_length <= GRID_X && y + win_length <= GRID_Y) {
        bool win = true;
        for (int i = 1; i < win_length; ++i) {
          if (grid_array[x + i][y + i] != player) {
            win = false;
            break;
          }
        }
        if (win) return true;
      }

      // Check diagonal down-left
      if (x - win_length + 1 >= 0 && y + win_length <= GRID_Y) {
        bool win = true;
        for (int i = 1; i < win_length; ++i) {
          if (grid_array[x - i][y + i] != player) {
            win = false;
            break;
          }
        }
        if (win) return true;
      }
    }
  }
    
  // No win found
  return false;
}



// Circle Grid
void Game::initCircles() {
  float grid_height = GRID_Y * (2 * CIRCLE_RADIUS + GRID_GAP) - GRID_GAP;
  float start_y = (window.getSize().y - grid_height) / 2.f;

  if (start_y < 200.f) {
    start_y = 200.f;
  }

  float grid_width = GRID_X * (2 * CIRCLE_RADIUS + GRID_GAP) - GRID_GAP;
  float start_x = (window.getSize().x - grid_width) / 2.f;

  for (int row = 0; row < GRID_X; ++row) {
    for (int col = 0; col < GRID_Y; ++col) {
      sf::CircleShape circle;
      circle.setRadius(CIRCLE_RADIUS);
      circle.setPosition({
        start_x + row * (2 * CIRCLE_RADIUS + GRID_GAP),
        start_y + col * (2 * CIRCLE_RADIUS + GRID_GAP)});
      circle.setOutlineColor(sf::Color::Black);
      circle.setPointCount(CIRCLE_RADIUS * CIRCLE_RADIUS * CIRCLE_RADIUS);
      circle.setOutlineThickness(5.f);

      if (grid_array[row][col] == 1) {
        circle.setFillColor(player_one_colour);
      } else if (grid_array[row][col] == 2) {
        circle.setFillColor(player_two_colour);
      }
      else {
        circle.setFillColor(empty_colour);
      }

      circle_grid[row][col] = circle;
    }
  }

}

void Game::updateCircles() {
  for (int row = 0; row < GRID_X; ++row) {
    for (int col = 0; col < GRID_Y; ++col) {
      sf::CircleShape circle = circle_grid[row][col];

      if (grid_array[row][col] == 1) {
        circle.setFillColor(player_one_colour);
      } else if (grid_array[row][col] == 2) {
        circle.setFillColor(player_two_colour);
      }
      else {
        circle.setFillColor(empty_colour);
      }

      circle_grid[row][col] = circle;
    }
  }
}

// Cursor
void Game::initCursor(float x_offset) {
  cursor_counter;
  cursor_counter.setRadius(CIRCLE_RADIUS);
  cursor_counter.setOrigin(cursor_counter.getLocalBounds().getCenter());
  cursor_counter.setOutlineColor(sf::Color::Black);
  cursor_counter.setPointCount(CIRCLE_RADIUS * CIRCLE_RADIUS * CIRCLE_RADIUS);
  cursor_counter.setOutlineThickness(5.f);
  cursor_counter.setFillColor(player_one_colour);
  cursor_counter.setPosition({x_offset, CURSOR_Y});
}

void Game::updateCursor(float dt) {
  sf::Vector2u window_size = window.getSize();
  float column_width = static_cast<float>(window_size.x) / GRID_X;

  sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
  sf::Vector2f world_mouse_position = window.mapPixelToCoords(mouse_position);

  cursor_column_index = static_cast<int>(world_mouse_position.x / column_width);
  cursor_column_index = std::clamp(cursor_column_index, 0, GRID_X - 1);

  float snapped_x = cursor_column_index * column_width + column_width / 2.f;

  cursor_counter.setPosition({snapped_x, CURSOR_Y});

  if (current_go == 1) {
    cursor_counter.setFillColor(player_one_colour);
  }
  else if (current_go == 2) {
    cursor_counter.setFillColor(player_two_colour);
  }

  if (grid_array[cursor_column_index][0] != 0) {
    sf::Color temp_colour = cursor_counter.getFillColor();
    temp_colour.a = 128;
    cursor_counter.setFillColor(temp_colour);
  }
}

void Game::cursorClicked() {
  std::cout << cursor_column_index << std::endl;

  if (grid_array[cursor_column_index][0] == 0) {
    draw_cursor = false;
    grid_array[cursor_column_index][0] = current_go;
    current_go = 3 - current_go;

    draw_cursor = true;
  }
  else {
    std::cerr << "Spot not free" << std::endl;
    warning_text->fade();

  }
}

void Game::drawPointer(sf::RenderWindow& window, int num_columns, float top_y, float triangle_height, sf::Color color) {
  float windowWidth = static_cast<float>(window.getSize().x);
  float columnWidth = windowWidth / num_columns;

  for (int col = 0; col < num_columns; ++col) {
    sf::ConvexShape triangle;
    triangle.setPointCount(3);

    float xCenter = col * columnWidth + columnWidth / 2.f;

    // Triangle pointing downwards
    triangle.setPoint(0, sf::Vector2f(xCenter - triangle_height / 2.f, top_y));       // left
    triangle.setPoint(1, sf::Vector2f(xCenter + triangle_height / 2.f, top_y));       // right
    triangle.setPoint(2, sf::Vector2f(xCenter, top_y + triangle_height));             // bottom

    triangle.setFillColor(color);

    window.draw(triangle);
  }
}




