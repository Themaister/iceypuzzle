#include "game.hpp"

int main()
{
   sf::RenderWindow app(sf::VideoMode(640, 480, 32), "Puzzle");
   Game::PuzzleGame game(app);
   game.run();
}
