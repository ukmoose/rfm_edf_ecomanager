/*
 * DynamicArray_test.cpp
 *
 *  Created on: 16 Oct 2012
 *      Author: jack
 */

#include <iostream>

#include "../CcTx.h"
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CcTxArrayTest
#include <boost/test/unit_test.hpp>

CcTxArray make_cc_tx_array()
{
    uint8_t length = 20;
    CcTxArray cc_txs;

    for (uint8_t i=1; i<=length; i++) {
        BOOST_CHECK(cc_txs.append(i*5));
    }

    return cc_txs;
}

BOOST_AUTO_TEST_CASE(testNullData)
{
    Logger::log_threshold = FATAL;

    // Test destructing if data = NULL
    // Try accessing operator[] when data == NULL
    {
        // Testing access on null data. Should generate an OUT OF RANGE WARNING
        // if log_threshold <= WARN

        CcTxArray cc_txs;
        cc_txs[0];

        // Test that print() doesn't blow up with null data
        cc_txs.print();

        // Test that find() doesn't blow up with null data
        index_t index;
        BOOST_CHECK(!cc_txs.find(100, index));
        BOOST_CHECK_EQUAL(index,  0);

        BOOST_CHECK(!cc_txs.find(0, index));
        BOOST_CHECK_EQUAL(index, 0);
    }
}

BOOST_AUTO_TEST_CASE(find)
{
    CcTxArray cc_txs;
    index_t index;

    cc_txs.append(10);
    BOOST_CHECK(!cc_txs.find(0, index));
    BOOST_CHECK_EQUAL(index, 0);

    BOOST_CHECK(cc_txs.find(10, index));
    BOOST_CHECK_EQUAL(index, 0);

    BOOST_CHECK(!cc_txs.find(9));
    BOOST_CHECK(cc_txs.find(10));
    BOOST_CHECK(!cc_txs.find(11));

    BOOST_CHECK(!cc_txs.find(50, index));
    BOOST_CHECK_EQUAL(index, 1);

    cc_txs = make_cc_tx_array();

    BOOST_CHECK( !cc_txs.find(0, index) );
    BOOST_CHECK_EQUAL(index, 0);

    BOOST_CHECK( !cc_txs.find(29, index) );
    BOOST_CHECK_EQUAL(index, 5);

    BOOST_CHECK( cc_txs.find(30, index) );
    BOOST_CHECK_EQUAL(index, 5);

    BOOST_CHECK( !cc_txs.find(31, index) );
    BOOST_CHECK_EQUAL(index, 6);

    BOOST_CHECK( cc_txs.find(95, index) );
    BOOST_CHECK_EQUAL(index, 18);

    BOOST_CHECK( cc_txs.find(100, index) );
    BOOST_CHECK_EQUAL(index, 19);

    BOOST_CHECK( !cc_txs.find(101, index) );
    BOOST_CHECK_EQUAL(index, 20);

    BOOST_CHECK( !cc_txs.find(0xFFFFFFFF, index) );
    BOOST_CHECK_EQUAL(index, 20);

}

BOOST_AUTO_TEST_CASE(testAppend)
{
    CcTxArray cc_txs = make_cc_tx_array();

    id_t first_array[] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};

    for (uint8_t i=0; i<20; i++) {
        BOOST_CHECK_EQUAL(cc_txs[i].id, first_array[i]);
    }

    cc_txs.append(41);

    id_t second_array[] = {5, 10, 15, 20, 25, 30, 35, 40, 41, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};

    for (uint8_t i=0; i<21; i++) {
        BOOST_CHECK_EQUAL(cc_txs[i].id, second_array[i]);
    }
}


BOOST_AUTO_TEST_CASE(copyConstructor)
{
    CcTxArray cc_txs = make_cc_tx_array();
    CcTxArray cc_txs2 = cc_txs;
    id_t first_array[] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};

    for (uint8_t i=0; i<20; i++) {
        BOOST_CHECK_EQUAL(cc_txs[i].id, first_array[i]);
        BOOST_CHECK_EQUAL(cc_txs2[i].id, first_array[i]);
    }

}

BOOST_AUTO_TEST_CASE(copyConstructorNullData)
{
    CcTxArray cc_txs, cc_txs2;
    cc_txs2 = cc_txs;
    BOOST_CHECK_EQUAL(cc_txs.get_n(), 0);
    BOOST_CHECK_EQUAL(cc_txs2.get_n(), 0);
}

