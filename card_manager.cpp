#include "card_manager.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
void CardManager::addCard(int id, const std::string& owner, const std::string& password) {
    Card newCard(id, owner, password);
    std::ofstream file("cards.csv", std::ios::app);  
    if (file.is_open()) {
        writeCardToFile(newCard, file);
        file.close();
    } else {
        std::cout << "无法打开文件，无法保存！" << std::endl;
    }
}
void CardManager::deleteCard(int id) {
    std::vector<Card> allCards = loadCardsFromFile("cards.csv"); 
    std::ofstream file("cards.csv", std::ios::trunc);  
    if (file.is_open()) {
        for (const auto& card : allCards) {
            if (card.getId() != id) {
                writeCardToFile(card, file);  
            }
        }
        file.close();
    } else {
        std::cout << "无法打开文件，无法删除！" << std::endl;
    }
}
void CardManager::printCardById(int id) {
    std::vector<Card> allCards = loadCardsFromFile("cards.csv");
    for (const auto& card : allCards) {
        if (card.getId() == id) {
            std::cout << "卡号: " << card.getId()
                      << ", 持卡人: " << card.getOwner()
                      << ", 状态: " << static_cast<int>(card.getStatus()) 
                      << ", 余额: " << std::fixed << std::setprecision(2) << card.getBalance() 
                      << ", 最后使用时间: " << card.getLastUsed() << std::endl;
            return;
        }
    }
    std::cout << "未找到指定的卡片" << std::endl;
}
std::vector<Card> CardManager::loadCardsFromFile(const std::string& filename) {
    std::vector<Card> cards;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "无法打开文件，无法读取" << std::endl;
        return cards;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::string idstr,owner, password, statusStr, balanceStr, lastUsedStr;
        int id;
        std::getline(stream, idstr, ',');
        std::getline(stream, owner, ',');
        std::getline(stream, password, ',');
        std::getline(stream, statusStr, ',');
        std::getline(stream, balanceStr, ',');
        std::getline(stream, lastUsedStr, ',');
        id = std::stoi(idstr);  
        Card::Status status = static_cast<Card::Status>(std::stoi(statusStr));
        double balance = std::stod(balanceStr);
        long long lastUsed = std::stoll(lastUsedStr);
        Card card(id, owner, password, status, balance, lastUsed);
        card.setStatus(status);
        card.setBalance(balance);
        card.setLastUsed(lastUsed);
        cards.push_back(card);
    }
    file.close();
    return cards;
}
void CardManager::writeCardToFile(const Card& card, std::ofstream& file) {
    file << card.getId() << ","
        << card.getOwner() << ","
        << card.getPassword() << ","
        << static_cast<int>(card.getStatus()) << ","
        << card.getBalance() << ","
        << card.getRawTimestamp() << "\n";
}
Card* CardManager::findCardById(int id) {
    auto allCards = loadCardsFromFile("cards.csv");
    for (auto& card : allCards) {
        if (card.getId() == id) {
            return &card;
        }
    }
    return nullptr;
}
