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
    // �̱��� �ν��Ͻ��� ��ȯ�ϴ� �޼���
    static Logger& getInstance() {
        static Logger instance;  // ���� ȣ�� �� �ν��Ͻ��� ������
        return instance;
    }

    // �α׸� ����ϴ� �޼���
    void log(const std::string& function, const std::string& message) {
        std::string timestamp = getTimestamp();
        std::string formattedMessage = formatMessage(function, message);

        manageLogFileSize();

        std::ofstream logFile(logFileName, std::ios::app);
        if (logFile.is_open()) {
            // Ÿ�ӽ������� ���˵� �޽����� �α� ���Ͽ� �߰�
            logFile << timestamp << " " << formattedMessage << std::endl;
            logFile.close();
        }
    }

private:
    std::string logFileName = "../TestShell/latest.log"; // �α� ���� �̸�

    Logger() {
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string getTimestamp() {
        auto now = std::time(nullptr);
        std::tm tm;
        // localtime_s�� ����Ͽ� �����ϰ� ��ȯ
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
            if (size > 2 * 1024) {  // ���� ũ�Ⱑ 1KB�� �ʰ��ϸ� (���� ���� �ʿ�)
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

        std::string year = timestamp.substr(1, 2);  // �⵵ �� �ڸ��� ���� (��: 25)
        std::string month = timestamp.substr(4, 2); // �� ���� (��: 04)
        std::string day = timestamp.substr(7, 2);   // �� ���� (��: 03)
        std::string hour = timestamp.substr(11, 2);  // �ð� ���� (��: 17)
        std::string minute = timestamp.substr(14, 2); // �� ���� (��: 12)
        std::string second = timestamp.substr(17, 2); // �� ���� (��: 11)

        std::string newLogFileName = "../TestShell/until_" + year + month + day + "_" + hour + "h_" + minute + "m_" + second + "s.log";
        std::filesystem::rename(logFileName, newLogFileName);  // ���� ������ �� �̸����� ����

        compressLogFiles(newLogFileName);
    }

    // �α� ������ �����մϴ�. (��: gzip ����)
    void compressLogFiles(const std::string& oldLogFileName) {
        // ���� ������ ���� �ڵ� (zlib�� ����� �� �ֽ��ϴ�)
        std::string zipFileName = oldLogFileName + ".zip";
        std::ofstream zipFile(zipFileName, std::ios::binary);

        // ���� ���� �߰� (��: zlib ���)
        std::cout << "Compressing log file: " << oldLogFileName << " to " << zipFileName << std::endl;
    }
};
