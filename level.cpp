#include "level.hpp"
#include <functional>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
using namespace sf;
using namespace Game;

EInvalidLevel::EInvalidLevel(string&& str) : m_err(move(str)) {};
const string& EInvalidLevel::err() const
{
   return m_err;
}

namespace Images
{
   enum Images
   {
      Hero = 0,
      Wall,
      Stone,
      Slip,
      Floor,
   };
}

Level::Level(RenderWindow& in) : app(in)
{
   app.UseVerticalSync(true);
   app.SetFramerateLimit(30);
   LoadPictures();
}

Level::Level(RenderWindow& in, const string& path) : app(in), m_path(path)
{
   LoadPictures();
   LoadLevelFromFile(m_path);
}

void Level::reset()
{
   entities.clear();
   floor.clear();
   images.clear();
   LoadPictures();
   LoadLevelFromFile(m_path);
}

void Level::LoadPictures()
{
   images = vector<Image>(5);
   images[Images::Hero].LoadFromFile("hero.png");
   images[Images::Wall].LoadFromFile("wall.png");
   images[Images::Stone].LoadFromFile("stone.png");
   images[Images::Slip].LoadFromFile("slip.png");
   images[Images::Floor].LoadFromFile("floor.png");

   tile_size = Vector2i(images[Images::Hero].GetWidth(), images[Images::Hero].GetHeight());
}

void Level::SetMovement(const Movement::Movement& in)
{
   movement = in;
}

void Level::SetButton(const Button::Button& in)
{
   button = in;
}

void Level::LoadLevelFromFile(const std::string& path)
{
   ifstream in(path);
   if (!in.is_open())
      return;

   bool loaded_char = false;
   string str;
   int x = 0;
   int y = 0;

   int max_x = 0;

   for (;!in.eof(); ++y)
   {
      if (x > max_x)
         max_x = x;

      x = 0;
      getline(in, str);
      for (auto itr = str.begin(); itr != str.end(); ++itr, ++x)
      {
         Entity_Ptr tmp;
         if (*itr == 'W')
         {
            tmp = Entity_Ptr(new Wall);
            tmp->tile_size(tile_size);
            tmp->SetImage(images[Images::Wall]);
            tmp->pos(Vector2i(x, y));
            entities.push_back(tmp);
         }
         else if (*itr == 'S')
         {
            tmp = Entity_Ptr(new Stone);
            tmp->tile_size(tile_size);
            tmp->SetImage(images[Images::Stone]);
            tmp->pos(Vector2i(x, y));
            entities.push_back(tmp);
         }
         else if (*itr == 'H')
         {
            if (loaded_char)
               throw EInvalidLevel(string("Cannot have more than one character"));

            character = Entity_Ptr(new Hero);
            character->tile_size(tile_size);
            character->SetImage(images[Images::Hero]);
            character->pos(Vector2i(x, y));
            entities.push_back(character);
            loaded_char = true;
         }
         else if (*itr == 'Z')
         {
            tmp = Entity_Ptr(new SlipStone);
            tmp->tile_size(tile_size);
            tmp->SetImage(images[Images::Slip]);
            tmp->pos(Vector2i(x, y));
            entities.push_back(tmp);
         }
         else if (*itr == 'G')
         {
            tmp = Entity_Ptr(new Floor);
            tmp->tile_size(tile_size);
            tmp->SetImage(images[Images::Floor]);
            tmp->pos(Vector2i(x, y));
            entities.push_back(tmp);
            floor.push_back(tmp);
         }
         else if (*itr == '.' || *itr == ' ')
         {}
         else
         {
            throw EInvalidLevel(string("Syntax error in file"));
         }
      }
   }

   app.Create(VideoMode(tile_size.x * max_x, tile_size.y * (y - 1), 32), string("Puzzle") + string("\"") + m_path + string("\""));
   app.UseVerticalSync(true);
   app.SetFramerateLimit(30);

   if (!loaded_char)
   {
      throw EInvalidLevel(string("Missing character in file"));
   }
   in.close();
}

