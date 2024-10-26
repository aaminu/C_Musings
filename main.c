#include <stdlib.h>
#include <stdio.h>

#include "munit.h"
#include "object.h"

/*MSVC warning about conditional expressions being constant*/
#if defined(_MSC_VER)
#pragma warning(disable : 4127)
#endif

static MunitResult test_ref_count(const MunitParameter params[], void *data)
{
    object_t *foo = new_integer(1);
    object_t *array = new_array(3);
    array_set(array, 0, foo);
    array_set(array, 1, foo);
    array_set(array, 2, foo);
    munit_assert_int(foo->refcount, ==, 4);

    object_t *value = array_get(array, 1);
    munit_assert_int(foo->refcount, ==, 4);

    object_t *value2 = array_get(array, 1);
    add_reference(value2);
    munit_assert_int(foo->refcount, ==, 5);

    munit_assert_false(object_free(&foo));
    munit_assert_int(foo->refcount, ==, 5);
    object_free(&array);
    munit_assert_null(array);
    munit_assert_int(foo->refcount, ==, 2);

    
    release_reference(&value2);
    munit_assert_int(foo->refcount, ==, 1);

    object_free(&foo);
    munit_assert_null(foo);
}

static MunitResult test_integer_add(const MunitParameter params[], void *data)
{
    (void)params;
    (void)data;

    object_t *one = new_integer(1);
    object_t *three = new_integer(3);
    object_t *four = add(one, three);

    munit_assert_int(one->refcount, ==, 1);
    munit_assert_int(three->refcount, ==, 1);
    munit_assert_not_null(four);
    munit_assert_int(four->kind, ==, INTEGER);
    munit_assert_int(four->data.v_int, ==, 4);

    object_free(&one);
    object_free(&three);
    object_free(&four);

    munit_assert_null(one);
    munit_assert_null(three);
    munit_assert_null(four);

    return MUNIT_OK;
}

static MunitResult test_float_add(const MunitParameter params[], void *data)
{
    (void)params;
    (void)data;

    object_t *one = new_float(1.5);
    object_t *three = new_float(3.5);
    object_t *five = add(one, three);

    munit_assert_not_null(five);
    munit_assert_int(five->kind, ==, FLOAT);
    munit_assert_float(five->data.v_float, ==, 1.5 + 3.5);

    object_free(&one);
    object_free(&three);
    object_free(&five);

    munit_assert_null(one);
    munit_assert_null(three);
    munit_assert_null(five);

    return MUNIT_OK;
}

static MunitResult test_string_add(const MunitParameter params[], void *data)
{
    object_t *hello = new_string("hello");
    object_t *world = new_string(", world");
    object_t *greeting = add(hello, world);

    munit_assert_not_null(greeting);
    munit_assert_int(greeting->kind, ==, STRING);
    munit_assert_string_equal(
        greeting->data.v_string, "hello, world");

    object_free(&hello);
    object_free(&world);
    object_free(&greeting);

    munit_assert_null(hello);
    munit_assert_null(world);
    munit_assert_null(greeting);

    return MUNIT_OK;
}

static MunitResult test_string_add_self(const MunitParameter params[], void *data)
{
    object_t *repeated = new_string("(repeated)");
    object_t *result = add(repeated, repeated);

    munit_assert_not_null(result);
    munit_assert_int(result->kind, ==, STRING);
    munit_assert_string_equal(
        result->data.v_string,
        "(repeated)(repeated)");

    object_free(&repeated);
    object_free(&result);
    munit_assert_null(repeated);
    munit_assert_null(result);
}

