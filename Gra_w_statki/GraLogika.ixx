export module GraLogika;
import GraWyglad;
import <SFML/Graphics.hpp>;
import <string>;
import <vector>;
import <filesystem>;
import <fstream>;
import <iostream>;
import Plansza;
import Statek;
import Gracz;
import PozycjaStatku;
import Strzaly;

namespace fs = std::filesystem;

export enum class StanGry { MENU, USTAWIANIE1, USTAWIANIE2, GRA, ZMIANA_TURY, KONIEC };

export class GraLogika {
    GraWyglad wyglad;
    StanGry stan = StanGry::MENU;
    Gracz gracz1, gracz2;
    Statek* przeciagany = nullptr;
    int tura = 1;
    bool czyWygrana = false;
    int zwyciezca = 0;
    bool pokazZatopiony = false;
    float czasPokazZatopiony = 0.0f;
    bool czekajNaZakonczRuch = false;
    bool graRozpoczeta = false;
    bool pokazNapisObrot1 = true;
    bool pokazNapisObrot2 = true;

    void rysujPlansze(const std::vector<std::vector<int>>& plansza, const std::vector<std::vector<int>>& strzaly,
        sf::Vector2f offset, bool pokazStatki = true, bool pokazX = true);
    bool czyWszystkoZatopione(const std::vector<PozycjaStatku>& ships, const std::vector<std::vector<int>>& strzaly);
    bool czyZatopionyShip(const PozycjaStatku& ship, const std::vector<std::vector<int>>& strzaly);
    void oznaczPudlaWokolStatku(const PozycjaStatku& ship, std::vector<std::vector<int>>& strzaly, const std::vector<std::vector<int>>& plansza);
    void flashTransition(StanGry nowyStan);

public:
    GraLogika();
    void uruchom();
    void zapiszZwyciezceDoPliku(int zwyciezca);
};

export
GraLogika::GraLogika()
    : gracz1(sf::Color::Blue), gracz2(sf::Color::Red)
{
    if (!wyglad.wczytanieCzcionki("JerseyLetters.ttf")) exit(1);
    if (!wyglad.wczytanieTextury("Start.jpg")) exit(1);
    wyglad.ustawTeksty();
}

// ================ Filesystem ================

export
void GraLogika::zapiszZwyciezceDoPliku(int zwyciezca) {
    try {
        fs::path sciezka = "ostatni_zwyciezca.txt";
        std::ofstream plik(sciezka);
        if (!plik) {
            std::cerr << "Nie mozna utworzyc pliku z wynikiem gry\n";
            return;
        }
        if (zwyciezca == 1) {
            plik << "Zwyciezca: GRACZ 1\n";
        }
        else if (zwyciezca == 2) {
            plik << "Zwyciezca: GRACZ 2\n";
        }
        else {
            plik << "Brak zwyciezcy.\n";
        }
        plik.close();
        std::cout << "Wynik gry zapisano do: " << sciezka << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Wyjatek podczas zapisu pliku: " << e.what() << std::endl;
    }
}

// ================================================

export
void GraLogika::rysujPlansze(const std::vector<std::vector<int>>& plansza, const std::vector<std::vector<int>>& strzaly,
    sf::Vector2f offset, bool pokazStatki, bool pokazX) {

    sf::RectangleShape tloPlanszy(sf::Vector2f(Plansza::ILOSC_POL * 50, Plansza::ILOSC_POL * 50));
    tloPlanszy.setPosition(offset.x, offset.y);
    tloPlanszy.setFillColor(sf::Color(120, 170, 255));
    wyglad.window.draw(tloPlanszy);

    sf::RectangleShape pole(sf::Vector2f(48, 48));
    sf::Color statkiCol = sf::Color(100, 100, 100);
    for (int x = 0; x < Plansza::ILOSC_POL; ++x)
        for (int y = 0; y < Plansza::ILOSC_POL; ++y) {
            int wartosc = plansza[x][y];
            pole.setFillColor(wartosc == 0 ? sf::Color::White : (pokazStatki ? statkiCol : sf::Color::White));
            pole.setPosition(offset.x + x * 50 + 1, offset.y + y * 50 + 1);
            wyglad.window.draw(pole);
            if (pokazX && strzaly[x][y] == 1) {
                sf::CircleShape miss(10);
                miss.setFillColor(sf::Color(180, 180, 180));
                miss.setOrigin(10, 10);
                miss.setPosition(offset.x + x * 50 + 25, offset.y + y * 50 + 25);
                wyglad.window.draw(miss);
            }
            else if (pokazX && strzaly[x][y] == 2) {
                sf::RectangleShape hit(sf::Vector2f(32, 6));
                hit.setFillColor(sf::Color::Red);
                hit.setOrigin(16, 3);
                hit.setPosition(offset.x + x * 50 + 25, offset.y + y * 50 + 25);
                hit.setRotation(45);
                wyglad.window.draw(hit);
                hit.setRotation(-45);
                wyglad.window.draw(hit);
            }
        }
}