// test with more realistic set of IDs
BOOST_AUTO_TEST_CASE(realisticIDs)
{
    CcTxArray cc_txs;
    index_t index;

    BOOST_CHECK( cc_txs.append(0x000000FF));
    BOOST_CHECK( cc_txs.find(0x000000FF, index) );
    BOOST_CHECK_EQUAL(index, 0);

    BOOST_CHECK( !cc_txs.find(0x0000000F, index) );
    BOOST_CHECK_EQUAL(index, 0);

    BOOST_CHECK( !cc_txs.find(0x00000FFF, index) );
    BOOST_CHECK_EQUAL(index, 1);

    BOOST_CHECK( cc_txs.append(0xABCDEFAB));
    BOOST_CHECK( cc_txs.find(0x000000FF, index) );
    BOOST_CHECK_EQUAL(index, 0);

    BOOST_CHECK( !cc_txs.find(0x0000000F, index) );
    BOOST_CHECK_EQUAL(index, 0);

    BOOST_CHECK( !cc_txs.find(0x00000FFF, index) );
    BOOST_CHECK_EQUAL(index, 1);

    BOOST_CHECK( cc_txs.find(0xABCDEFAB, index) );
    BOOST_CHECK_EQUAL(index, 1);

    BOOST_CHECK( !cc_txs.find(0xABCDEFFF, index) );
    BOOST_CHECK_EQUAL(index, 2);

    BOOST_CHECK( cc_txs.append(0xAABBCCDD));
    BOOST_CHECK( cc_txs.find(0x000000FF, index) );
    BOOST_CHECK_EQUAL(index, 0);
    BOOST_CHECK( cc_txs.find(0xAABBCCDD, index) );
    BOOST_CHECK_EQUAL(index, 1);
    BOOST_CHECK( cc_txs.find(0xABCDEFAB, index) );
    BOOST_CHECK_EQUAL(index, 2);


    BOOST_CHECK( cc_txs.append(0x00112233));
    BOOST_CHECK( cc_txs.find(0x000000FF, index) );
    BOOST_CHECK_EQUAL(index, 0);
    BOOST_CHECK( cc_txs.find(0x00112233, index) );
    BOOST_CHECK_EQUAL(index, 1);

    BOOST_CHECK( cc_txs.find(0xAABBCCDD, index) );
    BOOST_CHECK_EQUAL(index, 2);
    BOOST_CHECK( cc_txs.find(0xABCDEFAB, index) );
    BOOST_CHECK_EQUAL(index, 3);
}

BOOST_AUTO_TEST_CASE(setSize)
{
    CcTxArray cc_txs;
    index_t index;

    BOOST_CHECK(cc_txs.set_size(10));
    BOOST_CHECK( cc_txs.append(0x000000FF));

    BOOST_CHECK( cc_txs.find(0x000000FF, index) );
    BOOST_CHECK_EQUAL(index, 0);

    BOOST_CHECK( !cc_txs.find(0x0000000F, index) );
    BOOST_CHECK_EQUAL(index, 0);

    BOOST_CHECK( !cc_txs.find(0x00000FFF, index) );
    BOOST_CHECK_EQUAL(index, 1);

    BOOST_CHECK( cc_txs.append(0xABCDEFAB));

    BOOST_CHECK( cc_txs.find(0x000000FF, index) );
    BOOST_CHECK_EQUAL(index, 0);

    BOOST_CHECK( !cc_txs.find(0x0000000F, index) );
    BOOST_CHECK_EQUAL(index, 0);

    BOOST_CHECK( !cc_txs.find(0x00000FFF, index) );
    BOOST_CHECK_EQUAL(index, 1);

    BOOST_CHECK( cc_txs.find(0xABCDEFAB, index) );
    BOOST_CHECK_EQUAL(index, 1);

    BOOST_CHECK( !cc_txs.find(0xABCDEFFF, index) );
    BOOST_CHECK_EQUAL(index, 2);

    BOOST_CHECK( cc_txs.append(0xAABBCCDD));
    BOOST_CHECK( cc_txs.find(0x000000FF, index) );
    BOOST_CHECK_EQUAL(index, 0);
    BOOST_CHECK( cc_txs.find(0xAABBCCDD, index) );
    BOOST_CHECK_EQUAL(index, 1);
    BOOST_CHECK( cc_txs.find(0xABCDEFAB, index) );
    BOOST_CHECK_EQUAL(index, 2);


    BOOST_CHECK( cc_txs.append(0x00112233));
    BOOST_CHECK( cc_txs.find(0x000000FF, index) );
    BOOST_CHECK_EQUAL(index, 0);
    BOOST_CHECK( cc_txs.find(0x00112233, index) );
    BOOST_CHECK_EQUAL(index, 1);

    BOOST_CHECK( cc_txs.find(0xAABBCCDD, index) );
    BOOST_CHECK_EQUAL(index, 2);
    BOOST_CHECK( cc_txs.find(0xABCDEFAB, index) );
    BOOST_CHECK_EQUAL(index, 3);

    BOOST_CHECK( !cc_txs.append(0x00112233));
    BOOST_CHECK( cc_txs.find(0x000000FF, index) );
    BOOST_CHECK_EQUAL(index, 0);
    BOOST_CHECK( cc_txs.find(0x00112233, index) );
    BOOST_CHECK_EQUAL(index, 1);

    BOOST_CHECK( cc_txs.append(0x00112200));
    BOOST_CHECK( cc_txs.append(0x00000001));
    BOOST_CHECK( cc_txs.append(0x00000002));
    BOOST_CHECK( cc_txs.append(0x00000003));
    BOOST_CHECK( cc_txs.append(0x00000004));
    BOOST_CHECK( cc_txs.append(0x00000005));
    BOOST_CHECK( cc_txs.append(0x00000006));
    BOOST_CHECK( cc_txs.append(0x00000007));
    BOOST_CHECK( cc_txs.append(0x00000008));
    BOOST_CHECK( cc_txs.append(0x00000009));
}

