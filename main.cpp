#include "game.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char *argv[])
{
   sf::RenderWindow app(sf::VideoMode(800, 600, 32), "Puzzle");

   try 
   {
      if (argc == 1)
      {
         Game::PuzzleGame game(app);
         game.run();
      }
      else
      {
         int i = 0;
         bool running = true;
         while (running && ++i < argc)
         {
            Game::PuzzleGame game(app, argv[i]);
            running = game.run();
         }
      }
   }
   catch (std::exception& err)
   {
      std::cerr << err.what() << std::endl;
   }
   app.Close();
}
