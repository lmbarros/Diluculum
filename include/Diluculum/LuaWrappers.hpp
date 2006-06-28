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



/** Creates a \c lua_CFunction that wraps a method with the signature like the
 *  following one:
 *  <p><tt>Diluculum::LuaValueList Class::Method(
 *  const Diluculum::LuaValueList& params)</tt>
 *  <p>Notice that, thanks to the use of <tt>Diluculum::LuaValueList</tt>s, the
 *  wrapped method can effectively take and return an arbitrary number of
 *  values.
 *  @note The name of the wrapper function is created from the \c CLASS and
 *        \c METHOD parameters. The rule used to generate this name can be quite
 *        complicated and is subject to change in future releases of Diluculum,
 *        so don't try to use it directly. Actually, you don't need it.
 *  @param CLASS The class with a method being wrapped.
 *  @param METHOD The method being wrapped.
 */
#define DILUCULUM_WRAP_METHOD(CLASS, METHOD)                                  \
int Diluculum__ ## CLASS ## __ ## METHOD ## __Method_Wrapper_Function(        \
   lua_State* ls)                                                             \
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
      Diluculum::LuaValue ud = Diluculum::ToLuaValue (ls, 1);                 \
      Diluculum::LuaValueList params;                                         \
      for (int i = 2; i <= numParams; ++i)                                    \
         params.push_back (Diluculum::ToLuaValue (ls, i));                    \
      lua_pop (ls, numParams);                                                \
                                                                              \
      /* Get the object pointer and call the method */                        \
      CLASS** ppObj =                                                         \
         reinterpret_cast<CLASS**>(ud.asUserData().getData());                \
                                                                              \
      Diluculum::LuaValueList ret = (*ppObj)->METHOD (params);                \
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



/** Returns the name of the table that represent the class \c CLASS.
 *  @note This is used internally. Users can ignore this macro.
 */
#define DILUCULUM_CLASS_TABLE(CLASS) \
Diluculum__Class_Table__ ## CLASS



/** Starts a block of class wrapping macro calls. This must be followed by calls
 *  to \c DILUCULUM_CLASS_METHOD() for each method to be exported to Lua and a
 *  final call to \c DILUCULUM_END_CLASS().
 *  @param CLASS The class being exported.
 */
#define DILUCULUM_BEGIN_CLASS(CLASS)                                          \
/* The table representing the class */                                        \
Diluculum::LuaValueMap Diluculum__Class_Table__ ## CLASS;                     \
                                                                              \
/* The Constructor */                                                         \
int Diluculum__ ## CLASS ## __Constructor_Wrapper_Function (lua_State* ls)    \
{                                                                             \
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
      /* Construct the object, wrap it in a userdata, and return */           \
      CLASS* pObj = new CLASS (params);                                       \
                                                                              \
      CLASS** ud =                                                            \
         reinterpret_cast<CLASS**>(lua_newuserdata (ls, sizeof(CLASS*)));     \
                                                                              \
      *ud = pObj;                                                             \
                                                                              \
      lua_getglobal (ls, #CLASS);                                             \
      lua_setmetatable (ls, -2);                                              \
                                                                              \
      return 1;                                                               \
   }                                                                          \
   catch (Diluculum::LuaError& e)                                             \
   {                                                                          \
      Diluculum::Impl::ReportErrorFromCFunction (ls, e.what());               \
      return 0;                                                               \
   }                                                                          \
   catch(...)                                                                 \
   {                                                                          \
      ReportErrorFromCFunction (ls, "Unknown exception caught by wrapper.");  \
      return 0;                                                               \
   }                                                                          \
}                                                                             \
                                                                              \
/* Destructor */                                                              \
int Diluculum__ ## CLASS ## __Destructor_Wrapper_Function (lua_State* ls)     \
{                                                                             \
   CLASS** ppObj = reinterpret_cast<CLASS**>(lua_touserdata (ls, -1));        \
   delete *ppObj;                                                             \
   return 0;                                                                  \
}                                                                             \
                                                                              \
/* The function used to register the class in a 'LuaState' (first part) */    \
void Diluculum_Register_Class__ ## CLASS (Diluculum::LuaState& ls)            \
{                                                                             \
   Diluculum__Class_Table__ ## CLASS["classname"] = #CLASS;



/** Exports a given class' method. This macro must be called between calls to
 *  \c DILUCULUM_BEGIN_CLASS() and \c DILUCULUM_END_CLASS(). Also, the method
 *  must have been previously wrapped by a call to \c DILUCULUM_WRAP_METHOD().
 *  @param CLASS The class whose method is being exported.
 *  @param METHOD The method being exported.
 */
#define DILUCULUM_CLASS_METHOD(CLASS, METHOD)                              \
   Diluculum__Class_Table__ ## CLASS[#METHOD] =                            \
      Diluculum__ ## CLASS ## __ ## METHOD ## __Method_Wrapper_Function;



/** Ends a block of class wrapping macro calls (which was opened by a call to
 *  \c DILUCULUM_BEGIN_CLASS()).
 *  @param CLASS The class being exported.
 */
#define DILUCULUM_END_CLASS(CLASS)                             \
   Diluculum__Class_Table__ ## CLASS["new"] =                  \
      Diluculum__ ## CLASS ## __Constructor_Wrapper_Function;  \
                                                               \
   Diluculum__Class_Table__ ## CLASS["__gc"] =                 \
      Diluculum__ ## CLASS ## __Destructor_Wrapper_Function;   \
                                                               \
   DILUCULUM_CLASS_TABLE(CLASS)["__index"] =                   \
      Diluculum__Class_Table__ ## CLASS;                       \
                                                               \
   ls[#CLASS] = Diluculum__Class_Table__ ## CLASS;             \
} /* end of Diluculum_Register_Class__CLASS */



/** Registers a class in a given \c LuaState. The class must have been
 *  previously exported by calls to \c DILUCULUM_BEGIN_CLASS,
 *  \c DILUCULUM_END_CLASS() and probably \c DILUCULUM_CLASS_METHOD().
 *  @param LUA_STATE The \c LuaState in which the class will be available after
 *         this call.
 *  @param CLASS The class being registered.
 */
#define DILUCULUM_REGISTER_CLASS(LUA_STATE, CLASS)  \
   Diluculum_Register_Class__ ## CLASS (LUA_STATE);



#endif // _DILUCULUM_LUA_WRAPPERS_HPP_
