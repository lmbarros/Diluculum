/******************************************************************************\
* MakeLuaFunction.hpp                                                          *
* Macros (yes, macros) to create 'lua_CFunctions' from slightly more friendly  *
* functions taking (and possibly returning) 'LuaValue's.                       *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#ifndef _DILUCULUM_MAKE_LUA_FUNCTION_HPP_
#define _DILUCULUM_MAKE_LUA_FUNCTION_HPP_

#include "LuaValue.hpp"
#include "LuaUtils.hpp"

namespace Diluculum
{
   /** @todo Docs!
    */
   void MLF_IssueLuaErrorIf (lua_State* ls, bool condition,
                             const std::string& function,
                             const std::string& message);


   /** @todo Docs!
    */
   void MLF_ReadParameters (lua_State* ls, Diluculum::LuaValue params[], int n);

} // namespace Diluculum


/**
 *  @todo Should I raise an error when the wrong number of arguments is passed?
 *        Perhaps, in some situations it is better to get some 'nil's, and let
 *        the function deal with them (e.g., when creating functions with
 *        default or optional parameters). This note is valid for all the macros
 *        defined in this file.
 */
#define DILUCULUM_MAKE_LUA_FUNCTION_0_VOID(FUNC,NEW_FUNC,LUA_NAME)   \
int NEW_FUNC (lua_State* ls)                                         \
{                                                                    \
   try                                                               \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 0, LUA_NAME,                         \
         "this function does not expect any parameter");             \
      FUNC();                                                        \
      return 0;                                                      \
   }                                                                 \
   catch (Diluculum::LuaError& e)                                    \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 0;                                                      \
   }                                                                 \
   catch(...)                                                        \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");  \
      return 0;                                                      \
   }                                                                 \
}



#define DILUCULUM_MAKE_LUA_FUNCTION_0_RET(FUNC,NEW_FUNC,LUA_NAME)    \
int NEW_FUNC (lua_State* ls)                                         \
{                                                                    \
   try                                                               \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 0, LUA_NAME,                         \
         "this function does not expect any parameter");             \
                                                                     \
      Diluculum::LuaValue ret = FUNC();                              \
      Diluculum::PushLuaValue (ls, ret);                             \
      return 1;                                                      \
   }                                                                 \
   catch (Diluculum::LuaError& e)                                    \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 1;                                                      \
   }                                                                 \
   catch(...)                                                        \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");  \
      return 1;                                                      \
   }                                                                 \
}



#define DILUCULUM_MAKE_LUA_FUNCTION_1_VOID(FUNC,NEW_FUNC,LUA_NAME)   \
int NEW_FUNC (lua_State* ls)                                         \
{                                                                    \
   try                                                               \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 1, LUA_NAME,                         \
         "this function expects exactly one parameter");             \
                                                                     \
      Diluculum::LuaValue param;                                     \
      MLF_ReadParameters (ls, &param, 1);                            \
                                                                     \
      FUNC (param);                                                  \
      return 0;                                                      \
   }                                                                 \
   catch (Diluculum::LuaError& e)                                    \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 0;                                                      \
   }                                                                 \
   catch(...)                                                        \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");  \
      return 0;                                                      \
   }                                                                 \
}



#define DILUCULUM_MAKE_LUA_FUNCTION_1_RET(FUNC,NEW_FUNC,LUA_NAME)    \
int NEW_FUNC (lua_State* ls)                                         \
{                                                                    \
   try                                                               \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 1, LUA_NAME,                         \
         "this function expects exactly one parameter");             \
                                                                     \
      Diluculum::LuaValue param;                                     \
      MLF_ReadParameters (ls, &param, 1);                            \
                                                                     \
      Diluculum::LuaValue ret = FUNC (param);                        \
      Diluculum::PushLuaValue (ls, ret);                             \
      return 1;                                                      \
   }                                                                 \
   catch (Diluculum::LuaError& e)                                    \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 1;                                                      \
   }                                                                 \
   catch(...)                                                        \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");  \
      return 1;                                                      \
   }                                                                 \
}



