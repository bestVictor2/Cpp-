#include "menu.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <tuple>
#include <iomanip>

Menu::Menu() {}
void Menu::run() {
    int choice = -1;
    while (true) {
        init();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "请输入有效的数字" << std::endl;
            continue;
        }
        switch (choice) {
            case 1: handleAddCard(); break;
            case 2: handleQueryCard(); break;
            case 3: handleLogin(); break;
            case 4: handleLogout(); break;
            case 5: handleRecharge(); break;
            case 6: handleRefund(); break;
            case 7: handleQueryStats(); break;
            case 8: handleDeleteCard(); break;
            case 0:
                std::cout << "退出系统，感谢使用！" << std::endl;
                return;
            default:
                std::cout << "请输入有效选项（0-8）" << std::endl;
                break;
        }
    }
}
void Menu::init() {
    std::cout << "------菜单------" << '\n';
    std::cout << "1. 添加卡" << '\n';
    std::cout << "2. 查询卡" << '\n';
    std::cout << "3. 上机" << '\n';
    std::cout << "4. 下机" << '\n';
    std::cout << "5. 充值" << '\n';
    std::cout << "6. 退款" << '\n';
    std::cout << "7. 查询统计" << '\n';
    std::cout << "8. 注销卡" << '\n';
    std::cout << "0. 退出系统" << '\n';
    std::cout << "请输入对应的菜单选项" << std::endl;
}
void Menu::handleAddCard() {
    int id;
    std::string owner, password;
    std::cout << "请输入卡号：";
    std::cin >> id;
    std::cout << "请输入卡片持有人：";
    std::cin >> owner;
    std::cout << "请输入卡片密码：";
    std::cin >> password;
    cardManager.addCard(id, owner, password);
}
void Menu::handleQueryCard() {
    int cardId;
    std::cout << "请输入卡号进行查询：";
    std::cin >> cardId;
    cardManager.printCardById(cardId);
}
void Menu::handleLogin() {
    int id;
    std::string password;
    std::cout << "请输入卡号进行上机" << std::endl;
    std::cin >> id;
    std::cout << "请输入密码：";
    std::cin >> password;
    userManager.login(id, password);
}
void Menu::handleLogout() {
    int id;
    std::string password;
    std::cout << "请输入卡号进行下机" << std::endl;
    std::cin >> id;
    std::cout << "请输入密码：";
    std::cin >> password;
    userManager.logout(id, password);
}
void Menu::handleRecharge() {
    int id;
    double amount;
    std::cout << "请输入卡号和充值金额：" << std::endl;
    std::cin >> id >> amount;
    if (amount <= 0) {
        std::cout << "充值金额必须为正数！" << std::endl;
        return;
    }
    userManager.recharge(id, amount);
}
void Menu::handleRefund() {
    int id;
    double amount;
    std::cout << "请输入卡号和退款金额：" << std::endl;
    std::cin >> id >> amount;
    if (amount <= 0) {
        std::cout << "退款金额必须为正数！" << std::endl;
        return;
    }
    userManager.refund(id, amount);
}
void Menu::handleQueryStats() {
    while (true) {
        int statChoice;
        std::cout << "------统计菜单------\n";
        std::cout << "1. 统计每个卡号总消费\n";
        std::cout << "2. 统计某一天总消费\n";
        std::cout << "3. 统计每个卡号充值金额\n";
        std::cout << "4. 统计每个卡号退款金额\n";
        std::cout << "5. 统计某一天充值金额\n";
        std::cout << "6. 统计某一天退款金额\n";
        std::cout << "0. 返回主菜单\n";
        std::cout << "请输入统计选项：";
        std::cin >> statChoice;
        if (statChoice == 0) break;
        if (statChoice == 1) {
            // 统计每个卡号总消费，详细列出每笔消费
            std::ifstream recordFile("records.csv");
            std::map<int, std::vector<std::tuple<std::string, std::string, double>>> records;
            std::map<int, double> totalFee;
            std::string line;
            while (std::getline(recordFile, line)) {
                std::istringstream iss(line);
                std::string idStr, startStr, endStr, feeStr;
                std::getline(iss, idStr, ',');
                std::getline(iss, startStr, ',');
                std::getline(iss, endStr, ',');
                std::getline(iss, feeStr, ',');
                int id = std::stoi(idStr);
                double fee = std::stod(feeStr);
                records[id].emplace_back(startStr, endStr, fee);
                totalFee[id] += fee;
            }
            recordFile.close();
            for (const auto& [id, recs] : records) {
                std::cout << "卡号: " << id << std::endl;
                for (const auto& [start, end, fee] : recs) {
                    std::cout << "  上机时间: " << start << "  下机时间: " << end << "  消费: " << std::fixed << std::setprecision(2) << fee << " 元" << std::endl;
                }
                std::cout << "  总消费: " << std::fixed << std::setprecision(2) << totalFee[id] << " 元" << std::endl;
            }
        } else if (statChoice == 2) {
            // 统计某一天总消费，详细列出当天每个卡号的消费
            std::string date;
            std::cout << "请输入日期(格式:YYYY-MM-DD)：";
            std::cin >> date;
            std::ifstream recordFile("records.csv");
            std::map<int, std::vector<std::tuple<std::string, std::string, double>>> records;
            std::map<int, double> totalFee;
            double dayTotal = 0;
            std::string line;
            while (std::getline(recordFile, line)) {
                std::istringstream iss(line);
                std::string idStr, startStr, endStr, feeStr;
                std::getline(iss, idStr, ',');
                std::getline(iss, startStr, ',');
                std::getline(iss, endStr, ',');
                std::getline(iss, feeStr, ',');
                if (endStr.substr(0, 10) == date) {
                    int id = std::stoi(idStr);
                    double fee = std::stod(feeStr);
                    records[id].emplace_back(startStr, endStr, fee);
                    totalFee[id] += fee;
                    dayTotal += fee;
                }
            }
            recordFile.close();
            for (const auto& [id, recs] : records) {
                std::cout << "卡号: " << id << std::endl;
                for (const auto& [start, end, fee] : recs) {
                    std::cout << "  上机时间: " << start << "  下机时间: " << end << "  消费: " << std::fixed << std::setprecision(2) << fee << " 元" << std::endl;
                }
                std::cout << "  当天总消费: " << std::fixed << std::setprecision(2) << totalFee[id] << " 元" << std::endl;
            }
            std::cout << date << " 总消费: " << std::fixed << std::setprecision(2) << dayTotal << " 元" << std::endl;
        } else if (statChoice == 3) {
            // 统计每个卡号充值金额，详细列出每笔充值
            std::ifstream rechargeFile("recharge.csv");
            std::map<int, std::vector<std::pair<std::string, double>>> records;
            std::map<int, double> totalRecharge;
            std::string line;
            while (std::getline(rechargeFile, line)) {
                std::istringstream iss(line);
                std::string idStr, timeStr, amountStr;
                std::getline(iss, idStr, ',');
                std::getline(iss, timeStr, ',');
                std::getline(iss, amountStr, ',');
                int id = std::stoi(idStr);
                double amount = std::stod(amountStr);
                records[id].emplace_back(timeStr, amount);
                totalRecharge[id] += amount;
            }
            rechargeFile.close();
            for (const auto& [id, recs] : records) {
                std::cout << "卡号: " << id << std::endl;
                for (const auto& [time, amount] : recs) {
                    std::cout << "  充值时间: " << time << "  金额: " << std::fixed << std::setprecision(2) << amount << " 元" << std::endl;
                }
                std::cout << "  总充值: " << std::fixed << std::setprecision(2) << totalRecharge[id] << " 元" << std::endl;
            }
        } else if (statChoice == 4) {
            // 统计每个卡号退款金额，详细列出每笔退款
            std::ifstream refundFile("refund.csv");
            std::map<int, std::vector<std::pair<std::string, double>>> records;
            std::map<int, double> totalRefund;
            std::string line;
            while (std::getline(refundFile, line)) {
                std::istringstream iss(line);
                std::string idStr, timeStr, amountStr;
                std::getline(iss, idStr, ',');
                std::getline(iss, timeStr, ',');
                std::getline(iss, amountStr, ',');
                int id = std::stoi(idStr);
                double amount = std::stod(amountStr);
                records[id].emplace_back(timeStr, amount);
                totalRefund[id] += amount;
            }
            refundFile.close();
            for (const auto& [id, recs] : records) {
                std::cout << "卡号: " << id << std::endl;
                for (const auto& [time, amount] : recs) {
                    std::cout << "  退款时间: " << time << "  金额: " << std::fixed << std::setprecision(2) << amount << " 元" << std::endl;
                }
                std::cout << "  总退款: " << std::fixed << std::setprecision(2) << totalRefund[id] << " 元" << std::endl;
            }
        } else if (statChoice == 5) {
            // 统计某一天充值金额，详细列出当天每个卡号的充值
            std::string date;
            std::cout << "请输入日期(格式:YYYY-MM-DD)：";
            std::cin >> date;
            std::ifstream rechargeFile("recharge.csv");
            std::map<int, std::vector<std::pair<std::string, double>>> records;
            std::map<int, double> totalRecharge;
            double dayTotal = 0;
            std::string line;
            while (std::getline(rechargeFile, line)) {
                std::istringstream iss(line);
                std::string idStr, timeStr, amountStr;
                std::getline(iss, idStr, ',');
                std::getline(iss, timeStr, ',');
                std::getline(iss, amountStr, ',');
                if (timeStr.substr(0, 10) == date) {
                    int id = std::stoi(idStr);
                    double amount = std::stod(amountStr);
                    records[id].emplace_back(timeStr, amount);
                    totalRecharge[id] += amount;
                    dayTotal += amount;
                }
            }
            rechargeFile.close();
            for (const auto& [id, recs] : records) {
                std::cout << "卡号: " << id << std::endl;
                for (const auto& [time, amount] : recs) {
                    std::cout << "  充值时间: " << time << "  金额: " << std::fixed << std::setprecision(2) << amount << " 元" << std::endl;
                }
                std::cout << "  当天总充值: " << std::fixed << std::setprecision(2) << totalRecharge[id] << " 元" << std::endl;
            }
            std::cout << date << " 总充值: " << std::fixed << std::setprecision(2) << dayTotal << " 元" << std::endl;
        } else if (statChoice == 6) {
            // 统计某一天退款金额，详细列出当天每个卡号的退款
            std::string date;
            std::cout << "请输入日期(格式:YYYY-MM-DD)：";
            std::cin >> date;
            std::ifstream refundFile("refund.csv");
            std::map<int, std::vector<std::pair<std::string, double>>> records;
            std::map<int, double> totalRefund;
            double dayTotal = 0;
            std::string line;
            while (std::getline(refundFile, line)) {
                std::istringstream iss(line);
                std::string idStr, timeStr, amountStr;
                std::getline(iss, idStr, ',');
                std::getline(iss, timeStr, ',');
                std::getline(iss, amountStr, ',');
                if (timeStr.substr(0, 10) == date) {
                    int id = std::stoi(idStr);
                    double amount = std::stod(amountStr);
                    records[id].emplace_back(timeStr, amount);
                    totalRefund[id] += amount;
                    dayTotal += amount;
                }
            }
            refundFile.close();
            for (const auto& [id, recs] : records) {
                std::cout << "卡号: " << id << std::endl;
                for (const auto& [time, amount] : recs) {
                    std::cout << "  退款时间: " << time << "  金额: " << std::fixed << std::setprecision(2) << amount << " 元" << std::endl;
                }
                std::cout << "  当天总退款: " << std::fixed << std::setprecision(2) << totalRefund[id] << " 元" << std::endl;
            }
            std::cout << date << " 总退款: " << std::fixed << std::setprecision(2) << dayTotal << " 元" << std::endl;
        } else {
            std::cout << "无效的统计选项。" << std::endl;
        }
        std::cout << std::endl;
    }
}
void Menu::handleDeleteCard() {
    int id;
    std::cout << "请输入卡号进行注销：" << std::endl;
    std::cin >> id;
    cardManager.deleteCard(id);
}
