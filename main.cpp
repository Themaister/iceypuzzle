#include "game.hpp"
#include <iostream>

int main()
{
   sf::RenderWindow app(sf::VideoMode(800, 600, 32), "Puzzle");
   try 
   {
      Game::PuzzleGame game(app);
      game.run();
   }
   catch (const Game::EInvalidLevel& err)
   {
      std::cerr << err.err() << std::endl;
   }
   app.Close();
}
