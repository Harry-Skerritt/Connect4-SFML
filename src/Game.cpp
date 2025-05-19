
#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window) {
  srand(time(NULL));
}
Game::~Game()
{
}

bool Game::init()
{

  // Init grid
  for (int i = 0; i < GRID_X; i++) {
    for (int j = 0; j < GRID_Y; j++) {
      grid_array[i][j] = 0;
    }
  }

  initCircles();

  grid_background.setSize({
    static_cast<float>(window.getSize().x),
     static_cast<float>(window.getSize().y) * 0.8f
  });
  grid_background.setFillColor(background_colour);
  grid_background.setPosition({ 0, 200 - (GRID_GAP*1.5f) });

  return true;
}

void Game::update(float dt) {
  updateCircles();
}

void Game::render() {

  window.draw(grid_background);

  for (int i = 0; i < GRID_X; i++) {
    for (int j = 0; j < GRID_Y; j++) {
      window.draw(circle_grid[i][j]);
    }
  }

}

// *** Keyboard Events ***
void Game::keyPressed(sf::Event::KeyPressed key_press) {

}

void Game::keyReleased(sf::Event::KeyReleased key_released) {

}

// *** Mouse Events ***
void Game::mousePressed(sf::Event::MouseButtonPressed mouse_pressed) {

}

void Game::mouseReleased(sf::Event::MouseButtonReleased mouse_released) {

}

void Game::mouseScrolled(sf::Event::MouseWheelScrolled mouse_scroll) {

}

void Game::mouseMoved(sf::Event::MouseMoved mouse_moved) {

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




