export module GraWyglad;
import <SFML/Graphics.hpp>;
import <string>;
import <regex>;
import <iostream>;

export class GraWyglad {
public:
    sf::Font font;
    sf::Text tytul, komunikat, napisGracz1, napisGracz2, napisObrotPodPlansza, przyciskStartuGracza2, przyciskStartuGry, infoWygrany, infoZatopiony, przyciskZakonczRuch;
    sf::Sprite tlo;
    sf::Texture textureTla;
    sf::RenderWindow window;
    sf::Vector2f offsetPlanszy = sf::Vector2f(400.f, 105.f);
    sf::Vector2f offsetPlanszaL = sf::Vector2f(120.f, 105.f);
    sf::Vector2f offsetPlanszaP = sf::Vector2f(670.f, 105.f);
    sf::Clock menuPulsingClock;

    GraWyglad();

    bool wczytanieCzcionki(const std::string& filename);
    bool wczytanieTextury(const std::string& filename);
    void ustawTeksty();
};

export
GraWyglad::GraWyglad()
    : window(sf::VideoMode(1280, 720), "STATKI"){ }


// ================ Regex ================================
export
bool GraWyglad::wczytanieCzcionki(const std::string& filename) {
    std::regex wzorzec("^[A-Za-z0-9_\\-\\.]+\\.(ttf|otf)$");
    if (!std::regex_match(filename, wzorzec)) {
        std::cerr << "Blad: Nieprawidlowa nazwa pliku czcionki: " << filename << std::endl;
        return false;
    }
    if (!font.loadFromFile(filename)) {
        std::cerr << "Blad: Nie mo¿na wczytac pliku czcionki: " << filename << std::endl;
        return false;
    }
    return true;
}

export
bool GraWyglad::wczytanieTextury(const std::string& filename) {
    std::regex wzorzec("^[A-Za-z0-9_\\-\\.]+\\.(jpg|jpeg|png|bmp)$");
    if (!std::regex_match(filename, wzorzec)) {
        std::cerr << "Blad: Nieprawidlowa nazwa pliku tekstury: " << filename << std::endl;
        return false;
    }
    if (!textureTla.loadFromFile(filename)) {
        std::cerr << "Blad: Nie mo¿na wczytac pliku tekstury: " << filename << std::endl;
        return false;
    }
    tlo.setTexture(textureTla);
    tlo.setTextureRect(sf::IntRect(0, 0, 1280, 720));
    return true;
}

// ================================================================

export
void GraWyglad::ustawTeksty() {
    tytul.setFont(font);
    tytul.setString("STATKI");
    tytul.setCharacterSize(160);
    tytul.setPosition(290.f, 115.f);
    tytul.setFillColor(sf::Color::White);
    tytul.setStyle(sf::Text::Bold);

    komunikat.setFont(font);
    komunikat.setCharacterSize(40);
    komunikat.setFillColor(sf::Color::White);
    komunikat.setStyle(sf::Text::Bold);

    napisGracz1.setFont(font);
    napisGracz1.setString("GRACZ 1 - USTAW SWOJE STATKI");
    napisGracz1.setCharacterSize(45);
    napisGracz1.setPosition(230.f, 30.f);
    napisGracz1.setStyle(sf::Text::Bold);

    napisGracz2.setFont(font);
    napisGracz2.setString("GRACZ 2 - USTAW SWOJE STATKI");
    napisGracz2.setCharacterSize(45);
    napisGracz2.setPosition(230.f, 30.f);
    napisGracz2.setStyle(sf::Text::Bold);

    napisObrotPodPlansza.setFont(font);
    napisObrotPodPlansza.setString("Aby obrocic statek kliknij prawy przycisk myszy");
    napisObrotPodPlansza.setCharacterSize(40);
    napisObrotPodPlansza.setStyle(sf::Text::Bold);
    napisObrotPodPlansza.setPosition(60.f, 630.f);

    przyciskStartuGracza2.setFont(font);
    przyciskStartuGracza2.setString("Przejdz do Gracza 2");
    przyciskStartuGracza2.setCharacterSize(40);
    przyciskStartuGracza2.setFillColor(sf::Color::White);
    przyciskStartuGracza2.setPosition(770.f, 625.f);
    przyciskStartuGracza2.setStyle(sf::Text::Bold);

    przyciskStartuGry.setFont(font);
    przyciskStartuGry.setString("ROZPOCZNIJ GRE");
    przyciskStartuGry.setCharacterSize(40);
    przyciskStartuGry.setFillColor(sf::Color::White);
    przyciskStartuGry.setPosition(835.f, 635.f);
    przyciskStartuGry.setStyle(sf::Text::Bold);

    przyciskZakonczRuch.setFont(font);
    przyciskZakonczRuch.setString("ZAKONCZ RUCH");
    przyciskZakonczRuch.setCharacterSize(40);
    przyciskZakonczRuch.setFillColor(sf::Color::White);
    przyciskZakonczRuch.setPosition(835.f, 635.f);
    przyciskZakonczRuch.setStyle(sf::Text::Bold);

    infoWygrany.setFont(font);
    infoWygrany.setCharacterSize(44);
    infoWygrany.setStyle(sf::Text::Bold);
    infoWygrany.setFillColor(sf::Color::Green);

    infoZatopiony.setFont(font);
    infoZatopiony.setCharacterSize(48);
    infoZatopiony.setStyle(sf::Text::Bold);
    infoZatopiony.setFillColor(sf::Color(255, 35, 35));
    infoZatopiony.setString("STATEK ZATOPIONY!");
    infoZatopiony.setPosition(345, 280);
    infoZatopiony.setStyle(sf::Text::Bold);
}