#include "SsdTest.h"

TEST_F(SsdFixture, executeCommandSuccess)
{
    commandInvalidTest("W 2 0xAAAABBBB", true);
}

TEST_F(SsdFixture, executeCommandInvalidCommand)
{
    commandInvalidTest("D 2 0xAAAABBBB", false);
}

TEST_F(SsdFixture, executeCommandLBAOutOfRange)
{
    commandInvalidTest("W 100 0xAAAABBBB", false);
}

TEST_F(SsdFixture, executeCommandLBAValueErrorInvalidValue1)
{
    commandInvalidTest("W 2 0xAAAABB!!", false);
}

TEST_F(SsdFixture, executeCommandLBAValueErrorInvalidValue2)
{
    commandInvalidTest("W 2 AxAAAABBBB", false);
}

TEST_F(SsdFixture, executeCommandLBAValueErrorByteOutOfRange)
{
    commandInvalidTest("W 2 0xAAAABBAABBBB", false);
}

TEST_F(SsdFixture, commandResultWriteError)
{
    writeResultTest("W 100 0xAAAABBBB", "ERROR");
}

TEST_F(SsdFixture, commandResultReadError)
{
    readResultTest("W 100 0xAAAABBBB", "ERROR");
}

TEST_F(SsdFixture, commandResultWriteSuccess)
{
    writeResultTest("W 0 0xAAAABBBB", "0 0xAAAABBBB");
}

TEST_F(SsdFixture, commandResultReadSuccess)
{
    readResultTest("R 0", "0xAAAABBBB");
}