export
bool GraLogika::czyWszystkoZatopione(const std::vector<PozycjaStatku>& ships, const std::vector<std::vector<int>>& strzaly) {
    for (const auto& ship : ships) {
        for (int i = 0; i < ship.len; ++i) {
            int x = ship.x + (ship.vert ? 0 : i);
            int y = ship.y + (ship.vert ? i : 0);
            if (strzaly[x][y] != 2) return false;
        }
    }
    return true;
}

export
bool GraLogika::czyZatopionyShip(const PozycjaStatku& ship, const std::vector<std::vector<int>>& strzaly) {
    for (int i = 0; i < ship.len; ++i) {
        int x = ship.x + (ship.vert ? 0 : i);
        int y = ship.y + (ship.vert ? i : 0);
        if (strzaly[x][y] != 2) return false;
    }
    return true;
}

export
void GraLogika::oznaczPudlaWokolStatku(const PozycjaStatku& ship, std::vector<std::vector<int>>& strzaly, const std::vector<std::vector<int>>& plansza) {
    int x0 = ship.x - 1;
    int y0 = ship.y - 1;
    int x1 = ship.x + (ship.vert ? 1 : ship.len);
    int y1 = ship.y + (ship.vert ? ship.len : 1);

    for (int x = x0; x <= x1; ++x) {
        for (int y = y0; y <= y1; ++y) {
            if (x >= 0 && x < Plansza::ILOSC_POL && y >= 0 && y < Plansza::ILOSC_POL) {
                bool czyCzescStatku = false;
                for (int i = 0; i < ship.len; ++i) {
                    int sx = ship.x + (ship.vert ? 0 : i);
                    int sy = ship.y + (ship.vert ? i : 0);
                    if (x == sx && y == sy) {
                        czyCzescStatku = true;
                        break;
                    }
                }
                if (!czyCzescStatku && strzaly[x][y] == 0) {
                    strzaly[x][y] = 1;
                }
            }
        }
    }
}

export
void GraLogika::flashTransition(StanGry nowyStan) {
    sf::Clock lokalnyClock;
    sf::RectangleShape flash(sf::Vector2f(1280, 720));
    flash.setFillColor(sf::Color(255, 255, 255, 0));

    enum Faza { FADE_IN, HOLD, FADE_OUT };
    Faza faza = FADE_IN;

    while (wyglad.window.isOpen()) {
        float czas = lokalnyClock.getElapsedTime().asSeconds();
        sf::Event event;
        while (wyglad.window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                wyglad.window.close();
        }

        if (faza == FADE_IN) {
            int alpha = static_cast<int>(czas * 255 * 4);
            if (alpha >= 255) {
                alpha = 255;
                lokalnyClock.restart();
                faza = HOLD;
            }
            flash.setFillColor(sf::Color(255, 255, 255, alpha));
        }
        else if (faza == HOLD) {
            if (czas >= 0.4f) {
                lokalnyClock.restart();
                faza = FADE_OUT;
                stan = nowyStan;
            }
        }
        else if (faza == FADE_OUT) {
            int alpha = 255 - static_cast<int>(czas * 255 * 2);
            if (alpha <= 0) break;
            flash.setFillColor(sf::Color(255, 255, 255, alpha));
        }

        wyglad.window.clear();
        wyglad.window.draw(wyglad.tlo);
        wyglad.window.draw(flash);
        wyglad.window.display();
    }
}

