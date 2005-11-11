/******************************************************************************\
* TestLuaValue.cpp                                                             *
* Unit tests for things declared in 'LuaValue.hpp'.                            *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <boost/test/unit_test.hpp>
#include "LuaExceptions.hpp"
#include "LuaValue.hpp"


// - CLuaFunctionExample -------------------------------------------------------
int CLuaFunctionExample (lua_State* ls)
{
   return 0;
}

// - AnotherCLuaFunctionExample ------------------------------------------------
int AnotherCLuaFunctionExample (lua_State* ls)
{
   return 0;
}


// - TestLuaValueType ----------------------------------------------------------
void TestLuaValueType()
{
   using namespace Diluculum;

   LuaValue aNilValue;
   BOOST_CHECK (aNilValue.type() == LUA_TNIL);

   LuaValue aNumberValue1 (1.0);
   BOOST_CHECK (aNumberValue1.type() == LUA_TNUMBER);

   LuaValue aNumberValue2 (1.0f);
   BOOST_CHECK (aNumberValue2.type() == LUA_TNUMBER);

   LuaValue aNumberValue3 (1);
   BOOST_CHECK (aNumberValue3.type() == LUA_TNUMBER);

   LuaValue aNumberValue4 (1UL);
   BOOST_CHECK (aNumberValue4.type() == LUA_TNUMBER);

   LuaValue aStringValue ("Foo");
   BOOST_CHECK (aStringValue.type() == LUA_TSTRING);

   LuaValue aBooleanValue (true);
   BOOST_CHECK (aBooleanValue.type() == LUA_TBOOLEAN);

   LuaValueMap aLuaValueMap;
   LuaValue aTableValue (aLuaValueMap);
   BOOST_CHECK (aTableValue.type() == LUA_TTABLE);

   LuaValue aFunctionValue (CLuaFunctionExample);
   BOOST_CHECK (aFunctionValue.type() == LUA_TFUNCTION);
}



// - TestLuaValueTypeName ------------------------------------------------------
void TestLuaValueTypeName()
{
   using namespace Diluculum;

   LuaValue aNilValue;
   BOOST_CHECK (aNilValue.typeName() == "nil");

   LuaValue aNumberValue1 (1.0);
   BOOST_CHECK (aNumberValue1.typeName() == "number");

   LuaValue aNumberValue2 (1.0f);
   BOOST_CHECK (aNumberValue2.typeName() == "number");

   LuaValue aNumberValue3 (1);
   BOOST_CHECK (aNumberValue3.typeName() == "number");

   LuaValue aNumberValue4 (1U);
   BOOST_CHECK (aNumberValue4.typeName() == "number");

   LuaValue aStringValue ("Foo");
   BOOST_CHECK (aStringValue.typeName() == "string");

   LuaValue aBooleanValue (true);
   BOOST_CHECK (aBooleanValue.typeName() == "boolean");

   LuaValueMap aLuaValueMap;
   LuaValue aTableValue (aLuaValueMap);
   BOOST_CHECK (aTableValue.typeName() == "table");

   LuaValue aFunctionValue (CLuaFunctionExample);
   BOOST_CHECK (aFunctionValue.typeName() == "function");
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
   LuaValue aFunctionValue (CLuaFunctionExample);

   LuaValueMap lvm;
   lvm["Foo"] = false;
   lvm[2.3] = 4.3;
   lvm[5.4] = static_cast<lua_Number>(4);
   lvm[171] = "Hey!";
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
   BOOST_CHECK (aFunctionValue.asFunction() == CLuaFunctionExample);

   BOOST_CHECK (tableValue.asTable()["Foo"].asBoolean() == false);
   BOOST_CHECK (tableValue.asTable()[2.3].asNumber() == 4.3);
   BOOST_CHECK (tableValue.asTable()[5.4].asNumber() == 4);
   BOOST_CHECK (tableValue.asTable()[171].asString() == "Hey!");
   BOOST_CHECK (tableValue.asTable()[true].asString() == "Ahhhh!");

   // Ensure that we get the proper exception when doing the Wrong Thing
   BOOST_CHECK_THROW (aNilValue.asBoolean(), TypeMismatchError);
   BOOST_CHECK_THROW (aNilValue.asNumber(), TypeMismatchError);
   BOOST_CHECK_THROW (aNilValue.asString(), TypeMismatchError);
   BOOST_CHECK_THROW (aNilValue.asTable(), TypeMismatchError);
   BOOST_CHECK_THROW (aNilValue.asFunction(), TypeMismatchError);

   BOOST_CHECK_THROW (anotherNilValue.asBoolean(), TypeMismatchError);
   BOOST_CHECK_THROW (anotherNilValue.asNumber(), TypeMismatchError);
   BOOST_CHECK_THROW (anotherNilValue.asString(), TypeMismatchError);
   BOOST_CHECK_THROW (anotherNilValue.asTable(), TypeMismatchError);
   BOOST_CHECK_THROW (anotherNilValue.asFunction(), TypeMismatchError);

   BOOST_CHECK_THROW (aNumberValue1.asBoolean(), TypeMismatchError);
   BOOST_CHECK_THROW (aNumberValue1.asString(), TypeMismatchError);
   BOOST_CHECK_THROW (aNumberValue1.asTable(), TypeMismatchError);
   BOOST_CHECK_THROW (aNumberValue1.asFunction(), TypeMismatchError);

   BOOST_CHECK_THROW (aStringValueFoo.asBoolean(), TypeMismatchError);
   BOOST_CHECK_THROW (aStringValueFoo.asNumber(), TypeMismatchError);
   BOOST_CHECK_THROW (aStringValueFoo.asTable(), TypeMismatchError);
   BOOST_CHECK_THROW (aStringValueFoo.asFunction(), TypeMismatchError);

   BOOST_CHECK_THROW (aBooleanValue.asNumber(), TypeMismatchError);
   BOOST_CHECK_THROW (aBooleanValue.asString(), TypeMismatchError);
   BOOST_CHECK_THROW (aBooleanValue.asTable(), TypeMismatchError);
   BOOST_CHECK_THROW (aBooleanValue.asFunction(), TypeMismatchError);

   BOOST_CHECK_THROW (aFunctionValue.asNumber(), TypeMismatchError);
   BOOST_CHECK_THROW (aFunctionValue.asString(), TypeMismatchError);
   BOOST_CHECK_THROW (aFunctionValue.asTable(), TypeMismatchError);
   BOOST_CHECK_THROW (aFunctionValue.asBoolean(), TypeMismatchError);
}



// - TestLuaValueRelationalOperators -------------------------------------------
void TestLuaValueRelationalOperators()
{
   using namespace Diluculum;

   LuaValue aNilValue;
   LuaValue anotherNilValue;
   LuaValue aNumberValue1 (1.0);
   LuaValue aNumberValue2 (2);
   LuaValue aStringValueFoo ("Foo");
   LuaValue aStringValueBar ("Bar");
   LuaValue aBooleanValue (true);
   LuaValue anotherBooleanValue (true);
   LuaValue aFunctionValue (CLuaFunctionExample);
   LuaValue anotherFunctionValue (AnotherCLuaFunctionExample);

   LuaValueMap aLuaValueMap;
   LuaValue aTableValue (aLuaValueMap);

   // operator<
   BOOST_CHECK (aBooleanValue < aNilValue);
   BOOST_CHECK (aBooleanValue < aFunctionValue);
   BOOST_CHECK (aNilValue < aNumberValue1);
   BOOST_CHECK (aFunctionValue < aNilValue);
   BOOST_CHECK (aNumberValue2 < aStringValueFoo);
   BOOST_CHECK (aFunctionValue < aNumberValue2);
   BOOST_CHECK (aFunctionValue < aStringValueFoo);
   BOOST_CHECK (aNilValue < aStringValueFoo);
   BOOST_CHECK (aBooleanValue < aStringValueFoo);
   BOOST_CHECK (aBooleanValue < aTableValue);
   BOOST_CHECK (aNilValue < aTableValue);
   BOOST_CHECK (aBooleanValue < aTableValue);
   BOOST_CHECK (aNumberValue1 < aNumberValue2);
   BOOST_CHECK (aStringValueBar < aStringValueFoo);
   BOOST_CHECK (aNumberValue1 < 1.01);
   BOOST_CHECK (aNumberValue2 < 2.5L);
   BOOST_CHECK (aStringValueFoo < "Zzz");

   // ! operator>
   BOOST_CHECK (!(aBooleanValue > aNilValue));
   BOOST_CHECK (!(aBooleanValue > aFunctionValue));
   BOOST_CHECK (!(aNilValue > aNumberValue1));
   BOOST_CHECK (!(aFunctionValue > aNilValue));
   BOOST_CHECK (!(aNumberValue2 > aStringValueFoo));
   BOOST_CHECK (!(aFunctionValue > aNumberValue2));
   BOOST_CHECK (!(aFunctionValue > aStringValueFoo));
   BOOST_CHECK (!(aNilValue > aStringValueFoo));
   BOOST_CHECK (!(aBooleanValue > aStringValueFoo));
   BOOST_CHECK (!(aBooleanValue > aBooleanValue));
   BOOST_CHECK (!(aNilValue > aNilValue));
   BOOST_CHECK (!(aNumberValue1 > aNumberValue1));
   BOOST_CHECK (!(aNumberValue2 > aNumberValue2));
   BOOST_CHECK (!(aBooleanValue > anotherBooleanValue));
   BOOST_CHECK (!(aTableValue > aTableValue));
   BOOST_CHECK (!(aNumberValue1 > aNumberValue2));
   BOOST_CHECK (!(aStringValueBar > aStringValueFoo));
   BOOST_CHECK (!(aNumberValue1 > 1.01));
   BOOST_CHECK (!(aNumberValue2 > 2.5L));
   BOOST_CHECK (!(aStringValueFoo > "Zzz"));

   // operator>
   BOOST_CHECK (aNilValue > aBooleanValue);
   BOOST_CHECK (aNilValue > aFunctionValue);
   BOOST_CHECK (aNumberValue1 > aNilValue);
   BOOST_CHECK (aNumberValue1 > aFunctionValue);
   BOOST_CHECK (aStringValueFoo > aNumberValue2);
   BOOST_CHECK (aStringValueFoo > aNilValue);
   BOOST_CHECK (aStringValueFoo > aBooleanValue);
   BOOST_CHECK (aStringValueFoo > aFunctionValue);
   BOOST_CHECK (aFunctionValue > aBooleanValue);
   BOOST_CHECK (aTableValue > aBooleanValue);
   BOOST_CHECK (aTableValue > aNilValue);
   BOOST_CHECK (aTableValue > aBooleanValue);
   BOOST_CHECK (aTableValue > aStringValueFoo);
   BOOST_CHECK (aNumberValue2 > aNumberValue1);
   BOOST_CHECK (aStringValueFoo > aStringValueBar);
   BOOST_CHECK (aNumberValue1 > -10);
   BOOST_CHECK (aNumberValue2 > 1e-10f);
   BOOST_CHECK (aStringValueFoo > "");

   // ! operator<
   BOOST_CHECK (!(aNilValue < aBooleanValue));
   BOOST_CHECK (!(aNilValue < aFunctionValue));
   BOOST_CHECK (!(aNumberValue1 < aNilValue));
   BOOST_CHECK (!(aNumberValue1 < aFunctionValue));
   BOOST_CHECK (!(aStringValueFoo < aNumberValue2));
   BOOST_CHECK (!(aStringValueFoo < aNilValue));
   BOOST_CHECK (!(aStringValueFoo < aBooleanValue));
   BOOST_CHECK (!(aStringValueFoo < aFunctionValue));
   BOOST_CHECK (!(aFunctionValue < aBooleanValue));
   BOOST_CHECK (!(aBooleanValue < aBooleanValue));
   BOOST_CHECK (!(aNilValue < aNilValue));
   BOOST_CHECK (!(aTableValue < aStringValueFoo));
   BOOST_CHECK (!(aNumberValue1 < aNumberValue1));
   BOOST_CHECK (!(aNumberValue2 < aNumberValue2));
   BOOST_CHECK (!(aBooleanValue < anotherBooleanValue));
   BOOST_CHECK (!(aTableValue < aTableValue));
   BOOST_CHECK (!(aNumberValue2 < aNumberValue1));
   BOOST_CHECK (!(aStringValueFoo < aStringValueBar));
   BOOST_CHECK (!(aNumberValue1 < -10));
   BOOST_CHECK (!(aNumberValue2 < 1e-10f));
   BOOST_CHECK (!(aStringValueFoo < ""));

   // operator==
   BOOST_CHECK (aNumberValue1 == 1);
   BOOST_CHECK (aNumberValue1 == 1.0f);
   BOOST_CHECK (aNumberValue1 == 1.0);
   BOOST_CHECK (aNumberValue1 == 1.0L);
   BOOST_CHECK (aNumberValue1 == 1U);
   BOOST_CHECK (aNumberValue1 == 1L);
   BOOST_CHECK (aNumberValue1 == 1UL);
   BOOST_CHECK (aStringValueFoo == "Foo");
   BOOST_CHECK (aStringValueFoo == std::string("Foo"));
   BOOST_CHECK (aBooleanValue == true);
   BOOST_CHECK (aFunctionValue == CLuaFunctionExample);

   // ! operator!=
   BOOST_CHECK (!(aNumberValue1 != 1));
   BOOST_CHECK (!(aNumberValue1 != 1.0f));
   BOOST_CHECK (!(aNumberValue1 != 1.0));
   BOOST_CHECK (!(aNumberValue1 != 1.0L));
   BOOST_CHECK (!(aNumberValue1 != 1U));
   BOOST_CHECK (!(aNumberValue1 != 1L));
   BOOST_CHECK (!(aNumberValue1 != 1UL));
   BOOST_CHECK (!(aStringValueFoo != "Foo"));
   BOOST_CHECK (!(aStringValueFoo != std::string("Foo")));
   BOOST_CHECK (!(aBooleanValue != true));
   BOOST_CHECK (!(aFunctionValue != CLuaFunctionExample));

   // operator !=
   BOOST_CHECK (aFunctionValue != 0);
   BOOST_CHECK (anotherFunctionValue != 0);
   BOOST_CHECK (aFunctionValue != anotherFunctionValue);

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



// - TestLuaValueSubscriptOperator ---------------------------------------------
void TestLuaValueSubscriptOperator()
{
   using namespace Diluculum;

   LuaValueMap nestedLMV;
   nestedLMV["foo"] = "bar";
   nestedLMV[true] = 123.45;
   nestedLMV[2.0] = false;

   LuaValueMap lvm;
   lvm[4.0] = 6.5;
   lvm[555u] = "FiveFiveFive";
   lvm["nested"] = nestedLMV;
   lvm[true] = false;

   LuaValue tableValue (lvm);

   // Simple data access
   BOOST_CHECK (tableValue[4.0] == 6.5);
   BOOST_CHECK (tableValue[555] == "FiveFiveFive");
   BOOST_CHECK (tableValue["nested"]["foo"] == "bar");
   BOOST_CHECK (tableValue["nested"][true] == 123.45);
   BOOST_CHECK (tableValue["nested"][2.0] == false);
   BOOST_CHECK (tableValue[true] == false);

   // Change some fields
   tableValue[true] = 7.89;
   BOOST_CHECK (tableValue[true] == 7.89);
   tableValue["nested"][2.0] = "baz";
   BOOST_CHECK (tableValue["nested"][2.0] == "baz");

   // Add some new fields
   tableValue["dummy"];
   BOOST_CHECK (tableValue["dummy"] == Nil);

   tableValue["aaa"] = 2.5;
   BOOST_CHECK (tableValue["aaa"] == 2.5);

   tableValue["nested"][7.8] = 1.23;
   BOOST_CHECK (tableValue["nested"][7.8] == 1.23);

   // Try to subscript non-table values
   LuaValue nilValue;
   BOOST_CHECK_THROW (nilValue[1.0], TypeMismatchError);

   LuaValue numberValue(1.0);
   BOOST_CHECK_THROW (numberValue[true], TypeMismatchError);

   LuaValue stringValue("abc");
   BOOST_CHECK_THROW (stringValue["xyz"], TypeMismatchError);

   LuaValue boolValue(true);
   BOOST_CHECK_THROW (boolValue[false], TypeMismatchError);
}



// - TestLuaValueConstSubscriptOperator ----------------------------------------
void TestLuaValueConstSubscriptOperator()
{
   using namespace Diluculum;

   LuaValueMap nestedLMV;
   nestedLMV["foo"] = "bar";
   nestedLMV[true] = 123.45;
   nestedLMV[2.0] = false;

   LuaValueMap lvm;
   lvm[4.0] = 6.5;
   lvm["nested"] = nestedLMV;
   lvm[true] = false;
   lvm[123L] = 456;

   const LuaValue tableValue (lvm);

   // Simple data access
   BOOST_CHECK (tableValue[4.0] == 6.5);
   BOOST_CHECK (tableValue["nested"]["foo"] == "bar");
   BOOST_CHECK (tableValue["nested"][true] == 123.45);
   BOOST_CHECK (tableValue["nested"][2.0] == false);
   BOOST_CHECK (tableValue[true] == false);
   BOOST_CHECK (tableValue[123] == 456);

   // Then, ensure that accessing an invalid key returns 'Nil'
   BOOST_CHECK (tableValue["dummy"] == Nil);
   BOOST_CHECK (tableValue[123.456] == Nil);
   BOOST_CHECK (tableValue["nested"][7.8] == Nil);

   // Try to subscript non-table values
   LuaValue nilValue;
   BOOST_CHECK_THROW (nilValue[1.0], TypeMismatchError);

   LuaValue numberValue(1.0);
   BOOST_CHECK_THROW (numberValue[true], TypeMismatchError);

   LuaValue stringValue("abc");
   BOOST_CHECK_THROW (stringValue["xyz"], TypeMismatchError);

   LuaValue boolValue(true);
   BOOST_CHECK_THROW (boolValue[false], TypeMismatchError);
}



using boost::unit_test_framework::test_suite;

// - init_unit_test_suite ------------------------------------------------------
test_suite* init_unit_test_suite (int, char*[])
{
   test_suite* test = BOOST_TEST_SUITE ("'LuaValue' tests");
   test->add (BOOST_TEST_CASE (&TestLuaValueType));
   test->add (BOOST_TEST_CASE (&TestLuaValueTypeName));
   test->add (BOOST_TEST_CASE (&TestLuaValueAsSomethingFunctions));
   test->add (BOOST_TEST_CASE (&TestLuaValueRelationalOperators));
   test->add (BOOST_TEST_CASE (&TestLuaValueSubscriptOperator));
   test->add (BOOST_TEST_CASE (&TestLuaValueConstSubscriptOperator));

   return test;
}