BOOST_AUTO_TEST_CASE(remove_index)
{
    CcTxArray cc_txs;
    index_t index;

    BOOST_CHECK(cc_txs.set_size(10));
    BOOST_CHECK( cc_txs.append(0x000000FF) );
    BOOST_CHECK( cc_txs.append(0xABCDEFAB) );
    BOOST_CHECK( cc_txs.append(0xBBBBBBBB) );
    BOOST_CHECK( cc_txs.append(0xCCCCCCCC) );

    BOOST_CHECK( !cc_txs.remove_index(20) );
    BOOST_CHECK( !cc_txs.remove_index(5) );

    BOOST_CHECK( cc_txs.remove_index(0) );

    BOOST_CHECK( !cc_txs.find(0x000000FF, index) );
    BOOST_CHECK( cc_txs.find(0xABCDEFAB, index) );
    BOOST_CHECK_EQUAL(index, 0);
    BOOST_CHECK( cc_txs.find(0xBBBBBBBB, index) );
    BOOST_CHECK_EQUAL(index, 1);
    BOOST_CHECK( cc_txs.find(0xCCCCCCCC, index) );
    BOOST_CHECK_EQUAL(index, 2);

    BOOST_CHECK( !cc_txs.remove_index(3) );
    BOOST_CHECK( cc_txs.remove_index(2) );

    BOOST_CHECK( !cc_txs.find(0x000000FF, index) );
    BOOST_CHECK( cc_txs.find(0xABCDEFAB, index) );
    BOOST_CHECK_EQUAL(index, 0);
    BOOST_CHECK( cc_txs.find(0xBBBBBBBB, index) );
    BOOST_CHECK_EQUAL(index, 1);
    BOOST_CHECK( !cc_txs.find(0xCCCCCCCC, index) );
    BOOST_CHECK_EQUAL(index, 2);

    BOOST_CHECK( !cc_txs.remove_index(2) );
    BOOST_CHECK( cc_txs.remove_index(1) );
    BOOST_CHECK( !cc_txs.find(0x000000FF, index) );
    BOOST_CHECK( cc_txs.find(0xABCDEFAB, index) );
    BOOST_CHECK_EQUAL(index, 0);
    BOOST_CHECK( !cc_txs.find(0xBBBBBBBB, index) );
    BOOST_CHECK_EQUAL(index, 1);
    BOOST_CHECK( !cc_txs.find(0xCCCCCCCC, index) );
    BOOST_CHECK_EQUAL(index, 1);

    BOOST_CHECK( !cc_txs.remove_index(1) );
    BOOST_CHECK( cc_txs.remove_index(0) );
    BOOST_CHECK( !cc_txs.find(0x000000FF, index) );
    BOOST_CHECK( !cc_txs.find(0xABCDEFAB, index) );
    BOOST_CHECK_EQUAL(index, 0);
    BOOST_CHECK( !cc_txs.find(0xBBBBBBBB, index) );
    BOOST_CHECK_EQUAL(index, 0);
    BOOST_CHECK( !cc_txs.find(0xCCCCCCCC, index) );
    BOOST_CHECK_EQUAL(index, 0);

    BOOST_CHECK( !cc_txs.remove_index(0) );
    BOOST_CHECK( !cc_txs.remove_index(1) );
    BOOST_CHECK( !cc_txs.remove_index(255) );

}


