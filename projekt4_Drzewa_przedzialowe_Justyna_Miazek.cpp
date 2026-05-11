#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class DrzewoPrzedzialowe {
private:
    int n;
    int rozmiar_drzewa;

    vector<long long> drzewo_suma;
    vector<long long> drzewo_dodatek;

    void aktualizuj_wezel(int wierzcholek, int lewy, int prawy, int zap_lewy, int zap_prawy, long long wartosc) {
        if (zap_lewy <= lewy && prawy <= zap_prawy) {
            drzewo_dodatek[wierzcholek] += wartosc;
            return;
        }

        int srodek = (lewy + prawy) / 2;

        if (zap_lewy <= srodek) {
            aktualizuj_wezel(wierzcholek * 2, lewy, srodek, zap_lewy, zap_prawy, wartosc);
        }
        if (zap_prawy > srodek) {
            aktualizuj_wezel(wierzcholek * 2 + 1, srodek + 1, prawy, zap_lewy, zap_prawy, wartosc);
        }

        long long suma_lewe = drzewo_suma[wierzcholek * 2] + drzewo_dodatek[wierzcholek * 2] * (srodek - lewy + 1);
        long long suma_prawe = drzewo_suma[wierzcholek * 2 + 1] + drzewo_dodatek[wierzcholek * 2 + 1] * (prawy - srodek);

        drzewo_suma[wierzcholek] = suma_lewe + suma_prawe;
    }

    long long zapytaj_wezel(int wierzcholek, int lewy, int prawy, int zap_lewy, int zap_prawy) {
        if (zap_lewy <= lewy && prawy <= zap_prawy) {
            long long ilosc_elementow = prawy - lewy + 1;
            return drzewo_suma[wierzcholek] + (drzewo_dodatek[wierzcholek] * ilosc_elementow);
        }

        int srodek = (lewy + prawy) / 2;
        long long wynik = 0;

        if (zap_lewy <= srodek) {
            wynik += zapytaj_wezel(wierzcholek * 2, lewy, srodek, zap_lewy, zap_prawy);
        }
        if (zap_prawy > srodek) {
            wynik += zapytaj_wezel(wierzcholek * 2 + 1, srodek + 1, prawy, zap_lewy, zap_prawy);
        }

        long long poczatek_przeciecia = max(lewy, zap_lewy);
        long long koniec_przeciecia = min(prawy, zap_prawy);
        long long dlugosc_przeciecia = koniec_przeciecia - poczatek_przeciecia + 1;

        wynik += drzewo_dodatek[wierzcholek] * dlugosc_przeciecia;

        return wynik;
    }

public:
    DrzewoPrzedzialowe(const vector<long long>& tablica) {
        n = tablica.size();
        rozmiar_drzewa = 1;
        while (rozmiar_drzewa < n) {
            rozmiar_drzewa *= 2;
        }

        drzewo_suma.assign(2 * rozmiar_drzewa, 0);
        drzewo_dodatek.assign(2 * rozmiar_drzewa, 0);

        for (int i = 0; i < n; ++i) {
            drzewo_suma[rozmiar_drzewa + i] = tablica[i];
        }

        for (int i = rozmiar_drzewa - 1; i >= 1; --i) {
            drzewo_suma[i] = drzewo_suma[i * 2] + drzewo_suma[i * 2 + 1];
        }
    }

    void dodaj_na_przedziale(int p, int k, long long wartosc) {
        if (p > k || p < 0 || k >= n) return;
        aktualizuj_wezel(1, 0, rozmiar_drzewa - 1, p, k, wartosc);
    }

    long long suma_na_przedziale(int p, int k) {
        if (p > k || p < 0 || k >= n) return 0;
        return zapytaj_wezel(1, 0, rozmiar_drzewa - 1, p, k);
    }
};

int main() {
    int n;
    cout << "Podaj liczbe pracownikow (rozmiar tablicy): ";
    cin >> n;

    vector<long long> t(n);
    cout << "Podaj poczatkowe wyniki dla " << n << " pracownikow (oddziel je spacja):" << endl;
    for (int i = 0; i < n; ++i) {
        cin >> t[i];
    }

    DrzewoPrzedzialowe drzewo(t);
    cout << "\nDrzewo zostalo zbudowane! Firma dziala." << endl;

    while (true) {
        cout << "\n--- MENU PREZESA ---" << endl;
        cout << "1 - Dodaj premie na przedziale" << endl;
        cout << "2 - Zapytaj o sume na przedziale" << endl;
        cout << "3 - Zakoncz dzialanie firmy" << endl;
        cout << "Wybierz akcje: ";

        int wybor;
        cin >> wybor;

        if (wybor == 1) {
            int p, k;
            long long wartosc;
            cout << "Podaj numer pierwszego pracownika, ostatniego pracownika oraz wartosc premii (np. 1 3 10): ";
            cin >> p >> k >> wartosc;

            drzewo.dodaj_na_przedziale(p, k, wartosc);
            cout << "Premia zostala rozdane pomyslnie!" << endl;

        } else if (wybor == 2) {
            int p, k;
            cout << "Podaj numer pierwszego i ostatniego pracownika do podsumowania (np. 1 3): ";
            cin >> p >> k;

            long long wynik = drzewo.suma_na_przedziale(p, k);
            cout << "Suma w dziale [" << p << ", " << k << "] wynosi: " << wynik << endl;

        } else if (wybor == 3) {
            cout << "Zamykamy firme. Do widzenia!" << endl;
            break;

        } else {
            cout << "Nieznany rozkaz. Wybierz 1, 2 lub 3." << endl;
        }
    }

    return 0;
}
