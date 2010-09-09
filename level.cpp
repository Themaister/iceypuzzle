#include "level.hpp"
#include <functional>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace sf;
using namespace Game;

enum Images
{
   Hero = 0,
   Wall,
   Stone,
   Slip,
};

Level::Level(RenderWindow& in) : app(in)
{
   app.UseVerticalSync(true);
   LoadPictures();
}

Level::Level(RenderWindow& in, const string& path) : app(in), level_size(Vector2i(16, 16)), tile_size(Vector2i(16, 16))
{
   LoadPictures();
   LoadLevelFromFile(path);
}

void Level::LoadPictures()
{
   images = vector<Image>(4);
   images[0].LoadFromFile("hero.png");
   images[1].LoadFromFile("wall.png");
   images[2].LoadFromFile("stone.png");
   images[3].LoadFromFile("slip.png");
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

   string str;
   int x = 0;
   int y = 0;
   for (;!in.eof(); ++y)
   {
      x = 0;
      getline(in, str);
      for (auto itr = str.begin(); itr != str.end(); ++itr, ++x)
      {
         Entity_Ptr tmp;
         if (*itr == 'W')
         {
            tmp = Entity_Ptr(new Wall);
            tmp->tile_size(tile_size);
            tmp->SetImage(images[1]);
            tmp->pos(Vector2i(x, y));
            entities.push_back(tmp);
         }
         else if (*itr == 'S')
         {
            tmp = Entity_Ptr(new Stone);
            tmp->tile_size(tile_size);
            tmp->SetImage(images[2]);
            tmp->pos(Vector2i(x, y));
            entities.push_back(tmp);
         }
         else if (*itr == 'H')
         {
            character = Entity_Ptr(new Hero);
            character->tile_size(tile_size);
            character->SetImage(images[0]);
            character->pos(Vector2i(x, y));
            entities.push_back(character);
         }
         else if (*itr == 'Z')
         {
            tmp = Entity_Ptr(new SlipStone);
            tmp->tile_size(tile_size);
            tmp->SetImage(images[3]);
            tmp->pos(Vector2i(x, y));
            entities.push_back(tmp);
         }
         else if (*itr == '.')
         {}
         else
         {
            cout << "OMG ERROR IN FILE at: " << "(" << x << ", " << y << ")!" << endl;
         }
      }
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
            break;
         }
         else if (hero_pos.x == obj_pos.x && hero_pos.y + 1 == obj_pos.y && character->direction() == Movement::Down)
         {
            (*itr)->speed(Vector2i(0, 1));
            break;
         }
         else if (hero_pos.x == obj_pos.x + 1 && hero_pos.y == obj_pos.y && character->direction() == Movement::Left)
         {
            (*itr)->speed(Vector2i(-1, 0));
            break;
         }
         else if (hero_pos.x == obj_pos.x && hero_pos.y == obj_pos.y + 1 && character->direction() == Movement::Up)
         {
            (*itr)->speed(Vector2i(0, -1));
            break;
         }
      }
   }
}

void Level::check_collition()
{
   for (auto itr = entities.begin(); itr != entities.end(); ++itr)
   {
      for (auto itr2 = entities.begin(); itr2 != entities.end(); ++itr2)
      {
         if (&(*itr) == &(*itr2))
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
   for (auto itr = entities.begin(); itr != entities.end(); ++itr)
      (*itr)->render(app);
   app.Display();
}


