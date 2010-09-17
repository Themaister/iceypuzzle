#ifndef __MOVEMENT_HPP
#define __MOVEMENT_HPP

namespace Game
{
   namespace Movement
   {
      enum Movement : unsigned
      {
         None = 0,
         Up,
         Down,
         Left,
         Right,
      };
   }

   namespace Button
   {
      enum Button : unsigned
      {
         None = 0,
         A,
      };
   }
}

#endif


