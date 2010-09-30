#ifndef __GAME_HPP
#define __GAME_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "level.hpp"
#include "movement.hpp"


namespace Game 
{

class PuzzleGame
{
   public:
      PuzzleGame(sf::RenderWindow& in, const char* level = "level.txt");

      bool run();

   private:
      sf::RenderWindow& app;
      Level level;
      Movement movement;
      Button button;
      bool handle_input();

};
}

#endif
