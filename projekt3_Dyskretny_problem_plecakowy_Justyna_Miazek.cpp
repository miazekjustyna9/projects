#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Przedmiot {
    int waga;
    int wartosc;
    int oryginalny_indeks;
};

int main() {
cout << "=== PROBLEM PLECAKOWY Z MAGICZNYMI KULAMI ===\n\n";

    int pojemnosc_plecaka;
    cout << "Podaj pojemnosc plecaka: "; cin >> pojemnosc_plecaka;

    int liczba_przedmiotow;
    cout << "Podaj calkowita liczbe zwyklych przedmiotow: "; cin >> liczba_przedmiotow;

    vector<Przedmiot> przedmioty;

    cout << "\n--- Wprowadzanie przedmiotow ---\n";
    for (int i = 1; i <= liczba_przedmiotow; i++) {
        int wpisana_waga, wpisana_wartosc;
        cout << "Przedmiot nr " << i << " - Podaj wage i wartosc (oddzielone spacja): ";
        cin >> wpisana_waga >> wpisana_wartosc;

        przedmioty.push_back({wpisana_waga, wpisana_wartosc, i});
    }

    int maks_kul;
    int waga_kuli;
    cout << "\n--- Magiczne Kule ---\n";
    cout << "Podaj maksymalna liczbe magicznych kul: "; cin >> maks_kul;
    cout << "Podaj wage jednej magicznej kuli: "; cin >> waga_kuli;

    cout << "\n\n===== WYNIKI =====\n\n";

    vector<vector<int>> tablica_dp(liczba_przedmiotow + 1, vector<int>(pojemnosc_plecaka + 1, 0));

    for (int i = 1; i <= liczba_przedmiotow; i++) {
        for (int w = 0; w <= pojemnosc_plecaka; w++) {
            int waga_aktualnego = przedmioty[i - 1].waga;
            int wartosc_aktualnego = przedmioty[i - 1].wartosc;

            if (waga_aktualnego <= w) {
                tablica_dp[i][w] = max(tablica_dp[i - 1][w],
                                       tablica_dp[i - 1][w - waga_aktualnego] + wartosc_aktualnego);
            } else {
                tablica_dp[i][w] = tablica_dp[i - 1][w];
            }
        }
    }

    // kule
    int ostateczna_wartosc = 0;
    int optymalna_liczba_kul = 0;
    int optymalna_pojemnosc_przedmiotow = pojemnosc_plecaka;

    int zapamietany_mnoznik = 1;
    int mnoznik = 1;

    for (int k = 0; k <= maks_kul; k++) {
        int waga_zajeta_przez_kule = k * waga_kuli;

        if (waga_zajeta_przez_kule <= pojemnosc_plecaka) {

            int miejsce_na_przedmioty = pojemnosc_plecaka - waga_zajeta_przez_kule;

            int wartosc_bazowa = tablica_dp[liczba_przedmiotow][miejsce_na_przedmioty];
            int wartosc_z_mnoznikiem = wartosc_bazowa * mnoznik;

            if (wartosc_z_mnoznikiem > ostateczna_wartosc) {
                ostateczna_wartosc = wartosc_z_mnoznikiem;
                optymalna_liczba_kul = k;
                optymalna_pojemnosc_przedmiotow = miejsce_na_przedmioty;
                zapamietany_mnoznik = mnoznik;
            }
        }
        mnoznik = mnoznik * 2;
    }

    vector<Przedmiot> wybrane_przedmioty;
    int pozostala_pojemnosc = optymalna_pojemnosc_przedmiotow;

    for (int i = liczba_przedmiotow; i > 0; i--) {
        if (tablica_dp[i][pozostala_pojemnosc] != tablica_dp[i - 1][pozostala_pojemnosc]) {
            wybrane_przedmioty.push_back(przedmioty[i - 1]);
            pozostala_pojemnosc -= przedmioty[i - 1].waga;
        }
    }

    cout << "Maksymalna ostateczna wartosc: " << ostateczna_wartosc << "\n\n";

    cout << "Zapakowane przedmioty specjalne:\n";
    cout << "- Magiczne kule sztuk: " << optymalna_liczba_kul
         << " (Mnoznik: x" << zapamietany_mnoznik
         << ", Waga: " << (optymalna_liczba_kul * waga_kuli) << ")\n\n";

    cout << "Zapakowane zwykle przedmioty:\n";
    int laczna_waga_przedmiotow = 0;

    for (const auto& p : wybrane_przedmioty) {
        cout << "- Przedmiot " << p.oryginalny_indeks
             << " (Waga: " << p.waga << ", Wartosc: " << p.wartosc << ")\n";
        laczna_waga_przedmiotow += p.waga;
    }

    int waga_calkowita = laczna_waga_przedmiotow + (optymalna_liczba_kul * waga_kuli);
    cout << "\n------------------------------------\n\n";
    cout << "Waga kul: " << (optymalna_liczba_kul * waga_kuli) << "\n";
    cout << "Waga przedmiotow: " << laczna_waga_przedmiotow << "\n";
    cout << "Calkowita waga plecaka: " << waga_calkowita << " / " << pojemnosc_plecaka << "\n\n";

    return 0;
}
