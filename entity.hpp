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

      virtual void update() = 0;
      virtual void render(sf::RenderWindow& app) const = 0;

      sf::Vector2i pos() const;
      void pos(const sf::Vector2i& tile_pos);

      sf::Vector2i speed() const;
      void speed(const sf::Vector2i& speed);

      void tile_size(const sf::Vector2i& size);
      Movement::Movement direction() const;

      bool operator==(const Entity& in);

   protected:
      sf::Sprite sprite;
      sf::Vector2i m_tile_size;
      sf::Vector2i obj_speed;
      Movement::Movement dir;
};

class Wall : public Entity
{
   public:
      Wall();
      void update();
      void render(sf::RenderWindow& app) const;

};

class Hero : public Entity
{
   public:
      Hero();
      void update();
      void render(sf::RenderWindow& app) const;

};

class Stone : public Entity
{
   public:
      Stone();
      void update();
      void render(sf::RenderWindow& app) const;

};

class SlipStone : public Entity
{
   public:
      SlipStone();
      void update();
      void render(sf::RenderWindow& app) const;

};
}

#endif
