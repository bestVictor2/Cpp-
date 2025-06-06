#include "user_manager.h"
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <cmath> 
std::time_t stringToTimeT(const std::string& str) {
    std::tm tm = {};
    std::istringstream ss(str);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    return std::mktime(&tm);
}
void UserManager::logout(int cardId, const std::string& password) {
    std::vector<Card> allCards = cardManager.loadCardsFromFile("cards.csv");
    bool found = false;
    for (auto& card : allCards) {
        if (card.getId() == cardId) {
            if (card.getPassword() != password) {
                std::cout << "密码错误，无法下机" << std::endl;
                return;
            }
            if (loggedInUsers.find(cardId) != loggedInUsers.end() && !loggedInUsers[cardId]) {
                std::cout << "该卡未登录" << std::endl;
                return;
            }
            std::time_t endTime = std::time(nullptr);
            std::string lastUsedStr = card.getLastUsed();
            std::time_t startTime = stringToTimeT(lastUsedStr);
            std::cout << "startTime: " << startTime << " endTime: " << endTime << std::endl;
            if (startTime == 0 || endTime < startTime) {
                std::cout << "登录时间异常，无法计费，下机失败" << std::endl;
                return;
            }
            double durationSeconds = std::difftime(endTime, startTime);
            if (durationSeconds <= 0 || durationSeconds > 86400) {
                std::cout << "下机时间异常（" << durationSeconds << "秒），请联系管理员" << std::endl;
                return;
            }
            double feePerSecond = 0.0083;
            double totalFee = durationSeconds * feePerSecond;
            if (card.getBalance() < totalFee) {
                std::cout << "余额不足，上机时间：" << durationSeconds
                          << " 秒，应扣 " << std::fixed << std::setprecision(2) << totalFee << " 元，下机失败。" << std::endl;
                return;
            }
            card.refund(totalFee);
            card.setStatus(Card::Status::OFFLINE);
            card.setLastUsed(endTime);
            loggedInUsers[cardId] = false;
            // 新增：写入上机记录
            std::ofstream recordFile("records.csv", std::ios::app);
            if (recordFile.is_open()) {
                recordFile << card.getId() << ","
                           << lastUsedStr << ","
                           << std::put_time(std::localtime(&endTime), "%Y-%m-%d %H:%M:%S") << ","
                           << totalFee << std::endl;
                recordFile.close();
            }
            std::ofstream file("cards.csv", std::ios::trunc);
            if (file.is_open()) {
                for (auto& cardItem : allCards) {
                    cardManager.writeCardToFile(cardItem, file);
                }
                file.close();
            } else {
                std::cout << "无法打开文件，无法写入信息" << std::endl;
            }
            std::cout << "下机成功，上机时长" << durationSeconds << "秒，扣费" << std::fixed << std::setprecision(2) << totalFee << "元，余额" << std::fixed << std::setprecision(2) << card.getBalance() << "元" << std::endl;
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "未找到该卡，下机失败" << std::endl;
        return;
    }
}
void UserManager::login(int cardId, const std::string& password) {
    std::vector<Card> allCards = cardManager.loadCardsFromFile("cards.csv");
    bool found = false;
    for (auto& card : allCards) {
        if (card.getId() == cardId) {
            if (card.getPassword() != password) {
                std::cout << "密码错误，无法上机" << std::endl;
                return;
            }
            if (loggedInUsers.find(cardId) != loggedInUsers.end() && loggedInUsers[cardId]) {
                std::cout << "该卡已登录" << std::endl;
                return;
            }
            if (card.getBalance() < 1.0) {
                std::cout << "余额不足，无法上机" << std::endl;
                return;
            }
            std::time_t currentTime = std::time(nullptr);
            card.setStatus(Card::Status::ONLINE);
            card.setLastUsed(currentTime);
            loggedInUsers[cardId] = true;
            found = true;
            std::cout << "上机成功" << std::endl;
            break;
        }
    }
    if (!found) {
        std::cout << "未找到该卡，上机失败" << std::endl;
        return;
    }
    std::ofstream file("cards.csv", std::ios::trunc);  
    if (file.is_open()) {
        for (const auto& card : allCards) {
            cardManager.writeCardToFile(card, file); 
        }
        file.close();
    } else {
        std::cout << "无法打开文件，无法写入信息" << std::endl;
    }
}
void UserManager::recharge(int id, double amount) {
    std::vector<Card> allCards = cardManager.loadCardsFromFile("cards.csv");
    bool found = false;
    for (auto& card : allCards) {
        if (card.getId() == id) {
            card.recharge(amount);
            card.setLastUsed(std::time(nullptr));
            found = true;
            std::cout << "充值成功，当前余额" << std::fixed << std::setprecision(2) << card.getBalance() << "元" << std::endl;
            // 新增：写入充值记录
            std::ofstream rechargeFile("recharge.csv", std::ios::app);
            if (rechargeFile.is_open()) {
                std::time_t now = std::time(nullptr);
                rechargeFile << id << "," << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") << "," << amount << std::endl;
                rechargeFile.close();
            }
            break;
        }
    }
    if (!found) {
        std::cout << "未找到该卡，充值失败" << std::endl;
        return;
    }
    std::ofstream file("cards.csv", std::ios::trunc);
    if (file.is_open()) {
        for (const auto& card : allCards) {
            cardManager.writeCardToFile(card, file);
        }
        file.close();
    } else {
        std::cout << "无法打开文件，无法写入充值信息" << std::endl;
    }
}
void UserManager::refund(int id, double amount) {
    std::vector<Card> allCards = cardManager.loadCardsFromFile("cards.csv");
    bool found = false;
    for (auto& card : allCards) {
        if (card.getId() == id) {
            if (card.getBalance() < amount) {
                std::cout << "退款金额大于余额，退款失败" << std::endl;
                return;
            }
            card.refund(amount);
            card.setLastUsed(std::time(nullptr));
            found = true;
            std::cout << "退款成功，当前余额" << std::fixed << std::setprecision(2) << card.getBalance() << "元" << std::endl;
            // 新增：写入退款记录
            std::ofstream refundFile("refund.csv", std::ios::app);
            if (refundFile.is_open()) {
                std::time_t now = std::time(nullptr);
                refundFile << id << "," << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S") << "," << amount << std::endl;
                refundFile.close();
            }
            break;
        }
    }
    if (!found) {
        std::cout << "未找到该卡，退款失败" << std::endl;
        return;
    }
    std::ofstream file("cards.csv", std::ios::trunc);  // 覆写文件
    if (file.is_open()) {
        for (const auto& card : allCards) {
            cardManager.writeCardToFile(card, file);
        }
        file.close();
    } else {
        std::cout << "无法打开文件，无法写入退款信息" << std::endl;
    }
}
bool UserManager::isLoggedIn(int cardId) const {
    auto it = loggedInUsers.find(cardId);
    return it != loggedInUsers.end() && it->second;
}
