/******************************************************************************\
* TestLuaValue.cpp                                                             *
* Unit tests for things declared in 'LuaValue.hpp'.                            *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <boost/test/unit_test.hpp>
#include "LuaValue.hpp"


// - TestLuaValueTypeName ------------------------------------------------------
void TestLuaValueTypeName()
{
   using namespace Diluculum;

   LuaValue aNilValue;
   BOOST_CHECK (aNilValue.typeName() == "nil");

   LuaValue aNumberValue (1.0);
   BOOST_CHECK (aNumberValue.typeName() == "number");

   LuaValue aStringValue ("Foo");
   BOOST_CHECK (aStringValue.typeName() == "string");

   LuaValue aBooleanValue (true);
   BOOST_CHECK (aBooleanValue.typeName() == "boolean");

   LuaValueMap aLuaValueMap;
   LuaValue aTableValue (aLuaValueMap);
   BOOST_CHECK (aTableValue.typeName() == "table");
}



// - TestLuaValueAsSomethingFunctions ------------------------------------------
void TestLuaValueAsSomethingFunctions()
{
   using namespace Diluculum;

   LuaValue aNilValue;
   LuaValue anotherNilValue = Nil;
   LuaValue aNumberValue1 (1.0);
   LuaValue aNumberValue2 (2.0);
   LuaValue aStringValueFoo ("Foo");
   LuaValue aStringValueBar ("Bar");
   LuaValue aBooleanValue (true);
   LuaValue anotherBooleanValue (false);

   LuaValueMap lvm;
   lvm["Foo"] = false;
   lvm[2.3] = 4.3;
   lvm[5.4] = static_cast<lua_Number>(4);
   lvm[false] = Nil;
   lvm[true] = std::string("Ahhhh!");
   LuaValue tableValue (lvm);

   // Try to get the proper values.
   BOOST_CHECK (aNumberValue1.asNumber() == 1.0);
   BOOST_CHECK (aNumberValue2.asNumber() == 2.0);
   BOOST_CHECK (aStringValueFoo.asString() == "Foo");
   BOOST_CHECK (aStringValueBar.asString() == "Bar");
   BOOST_CHECK (aBooleanValue.asBoolean() == true);
   BOOST_CHECK (anotherBooleanValue.asBoolean() == false);

   // Ensure that we get the proper exception when doing the Wrong Thing
   BOOST_CHECK_THROW (aNilValue.asBoolean(), TypeMismatchError);
   BOOST_CHECK_THROW (aNilValue.asNumber(), TypeMismatchError);
   BOOST_CHECK_THROW (aNilValue.asString(), TypeMismatchError);
   BOOST_CHECK_THROW (aNilValue.asTable(), TypeMismatchError);

   BOOST_CHECK_THROW (anotherNilValue.asBoolean(), TypeMismatchError);
   BOOST_CHECK_THROW (anotherNilValue.asNumber(), TypeMismatchError);
   BOOST_CHECK_THROW (anotherNilValue.asString(), TypeMismatchError);
   BOOST_CHECK_THROW (anotherNilValue.asTable(), TypeMismatchError);

   BOOST_CHECK_THROW (aNumberValue1.asBoolean(), TypeMismatchError);
   BOOST_CHECK_THROW (aNumberValue1.asString(), TypeMismatchError);
   BOOST_CHECK_THROW (aNumberValue1.asTable(), TypeMismatchError);

   BOOST_CHECK_THROW (aStringValueFoo.asBoolean(), TypeMismatchError);
   BOOST_CHECK_THROW (aStringValueFoo.asNumber(), TypeMismatchError);
   BOOST_CHECK_THROW (aStringValueFoo.asTable(), TypeMismatchError);

   BOOST_CHECK_THROW (aBooleanValue.asNumber(), TypeMismatchError);
   BOOST_CHECK_THROW (aBooleanValue.asString(), TypeMismatchError);
   BOOST_CHECK_THROW (aBooleanValue.asTable(), TypeMismatchError);
}



// - TestLuaValueOrderOperators-------------------------------------------------
void TestLuaValueOrderOperators()
{
   using namespace Diluculum;

   LuaValue aNilValue;
   LuaValue anotherNilValue;
   LuaValue aNumberValue1 (1.0);
   LuaValue aNumberValue2 (2.0);
   LuaValue aStringValueFoo ("Foo");
   LuaValue aStringValueBar ("Bar");
   LuaValue aBooleanValue (true);
   LuaValue anotherBooleanValue (true);

   LuaValueMap aLuaValueMap;
   LuaValue aTableValue (aLuaValueMap);

   // operator<
   BOOST_CHECK (aBooleanValue < aNilValue);
   BOOST_CHECK (aNilValue < aNumberValue1);
   BOOST_CHECK (aNumberValue2 < aStringValueFoo);
   BOOST_CHECK (aNilValue < aStringValueFoo);
   BOOST_CHECK (aBooleanValue < aStringValueFoo);
   BOOST_CHECK (aBooleanValue < aTableValue);
   BOOST_CHECK (aNilValue < aTableValue);
   BOOST_CHECK (aBooleanValue < aTableValue);
   BOOST_CHECK (aNumberValue1 < aNumberValue2);
   BOOST_CHECK (aStringValueBar < aStringValueFoo);

   // ! operator>
   BOOST_CHECK (!(aBooleanValue > aNilValue));
   BOOST_CHECK (!(aNilValue > aNumberValue1));
   BOOST_CHECK (!(aNumberValue2 > aStringValueFoo));
   BOOST_CHECK (!(aNilValue > aStringValueFoo));
   BOOST_CHECK (!(aBooleanValue > aStringValueFoo));
   BOOST_CHECK (!(aBooleanValue > aBooleanValue));
   BOOST_CHECK (!(aNilValue > aNilValue));
   BOOST_CHECK (!(aNumberValue1 > aNumberValue1));
   BOOST_CHECK (!(aNumberValue2 > aNumberValue2));
   BOOST_CHECK (!(aBooleanValue > anotherBooleanValue));
   BOOST_CHECK (!(aTableValue > aTableValue));
   BOOST_CHECK (!(aNumberValue2 < aNumberValue1));
   BOOST_CHECK (!(aStringValueFoo < aStringValueBar));

   // operator>
   BOOST_CHECK (aNilValue > aBooleanValue);
   BOOST_CHECK (aNumberValue1 > aNilValue);
   BOOST_CHECK (aStringValueFoo > aNumberValue2);
   BOOST_CHECK (aStringValueFoo > aNilValue);
   BOOST_CHECK (aStringValueFoo > aBooleanValue);
   BOOST_CHECK (aTableValue > aBooleanValue);
   BOOST_CHECK (aTableValue > aNilValue);
   BOOST_CHECK (aTableValue > aBooleanValue);
   BOOST_CHECK (aNumberValue2 > aNumberValue1);
   BOOST_CHECK (aStringValueFoo > aStringValueBar);

   // ! operator<
   BOOST_CHECK (!(aNilValue < aBooleanValue));
   BOOST_CHECK (!(aNumberValue1 < aNilValue));
   BOOST_CHECK (!(aStringValueFoo < aNumberValue2));
   BOOST_CHECK (!(aStringValueFoo < aNilValue));
   BOOST_CHECK (!(aStringValueFoo < aBooleanValue));
   BOOST_CHECK (!(aBooleanValue < aBooleanValue));
   BOOST_CHECK (!(aNilValue < aNilValue));
   BOOST_CHECK (!(aNumberValue1 < aNumberValue1));
   BOOST_CHECK (!(aNumberValue2 < aNumberValue2));
   BOOST_CHECK (!(aBooleanValue < anotherBooleanValue));
   BOOST_CHECK (!(aTableValue < aTableValue));
   BOOST_CHECK (!(aNumberValue2 < aNumberValue1));
   BOOST_CHECK (!(aStringValueFoo < aStringValueBar));

   // some tests with more complex tables
   LuaValueMap lvmEmpty;
   LuaValue emptyTableValue (lvmEmpty);

   LuaValueMap lvmShort;
   lvmShort["Foo"] = 1.2;
   lvmShort[false] = "Bar";
   LuaValue shortTableValue (lvmShort);

   BOOST_CHECK (emptyTableValue < shortTableValue);
   BOOST_CHECK (!(emptyTableValue > shortTableValue));
   BOOST_CHECK (shortTableValue > emptyTableValue);
   BOOST_CHECK (!(shortTableValue < emptyTableValue));

   LuaValueMap lvmShortClone;
   lvmShortClone["Foo"] = 1.2;
   lvmShortClone[false] = "Bar";
   LuaValue shortCloneTableValue (lvmShortClone);

   BOOST_CHECK (!(shortTableValue > shortCloneTableValue));
   BOOST_CHECK (!(shortCloneTableValue < shortTableValue));

   LuaValueMap lvmLong;
   lvmLong["Foo"] = false;
   lvmLong[2.3] = 4.3;
   lvmLong[5.4] = lvmShort;
   lvmLong[true] = std::string("Ahhhh!");
   LuaValue longTableValue (lvmLong);

   BOOST_CHECK (shortTableValue < longTableValue);
   BOOST_CHECK (!(shortTableValue > longTableValue));
   BOOST_CHECK (longTableValue > shortTableValue);
   BOOST_CHECK (!(longTableValue < shortTableValue));

   LuaValueMap lvmShortGreater;
   lvmShortGreater["Foo"] = 1.5;
   lvmShortGreater[false] = "Bar";
   LuaValue shortGreaterTableValue (lvmShortGreater);

   BOOST_CHECK (shortTableValue < shortGreaterTableValue);
   BOOST_CHECK (!(shortTableValue > shortGreaterTableValue));
   BOOST_CHECK (shortGreaterTableValue > shortTableValue);
   BOOST_CHECK (!(shortGreaterTableValue < shortTableValue));

   LuaValueMap lvmLongLess;
   lvmLongLess["Foo"] = false;
   lvmLongLess[2.3] = 4.3;
   lvmLongLess[5.4] = lvmShort;
   lvmLongLess[false] = std::string("Ahhhh!");
   LuaValue longLessTableValue (lvmLongLess);

   BOOST_CHECK (longLessTableValue < longTableValue);
   BOOST_CHECK (!(longLessTableValue > longTableValue));
   BOOST_CHECK (longTableValue > longLessTableValue);
   BOOST_CHECK (!(longTableValue < longLessTableValue));
}



using boost::unit_test_framework::test_suite;

// - init_unit_test_suite ------------------------------------------------------
test_suite* init_unit_test_suite (int, char*[])
{
   test_suite* test = BOOST_TEST_SUITE ("'LuaValue' tests.");
   test->add (BOOST_TEST_CASE (&TestLuaValueTypeName));
   test->add (BOOST_TEST_CASE (&TestLuaValueAsSomethingFunctions));
   test->add (BOOST_TEST_CASE (&TestLuaValueOrderOperators));

   return test;
}