export
void GraLogika::uruchom() {
    std::vector<bool> zatopione1, zatopione2;
    sf::Clock clock;
    bool zapisanoZwyciezce = false;
    while (wyglad.window.isOpen()) {
        float delta = clock.restart().asSeconds();
        if (pokazZatopiony) {
            czasPokazZatopiony -= delta;
            if (czasPokazZatopiony <= 0.f) {
                pokazZatopiony = false;
            }
        }
        sf::Event ev;
        while (wyglad.window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) wyglad.window.close();

            switch (stan) {
            case StanGry::MENU:
                if ((ev.type == sf::Event::KeyPressed || ev.type == sf::Event::MouseButtonPressed)) {
                    flashTransition(StanGry::USTAWIANIE1);
                }
                break;

            case StanGry::USTAWIANIE1:
            case StanGry::USTAWIANIE2: {
                Gracz& gracz = (stan == StanGry::USTAWIANIE1) ? gracz1 : gracz2;
                std::vector<std::vector<int>>& plansza = gracz.plansza.pola;

                if (stan == StanGry::USTAWIANIE1 && pokazNapisObrot1 && gracz1.czyJakikolwiekUstawiony()) {
                    pokazNapisObrot1 = false;
                }
                if (stan == StanGry::USTAWIANIE2 && pokazNapisObrot2 && gracz2.czyJakikolwiekUstawiony()) {
                    pokazNapisObrot2 = false;
                }

                if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                    for (auto it = gracz.statki.rbegin(); it != gracz.statki.rend(); ++it) {
                        if (it->czyKlikniety(wyglad.window, ev)) {
                            if (it->ustawiony) Plansza::zdejmijZPlanszy(plansza, it->gridX, it->gridY, it->pion ? 1 : it->dlugosc, it->pion ? it->dlugosc : 1);
                            it->ustawiony = false;
                            przeciagany = &(*it);
                            przeciagany->zacznijPrzeciaganie(wyglad.window, ev);
                            break;
                        }
                    }
                    if (stan == StanGry::USTAWIANIE1 && gracz1.czyWszystkieUstawione()) {
                        sf::Vector2i mouse = sf::Mouse::getPosition(wyglad.window);
                        if (wyglad.przyciskStartuGracza2.getGlobalBounds().contains(mouse.x, mouse.y)) {
                            flashTransition(StanGry::USTAWIANIE2);
                        }
                    }
                    else if (stan == StanGry::USTAWIANIE2 && gracz2.czyWszystkieUstawione()) {
                        sf::Vector2i mouse = sf::Mouse::getPosition(wyglad.window);
                        if (wyglad.przyciskStartuGry.getGlobalBounds().contains(mouse.x, mouse.y)) {
                            flashTransition(StanGry::GRA);
                            graRozpoczeta = true;
                        }
                    }
                }
                if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Right) {
                    if (przeciagany) {
                        przeciagany->obrot(plansza, wyglad.offsetPlanszy);
                    }
                }
                if (ev.type == sf::Event::MouseButtonReleased && ev.mouseButton.button == sf::Mouse::Left) {
                    if (przeciagany) {
                        przeciagany->zakonczPrzeciaganie(wyglad.window, ev, plansza, wyglad.offsetPlanszy);
                        przeciagany = nullptr;
                    }
                }
                break;
            }

            case StanGry::GRA:
                if (!czyWygrana && !pokazZatopiony) {
                    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mouse = sf::Mouse::getPosition(wyglad.window);
                        sf::Vector2f mpos = wyglad.window.mapPixelToCoords(mouse);
                        int col = int((mpos.x - wyglad.offsetPlanszaP.x) / 50);
                        int row = int((mpos.y - wyglad.offsetPlanszaP.y) / 50);

                        if (!czekajNaZakonczRuch && col >= 0 && col < Plansza::ILOSC_POL && row >= 0 && row < Plansza::ILOSC_POL) {
                            if (tura == 1 && gracz1.strzaly.poleStrzalow[col][row] == 0) {
                                if (gracz2.plansza.pola[col][row]) {
                                    gracz1.strzaly.poleStrzalow[col][row] = 2;
                                    bool nowyZatopiony = false;
                                    for (size_t i = 0; i < gracz2.pozycjeStatkow.size(); ++i) {
                                        if (!zatopione2[i] && czyZatopionyShip(gracz2.pozycjeStatkow[i], gracz1.strzaly.poleStrzalow)) {
                                            zatopione2[i] = true;
                                            oznaczPudlaWokolStatku(gracz2.pozycjeStatkow[i], gracz1.strzaly.poleStrzalow, gracz2.plansza.pola);
                                            nowyZatopiony = true;
                                        }
                                    }
                                    if (nowyZatopiony) {
                                        pokazZatopiony = true;
                                        czasPokazZatopiony = 1.2f;
                                    }
                                    if (czyWszystkoZatopione(gracz2.pozycjeStatkow, gracz1.strzaly.poleStrzalow)) {
                                        czyWygrana = true;
                                        zwyciezca = 1;
                                        flashTransition(StanGry::KONIEC);
                                    }
                                }
                                else {
                                    gracz1.strzaly.poleStrzalow[col][row] = 1;
                                    czekajNaZakonczRuch = true;
                                }
                            }
                            else if (tura == 2 && gracz2.strzaly.poleStrzalow[col][row] == 0) {
                                if (gracz1.plansza.pola[col][row]) {
                                    gracz2.strzaly.poleStrzalow[col][row] = 2;
                                    bool nowyZatopiony = false;
                                    for (size_t i = 0; i < gracz1.pozycjeStatkow.size(); ++i) {
                                        if (!zatopione1[i] && czyZatopionyShip(gracz1.pozycjeStatkow[i], gracz2.strzaly.poleStrzalow)) {
                                            zatopione1[i] = true;
                                            oznaczPudlaWokolStatku(gracz1.pozycjeStatkow[i], gracz2.strzaly.poleStrzalow, gracz1.plansza.pola);
                                            nowyZatopiony = true;
                                        }
                                    }
                                    if (nowyZatopiony) {
                                        pokazZatopiony = true;
                                        czasPokazZatopiony = 1.2f;
                                    }
                                    if (czyWszystkoZatopione(gracz1.pozycjeStatkow, gracz2.strzaly.poleStrzalow)) {
                                        czyWygrana = true;
                                        zwyciezca = 2;
                                        flashTransition(StanGry::KONIEC);
                                    }
                                }
                                else {
                                    gracz2.strzaly.poleStrzalow[col][row] = 1;
                                    czekajNaZakonczRuch = true;
                                }
                            }
                        }
                        if (czekajNaZakonczRuch && wyglad.przyciskZakonczRuch.getGlobalBounds().contains(mouse.x, mouse.y)) {
                            flashTransition(StanGry::GRA);
                            tura = (tura == 1 ? 2 : 1);
                            czekajNaZakonczRuch = false;
                        }
                    }
                }
                break;

            case StanGry::KONIEC:
                if (!zapisanoZwyciezce) {
                    zapiszZwyciezceDoPliku(zwyciezca);
                    zapisanoZwyciezce = true;
                }
                if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
                    gracz1 = Gracz(sf::Color(80, 170, 255));
                    gracz2 = Gracz(sf::Color(255, 80, 80));
                    czyWygrana = false;
                    zwyciezca = 0;
                    tura = 1;
                    czekajNaZakonczRuch = false;
                    graRozpoczeta = false;
                    pokazNapisObrot1 = true;
                    pokazNapisObrot2 = true;
                    zapisanoZwyciezce = false;
                    flashTransition(StanGry::MENU);
                }
                break;
            default:
                break;
            }
        }

        if (stan == StanGry::GRA && graRozpoczeta) {
            gracz1.buildShipListFromStatki();
            gracz2.buildShipListFromStatki();
            zatopione1.assign(gracz1.pozycjeStatkow.size(), false);
            zatopione2.assign(gracz2.pozycjeStatkow.size(), false);
            pokazZatopiony = false;
            czekajNaZakonczRuch = false;
            graRozpoczeta = false;
        }

        if ((stan == StanGry::USTAWIANIE1 || stan == StanGry::USTAWIANIE2) && przeciagany) przeciagany->aktualizuj(wyglad.window);

        wyglad.window.clear(sf::Color(35, 35, 40));
        wyglad.window.draw(wyglad.tlo);

        switch (stan) {
        case StanGry::MENU: {
            wyglad.tytul.setPosition(290.f, 115.f);
            wyglad.window.draw(wyglad.tytul);

            float czas = wyglad.menuPulsingClock.getElapsedTime().asSeconds();
            float skala = 1.0f + 0.03f * std::sin(czas * 3);
            wyglad.komunikat.setScale(skala, skala);

            wyglad.komunikat.setString("Kliknij dowolny przycisk aby rozpoczac");
            wyglad.komunikat.setCharacterSize(40);
            wyglad.komunikat.setFillColor(sf::Color::White);
            wyglad.komunikat.setStyle(sf::Text::Bold);
            wyglad.komunikat.setOrigin(wyglad.komunikat.getLocalBounds().width / 2.f, wyglad.komunikat.getLocalBounds().height / 2.f);
            wyglad.komunikat.setPosition(640.f, 400.f);

            wyglad.window.draw(wyglad.komunikat);
            break;
        }
        case StanGry::USTAWIANIE1:
            wyglad.window.draw(wyglad.napisGracz1);
            rysujPlansze(gracz1.plansza.pola, gracz2.strzaly.poleStrzalow, wyglad.offsetPlanszy, true, true);
            for (auto& s : gracz1.statki) s.rysuj(wyglad.window);
            if (pokazNapisObrot1) wyglad.window.draw(wyglad.napisObrotPodPlansza);
            if (gracz1.czyWszystkieUstawione()) {
                wyglad.window.draw(wyglad.przyciskStartuGracza2);
            }
            break;
        case StanGry::USTAWIANIE2:
            wyglad.window.draw(wyglad.napisGracz2);
            rysujPlansze(gracz2.plansza.pola, gracz1.strzaly.poleStrzalow, wyglad.offsetPlanszy, true, true);
            for (auto& s : gracz2.statki) s.rysuj(wyglad.window);
            if (pokazNapisObrot2) wyglad.window.draw(wyglad.napisObrotPodPlansza);
            if (gracz2.czyWszystkieUstawione()) {
                wyglad.window.draw(wyglad.przyciskStartuGry);
            }
            break;
        case StanGry::GRA:
            if (tura == 1) {
                rysujPlansze(gracz1.plansza.pola, gracz2.strzaly.poleStrzalow, wyglad.offsetPlanszaL, true, true);
                rysujPlansze(gracz2.plansza.pola, gracz1.strzaly.poleStrzalow, wyglad.offsetPlanszaP, false, true);
                wyglad.komunikat.setString("GRACZ 1 - STRZELAJ NA PRAWEJ PLANSZY");
            }
            else {
                rysujPlansze(gracz2.plansza.pola, gracz1.strzaly.poleStrzalow, wyglad.offsetPlanszaL, true, true);
                rysujPlansze(gracz1.plansza.pola, gracz2.strzaly.poleStrzalow, wyglad.offsetPlanszaP, false, true);
                wyglad.komunikat.setString("GRACZ 2 - STRZELAJ NA PRAWEJ PLANSZY");
            }
            wyglad.komunikat.setPosition(625, 40);
            wyglad.window.draw(wyglad.komunikat);

            if (pokazZatopiony) {
                wyglad.window.draw(wyglad.infoZatopiony);
            }

            if (czekajNaZakonczRuch) {
                wyglad.window.draw(wyglad.przyciskZakonczRuch);
            }
            break;
        case StanGry::KONIEC: {
            std::string msg = (zwyciezca == 1) ? "GRACZ 1 WYGRYWA!" : "GRACZ 2 WYGRYWA!";
            wyglad.infoWygrany.setString(msg + "\n\nESC - restart gry");
            wyglad.infoWygrany.setPosition(270, 200);
            wyglad.infoWygrany.setCharacterSize(65);
            //tytul.setPosition(290.f, 115.f);
            wyglad.infoWygrany.setFillColor(sf::Color::White);
            wyglad.infoWygrany.setStyle(sf::Text::Bold);
            wyglad.window.draw(wyglad.infoWygrany);
            break;
        }
        default:
            break;
        }
        wyglad.window.display();
    }
}