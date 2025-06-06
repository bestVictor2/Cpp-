#ifndef CARD_H
#define CARD_H
#include <string>
#include <ctime>
class Card {
public:
    enum class Status { ONLINE, OFFLINE };
    Card() = default;  
    Card(int id, const std::string& owner, const std::string& password);
    Card(int id, const std::string& owner, const std::string& password, Status status, double balance, long long lastUsed);  // ���ļ���ȡ��
    int getId() const;
    std::string getOwner() const;
    std::string getPassword() const;
    Status getStatus() const;
    double getBalance() const;
    std::string getLastUsed() const;
    long long getRawTimestamp() const ;
    void setStatus(Status status);
    void setBalance(double balance);
    void setLastUsed(long long timestamp);
    void recharge(double amount);
    void refund(double amount);
private:
    int id;
    std::string owner;
    std::string password;
    Status status;
    double balance;
    long long lastUsed;
};
#endif