#include "game.hpp"
#include <iostream>
#include <stdexcept>

int main()
{
   sf::RenderWindow app(sf::VideoMode(800, 600, 32), "Puzzle");
   try 
   {
      Game::PuzzleGame game(app);
      game.run();
   }
   catch (std::exception& err)
   {
      std::cerr << err.what() << std::endl;
   }
   app.Close();
}
