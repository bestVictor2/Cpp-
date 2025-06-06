#ifndef MENU_H
#define MENU_H
#include "card_manager.h"
#include "user_manager.h"
class Menu {
public:
    Menu();
    void run();
private:
    void init();
    void handleAddCard();
    void handleQueryCard();
    void handleLogin();
    void handleLogout();
    void handleRecharge();
    void handleRefund();
    void handleQueryStats();
    void handleDeleteCard();
    CardManager cardManager;
    UserManager userManager;
};
#endif
