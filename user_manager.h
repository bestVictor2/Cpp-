#ifndef USER_MANAGER_H
#define USER_MANAGER_H
#include <string>
#include <map>
#include "card.h"
#include "card_manager.h"
class UserManager {
public:
    void login(int cardId, const std::string& password);
    void logout(int cardId, const std::string& password);
    void recharge(int id, double amount);
    void refund(int id, double amount);
    bool isLoggedIn(int cardId) const;

private:
    std::map<int, bool> loggedInUsers;
    CardManager cardManager;
};
#endif
