#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

class SlownikLancuchowy {
private:
    int pojemnosc;
    vector<vector<int>> tablica;
    int liczba_porownan;
    int rozmiar_slownika;

    int haszuj(int klucz) {
        return klucz % pojemnosc;
    }

    void rehaszuj(int nowa_pojemnosc) {
        vector<vector<int>> stara_tablica = tablica;
        int stara_pojemnosc = pojemnosc;

        pojemnosc = nowa_pojemnosc;
        tablica.clear();
        tablica.resize(pojemnosc);
        rozmiar_slownika = 0;

        for (int i = 0; i < stara_pojemnosc; i++) {
            for (int j = 0; j < stara_tablica[i].size(); j++) {
                wstaw(stara_tablica[i][j]);
            }
        }
    }

public:
    SlownikLancuchowy(int rozmiar) {
        pojemnosc = rozmiar;
        tablica.resize(pojemnosc);
        liczba_porownan = 0;
        rozmiar_slownika = 0;
    }

    bool znajdz(int klucz) {
        int indeks = haszuj(klucz);

        for (int i = 0; i < tablica[indeks].size(); i++) {
            liczba_porownan++;
            if (tablica[indeks][i] == klucz) {
                if (i > 0) {
                    swap(tablica[indeks][i], tablica[indeks][i - 1]);
                }
                return true;
            }
        }
        return false;
    }

    void wstaw(int klucz) {
        if (znajdz(klucz)) {
            return;
        }

        if (rozmiar_slownika >= pojemnosc) {
            rehaszuj(pojemnosc * 2);
        }

        int indeks = haszuj(klucz);
        tablica[indeks].push_back(klucz);
        rozmiar_slownika++;
    }

    void usun(int klucz) {
        int indeks = haszuj(klucz);

        for (int i = 0; i < tablica[indeks].size(); i++) {
            liczba_porownan++;
            if (tablica[indeks][i] == klucz) {
                tablica[indeks][i] = tablica[indeks].back();
                tablica[indeks].pop_back();
                rozmiar_slownika--;
                return;
            }
        }
    }

    int pobierzLiczbePorownan() { return liczba_porownan; }
    void resetujLicznikPorownan() { liczba_porownan = 0; }
};

void generujDaneDoWykresu() {
    SlownikLancuchowy slownik(16);
    srand(time(NULL));

    ofstream plik("dane_lancuchowy.txt");

    if (!plik.is_open()) {
        cout << "Blad pliku\n";
        return;
    }

    plik << "Ilosc_Elementow;Srednia_Liczba_Porownan\n";

    for (int ilosc_w_slowniku = 1; ilosc_w_slowniku <= 1000; ilosc_w_slowniku++) {
        while (true) {
            int klucz = rand() % 20000;
            if (!slownik.znajdz(klucz)) {
                slownik.wstaw(klucz);
                break;
            }
        }

        int suma_porownan = 0;
        int proby = 10;

        for (int p = 0; p < proby; p++) {
            int losowy_klucz_do_szukania = rand() % 20000;
            slownik.resetujLicznikPorownan();
            slownik.znajdz(losowy_klucz_do_szukania);
            suma_porownan += slownik.pobierzLiczbePorownan();
        }

        if (ilosc_w_slowniku % 5 == 0) {
            plik << ilosc_w_slowniku << ";" << (double)suma_porownan / proby << "\n";
        }
    }

    plik.close();
    cout << "Dane do wykresu zapisane w pliku\n\n";
}

int main() {
    cout << "METODA LANCUCHOWA\n\n";

    SlownikLancuchowy slownik(16);

    slownik.wstaw(15);
    slownik.wstaw(25);
    slownik.wstaw(35);

    cout << "Czy slownik znalazl 25? " << (slownik.znajdz(25) ? "Tak!" : "Nie") << endl << endl;

    generujDaneDoWykresu();

    return 0;
}