#define DILUCULUM_MAKE_LUA_FUNCTION_2_VOID(FUNC,NEW_FUNC,LUA_NAME)   \
int NEW_FUNC (lua_State* ls)                                         \
{                                                                    \
   try                                                               \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 2, LUA_NAME,                         \
         "this function expects exactly two parameters");            \
                                                                     \
      Diluculum::LuaValue params[2];                                 \
      Diluculum::MLF_ReadParameters (ls, params, 2);                 \
                                                                     \
      FUNC (params[0], params[1]);                                   \
      return 0;                                                      \
   }                                                                 \
   catch (Diluculum::LuaError& e)                                    \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 0;                                                      \
   }                                                                 \
   catch(...)                                                        \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");  \
      return 0;                                                      \
   }                                                                 \
}



#define DILUCULUM_MAKE_LUA_FUNCTION_2_RET(FUNC,NEW_FUNC,LUA_NAME)    \
int NEW_FUNC (lua_State* ls)                                         \
{                                                                    \
   try                                                               \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 2, LUA_NAME,                         \
         "this function expects exactly two parameters");            \
                                                                     \
      Diluculum::LuaValue params[2];                                 \
      Diluculum::MLF_ReadParameters (ls, params, 2);                 \
                                                                     \
      Diluculum::LuaValue ret = FUNC (params[0], params[1]);         \
      Diluculum::PushLuaValue (ls, ret);                             \
      return 1;                                                      \
   }                                                                 \
   catch (Diluculum::LuaError& e)                                    \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 1;                                                      \
   }                                                                 \
   catch(...)                                                        \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");  \
      return 1;                                                      \
   }                                                                 \
}



#define DILUCULUM_MAKE_LUA_FUNCTION_3_VOID(FUNC,NEW_FUNC,LUA_NAME)   \
int NEW_FUNC (lua_State* ls)                                         \
{                                                                    \
   try                                                               \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 3, LUA_NAME,                         \
         "this function expects exactly three parameters");          \
                                                                     \
      Diluculum::LuaValue params[3];                                 \
      Diluculum::MLF_ReadParameters (ls, params, 3);                 \
                                                                     \
      FUNC (params[0], params[1], params[2]);                        \
      return 0;                                                      \
   }                                                                 \
   catch (Diluculum::LuaError& e)                                    \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 0;                                                      \
   }                                                                 \
   catch(...)                                                        \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");  \
      return 0;                                                      \
   }                                                                 \
}



#define DILUCULUM_MAKE_LUA_FUNCTION_3_RET(FUNC,NEW_FUNC,LUA_NAME)       \
int NEW_FUNC (lua_State* ls)                                            \
{                                                                       \
   try                                                                  \
   {                                                                    \
      Diluculum::MLF_IssueLuaErrorIf(                                   \
         ls, lua_gettop (ls) != 3, LUA_NAME,                            \
         "this function expects exactly three parameters");             \
                                                                        \
      Diluculum::LuaValue params[3];                                    \
      Diluculum::MLF_ReadParameters (ls, params, 3);                    \
                                                                        \
      Diluculum::LuaValue ret = FUNC (params[0], params[1], params[2]); \
      Diluculum::PushLuaValue (ls, ret);                                \
      return 1;                                                         \
   }                                                                    \
   catch (Diluculum::LuaError& e)                                       \
   {                                                                    \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what());    \
      return 1;                                                         \
   }                                                                    \
   catch(...)                                                           \
   {                                                                    \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,               \
                                      "Unknown exception caught.");     \
      return 1;                                                         \
   }                                                                    \
}



