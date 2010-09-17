#ifndef __UTILS_HPP
#define __UTILS_HPP

#include <string>
#include <sstream>

namespace Game {

   namespace Internal {

      static void join(std::ostringstream&)
      {}

      template<class T, class... Args>
         static void join(std::ostringstream& stream, const T& first, const Args&... rest)
         {
            stream << first;
            join(stream, rest...);
         }
   }


template<class T, class... Args>
static std::string join(const T& first, const Args&... rest)
{
   std::ostringstream buf;
   buf << first;
   Internal::join(buf, rest...);
   return buf.str();
}
}

#endif
