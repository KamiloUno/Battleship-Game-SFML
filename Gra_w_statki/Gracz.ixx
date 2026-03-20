export module Gracz;
import <vector>;
import <ranges>;
import <SFML/Graphics.hpp>;
import Plansza;
import Statek;
import PozycjaStatku;
import Strzaly;

export class Gracz {
public:
    Plansza plansza;
    Strzaly strzaly;
    std::vector<Statek> statki;
    std::vector<PozycjaStatku> pozycjeStatkow;

    Gracz(sf::Color kolor);
    std::vector<Statek> utworzStatki(sf::Color kolor);
    void buildShipListFromStatki();
    bool czyWszystkieUstawione();
    bool czyJakikolwiekUstawiony();
};

export
Gracz::Gracz(sf::Color kolor) {
    statki = utworzStatki(kolor);
}

export
std::vector<Statek> Gracz::utworzStatki(sf::Color kolor) {
    std::vector<Statek> s;
    s.push_back(Statek(4, sf::Vector2f(100, 200), kolor));
    s.push_back(Statek(3, sf::Vector2f(100, 300), kolor));
    s.push_back(Statek(3, sf::Vector2f(100, 300), kolor));
    s.push_back(Statek(2, sf::Vector2f(100, 400), kolor));
    s.push_back(Statek(2, sf::Vector2f(100, 400), kolor));
    s.push_back(Statek(2, sf::Vector2f(100, 400), kolor));
    s.push_back(Statek(1, sf::Vector2f(100, 500), kolor));
    s.push_back(Statek(1, sf::Vector2f(100, 500), kolor));
    s.push_back(Statek(1, sf::Vector2f(100, 500), kolor));
    s.push_back(Statek(1, sf::Vector2f(100, 500), kolor));
    return s;
}

export
void Gracz::buildShipListFromStatki() {
    pozycjeStatkow.clear();
    auto ustawione = statki | std::views::filter([](const Statek& s) { return s.ustawiony; });
    for (const auto& s : ustawione) {
        PozycjaStatku ps;
        ps.x = s.gridX;
        ps.y = s.gridY;
        ps.len = s.dlugosc;
        ps.vert = s.pion;
        pozycjeStatkow.push_back(ps);
    }
}

// ========= RANGES ==========

export
bool Gracz::czyWszystkieUstawione() {
    return std::ranges::all_of(statki, [](const Statek& s) { return s.ustawiony; });
}

export
bool Gracz::czyJakikolwiekUstawiony() {
    return std::ranges::any_of(statki, [](const Statek& s) { return s.ustawiony; });
}

