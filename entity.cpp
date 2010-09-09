#include "entity.hpp"

using namespace std;
using namespace sf;
using namespace Game;

Entity::Entity() : dir(Movement::None)
{}

void Entity::SetImage(const Image& img)
{
   sprite.SetImage(img);
}

Vector2i Entity::pos() const
{
   Vector2f pos_f = sprite.TransformToGlobal(Vector2f(0.0, 0.0));
   Vector2i pos(pos_f.x, pos_f.y);
   pos.x /= m_tile_size.x; 
   pos.y /= m_tile_size.y; 
   return pos;
}

void Entity::pos(const Vector2i& tile_pos)
{
   sprite.SetPosition(tile_pos.x * m_tile_size.x, tile_pos.y * m_tile_size.y);
}

Vector2i Entity::speed() const
{
   return obj_speed;
}

void Entity::speed(const Vector2i& speed)
{
   obj_speed = speed;
   if (obj_speed.x > 0)
      dir = Movement::Right;
   else if (obj_speed.x < 0)
      dir = Movement::Left;
   else if (obj_speed.y > 0)
      dir = Movement::Down;
   else if (obj_speed.y < 0)
      dir = Movement::Up;
}

Movement::Movement Entity::direction() const
{
   return dir;
}

void Entity::tile_size(const Vector2i& size)
{
   m_tile_size = size;
}

Wall::Wall() {}
void Wall::update()
{
   obj_speed = Vector2i(0, 0);
}

void Wall::render(RenderWindow& app) const
{
   app.Draw(sprite);
}

Stone::Stone() {}
void Stone::update()
{
   sprite.Move(obj_speed.x * m_tile_size.x, obj_speed.y * m_tile_size.y);
   obj_speed = Vector2i(0, 0);
}

void Stone::render(RenderWindow& app) const
{
   app.Draw(sprite);
}

SlipStone::SlipStone() {}
void SlipStone::update()
{
   sprite.Move(obj_speed.x * m_tile_size.x, obj_speed.y * m_tile_size.y);
}

void SlipStone::render(RenderWindow& app) const
{
   app.Draw(sprite);
}

Hero::Hero() {}
void Hero::update()
{
   sprite.Move(obj_speed.x * m_tile_size.x, obj_speed.y * m_tile_size.y);
   obj_speed = Vector2i(0, 0);
}

void Hero::render(RenderWindow& app) const
{
   app.Draw(sprite);
}