static MunitResult test_vector3_add(const MunitParameter params[], void *data)
{
    object_t *one = new_float(1.0);
    object_t *two = new_float(2.0);
    object_t *three = new_float(3.0);
    object_t *four = new_float(4.0);
    object_t *five = new_float(5.0);
    object_t *six = new_float(6.0);

    object_t *v1 = new_vector3(one, two, three);
    object_t *v2 = new_vector3(four, five, six);
    object_t *result = add(v1, v2);

    munit_assert_not_null(result);
    munit_assert_int(result->kind, ==, VECTOR3);

    munit_assert_float(result->data.v_vector3.x->data.v_float, ==, 5.0);
    munit_assert_float(result->data.v_vector3.y->data.v_float, ==, 7.0);
    munit_assert_float(result->data.v_vector3.z->data.v_float, ==, 9.0);

    munit_assert_int(one->refcount, ==, 2);
    munit_assert_int(two->refcount, ==, 2);
    munit_assert_int(three->refcount, ==, 2);
    munit_assert_int(four->refcount, ==, 2);
    munit_assert_int(five->refcount, ==, 2);
    munit_assert_int(six->refcount, ==, 2);

    object_free(&v1);
    munit_assert_null(v1);

    object_free(&v2);
    munit_assert_null(v2);
    munit_assert_int(one->refcount, ==, 1);
    munit_assert_int(two->refcount, ==, 1);
    munit_assert_int(three->refcount, ==, 1);
    munit_assert_int(four->refcount, ==, 1);
    munit_assert_int(five->refcount, ==, 1);
    munit_assert_int(six->refcount, ==, 1);

    munit_assert_int(result->data.v_vector3.x->refcount, ==, 1);
    munit_assert_int(result->data.v_vector3.y->refcount, ==, 1);
    munit_assert_int(result->data.v_vector3.z->refcount, ==, 1);
    object_free(&result);
    munit_assert_null(result);

    release_reference(&one);
    release_reference(&two);
    release_reference(&three);
    object_free(&four);
    object_free(&five);
    object_free(&six);

    munit_assert_null(one);
    munit_assert_null(two);
    munit_assert_null(three);
    munit_assert_null(four);
    munit_assert_null(five);
    munit_assert_null(six);

    return MUNIT_OK;
}

static MunitResult test_array_add(const MunitParameter params[], void *data)
{
    object_t *one = new_integer(1);
    object_t *ones = new_array(2);
    munit_assert(array_set(ones, 0, one));
    munit_assert(array_set(ones, 1, one));
    munit_assert_int(one->refcount, ==, 3);
    
    object_t *hi = new_string("hi");
    object_t *hellos = new_array(3);
    munit_assert(array_set(hellos, 0, hi));
    munit_assert(array_set(hellos, 1, hi));
    munit_assert(array_set(hellos, 2, hi));
    munit_assert_int(hi->refcount, ==, 4);
    

    object_t *result = add(ones, hellos);

    munit_assert_not_null(result);
    munit_assert_int(result->kind, ==, ARRAY);

    object_t *first = array_get(result, 0);
    munit_assert_not_null(first);
    munit_assert_int(first->data.v_int, ==, 1);

    object_t *third = array_get(result, 2);
    munit_assert_not_null(third);
    munit_assert_string_equal(third->data.v_string, "hi");

    // Check for deeply nested reference
    munit_assert_int(one->refcount, ==, 5);
    munit_assert_int(hi->refcount, ==, 7);
    munit_assert_int(hellos->refcount, ==, 1);
    munit_assert_int(ones->refcount, ==, 1);

    object_free(&result);
    munit_assert_int(one->refcount, ==, 3);
    munit_assert_int(hi->refcount, ==, 4);

    object_free(&hellos);
    object_free(&ones);
    munit_assert_null(ones);
    munit_assert_null(hellos);
    munit_assert_int(one->refcount, ==, 1);
    munit_assert_int(hi->refcount, ==, 1);
    
    object_free(&hi);
    object_free(&one);
    munit_assert_null(one);
    munit_assert_null(hi);

    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
    {(char *)"/test/ref_count", test_ref_count, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/test/integer_add", test_integer_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/test/float_add", test_float_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/test/string_add", test_string_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/test/string_self_add", test_string_add_self, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/test/vetor3_add", test_vector3_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {(char *)"/test/array_add", test_array_add, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static const MunitSuite test_suite = {
    (char *)"Object",
    test_suite_tests,
    NULL,
    0,
    MUNIT_SUITE_OPTION_NONE};

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)])
{

    return munit_suite_main(&test_suite, (void *)"µnit", argc, argv);
}
