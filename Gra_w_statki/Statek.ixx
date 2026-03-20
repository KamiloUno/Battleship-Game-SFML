export module Statek;
import <SFML/Graphics.hpp>;
import Plansza;

export class Statek {
public:
    int dlugosc;
    bool pion;
    bool przeciagany = false;
    bool ustawiony = false;
    sf::Vector2f pozycjaStartowa;
    sf::RectangleShape ksztalt;
    sf::Vector2f offset;
    int gridX = -1, gridY = -1;

    Statek(int dl, sf::Vector2f pos, sf::Color kolor = sf::Color::White);
    void rysuj(sf::RenderWindow& win);
    bool czyKlikniety(const sf::RenderWindow& win, const sf::Event& ev);
    void zacznijPrzeciaganie(const sf::RenderWindow& win, const sf::Event& ev);
    void aktualizuj(const sf::RenderWindow& win);
    void zakonczPrzeciaganie(const sf::RenderWindow& win, const sf::Event& ev, std::vector<std::vector<int>>& plansza, sf::Vector2f offsetPlanszy);
    void obrot(std::vector<std::vector<int>>& plansza, sf::Vector2f offsetPlanszy);
};

constexpr int ROZMIAR_POLA = 50;

export
Statek::Statek(int dl, sf::Vector2f pos, sf::Color kolor)
    : dlugosc(dl), pion(false), pozycjaStartowa(pos) {
    ksztalt.setPosition(pos);
    ksztalt.setSize(sf::Vector2f(dl * ROZMIAR_POLA, ROZMIAR_POLA));
    ksztalt.setFillColor(sf::Color::White);
    ksztalt.setOutlineColor(kolor);
    ksztalt.setOutlineThickness(-5);
}

export
void Statek::rysuj(sf::RenderWindow& win) { win.draw(ksztalt); }

export
bool Statek::czyKlikniety(const sf::RenderWindow& win, const sf::Event& ev) {
    return ksztalt.getGlobalBounds().contains(win.mapPixelToCoords({ ev.mouseButton.x, ev.mouseButton.y }));
}

export
void Statek::zacznijPrzeciaganie(const sf::RenderWindow& win, const sf::Event& ev) {
    przeciagany = true;
    offset = ksztalt.getPosition() - win.mapPixelToCoords({ ev.mouseButton.x, ev.mouseButton.y });
}

export
void Statek::aktualizuj(const sf::RenderWindow& win) {
    if (przeciagany) {
        sf::Vector2i mouse = sf::Mouse::getPosition(win);
        ksztalt.setPosition(win.mapPixelToCoords(mouse) + offset);
    }
}

export
void Statek::zakonczPrzeciaganie(const sf::RenderWindow& win, const sf::Event& ev, std::vector<std::vector<int>>& plansza, sf::Vector2f offsetPlanszy) {
    przeciagany = false;
    int szer = pion ? 1 : dlugosc;
    int wys = pion ? dlugosc : 1;
    sf::Vector2f mouse = win.mapPixelToCoords({ ev.mouseButton.x, ev.mouseButton.y }) + offset;
    int col = int((mouse.x - offsetPlanszy.x) / ROZMIAR_POLA + 0.5f);
    int row = int((mouse.y - offsetPlanszy.y) / ROZMIAR_POLA + 0.5f);

    if (col < 0) col = 0;
    if (row < 0) row = 0;
    if (col + szer > Plansza::ILOSC_POL) col = Plansza::ILOSC_POL - szer;
    if (row + wys > Plansza::ILOSC_POL) row = Plansza::ILOSC_POL - wys;

    if (Plansza::czyNaPlanszy(col, row, szer, wys) && Plansza::czyMoznaUstawic(plansza, col, row, szer, wys)) {
        ksztalt.setPosition(offsetPlanszy.x + col * ROZMIAR_POLA, offsetPlanszy.y + row * ROZMIAR_POLA);
        ksztalt.setSize(sf::Vector2f(szer * ROZMIAR_POLA, wys * ROZMIAR_POLA));
        ustawiony = true;
        gridX = col; gridY = row;
        Plansza::ustawNaPlanszy(plansza, col, row, szer, wys);
    }
    else {
        pion = false;
        ksztalt.setRotation(0);
        ksztalt.setSize(sf::Vector2f(dlugosc * ROZMIAR_POLA, ROZMIAR_POLA));
        ksztalt.setPosition(pozycjaStartowa);
        ustawiony = false;
        gridX = gridY = -1;
    }
}

export
void Statek::obrot(std::vector<std::vector<int>>& plansza, sf::Vector2f offsetPlanszy) {
    if (ustawiony) Plansza::zdejmijZPlanszy(plansza, gridX, gridY, pion ? 1 : dlugosc, pion ? dlugosc : 1);
    bool nowyPion = !pion;
    int szer = nowyPion ? 1 : dlugosc;
    int wys = nowyPion ? dlugosc : 1;
    sf::Vector2f pos = ksztalt.getPosition();
    int col = int((pos.x - offsetPlanszy.x) / ROZMIAR_POLA + 0.5f);
    int row = int((pos.y - offsetPlanszy.y) / ROZMIAR_POLA + 0.5f);
    if (col < 0) col = 0;
    if (row < 0) row = 0;
    if (col + szer > Plansza::ILOSC_POL) col = Plansza::ILOSC_POL - szer;
    if (row + wys > Plansza::ILOSC_POL) row = Plansza::ILOSC_POL - wys;
    if (Plansza::czyNaPlanszy(col, row, szer, wys) && Plansza::czyMoznaUstawic(plansza, col, row, szer, wys)) {
        pion = nowyPion;
        ksztalt.setPosition(offsetPlanszy.x + col * ROZMIAR_POLA, offsetPlanszy.y + row * ROZMIAR_POLA);
        ksztalt.setSize(sf::Vector2f(szer * ROZMIAR_POLA, wys * ROZMIAR_POLA));
        gridX = col; gridY = row;
        if (ustawiony) Plansza::ustawNaPlanszy(plansza, col, row, szer, wys);
    }
    else {
        pion = false;
        ksztalt.setSize(sf::Vector2f(dlugosc * ROZMIAR_POLA, ROZMIAR_POLA));
        ksztalt.setPosition(pozycjaStartowa);
        ustawiony = false;
        gridX = gridY = -1;
    }
}