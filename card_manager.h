#ifndef CARD_MANAGER_H
#define CARD_MANAGER_H
#include "card.h"
#include <vector>
#include <fstream>
class CardManager {
public:
    void addCard(int id, const std::string& owner, const std::string& password);
    void deleteCard(int id);
    void printCardById(int id);
    Card* findCardById(int id);
    std::vector<Card> loadCardsFromFile(const std::string& filename);
    void writeCardToFile(const Card& card, std::ofstream& file);
private:
    std::vector<Card> cards;
};
#endif
