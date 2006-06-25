/******************************************************************************\
* LuaWrappers.hpp                                                              *
* Making C++ stuff accessible from Lua.                                        *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#ifndef _DILUCULUM_LUA_WRAPPERS_HPP_
#define _DILUCULUM_LUA_WRAPPERS_HPP_

#include <algorithm>
#include <string>
#include <boost/bind.hpp>
#include <Diluculum/LuaUtils.hpp>


namespace Diluculum
{
   namespace Impl
   {
      /** Calls \c lua_error() with a proper error message. The error message is
       *  composed of things that may help to find out what's the error, like
       *  the function name.
       *  @param ls The <tt>lua_State*</tt> on which the function will operate.
       *  @param what A message to be included in the error message created by
       *         this function.
       *  @note This is not intended to be called by Diluculum users.
       */
      void ReportErrorFromCFunction (lua_State* ls, const::std::string& what);
   }
}



/** Creates a \c lua_CFunction that wraps a function with the signature like the
 *  following one:
 *  <p><tt>Diluculum::LuaValueList Func (const Diluculum::LuaValueList& params)</tt>
 *  <p>Notice that, thanks to the use of <tt>Diluculum::LuaValueList</tt>s, the
 *  wrapped function can effectively take and return an arbitrary number of
 *  values.
 *  @note The name of the created wrapper function is a decorated version of the
 *        \c FUNC parameter. The decoration scheme can be quite complicated and
 *        is subject to change in future releases of Diluculum, so don't try to
 *        use it directly. Use the \c DILUCULUM_WRAPPER_FUNCTION() macro to
 *        obtain it instead.
 *  @see DILUCULUM_WRAPPER_FUNCTION() To find out the name of the created
 *       wrapper function.
 *  @param FUNC The function to be wrapped.
 */
#define DILUCULUM_WRAP_FUNCTION(FUNC)                                         \
int Diluculum__ ## FUNC ## __Wrapper_Function (lua_State* ls)                 \
{                                                                             \
   using std::for_each;                                                       \
   using boost::bind;                                                         \
   using Diluculum::PushLuaValue;                                             \
   using Diluculum::Impl::ReportErrorFromCFunction;                           \
                                                                              \
   try                                                                        \
   {                                                                          \
      /* Read parameters and empty the stack */                               \
      const int numParams = lua_gettop (ls);                                  \
      Diluculum::LuaValueList params;                                         \
      for (int i = 1; i <= numParams; ++i)                                    \
         params.push_back (Diluculum::ToLuaValue (ls, i));                    \
      lua_pop (ls, numParams);                                                \
                                                                              \
      /* Call the wrapped function */                                         \
      Diluculum::LuaValueList ret = FUNC (params);                            \
                                                                              \
      /* Push the return values and return */                                 \
      for_each (ret.begin(), ret.end(), bind (PushLuaValue, ls, _1));         \
                                                                              \
      return ret.size();                                                      \
   }                                                                          \
   catch (Diluculum::LuaError& e)                                             \
   {                                                                          \
      ReportErrorFromCFunction (ls, e.what());                                \
      return 0;                                                               \
   }                                                                          \
   catch(...)                                                                 \
   {                                                                          \
      ReportErrorFromCFunction (ls, "Unknown exception caught by wrapper.");  \
      return 0;                                                               \
   }                                                                          \
}



/** Returns the name of the wrapper function that is created by
 *  \c DILUCULUM_WRAP_FUNCTION() for a given function name.
 *  @param FUNC The function whose wrapper name is desired.
 */
#define DILUCULUM_WRAPPER_FUNCTION(FUNC) \
Diluculum__ ## FUNC ## __Wrapper_Function


#endif // _DILUCULUM_LUA_WRAPPERS_HPP_
