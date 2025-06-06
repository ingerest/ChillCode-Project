#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <filesystem>

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(const std::string& function, const std::string& message) {
        std::string timestamp = getTimestamp();
        std::string formattedMessage = formatMessage(function, message);

        manageFileSize();

        std::ofstream logFile(logFileName, std::ios::app);
        if (!logFile.is_open()) {
            return;
        }

        logFile << timestamp << " " << formattedMessage << std::endl;
    }

    void manageFileSize() {
        std::ifstream infile(logFileName, std::ios::binary);
        if (!infile.is_open()) {
            return;
        }

        infile.seekg(0, std::ios::end);
        std::streampos fileSize = infile.tellg();
        infile.close();

        if (fileSize >= maxLogFileSize) {
            rotateLogFile();
        }
    }

private:
    const std::string logFileName = "../Release/Log/latest.log";
    const std::string logDir = "../Release/Log/";
    const std::size_t maxLogFileSize = 10 * 1024; // 10KB

    Logger() {
        createLogDirectoryIfNotExists();
        clearLogDirectory();
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void clearLogDirectory() {
        std::filesystem::path logPath(logDir);

        if (!std::filesystem::exists(logPath) || !std::filesystem::is_directory(logPath)) {
            return;
        }

        for (const auto& entry : std::filesystem::directory_iterator(logPath)) {
            if (entry.is_regular_file()) {
                std::filesystem::remove(entry.path());
            }
        }
    }

    void createLogDirectoryIfNotExists() {
        std::filesystem::path logPath(logDir);
        if (!std::filesystem::exists(logPath)) {
            std::filesystem::create_directories(logPath);
        }
    }

    std::string getTimestamp() {
        auto now = std::time(nullptr);
        std::tm tm;
        localtime_s(&tm, &now);

        std::ostringstream timestamp;
        timestamp << std::put_time(&tm, "[%y.%m.%d %H:%M:%S]");
        return timestamp.str();
    }

    std::string formatMessage(const std::string& function, const std::string& message) {
        std::ostringstream oss;
        oss << std::left << std::setw(40) << function << " : " << message;
        return oss.str();
    }

    void rotateLogFile() {
        std::string timestamp = getTimestamp();

        std::string year = timestamp.substr(1, 2);
        std::string month = timestamp.substr(4, 2);
        std::string day = timestamp.substr(7, 2);
        std::string hour = timestamp.substr(10, 2);
        std::string minute = timestamp.substr(13, 2);
        std::string second = timestamp.substr(16, 2);

        std::string oldLogFileName = logDir + "until_" + year + month + day + "_" + hour + "h_" + minute + "m_" + second + "s.log";
        std::filesystem::rename(logFileName, oldLogFileName);

        compressOldestLogFile();
    }

    void compressOldestLogFile() {
        std::string oldLogFileName = findOldestLogFile(logDir);
        if (oldLogFileName.empty()) return;

        std::string zipFileName = oldLogFileName;
        zipFileName.replace(zipFileName.find(".log"), 4, ".zip");

        std::filesystem::rename(logDir + oldLogFileName, logDir + zipFileName);
    }

    long long parseLogFileNameToNumber(const std::string& filename) {
        int month, day, hour, minute, second;
        char unused;

        std::istringstream iss(filename);
        iss.ignore(5);
        iss >> month >> day >> unused >> hour >> unused >> minute >> unused >> second;

        return static_cast<long long>(month) * 10000000000LL +
            static_cast<long long>(day) * 100000000LL +
            static_cast<long long>(hour) * 1000000LL +
            static_cast<long long>(minute) * 10000LL +
            static_cast<long long>(second);
    }

    std::string findOldestLogFile(const std::string& folderPath) {
        std::filesystem::path folder(folderPath);

        if (!std::filesystem::exists(folder) || !std::filesystem::is_directory(folder)) {
            return "";
        }

        std::string oldestFile;
        long long oldestTimestamp = LLONG_MAX;

        for (const auto& entry : std::filesystem::directory_iterator(folder)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();

                if (filename.find("until_") == 0 && filename.find(".log") != std::string::npos) {
                    long long fileTimestamp = parseLogFileNameToNumber(filename);

                    if (fileTimestamp < oldestTimestamp) {
                        oldestFile = filename;
                        oldestTimestamp = fileTimestamp;
                    }
                }
            }
        }

        return oldestFile;
    }
};
