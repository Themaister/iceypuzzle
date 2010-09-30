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

Movement Entity::direction() const
{
   return dir;
}

void Entity::tile_size(const Vector2i& size)
{
   m_tile_size = size;
}

bool Entity::operator==(const Entity& in)
{
   return this == &in;
}

void Entity::update(bool can_stop)
{
   (void)can_stop;
   obj_speed = Vector2i(0, 0);
}

void Entity::render(RenderWindow& app) const
{
   app.Draw(sprite);
}

bool Entity::is_floor(bool slippery) const
{
   (void)slippery;
   return false;
}

void Stone::update(bool can_stop)
{
   sprite.Move(obj_speed.x * m_tile_size.x, obj_speed.y * m_tile_size.y);
   if (can_stop)
      obj_speed = Vector2i(0, 0);
}

void SlipStone::update(bool can_stop)
{
   (void)can_stop;
   sprite.Move(obj_speed.x * m_tile_size.x, obj_speed.y * m_tile_size.y);
}

void Hero::update(bool can_stop)
{
   sprite.Move(obj_speed.x * m_tile_size.x, obj_speed.y * m_tile_size.y);
   if (can_stop)
      obj_speed = Vector2i(0, 0);
}

bool Floor::is_floor(bool slippery) const
{
   return !slippery;
}

bool SlipperyFloor::is_floor(bool slippery) const
{
   (void)slippery;
   return true;
}