void Level::handle_logic()
{
   if (movement == Movement::Up)
      character->speed(Vector2i(0, -1));
   else if (movement == Movement::Down)
      character->speed(Vector2i(0, 1));
   else if (movement == Movement::Left)
      character->speed(Vector2i(-1, 0));
   else if (movement == Movement::Right)
      character->speed(Vector2i(1, 0));
   else
      character->speed(Vector2i(0, 0));

   if (button == Button::A)
   {
      Vector2i hero_pos = character->pos();
      for (auto itr = entities.begin(); itr != entities.end(); ++itr)
      {
         Vector2i obj_pos = (*itr)->pos();
         if (hero_pos.x + 1 == obj_pos.x && hero_pos.y == obj_pos.y && character->direction() == Movement::Right)
         {
            (*itr)->speed(Vector2i(1, 0));
            if ((*itr)->is_floor())
               continue;
            break;
         }
         else if (hero_pos.x == obj_pos.x && hero_pos.y + 1 == obj_pos.y && character->direction() == Movement::Down)
         {
            (*itr)->speed(Vector2i(0, 1));
            if ((*itr)->is_floor())
               continue;
            break;
         }
         else if (hero_pos.x == obj_pos.x + 1 && hero_pos.y == obj_pos.y && character->direction() == Movement::Left)
         {
            (*itr)->speed(Vector2i(-1, 0));
            if ((*itr)->is_floor())
               continue;
            break;
         }
         else if (hero_pos.x == obj_pos.x && hero_pos.y == obj_pos.y + 1 && character->direction() == Movement::Up)
         {
            (*itr)->speed(Vector2i(0, -1));
            if ((*itr)->is_floor())
               continue;
            break;
         }
      }
   }
}

bool Level::is_won() const
{
   int won_count = 0;
   int win_condition = floor.size();

   for (auto itr = floor.begin(); itr != floor.end(); ++itr)
   {
      for (auto itr2 = entities.begin(); itr2 != entities.end(); ++itr2)
      {
         if (*(*itr2) == *character)
            continue;
         else if (*(*itr) == *(*itr2))
            continue;

         if ((*itr)->pos() == (*itr2)->pos())
         {
            won_count++;
            continue;
         }
      }
   }
   return won_count == win_condition;
}

void Level::check_collition()
{
   for (auto itr = entities.begin(); itr != entities.end(); ++itr)
   {
      for (auto itr2 = entities.begin(); itr2 != entities.end(); ++itr2)
      {
         if (*(*itr) == *(*itr2))
            continue;
         else if ((*itr)->is_floor() || (*itr2)->is_floor())
            continue;

         Vector2i obj_pos[2] = { (*itr)->pos(), (*itr2)->pos() };
         if (obj_pos[0].x + 1 == obj_pos[1].x && obj_pos[0].y == obj_pos[1].y && (*itr)->speed().x > 0)
         {
            (*itr)->speed(Vector2i(0, 0));
            break;
         }
         else if (obj_pos[0].x == obj_pos[1].x && obj_pos[0].y + 1 == obj_pos[1].y && (*itr)->speed().y > 0)
         {
            (*itr)->speed(Vector2i(0, 0));
            break;
         }
         else if (obj_pos[0].x == obj_pos[1].x + 1 && obj_pos[0].y == obj_pos[1].y && (*itr)->speed().x < 0)
         {
            (*itr)->speed(Vector2i(0, 0));
            break;
         }
         else if (obj_pos[0].x == obj_pos[1].x && obj_pos[0].y == obj_pos[1].y + 1 && (*itr)->speed().y < 0)
         {
            (*itr)->speed(Vector2i(0, 0));
            break;
         }
      }
   }
}

void Level::update()
{
   for_each(entities.begin(), entities.end(), [](const Entity_Ptr& ptr) {
         ptr->update();
   });
}

void Level::render()
{
   app.Clear();
   for (auto itr = floor.begin(); itr != floor.end(); ++itr)
      (*itr)->render(app);
   for (auto itr = entities.begin(); itr != entities.end(); ++itr)
   {
      if (!(*itr)->is_floor())
         (*itr)->render(app);
   }
   app.Display();
}


