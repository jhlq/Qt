#define BOOST_TEST_MODULE BoardTests
#include <boost/test/unit_test.hpp>

#include "board.h"

BOOST_AUTO_TEST_CASE(test0)
{
    Board board=Board(7);
    Triangle t=Triangle(1,1,1);
    BOOST_CHECK_EQUAL(board.isValidMove(t),true);
    t=Triangle(1,6,1);
    BOOST_CHECK_EQUAL(board.isValidMove(t),false);
    board.tg.set(1,0,2);
    t=Triangle(0,0,1);
    BOOST_CHECK_EQUAL(board.isValidMove(t),false);
}
BOOST_AUTO_TEST_CASE(test1)
{
    Board board=Board(7);
    Board bc=Board(board);//board.deepcopy();
    bc.tg.set(0,0,1);
    BOOST_CHECK_EQUAL(board.tg.get(0,0).player,0);
}
