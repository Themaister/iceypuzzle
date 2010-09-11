#include "game.hpp"
#include <iostream>

using namespace Game;
using namespace sf;
using namespace std;

PuzzleGame::PuzzleGame(RenderWindow& in) : app(in), level(Level(app, "level.txt"))
{
}

void PuzzleGame::run()
{
   bool running = true;
   while (running)
   {
      if (level.is_won())
      {
         cout << "OMG U WON, SO 1337" << endl;
         break;
      }
      running = handle_input();
      level.handle_logic();
      level.check_collition();
      level.update();
      level.render();
   }
}

bool PuzzleGame::handle_input()
{
   Event event;
   movement = Movement::None;
   button = Button::None;
   while (app.GetEvent(event))
   {
      if (event.Type == Event::KeyPressed)
      {
         auto code = event.Key.Code;
         if (code == Key::Escape)
            return false;

         if (code == Key::Up)
            movement = Movement::Up;
         else if (code == Key::Down)
            movement = Movement::Down;
         else if (code == Key::Left)
            movement = Movement::Left;
         else if (code == Key::Right)
            movement = Movement::Right;
         else
            movement = Movement::None;

         if (code == Key::Z)
            button = Button::A;
         else if (code == Key::R)
         {
            level.reset();
            break;
         }
         else
            button = Button::None;
      }
      else if (event.Type == Event::Closed)
         return false;
   }

   level.SetMovement(movement);
   level.SetButton(button);
   return true;
}

