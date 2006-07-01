/******************************************************************************\
* Namespaces.hpp                                                               *
* Additional Doxygen documentation for Diluculum: namespaces.                  *
* Leandro Motta Barros                                                         *
\******************************************************************************/

/** @brief Everything in Diluculum is declared in this namespace.
 *
 *  Well, everything but the macros defined in
 *  <tt>Diluculum/LuaWrappers.hpp</tt> (because preprocessor macros don't know
 *  what are namespaces.)
 */
namespace Diluculum
{
   /** @brief Implementation details of Diluculum.
    *
    *  Regular users shouldn't bother to anything in this namespace.
    */
   namespace Impl { }
}
