export module Strzaly;
import <vector>;

export class Strzaly {
public:
    static constexpr int ILOSC_POL = 10;
    std::vector<std::vector<int>> poleStrzalow;
    Strzaly();
};

export
Strzaly::Strzaly() : poleStrzalow(ILOSC_POL, std::vector<int>(ILOSC_POL, 0)) {}