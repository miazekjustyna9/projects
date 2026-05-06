#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

class SlownikAdresowanieOtwarte {
private:
    int pojemnosc;
    vector<int> tablica;
    int liczba_porownan;
    int rozmiar_slownika;

    const int puste = -1;
    const int usuniete = -2;

    int haszuj(int klucz) {
        return klucz % pojemnosc;
    }

    int haszuj2(int klucz) {
        return 1 + 2 * (klucz % 100);
    }

    void rehaszuj(int nowa_pojemnosc) {
        vector<int> stara_tablica = tablica;

        pojemnosc = nowa_pojemnosc;
        tablica.assign(pojemnosc, puste);
        rozmiar_slownika = 0;

        for (int i = 0; i < stara_tablica.size(); i++) {
            if (stara_tablica[i] != puste && stara_tablica[i] != usuniete) {
                wstaw(stara_tablica[i]);
            }
        }
    }

public:
    SlownikAdresowanieOtwarte(int rozmiar) {
        pojemnosc = rozmiar;
        tablica.assign(pojemnosc, puste);
        liczba_porownan = 0;
        rozmiar_slownika = 0;
    }

    bool znajdz(int klucz) {
        int indeks = haszuj(klucz);
        int krok = haszuj2(klucz);

        for (int i = 0; i < pojemnosc; i++) {
            if (tablica[indeks] == puste) {
                return false;
            }

            if (tablica[indeks] != usuniete) {
                liczba_porownan++;
                if (tablica[indeks] == klucz) {
                    return true;
                }
            }
            indeks = (indeks + krok) % pojemnosc;
        }
        return false;
    }

    void wstaw(int klucz) {
        if (znajdz(klucz)) {
            return;
        }

        if (rozmiar_slownika >= pojemnosc * 0.7) {
            rehaszuj(pojemnosc * 2);
        }

        int indeks = haszuj(klucz);
        int krok = haszuj2(klucz);

        for (int i = 0; i < pojemnosc; i++) {
            if (tablica[indeks] == puste || tablica[indeks] == usuniete) {
                tablica[indeks] = klucz;
                rozmiar_slownika++;
                return;
            }
            indeks = (indeks + krok) % pojemnosc;
        }
    }

    void usun(int klucz) {
        int indeks = haszuj(klucz);
        int krok = haszuj2(klucz);

        for (int i = 0; i < pojemnosc; i++) {
            if (tablica[indeks] == puste) {
                return;
            }

            if (tablica[indeks] != usuniete) {
                liczba_porownan++;
                if (tablica[indeks] == klucz) {
                    tablica[indeks] = usuniete;
                    rozmiar_slownika--;

                    if (rozmiar_slownika <= pojemnosc * 0.25 && pojemnosc > 16) {
                        rehaszuj(pojemnosc / 2);
                    }
                    return;
                }
            }
            indeks = (indeks + krok) % pojemnosc;
        }
    }

    int pobierzLiczbePorownan() { return liczba_porownan; }
    void resetujLicznikPorownan() { liczba_porownan = 0; }
};

void generujDaneDoWykresu() {
    SlownikAdresowanieOtwarte slownik(16);
    srand(time(NULL));

    ofstream plik("dane_adresowanie_otwarte.txt");

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
    cout << "ADRESOWANIE OTWARTE\n\n";

    SlownikAdresowanieOtwarte slownik(16);

    for(int i = 1; i <= 20; i++) {
        slownik.wstaw(i * 10);
    }

    cout << "Czy slownik pomiescil i znalazl liczbe 200? " << (slownik.znajdz(200) ? "Tak!" : "Nie") << endl << endl;

    generujDaneDoWykresu();

    return 0;
}
