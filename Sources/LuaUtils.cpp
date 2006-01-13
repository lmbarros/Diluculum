/******************************************************************************\
* LuaUtils.cpp                                                                 *
* Some utilities related to Lua.                                               *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <Diluculum/LuaUtils.hpp>
#include <Diluculum/LuaExceptions.hpp>
#include <boost/lexical_cast.hpp>


namespace Diluculum
{

   // - ToLuaValue -------------------------------------------------------------
   LuaValue ToLuaValue (lua_State* state, int index)
   {
      switch (lua_type (state, index))
      {
         case LUA_TNIL:
            return Nil;

         case LUA_TNUMBER:
            return lua_tonumber (state, index);

         case LUA_TBOOLEAN:
            return static_cast<bool>(lua_toboolean (state, index));

         case LUA_TSTRING:
            return lua_tostring (state, index);

         case LUA_TUSERDATA:
         {
            void* addr = lua_touserdata (state, index);
            size_t size = lua_objlen (state, index);
            LuaUserData ud (size);
            memcpy (ud.getData(), addr, size);
            return ud;
         }

         case LUA_TTABLE:
         {
            // Make the index positive if necessary (using a negative index here
            // will be *bad*, because the stack will be changed in the
            // 'lua_next()' and a negative index will mess everything).
            if (index < 0)
               index = lua_gettop(state) + index + 1;

            // Traverse the table adding the key/value pairs to 'ret'
            LuaValueMap ret;

            lua_pushnil (state);
            while (lua_next (state, index) != 0)
            {
               ret[ToLuaValue (state, -2)] = ToLuaValue (state, -1);
               lua_pop (state, 1);
            }

            // Alright, return the result
            return ret;
         }

         case LUA_TFUNCTION:
         {
            if (lua_iscfunction (state, index))
               return lua_tocfunction (state, index);
            else
            {
               throw LuaTypeError(
                  "Lua functions not supported by 'ToLuaValue()'.");
            }
         }

         default:
         {
            throw LuaTypeError(
               ("Unsupported type found in call to 'ToLuaValue()': "
                + boost::lexical_cast<std::string>(lua_type (state, index))
                + " (typename: \'" + luaL_typename (state, index)
                + "')").c_str());
         }
      }
   }



   // - PushLuaValue -----------------------------------------------------------
   void PushLuaValue (lua_State* state, const LuaValue& value)
   {
      switch (value.type())
      {
         case LUA_TNIL:
            lua_pushnil (state);
            break;

         case LUA_TNUMBER:
            lua_pushnumber (state, value.asNumber());
            break;

         case LUA_TSTRING:
            lua_pushstring (state, value.asString().c_str());
            break;

         case LUA_TBOOLEAN:
            lua_pushboolean (state, value.asBoolean());
            break;

         case LUA_TUSERDATA:
         {
            size_t size = value.asUserData().getSize();
            void* addr = lua_newuserdata (state, size);
            memcpy (addr, value.asUserData().getData(), size);
            break;
         }

         case LUA_TTABLE:
         {
            lua_newtable (state);

            typedef LuaValueMap::const_iterator iter_t;
            const LuaValueMap table = value.asTable();
            for (iter_t p = table.begin(); p != table.end(); ++p)
            {
               if (p->first != Nil) // Ignore 'Nil'-indexed entries
               {
                  PushLuaValue (state, p->first);
                  PushLuaValue (state, p->second);
                  lua_settable (state, -3);
               }
            }

            break;
         }

         case LUA_TFUNCTION:
            lua_pushcfunction (state, value.asFunction());
            break;

         default:
         {
            throw LuaTypeError(
               ("Unsupported type found in call to 'PushLuaValue()': "
                + boost::lexical_cast<std::string>(value.type())
                + " (typename: \'" + value.typeName() + "')").c_str());
         }
      }
   }

} // namespace Diluculum
