#ifndef __LEVEL_HPP
#define __LEVEL_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "entity.hpp"
//#include "game.hpp"
#include "movement.hpp"

namespace Game {

class Entity;

class Level
{
   public:
      Level(sf::RenderWindow& app);
      Level(sf::RenderWindow& app, const std::string& in);

      void LoadLevelFromFile(const std::string& in);
      void SetMovement(const Movement::Movement& mov);
      void SetButton(const Button::Button& btn);

      void handle_logic();
      void check_collition();
      void update();
      void render();
      bool is_won() const;
      void reset();

   private:
      std::vector<sf::Image> images;
      typedef std::shared_ptr<Entity> Entity_Ptr;
      std::vector<Entity_Ptr> entities;
      std::vector<Entity_Ptr> floor;
      Entity_Ptr character;

      sf::RenderWindow& app;
      sf::Vector2i level_size;
      sf::Vector2i tile_size;
      Movement::Movement movement;
      Button::Button button;
      std::string m_path;

      void LoadPictures();
};
}

#endif
