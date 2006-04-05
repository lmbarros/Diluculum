/******************************************************************************\
* TestLuaUSerData.cpp                                                          *
* Unit tests for things declared in 'LuaUserData.hpp'.                         *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <boost/test/unit_test.hpp>
#include <Diluculum/LuaUserData.hpp>


/** A macro that helps defining 'LuaUserData's with some data. It creates a
 *  'LuaUserData' named \c NAME, with \c N bytes of data (\c N > 2, please). The
 *  first 3 bytes are filled with the values \c A, \c B and \c C. The rest is
 *  filled with zeros.
 */
#define CREATE_USERDATA(NAME, N, A, B, C)                         \
unsigned char NAME ## _Bytes[N] = { A, B, C };                    \
LuaUserData NAME (sizeof(NAME ## _Bytes));                        \
memcpy (NAME.getData(), NAME ## _Bytes, sizeof(NAME ## _Bytes));


// - TestUserDataConstruction --------------------------------------------------
void TestUserDataConstruction()
{
   using namespace Diluculum;

   // Create three 'LuaUserData's, of different sizes
   LuaUserData ud1 (4);
   const LuaUserData ud2 (1200);
   LuaUserData ud3 (8765);

   // Ensure that the size returned by 'getSize()' matches the expected
   BOOST_CHECK (ud1.getSize() == 4);
   BOOST_CHECK (ud2.getSize() == 1200);
   BOOST_CHECK (ud3.getSize() == 8765);

   // The address returned by 'getData()' should be valid (non-NULL)
   BOOST_CHECK (ud1.getData() != 0);
   BOOST_CHECK (ud2.getData() != 0);
   BOOST_CHECK (ud3.getData() != 0);

   // Just in case, check that the address of the various 'LuaUserData's are
   // different.
   BOOST_CHECK (ud1.getData() != ud2.getData());
   BOOST_CHECK (ud1.getData() != ud3.getData());
   BOOST_CHECK (ud2.getData() != ud3.getData());
}



// - TestUserDataGetData -------------------------------------------------------
void TestUserDataGetData()
{
   using namespace Diluculum;

   CREATE_USERDATA (ud1, 33, 128, 129, 130);
   unsigned char data1[33] = { 128, 129, 130 };

   CREATE_USERDATA (ud2, 33, 128, 129, 130);
   unsigned char data2[33] = { 128, 129, 130 };

   CREATE_USERDATA (ud3, 33, 128, 129, 255);
   unsigned char data3[33] = { 128, 129, 255 };

   // Check every userdata with the data it is expected to contain
   BOOST_CHECK (memcmp (ud1.getData(), data1, sizeof(data1)) == 0);
   BOOST_CHECK (memcmp (ud1.getData(), data2, sizeof(data2)) == 0);
   BOOST_CHECK (memcmp (ud3.getData(), data3, sizeof(data3)) == 0);

   // Compare data among the 'LuaUserData's
   BOOST_CHECK (memcmp (ud1.getData(), ud2.getData(), sizeof(data1)) == 0);
   BOOST_CHECK (memcmp (ud1.getData(), ud3.getData(), sizeof(data1)) != 0);
   BOOST_CHECK (memcmp (ud2.getData(), ud3.getData(), sizeof(data2)) != 0);
}



// - TestRelationalOperators ---------------------------------------------------
void TestRelationalOperators()
{
   using namespace Diluculum;

   CREATE_USERDATA (ud1, 10, 3, 4, 5);
   CREATE_USERDATA (ud2, 11, 3, 4, 5);
   CREATE_USERDATA (ud3, 9,  3, 4, 5);
   CREATE_USERDATA (ud4, 10, 3, 5, 5);
   CREATE_USERDATA (ud5, 10, 3, 4, 5);

   // operator<
   BOOST_CHECK (ud1 < ud2);
   BOOST_CHECK (ud3 < ud1);
   BOOST_CHECK (ud1 < ud4);
   BOOST_CHECK (!(ud1 < ud5));
   BOOST_CHECK (!(ud2 < ud2));

   // operator>
   BOOST_CHECK (ud2 > ud1);
   BOOST_CHECK (ud1 > ud3);
   BOOST_CHECK (ud4 > ud1);
   BOOST_CHECK (!(ud5 > ud1));
   BOOST_CHECK (!(ud2 > ud2));

   // operator==
   BOOST_CHECK (ud1 == ud5);
   BOOST_CHECK (ud5 == ud1);
   BOOST_CHECK (!(ud2 == ud1));
   BOOST_CHECK (!(ud1 == ud2));
   BOOST_CHECK (!(ud1 == ud3));
   BOOST_CHECK (!(ud3 == ud1));
   BOOST_CHECK (!(ud4 == ud1));
   BOOST_CHECK (!(ud1 == ud4));

   // operator!=
   BOOST_CHECK (ud2 != ud1);
   BOOST_CHECK (ud1 != ud2);
   BOOST_CHECK (ud1 != ud3);
   BOOST_CHECK (ud3 != ud1);
   BOOST_CHECK (ud4 != ud1);
   BOOST_CHECK (ud1 != ud4);
   BOOST_CHECK (!(ud1 != ud5));
   BOOST_CHECK (!(ud5 != ud1));
}



// - TestUserDataCopies --------------------------------------------------------
void TestUserDataCopies()
{
   using namespace Diluculum;

   CREATE_USERDATA (ud1, 10, 3, 4, 5);
   unsigned char data1[10] = { 3, 4, 5 };
   CREATE_USERDATA (ud2, 11, 3, 4, 5);
   CREATE_USERDATA (ud3, 9,  3, 4, 5);
   CREATE_USERDATA (ud4, 10, 3, 5, 5);
   unsigned char data4[10] = { 3, 5, 5 };
   CREATE_USERDATA (ud5, 10, 3, 4, 5);

   // Try a "chained assignment"
   BOOST_REQUIRE (ud2 != ud1);
   BOOST_REQUIRE (ud3 != ud1);

   ud2 = ud3 = ud1;

   BOOST_ASSERT (memcmp (ud1.getData(), data1, sizeof(data1)) == 0);
   BOOST_ASSERT (ud2 == ud1);
   BOOST_ASSERT (ud3 == ud1);

   // And a simple one...
   BOOST_REQUIRE (ud4 != ud5);

   ud5 = ud4;

   BOOST_ASSERT (memcmp (ud4.getData(), data4, sizeof(data4)) == 0);
   BOOST_ASSERT (ud5 == ud4);
}


using boost::unit_test_framework::test_suite;

// - init_unit_test_suite ------------------------------------------------------
test_suite* init_unit_test_suite (int, char*[])
{
   test_suite* test = BOOST_TEST_SUITE ("'LuaUserData' tests");
   test->add (BOOST_TEST_CASE (&TestUserDataConstruction));
   test->add (BOOST_TEST_CASE (&TestUserDataGetData));
   test->add (BOOST_TEST_CASE (&TestRelationalOperators));
   test->add (BOOST_TEST_CASE (&TestUserDataCopies));
   return test;
}