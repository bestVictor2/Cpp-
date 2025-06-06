#include <iostream>
#include "menu.h"
#include "card.h"
#include "card_manager.h"
#include "user_manager.h"
#include <windows.h>
int main(){
    SetConsoleOutputCP(65001);
    Menu me;
    me.run();    
    return 0;
}