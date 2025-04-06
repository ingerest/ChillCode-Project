#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <fstream>
#include <cstdlib>
#include <windows.h>
#include <shellapi.h>
#include <io.h>
#include <filesystem>
#include <random>
#include <algorithm>
#include "TestShell.h"
#include "Logger.cpp"

using namespace std;

class TestScript {
public:
    static const int INPUT_ARRAY_SIZE = 100;
    static const int PARTIAL_LBA_LIMIT = 30;
    static const int COMPARE_BATCH_SIZE = 5;

    TestScript(TestShell* testShell) : m_testShell(testShell) {}

    bool isTestScriptCommand(const string& command) {
        return command == "1_FullWriteAndReadCompare" || command == "1_" ||
            command == "2_PartialLBAWrite" || command == "2_" ||
            command == "3_WriteReadAging" || command == "3_" ||
            command == "4_EraseAndWriteAging" || command == "4_";
    }

    string handleTestScriptCommand(const string& command) {
        Logger::getInstance().log("handleTestScriptCommand", "Handling test script command: " + command);

        if (command == "1_FullWriteAndReadCompare" || command == "1_")
            return handle1_FullWriteAndReadCompareCommand();
        if (command == "2_PartialLBAWrite" || command == "2_")
            return handle2_PartialLBACommand();
        if (command == "3_WriteReadAging" || command == "3_")
            return handle3_WriteReadAgingCommand();
        if (command == "4_EraseAndWriteAging" || command == "4_")
            return handle4_EraseAndWriteAgingCommand();

        Logger::getInstance().log("handleTestScriptCommand", "Test script command not recognized: " + command);
        return "INVALID COMMAND";
    }

    unsigned int generateRandomHex() {
        // 난수 생성 엔진 설정
        std::random_device rd;
        std::mt19937 gen(rd()); // Mersenne Twister 엔진
        std::uniform_int_distribution<unsigned int> dist(0, 0xFFFFFFFF);

        // 랜덤 값 생성
        return dist(gen);
    }

    std::string unsignedIntToHexString(unsigned int value) {
        std::stringstream ss;
        ss << "0x"
            << std::setw(8)       // 8자리로 설정 (32비트 크기)
            << std::setfill('0')  // 0으로 채우기
            << std::hex           // 16진수 형식으로 출력
            << std::uppercase     // 대문자로 출력
            << value;             // 정수 값 삽입
        return ss.str();
    }

    std::string intToHexString(int value) {
        std::stringstream ss;
        ss << "0x"
            << std::setw(8)       // 8자리로 설정 (32비트 크기)
            << std::setfill('0')  // 0으로 채우기
            << std::hex           // 16진수 형식으로 출력
            << std::uppercase     // 대문자로 출력
            << value;             // 정수 값 삽입
        return ss.str();
    }

    bool writeScriptSubCommand(int lba, string value) {
        string input = "write " + std::to_string(lba) + " " + value;
        istringstream stream(input);
        string command;
        stream >> command;

        string result = m_testShell->handleWriteCommand(stream);

        if (result == "[Write] Done") return true;
        return false;
    };

    bool readCompare(int lba, string expected) {
        string input = "read " + std::to_string(lba);
        istringstream stream(input);
        string command;
        stream >> command;

        string result = m_testShell->handleReadCommand(stream).substr(16, 26);

        if (result == expected) return true;
        return false;
    };

    string handle1_FullWriteAndReadCompareCommand() {
        string inputs[INPUT_ARRAY_SIZE];

        for (int i = 0; i < INPUT_ARRAY_SIZE; i++) {
            inputs[i] = intToHexString(i);
            writeScriptSubCommand(i, inputs[i]);

            if (i % COMPARE_BATCH_SIZE != 4) continue;

            for (int j = 0; j < COMPARE_BATCH_SIZE; j++) {
                int idx = i - 4 + j;
                if (readCompare(idx, inputs[idx]) == false) {
                    return "FAIL";
                }
            }

        }
        return "PASS";
    }

    string handle2_PartialLBACommand() {
        string inputs[PARTIAL_LBA_LIMIT];

        for (int i = 0; i < PARTIAL_LBA_LIMIT; i++) {
            inputs[i] = intToHexString(i);
            writeScriptSubCommand(4, inputs[i]);
            writeScriptSubCommand(0, inputs[i]);
            writeScriptSubCommand(3, inputs[i]);
            writeScriptSubCommand(1, inputs[i]);
            writeScriptSubCommand(2, inputs[i]);

            for (int j = 0; j < COMPARE_BATCH_SIZE; j++) {
                int idx = j;
                if (readCompare(idx, inputs[i]) == false) {
                    return "FAIL";
                }
            }
        }
        return "PASS";
    }

    string handle3_WriteReadAgingCommand() {
        string inputs[200];

        for (int i = 0; i < PARTIAL_LBA_LIMIT; i++) {
            inputs[i] = unsignedIntToHexString(generateRandomHex());
            writeScriptSubCommand(0, inputs[i]);
            writeScriptSubCommand(99, inputs[i]);

            if (readCompare(0, inputs[i]) == false) return "FAIL";
            if (readCompare(99, inputs[i]) == false) return "FAIL";
        }
        return "PASS";
    }

    bool eraseRangeScriptSubCommand(int s, int e) {
        string input = "erase_range " + std::to_string(s) + " " + std::to_string(e);
        istringstream stream(input);
        string command;
        stream >> command;

        string result = m_testShell->handleEraseRangeCommand(stream);

        if (result == "[Erase] Done") return true;
        return false;
    };

    string handle4_EraseAndWriteAgingCommand() {

        string inputs[2];

        for (int i = 0; i < PARTIAL_LBA_LIMIT; i++) {

            for (int j = 2; j < 99; j += 2) {
                inputs[0] = unsignedIntToHexString(generateRandomHex());
                inputs[1] = unsignedIntToHexString(generateRandomHex());

                if (writeScriptSubCommand(j, inputs[0]) == false) return "FAIL";
                if (writeScriptSubCommand(j, inputs[1]) == false) return "FAIL";
                if (eraseRangeScriptSubCommand(j, (((j + 2) < INPUT_ARRAY_SIZE) ? (j + 2) : 99)) == false) return "FAIL";
            }
        }
        return "PASS";
    }

private:
    TestShell* m_testShell;
};