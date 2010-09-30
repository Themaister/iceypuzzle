#ifndef __ENTITY_HPP
#define __ENTITY_HPP

#include <SFML/Graphics.hpp>
#include "level.hpp"
#include "movement.hpp"

namespace Game {
class Entity
{
   public:
      Entity();

      void SetImage(const sf::Image& img);

      virtual void update(bool can_stop = true);
      virtual void render(sf::RenderWindow& app) const;
      virtual bool is_floor(bool slippery = false) const;

      sf::Vector2i pos() const;
      void pos(const sf::Vector2i& tile_pos);

      sf::Vector2i speed() const;
      void speed(const sf::Vector2i& speed);

      void tile_size(const sf::Vector2i& size);
      Movement direction() const;

      bool operator==(const Entity& in);

      virtual ~Entity() {}

   protected:
      sf::Sprite sprite;
      sf::Vector2i m_tile_size;
      sf::Vector2i obj_speed;
      Movement dir;
};

class Wall : public Entity
{};

class Hero : public Entity
{
   public:
      void update(bool can_stop);
};

class Stone : public Entity
{
   public:
      void update(bool can_stop);
};

class SlipStone : public Entity
{
   public:
      void update(bool can_stop);
};

class Floor : public Entity
{
   public:
      bool is_floor(bool slippery) const;
};

class SlipperyFloor : public Entity
{
   public:
      bool is_floor(bool slippery) const;

};

}

#endif