#define DILUCULUM_MAKE_LUA_FUNCTION_4_VOID(FUNC,NEW_FUNC,LUA_NAME)   \
int NEW_FUNC (lua_State* ls)                                         \
{                                                                    \
   try                                                               \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 4, LUA_NAME,                         \
         "this function expects exactly four parameters");           \
                                                                     \
      Diluculum::LuaValue params[4];                                 \
      Diluculum::MLF_ReadParameters (ls, params, 4);                 \
                                                                     \
      FUNC (params[0], params[1], params[2], params[3]);             \
      return 0;                                                      \
   }                                                                 \
   catch (Diluculum::LuaError& e)                                    \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 0;                                                      \
   }                                                                 \
   catch(...)                                                        \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");  \
      return 0;                                                      \
   }                                                                 \
}



#define DILUCULUM_MAKE_LUA_FUNCTION_4_RET(FUNC,NEW_FUNC,LUA_NAME)       \
int NEW_FUNC (lua_State* ls)                                            \
{                                                                       \
   try                                                                  \
   {                                                                    \
      Diluculum::MLF_IssueLuaErrorIf(                                   \
         ls, lua_gettop (ls) != 4, LUA_NAME,                            \
         "this function expects exactly four parameters");              \
                                                                        \
      Diluculum::LuaValue params[4];                                    \
      Diluculum::MLF_ReadParameters (ls, params, 4);                    \
                                                                        \
      Diluculum::LuaValue ret = FUNC (params[0], params[1], params[2],  \
                                   params[3]);                          \
      Diluculum::PushLuaValue (ls, ret);                                \
      return 1;                                                         \
   }                                                                    \
   catch (Diluculum::LuaError& e)                                       \
   {                                                                    \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what());    \
      return 1;                                                         \
   }                                                                    \
   catch(...)                                                           \
   {                                                                    \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,               \
                                      "Unknown exception caught.");     \
      return 1;                                                         \
   }                                                                    \
}



#define DILUCULUM_MAKE_LUA_FUNCTION_5_VOID(FUNC,NEW_FUNC,LUA_NAME)   \
int NEW_FUNC (lua_State* ls)                                         \
{                                                                    \
   try                                                               \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf(                                \
         ls, lua_gettop (ls) != 5, LUA_NAME,                         \
         "this function expects exactly five parameters");           \
                                                                     \
      Diluculum::LuaValue params[5];                                 \
      Diluculum::MLF_ReadParameters (ls, params, 5);                 \
                                                                     \
      FUNC (params[0], params[1], params[2], params[3], params[4]);  \
      return 0;                                                      \
   }                                                                 \
   catch (Diluculum::LuaError& e)                                    \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what()); \
      return 0;                                                      \
   }                                                                 \
   catch(...)                                                        \
   {                                                                 \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,            \
                                      "Unknown exception caught.");  \
      return 0;                                                      \
   }                                                                 \
}



#define DILUCULUM_MAKE_LUA_FUNCTION_5_RET(FUNC,NEW_FUNC,LUA_NAME)       \
int NEW_FUNC (lua_State* ls)                                            \
{                                                                       \
   try                                                                  \
   {                                                                    \
      Diluculum::MLF_IssueLuaErrorIf(                                   \
         ls, lua_gettop (ls) != 5, LUA_NAME,                            \
         "this function expects exactly five parameters");              \
                                                                        \
      Diluculum::LuaValue params[5];                                    \
      Diluculum::MLF_ReadParameters (ls, params, 5);                    \
                                                                        \
      Diluculum::LuaValue ret = FUNC (params[0], params[1], params[2],  \
                                      params[3], params[4]);            \
      Diluculum::PushLuaValue (ls, ret);                                \
      return 1;                                                         \
   }                                                                    \
   catch (Diluculum::LuaError& e)                                       \
   {                                                                    \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME, e.what());    \
      return 1;                                                         \
   }                                                                    \
   catch(...)                                                           \
   {                                                                    \
      Diluculum::MLF_IssueLuaErrorIf (ls, true, LUA_NAME,               \
                                      "Unknown exception caught.");     \
      return 1;                                                         \
   }                                                                    \
}

#endif // _DILUCULUM_MAKE_LUA_FUNCTION_HPP_
