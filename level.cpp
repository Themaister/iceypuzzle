#include "level.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <utility>
#include "utils.hpp"

using namespace std;
using namespace sf;
using namespace Game;


Level::Level(RenderWindow& in, const char *path) : app(in), m_path(path)
{
   app.UseVerticalSync(true);
   app.SetFramerateLimit(30);
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
   images[(unsigned)ImageType::Hero].LoadFromFile("pics/hero.png");
   images[(unsigned)ImageType::Wall].LoadFromFile("pics/wall.png");
   images[(unsigned)ImageType::Stone].LoadFromFile("pics/stone.png");
   images[(unsigned)ImageType::Slip].LoadFromFile("pics/slip.png");
   images[(unsigned)ImageType::Floor].LoadFromFile("pics/floor.png");
   images[(unsigned)ImageType::SwitchFloor].LoadFromFile("pics/switch_floor.png");
   images[(unsigned)ImageType::SlipFloor].LoadFromFile("pics/slipfloor.png");

   tile_size = Vector2i(images[(unsigned)ImageType::Hero].GetWidth(), images[(unsigned)ImageType::Hero].GetHeight());
}

void Level::LoadSprite(const ImageType& type, int x, int y)
{
   Entity_Ptr tmp;
   switch (type)
   {
      case ImageType::Wall:
         tmp = make_shared<Wall>();
         break;
      case ImageType::Hero:
         tmp = make_shared<Hero>();
         character = tmp;
         break;
      case ImageType::Stone:
         tmp = make_shared<Stone>();
         break;
      case ImageType::Floor:
         tmp = make_shared<Floor>();
         floor.push_back(tmp);
         break;
      case ImageType::SlipFloor:
         tmp = make_shared<SlipperyFloor>();
         floor.push_back(tmp);
         break;
      case ImageType::SwitchFloor:
         tmp = make_shared<Floor>();
         floor.push_back(tmp);
         switch_floor.push_back(tmp);
         break;
      case ImageType::Slip:
         tmp = make_shared<SlipStone>();
         break;
   }
   tmp->tile_size(tile_size);
   tmp->SetImage(images[(unsigned)type]);
   tmp->pos(Vector2i(x, y));
   entities.push_back(tmp);
}

void Level::SetMovement(const Movement& in)
{
   movement = in;
}

void Level::SetButton(const Button& in)
{
   button = in;
}

void Level::LoadLevelFromFile(const char *path)
{
   ifstream in(path);
   if (!in.is_open())
   {
      throw runtime_error(join("Could not open level ", path, " ..."));
   }

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
            LoadSprite(ImageType::Wall, x, y);
         }
         else if (*itr == 'S')
         {
            LoadSprite(ImageType::Stone, x, y);
         }
         else if (*itr == 'H')
         {
            if (loaded_char)
               throw logic_error(join("Cannot have more than one character on map: (", x+1, ", ", y+1, ")"));

            LoadSprite(ImageType::Hero, x, y);
            loaded_char = true;
         }
         else if (*itr == 'Z')
         {
            LoadSprite(ImageType::Slip, x, y);
         }
         else if (*itr == 'G')
         {
            LoadSprite(ImageType::SwitchFloor, x, y);
         }
         else if (*itr == 'X')
         {
            LoadSprite(ImageType::SwitchFloor, x, y);
            LoadSprite(ImageType::Slip, x, y);
         }
         else if (*itr == 'Y')
         {
            LoadSprite(ImageType::SlipFloor, x, y);
         }
         else if (*itr == '.' || *itr == ' ')
         {}
         else
            throw logic_error(join("Syntax error in file at: (", x+1, ", ", y+1, ")"));

         if (*itr != 'W' && *itr != 'G' && *itr != 'Y')
            LoadSprite(ImageType::Floor, x, y);
      }
   }

   app.Create(VideoMode(tile_size.x * max_x, tile_size.y * (y - 1), 32), join("Puzzle", "|| \"", m_path, "\""));
   app.UseVerticalSync(true);
   app.SetFramerateLimit(30);

   if (!loaded_char)
   {
      throw logic_error(join("Missing character in file"));
   }

   if (switch_floor.size() == 0)
   {
      throw logic_error(join("Cannot have level without a win condition."));
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


