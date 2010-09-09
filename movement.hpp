#ifndef __MOVEMENT_HPP
#define __MOVEMENT_HPP

namespace Game
{
   namespace Movement
   {
      typedef enum Movement
      {
         None = 0,
         Up,
         Down,
         Left,
         Right,
      } Movement;
   }

   namespace Button
   {
      typedef enum Button
      {
         None = 0,
         A,
      } Button;
   }
}

#endif


