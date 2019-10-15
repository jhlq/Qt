#define BOOST_TEST_MODULE TriangleGridTests
#include <boost/test/unit_test.hpp>
#include <cmath>

#include "trianglegrid.h"

BOOST_AUTO_TEST_CASE(test0)
{
    int sideLength=7;
    TriangleGrid tg(sideLength);
    int ntriangles=tg.nTriangles();
    BOOST_CHECK_EQUAL(pow(sideLength,2),ntriangles);
}



