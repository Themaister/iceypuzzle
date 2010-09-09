#include "game.hpp"

int main()
{
   sf::RenderWindow app(sf::VideoMode(256, 256, 32), "Puzzle");
   Game::PuzzleGame game(app);
   game.run();
}
