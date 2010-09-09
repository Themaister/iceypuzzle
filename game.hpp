#ifndef __GAME_HPP
#define __GAME_HPP

#include <SFML/Graphics.hpp>
#include "level.hpp"
#include "movement.hpp"


namespace Game 
{

class PuzzleGame
{
   public:
      PuzzleGame(sf::RenderWindow& in);

      void run();

   private:
      sf::RenderWindow& app;
      Level level;
      Movement::Movement movement;
      Button::Button button;
      bool handle_input();

};
}

#endif
