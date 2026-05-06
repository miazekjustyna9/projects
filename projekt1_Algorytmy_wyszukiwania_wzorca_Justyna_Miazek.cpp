#include <iostream>
#include <string>

using namespace std;

bool czyPasujeNaPozycji(string tekst, int p, string wzorzec, int& licznik) {
    for (int i=0; i < wzorzec.length(); i++) {
        licznik++;
        if (wzorzec[i] != tekst[p + i]) {
            return false;
        }
    }
    return true;
}

int wyszukiwanieNaiwne(string tekst, string wzorzec) {
    int dlugoscTekstu = tekst.length();
    int dlugoscWzorca = wzorzec.length();
    int licznik = 0;

    if (dlugoscWzorca == 0 || dlugoscWzorca > dlugoscTekstu) return 0;

    for (int p=0; p <= dlugoscTekstu - dlugoscWzorca; p++) {
        if (czyPasujeNaPozycji(tekst, p, wzorzec, licznik)) {
            cout << "Algorytm Naiwny: Znaleziono wzorzec na pozycji: " << p << endl;
        }
    }
    return licznik;
}

int wyszukiwanieSunday(string tekst, string wzorzec) {
    int dlugoscTekstu = tekst.length();
    int dlugoscWzorca = wzorzec.length();
    int licznik = 0;

    if (dlugoscWzorca == 0 || dlugoscWzorca > dlugoscTekstu) return 0;

    int ostatniaPozycja[256];
    for (int i=0; i < 256; i++) {
        ostatniaPozycja[i] = -1;
    }

    for (int i=0; i < dlugoscWzorca; i++) {
        ostatniaPozycja[(unsigned char)wzorzec[i]] = i;
    }

    int p = 0;
    while (p <= dlugoscTekstu - dlugoscWzorca) {
        if (czyPasujeNaPozycji(tekst, p, wzorzec, licznik)) {
            cout << "Algorytm Sunday'a: Znaleziono wzorzec na pozycji: " << p << endl;
        }
        p = p + dlugoscWzorca;

        if (p < dlugoscTekstu) {
            p = p - ostatniaPozycja[(unsigned char)tekst[p]];
        }
    }
    return licznik;
}

int wyszukiwanieSundayPary(string tekst, string wzorzec) {
    int dlugoscTekstu = tekst.length();
    int dlugoscWzorca = wzorzec.length();
    int licznik = 0;

    if (dlugoscWzorca == 0 || dlugoscWzorca > dlugoscTekstu) return 0;

    if (dlugoscWzorca < 2) {
        cout << "Wzorzec jest za krótki" << endl;
        return 0;
    }

    int przesuniecie[256][256];

    // nie ma tych liczb np. x y sposrod abc
    for (int i=0; i < 256; i++) {
        for (int j=0; j < 256; j++) {
            przesuniecie[i][j] = dlugoscWzorca + 2;
        }
    }

    // druga litera za oknem to pierwsza litera wzorca
    for (int i=0; i < 256; i++) {
        przesuniecie[i][(unsigned char)wzorzec[0]] = dlugoscWzorca + 1;
    }

    // pary ktore s¹ we wzorcu w srodku
    for (int i=0; i < dlugoscWzorca - 1; i++) {
        przesuniecie[(unsigned char)wzorzec[i]][(unsigned char)wzorzec[i + 1]] = dlugoscWzorca - i;
    }

    // pierwsza litera za oknem to ostatnia litera wzorca
    for (int j=0; j < 256; j++) {
        przesuniecie[(unsigned char)wzorzec[dlugoscWzorca - 1]][j] = 1;
    }

    int p = 0;
    while (p <= dlugoscTekstu - dlugoscWzorca) {
        if (czyPasujeNaPozycji(tekst, p, wzorzec, licznik)) {
            cout << "Algorytm Sunday'a Pary: Znaleziono wzorzec na pozycji: " << p << endl;
        }

        if (p + dlugoscWzorca + 1 < dlugoscTekstu) {
            unsigned char a1 = tekst[p + dlugoscWzorca];
            unsigned char a2 = tekst[p + dlugoscWzorca + 1];
            p = p + przesuniecie[a1][a2];
        } else {
            p++;
        }
    }
    return licznik;
}

int wyszukiwanieKarpRabin(string tekst, string wzorzec) {
    int dlugoscTekstu = tekst.length();
    int dlugoscWzorca = wzorzec.length();
    int licznik = 0;

    if (dlugoscWzorca == 0 || dlugoscWzorca > dlugoscTekstu) return 0;

    int podstawa = 256;
    int liczbaPierwsza = 101;
    int haszWzorca = 0;
    int haszOkienka = 0;
    int mnoznik = 1;

    for (int i=0; i < dlugoscWzorca - 1; i++) {
        mnoznik = (mnoznik * podstawa) % liczbaPierwsza;
    }

    for (int i=0; i < dlugoscWzorca; i++) {
        haszWzorca = (podstawa * haszWzorca + wzorzec[i]) % liczbaPierwsza;
        haszOkienka = (podstawa * haszOkienka + tekst[i]) % liczbaPierwsza;
    }

    for (int p=0; p <= dlugoscTekstu - dlugoscWzorca; p++) {
        if (haszWzorca == haszOkienka) {
            if (czyPasujeNaPozycji(tekst, p, wzorzec, licznik)) {
                cout << "Algorytm Karpa-Rabina: Znaleziono wzorzec na pozycji: " << p << endl;
            }
        }

        if (p < dlugoscTekstu - dlugoscWzorca) {
            haszOkienka = (podstawa * (haszOkienka - tekst[p] * mnoznik) + tekst[p + dlugoscWzorca]) % liczbaPierwsza;
            if (haszOkienka < 0) {
                haszOkienka = haszOkienka + liczbaPierwsza;
            }
        }
    }
    return licznik;
}

int main() {
    string tekst;
    string wzorzec;

    cout << "Podaj tekst: ";
    getline(cin, tekst);
    cout << "Podaj wzorzec: ";
    getline(cin, wzorzec);
    cout << endl;

    int licznikNaiwny = wyszukiwanieNaiwne(tekst, wzorzec);
    cout << "Liczba porownan znakow: " << licznikNaiwny << "\n\n";

    int licznikSunday = wyszukiwanieSunday(tekst, wzorzec);
    cout << "Liczba porownan znakow: " << licznikSunday << "\n\n";

    int licznikSundayPary = wyszukiwanieSundayPary(tekst, wzorzec);
    cout << "Liczba porownan znakow: " << licznikSundayPary << "\n\n";

    int licznikKarpRabin = wyszukiwanieKarpRabin(tekst, wzorzec);
    cout << "Liczba porownan znakow: " << licznikKarpRabin << "\n\n";

    return 0;
}
