#include "level.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include "utils.hpp"

using namespace std;
using namespace sf;
using namespace Game;


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
   switch_floor.clear();
   images.clear();
   LoadPictures();
   LoadLevelFromFile(m_path);
}

void Level::LoadPictures()
{
   images = vector<Image>(7);
   images[Images::Hero].LoadFromFile("pics/hero.png");
   images[Images::Wall].LoadFromFile("pics/wall.png");
   images[Images::Stone].LoadFromFile("pics/stone.png");
   images[Images::Slip].LoadFromFile("pics/slip.png");
   images[Images::Floor].LoadFromFile("pics/floor.png");
   images[Images::SwitchFloor].LoadFromFile("pics/switch_floor.png");
   images[Images::SlipFloor].LoadFromFile("pics/slipfloor.png");

   tile_size = Vector2i(images[Images::Hero].GetWidth(), images[Images::Hero].GetHeight());
}

void Level::LoadSprite(const Images::Images& type, int x, int y)
{
   Entity_Ptr tmp;
   switch (type)
   {
      case Images::Wall:
         tmp = Entity_Ptr(new Wall);
         break;
      case Images::Hero:
         tmp = Entity_Ptr(new Hero);
         character = tmp;
         break;
      case Images::Stone:
         tmp = Entity_Ptr(new Stone);
         break;
      case Images::Floor:
         tmp = Entity_Ptr(new Floor);
         floor.push_back(tmp);
         break;
      case Images::SlipFloor:
         tmp = Entity_Ptr(new SlipperyFloor);
         floor.push_back(tmp);
         break;
      case Images::SwitchFloor:
         tmp = Entity_Ptr(new Floor);
         floor.push_back(tmp);
         switch_floor.push_back(tmp);
         break;
      case Images::Slip:
         tmp = Entity_Ptr(new SlipStone);
         break;
   }
   tmp->tile_size(tile_size);
   tmp->SetImage(images[type]);
   tmp->pos(Vector2i(x, y));
   entities.push_back(tmp);
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
            LoadSprite(Images::Wall, x, y);
         }
         else if (*itr == 'S')
         {
            LoadSprite(Images::Stone, x, y);
         }
         else if (*itr == 'H')
         {
            if (loaded_char)
               throw runtime_error(join("Cannot have more than one character on map: (", x+1, ", ", y+1, ")"));

            LoadSprite(Images::Hero, x, y);
            loaded_char = true;
         }
         else if (*itr == 'Z')
         {
            LoadSprite(Images::Slip, x, y);
         }
         else if (*itr == 'G')
         {
            LoadSprite(Images::SwitchFloor, x, y);
         }
         else if (*itr == 'X')
         {
            LoadSprite(Images::SwitchFloor, x, y);
            LoadSprite(Images::Slip, x, y);
         }
         else if (*itr == 'Y')
         {
            LoadSprite(Images::SlipFloor, x, y);
         }
         else if (*itr == '.' || *itr == ' ')
         {}
         else
            throw runtime_error(join("Syntax error in file at: (", x+1, ", ", y+1, ")"));

         if (*itr != 'W' && *itr != 'G' && *itr != 'Y')
            LoadSprite(Images::Floor, x, y);
      }
   }

   app.Create(VideoMode(tile_size.x * max_x, tile_size.y * (y - 1), 32), join("Puzzle", "|| \"", m_path, "\""));
   app.UseVerticalSync(true);
   app.SetFramerateLimit(30);

   if (!loaded_char)
   {
      throw runtime_error(join("Missing character in file"));
   }
   in.close();

   if (switch_floor.size() == 0)
   {
      throw runtime_error(join("Cannot have level without a win condition."));
   }
}

void Level::handle_logic()
{

   if (character->speed() != Vector2i(0, 0))
      return;

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
   int win_condition = switch_floor.size();

   for (auto itr = switch_floor.begin(); itr != switch_floor.end(); ++itr)
   {
      for (auto itr2 = entities.begin(); itr2 != entities.end(); ++itr2)
      {
         if (*(*itr2) == *character)
            continue;
         else if (*(*itr) == *(*itr2))
            continue;

         if ((*itr)->pos() == (*itr2)->pos() && (*itr2)->speed() == Vector2i(0, 0))
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
   for (auto itr = entities.begin(); itr != entities.end(); ++itr)
   {
      bool is_slippery = false;
      for (auto floor_itr = floor.begin(); floor_itr != floor.end(); ++floor_itr)
      {
         if (*(*itr) == *(*floor_itr))
            continue;
         else if (!(*floor_itr)->is_floor(true))
            continue;

         auto itr_pos = (*itr)->pos();
         auto floor_pos = (*floor_itr)->pos();

         switch ((*itr)->direction())
         {
            case Movement::Up:
               if (itr_pos + Vector2i(0, -1) == floor_pos)
                  is_slippery = true;
               break;
            case Movement::Down:
               if (itr_pos + Vector2i(0, 1) == floor_pos)
                  is_slippery = true;
               break;
            case Movement::Left:
               if (itr_pos + Vector2i(-1, 0) == floor_pos)
                  is_slippery = true;
               break;
            case Movement::Right:
               if (itr_pos + Vector2i(1, 0) == floor_pos)
                  is_slippery = true;
               break;
            default:
               break;
         }
      }
      if (is_slippery)
         (*itr)->update(false);
      else
         (*itr)->update();
   }
}

void Level::render()
{
   app.Clear();
   for_each(floor.begin(), floor.end(), [this](const Entity_Ptr& itr) {
      itr->render(app);
   });
   for_each(entities.begin(), entities.end(), [this](const Entity_Ptr& itr) {
      if (!itr->is_floor())
         itr->render(app);
   });
   app.Display();
}


