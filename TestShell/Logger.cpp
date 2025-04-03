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
    // 싱글턴 인스턴스를 반환하는 메서드
    static Logger& getInstance() {
        static Logger instance;  // 최초 호출 시 인스턴스가 생성됨
        return instance;
    }

    // 로그를 기록하는 메서드
    void log(const std::string& function, const std::string& message) {
        std::string timestamp = getTimestamp();
        std::string formattedMessage = formatMessage(function, message);

        manageLogFileSize();

        std::ofstream logFile(logFileName, std::ios::app);
        if (logFile.is_open()) {
            // 타임스탬프와 포맷된 메시지를 로그 파일에 추가
            logFile << timestamp << " " << formattedMessage << std::endl;
            logFile.close();
        }
    }

private:
    std::string logFileName = "../TestShell/latest.log"; // 로그 파일 이름

    Logger() {
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string getTimestamp() {
        auto now = std::time(nullptr);
        std::tm tm;
        // localtime_s를 사용하여 안전하게 변환
        localtime_s(&tm, &now);

        std::ostringstream timestamp;
        timestamp << std::put_time(&tm, "[%y.%m.%d %H:%M:%S]");
        return timestamp.str();
    }

    std::string formatMessage(const std::string& function, const std::string& message) {
        std::ostringstream oss;
        oss << std::left << std::setw(30) << function << " : " << message;
        return oss.str();
    }

    void manageLogFileSize() {
        std::ifstream logFile(logFileName, std::ios::binary | std::ios::ate);

        if (logFile.is_open()) {
            std::streamsize size = logFile.tellg();
            bool needRotate = false;
            if (size > 2 * 1024) {  // 파일 크기가 1KB를 초과하면 (단위 수정 필요)
                needRotate = true;
            }

            logFile.close();
            if (needRotate == true)
            {
                rotateLogFile();
            }
        }
    }

    void rotateLogFile() {
        std::string timestamp = getTimestamp();

        std::string year = timestamp.substr(1, 2);  // 년도 두 자리를 추출 (예: 25)
        std::string month = timestamp.substr(4, 2); // 월 추출 (예: 04)
        std::string day = timestamp.substr(7, 2);   // 일 추출 (예: 03)
        std::string hour = timestamp.substr(11, 2);  // 시간 추출 (예: 17)
        std::string minute = timestamp.substr(14, 2); // 분 추출 (예: 12)
        std::string second = timestamp.substr(17, 2); // 초 추출 (예: 11)

        std::string newLogFileName = "../TestShell/until_" + year + month + day + "_" + hour + "h_" + minute + "m_" + second + "s.log";
        std::filesystem::rename(logFileName, newLogFileName);  // 기존 파일을 새 이름으로 변경

        compressLogFiles(newLogFileName);
    }

    // 로그 파일을 압축합니다. (예: gzip 압축)
    void compressLogFiles(const std::string& oldLogFileName) {
        // 실제 압축을 위한 코드 (zlib을 사용할 수 있습니다)
        std::string zipFileName = oldLogFileName + ".zip";
        std::ofstream zipFile(zipFileName, std::ios::binary);

        // 압축 로직 추가 (예: zlib 사용)
        std::cout << "Compressing log file: " << oldLogFileName << " to " << zipFileName << std::endl;
    }
};