BOOST_AUTO_TEST_CASE(remove_id)
{
    CcTxArray cc_txs;
    index_t index;

    BOOST_CHECK(cc_txs.set_size(10));
    BOOST_CHECK( cc_txs.append(0x000000FF) );
    BOOST_CHECK( cc_txs.append(0xABCDEFAB) );
    BOOST_CHECK( cc_txs.append(0xBBBBBBBB) );
    BOOST_CHECK( cc_txs.append(0xCCCCCCCC) );

    BOOST_CHECK( !cc_txs.remove_id(20) );
    BOOST_CHECK( !cc_txs.remove_id(5) );

    BOOST_CHECK( cc_txs.remove_id(0x000000FF) );

    BOOST_CHECK( !cc_txs.find(0x000000FF, index) );
    BOOST_CHECK( cc_txs.find(0xABCDEFAB, index) );
    BOOST_CHECK_EQUAL(index, 0);
    BOOST_CHECK( cc_txs.find(0xBBBBBBBB, index) );
    BOOST_CHECK_EQUAL(index, 1);
    BOOST_CHECK( cc_txs.find(0xCCCCCCCC, index) );
    BOOST_CHECK_EQUAL(index, 2);

    BOOST_CHECK( !cc_txs.remove_id(0x000000FF) );
    BOOST_CHECK( cc_txs.remove_id(0xCCCCCCCC) );

    BOOST_CHECK( !cc_txs.find(0x000000FF, index) );
    BOOST_CHECK( cc_txs.find(0xABCDEFAB, index) );
    BOOST_CHECK_EQUAL(index, 0);
    BOOST_CHECK( cc_txs.find(0xBBBBBBBB, index) );
    BOOST_CHECK_EQUAL(index, 1);
    BOOST_CHECK( !cc_txs.find(0xCCCCCCCC, index) );
    BOOST_CHECK_EQUAL(index, 2);

    BOOST_CHECK( !cc_txs.remove_id(0xCCCCCCCC) );
    BOOST_CHECK( cc_txs.remove_id(0xBBBBBBBB) );
    BOOST_CHECK( !cc_txs.find(0x000000FF, index) );
    BOOST_CHECK( cc_txs.find(0xABCDEFAB, index) );
    BOOST_CHECK_EQUAL(index, 0);
    BOOST_CHECK( !cc_txs.find(0xBBBBBBBB, index) );
    BOOST_CHECK_EQUAL(index, 1);
    BOOST_CHECK( !cc_txs.find(0xCCCCCCCC, index) );
    BOOST_CHECK_EQUAL(index, 1);

    BOOST_CHECK( !cc_txs.remove_id(0xBBBBBBBB) );
    BOOST_CHECK( cc_txs.remove_id(0xABCDEFAB) );
    BOOST_CHECK( !cc_txs.find(0x000000FF, index) );
    BOOST_CHECK( !cc_txs.find(0xABCDEFAB, index) );
    BOOST_CHECK_EQUAL(index, 0);
    BOOST_CHECK( !cc_txs.find(0xBBBBBBBB, index) );
    BOOST_CHECK_EQUAL(index, 0);
    BOOST_CHECK( !cc_txs.find(0xCCCCCCCC, index) );
    BOOST_CHECK_EQUAL(index, 0);

    BOOST_CHECK( !cc_txs.remove_index(0) );
    BOOST_CHECK( !cc_txs.remove_index(1) );
    BOOST_CHECK( !cc_txs.remove_index(255) );

    BOOST_CHECK( !cc_txs.remove_id(0xBBBBBBBB) );
    BOOST_CHECK( !cc_txs.remove_id(0xABCDEFAB) );

}

