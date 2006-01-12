/******************************************************************************\
* Types.hpp                                                                    *
* The place for assorted types.                                                *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#ifndef _DILUCULUM_TYPES_HPP_
#define _DILUCULUM_TYPES_HPP_

#include <map>
#include <vector>


namespace Diluculum
{
   class LuaValue;

   /** A list of <tt>LuaValue</tt>s. Used, for example, to represent the return
    *  value of a Lua function call. In this case, the first return value is
    *  stored at the 0th \c vector position, the second return value at the 1st
    *  \c vector position and so on.
    */
   typedef std::vector<LuaValue> LuaValueList;

   /** Type mapping from <tt>LuaValue</tt>s to <tt>LuaValue</tt>s. Think of it
    *  as a C++ approximation of a Lua table.
    */
   typedef std::map<LuaValue, LuaValue> LuaValueMap;

} // namespace Diluculum

#endif // _DILUCULUM_TYPES_HPP_
