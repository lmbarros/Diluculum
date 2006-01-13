/******************************************************************************\
* LuaUserData.cpp                                                              *
* A C++ equivalent of a Lua userdata.                                          *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <Diluculum/LuaUserData.hpp>
#include <cstring>


namespace Diluculum
{
   // - LuaUserData::LuaUserData -----------------------------------------------
   LuaUserData::LuaUserData (size_t size)
      : size_(size), data_ (new char[size_])
   { }


   LuaUserData::LuaUserData (const LuaUserData& other)
      : size_(other.getSize()), data_ (new char[size_])
   {
      memcpy (data_.get(), other.getData(), getSize());
   }



   // - LuaUserData::operator= -------------------------------------------------
   const LuaUserData& LuaUserData::operator= (const LuaUserData& rhs)
   {
      size_ = rhs.getSize();
      data_.reset (new char[getSize()]);
      memcpy (getData(), rhs.getData(), getSize());
      return *this;
   }



   // - LuaUserData::operator> -------------------------------------------------
   bool LuaUserData::operator> (const LuaUserData& rhs)
   {
      if (getSize() > rhs.getSize())
         return true;
      else if (getSize() < rhs.getSize())
         return false;
      else // getSize() == rhs.getSize()
         return memcmp (getData(), rhs.getData(), getSize()) > 0;
   }



   // - LuaUserData::operator< -------------------------------------------------
   bool LuaUserData::operator< (const LuaUserData& rhs)
   {
      if (getSize() < rhs.getSize())
         return true;
      else if (getSize() > rhs.getSize())
         return false;
      else // getSize() == rhs.getSize()
         return memcmp (getData(), rhs.getData(), getSize()) < 0;
   }



   // - LuaUserData::operator== ------------------------------------------------
   bool LuaUserData::operator== (const LuaUserData& rhs)
   {
      return getSize() == rhs.getSize()
         && memcmp (getData(), rhs.getData(), getSize()) == 0;
   }



   // - LuaUserData::operator!= ------------------------------------------------
   bool LuaUserData::operator!= (const LuaUserData& rhs)
   {
      return getSize() != rhs.getSize()
         || memcmp (getData(), rhs.getData(), getSize()) != 0;
   }

} // namespace Diluculum
