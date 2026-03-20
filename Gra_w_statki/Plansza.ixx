export module Plansza;
import <vector>;

export class Plansza {
public:
    static constexpr int ILOSC_POL = 10;
    std::vector<std::vector<int>> pola;
    Plansza();
    static bool czyNaPlanszy(int x0, int y0, int szer, int wys);
    static bool czyMoznaUstawic(const std::vector<std::vector<int>>& plansza, int x0, int y0, int szer, int wys);
    static void ustawNaPlanszy(std::vector<std::vector<int>>& plansza, int x0, int y0, int szer, int wys);
    static void zdejmijZPlanszy(std::vector<std::vector<int>>& plansza, int x0, int y0, int szer, int wys);
};

export
Plansza::Plansza() : pola(ILOSC_POL, std::vector<int>(ILOSC_POL, 0)) {}

export
bool Plansza::czyNaPlanszy(int x0, int y0, int szer, int wys) {
    return x0 >= 0 && y0 >= 0 && x0 + szer <= ILOSC_POL && y0 + wys <= ILOSC_POL;
}

export
bool Plansza::czyMoznaUstawic(const std::vector<std::vector<int>>& plansza, int x0, int y0, int szer, int wys) {
    for (int dx = -1; dx <= szer; ++dx)
        for (int dy = -1; dy <= wys; ++dy) {
            int x = x0 + dx;
            int y = y0 + dy;
            if (x >= 0 && x < ILOSC_POL && y >= 0 && y < ILOSC_POL)
                if (plansza[x][y]) return false;
        }
    return true;
}

export
void Plansza::ustawNaPlanszy(std::vector<std::vector<int>>& plansza, int x0, int y0, int szer, int wys) {
    for (int dx = 0; dx < szer; ++dx)
        for (int dy = 0; dy < wys; ++dy)
            plansza[x0 + dx][y0 + dy] = 1;
}

export
void Plansza::zdejmijZPlanszy(std::vector<std::vector<int>>& plansza, int x0, int y0, int szer, int wys) {
    for (int dx = 0; dx < szer; ++dx)
        for (int dy = 0; dy < wys; ++dy)
            plansza[x0 + dx][y0 + dy] = 0;
}