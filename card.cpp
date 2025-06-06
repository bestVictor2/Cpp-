#include "card.h"
#include <iomanip>   // for std::put_time
#include <sstream>   // for std::ostringstream
#include <ctime>     // for std::localtime
// 实现Card类的成员函数
Card::Card(int id, const std::string& owner, const std::string& password): id(id), owner(owner), password(password), status(Status::OFFLINE), balance(0.0), lastUsed(0) {
    setLastUsed(std::time(nullptr));
}
Card::Card(int id, const std::string& owner, const std::string& password, Status status, double balance, long long lastUsed)
    : id(id), owner(owner), password(password), status(status), balance(balance), lastUsed(lastUsed) {}
int Card::getId() const { return id; }
std::string Card::getOwner() const { return owner; }
std::string Card::getPassword() const { return password; }
Card::Status Card::getStatus() const { return status; }
double Card::getBalance() const { return balance; }
std::string Card::getLastUsed() const {
    std::time_t t = static_cast<std::time_t>(lastUsed);
    std::tm* tmPtr = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S");  
    return oss.str();
}
long long Card::getRawTimestamp() const { return lastUsed; }
void Card::setLastUsed(long long t) {
    lastUsed = t; 
}
void Card::setStatus(Status s) { status = s; }
void Card::setBalance(double b) { balance = b; }
void Card::recharge(double amount) { balance += amount; }
void Card::refund(double amount) { balance -= amount; }
