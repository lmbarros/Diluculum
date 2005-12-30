/******************************************************************************\
* LuaVariable.cpp                                                              *
* A variable living in a Lua interpreter.                                      *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <cassert>
#include <boost/lexical_cast.hpp>
#include <Diluculum/LuaState.hpp>
#include <Diluculum/LuaUtils.hpp>
#include <Diluculum/LuaVariable.hpp>


namespace Diluculum
{
   // - LuaVariable::LuaVariable -----------------------------------------------
   LuaVariable::LuaVariable (lua_State* state, const LuaValue& key,
                             const KeyList& predKeys)
      : state_(state), keys_(predKeys)
   {
      keys_.push_back (key);
   }



   // - LuaVariable::operator= -------------------------------------------------
   const LuaValue& LuaVariable::operator= (const LuaValue& rhs)
   {
      // Push the globals table onto the stack
      lua_pushstring (state_, "_G");
      lua_gettable (state_, LUA_GLOBALSINDEX);

      // Reach the "final" table (and leave it at the stack top)
      typedef std::vector<LuaValue>::const_iterator iter_t;

      assert (keys_.size() > 0 && "At least one key should be present here.");

      iter_t end = keys_.end();
      --end;

      for (iter_t p = keys_.begin(); p != end; ++p)
      {
         PushLuaValue (state_, *p);
         lua_gettable (state_, -2);
         if (!lua_istable (state_, -1))
         {
            throw TypeMismatchError ("table",
                                     ToLuaValue (state_, -1).typeName());
         }

         lua_remove (state_, -2);
      }

      // Write the new value
      PushLuaValue (state_, keys_.back());
      PushLuaValue (state_, rhs);
      lua_settable (state_, -3);
      lua_pop (state_, 1);

      return rhs;
   }



   // - LuaVariable::value -----------------------------------------------------
   LuaValue LuaVariable::value() const
   {
      pushTheReferencedValue();
      LuaValue ret = ToLuaValue (state_, -1);
      lua_pop (state_, 1);
      return ret;
   }



   // - LuaVariable::operator[] ------------------------------------------------
   LuaVariable LuaVariable::operator[] (const LuaValue& key) const
   {
      return LuaVariable (state_, key, keys_);
   }



   // - LuaVariable::operator() ------------------------------------------------
   LuaValueList LuaVariable::operator() (const LuaValueList& params)
   {
      int topBefore = lua_gettop (state_);

      pushTheReferencedValue();

      if (lua_type (state_, -1) != LUA_TFUNCTION)
         throw TypeMismatchError ("function", lua_typename (state_, -1));

      typedef LuaValueList::const_iterator iter_t;
      for (iter_t p = params.begin(); p != params.end(); ++p)
         PushLuaValue (state_, *p);

      int ret = lua_pcall (state_, params.size(), LUA_MULTRET, 0);

      if (ret != 0)
      {
         std::string errMessage = lua_tostring (state_, -1);
         lua_pop (state_, 1);

         switch (ret)
         {
            case LUA_ERRRUN:
               throw LuaRunTimeError(
                  ("'LUA_ERRRUN' returned while calling function from Lua. "
                   "Additional error message: '" + errMessage + "'.").c_str());

            case LUA_ERRMEM:
               throw LuaRunTimeError(
                  ("'LUA_ERRMEM' returned while calling function from Lua. "
                   "Additional error message: '" + errMessage + "'.").c_str());

            case LUA_ERRERR:
               throw LuaRunTimeError(
                  ("'LUA_ERR' returned while calling function from Lua. "
                   "Additional error message: '" + errMessage + "'.").c_str());

            default:
               throw LuaRunTimeError(
                  ("Unknown error code ("
                   + boost::lexical_cast<std::string>(ret)
                   + ") returned while calling function from Lua. "
                   + "Additional error message: '" + errMessage
                   + "'.").c_str());
         }
      }

      int numResults = lua_gettop (state_) - topBefore;

      LuaValueList results;

      for (int i = numResults; i > 0; --i)
         results.push_back (ToLuaValue (state_, -i));

      lua_pop (state_, numResults);

      return results;
   }

   LuaValueList LuaVariable::operator()()
   {
      return (*this)(LuaValueList());
   }

   LuaValueList LuaVariable::operator() (const LuaValue& param)
   {
      LuaValueList params;
      params.push_back (param);
      return (*this)(params);
   }

   LuaValueList LuaVariable::operator() (const LuaValue& param1,
                                         const LuaValue& param2)
   {
      LuaValueList params;
      params.push_back (param1);
      params.push_back (param2);
      return (*this)(params);
   }

   LuaValueList LuaVariable::operator() (const LuaValue& param1,
                                         const LuaValue& param2,
                                         const LuaValue& param3)
   {
      LuaValueList params;
      params.push_back (param1);
      params.push_back (param2);
      params.push_back (param3);
      return (*this)(params);
   }

   LuaValueList LuaVariable::operator() (const LuaValue& param1,
                                         const LuaValue& param2,
                                         const LuaValue& param3,
                                         const LuaValue& param4)
   {
      LuaValueList params;
      params.push_back (param1);
      params.push_back (param2);
      params.push_back (param3);
      params.push_back (param4);
      return (*this)(params);
   }

   LuaValueList LuaVariable::operator() (const LuaValue& param1,
                                         const LuaValue& param2,
                                         const LuaValue& param3,
                                         const LuaValue& param4,
                                         const LuaValue& param5)
   {
      LuaValueList params;
      params.push_back (param1);
      params.push_back (param2);
      params.push_back (param3);
      params.push_back (param4);
      params.push_back (param5);
      return (*this)(params);
   }



   // - LuaVariable::pushTheReferencedValue ------------------------------------
   void LuaVariable::pushTheReferencedValue() const
   {
      int index = LUA_GLOBALSINDEX;

      typedef std::vector<LuaValue>::const_iterator iter_t;
      for (iter_t p = keys_.begin(); p != keys_.end(); ++p)
      {
         PushLuaValue (state_, *p);
         lua_gettable (state_, index);

         assert (keys_.size() > 0 && "There should be at least one key here.");

         if (keys_.size() > 1 && p != keys_.end()-1 && !lua_istable(state_, -1))
            throw TypeMismatchError ("table", p->typeName());

         if (index != LUA_GLOBALSINDEX)
            lua_remove (state_, -2);
         else
            index = -2;
      }
   }

} // namespace Diluculum
