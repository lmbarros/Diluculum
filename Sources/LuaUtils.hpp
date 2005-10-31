/******************************************************************************\
* LuaUtils.hpp                                                                 *
* Some utilities related to Lua.                                               *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#ifndef _DILUCULUM_LUA_UTILS_HPP_
#define _DILUCULUM_LUA_UTILS_HPP_

#include "LuaValue.hpp"

namespace Diluculum
{

   /** Converts and returns the element at index \c index on the stack to a
    *  \c LuaValue. This keeps the Lua stack untouched. Oh, yes, and it accepts
    *  both positive and negative indices, just like the standard functions on
    *  the Lua C API.
    *  @todo There are no tests for this function!
    *  @throw LuaTypeError If the element at \c index cannot be converted to a
    *         \c LuaValue. This can happen if the value at that position is, for
    *         example, a "Lua Thread" that is not supported by \c LuaValue.
    */
   LuaValue ToLuaValue (lua_State* state, int index);

   /** Calls the appropriate <tt>lua_push*()</tt> function, in order to push the
    *  value stored at \c value to the Lua stack.
    *  @todo There are no tests for this function!
    *  @todo There is at least one unhandled error situation: a \c LuaValueMap
    *        can use \c Nil as index, but a Lua table can't; something bad will
    *        most likely happen if we try to \c PushLuaValue() a \c LuaValueMap
    *        with a \c Nil index.
    */
   void PushLuaValue (lua_State* state, const LuaValue& value);

} // namespace Diluculum

#endif // _DILUCULUM_LUA_UTILS_HPP_
