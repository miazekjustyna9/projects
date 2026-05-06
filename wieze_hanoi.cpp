#include <iostream>
#include <cmath>

using namespace std;

void ruch(int disk, int from, int to)
{
    cout << from << " " << to << "\n";
}

void hanoi3(int n, int from, int to, int k)
{
    if (n == 0) return;

    hanoi3(n - 1, from, k, to);
    ruch(n, from, to);
    hanoi3(n - 1, k, to, from);
}

void hanoi4(int n, int from, int to, int wieza3, int wieza4)
{
    if (n == 0)
    {
        return;
    }
    if (n == 1)
    {
        ruch(1, from, to);
        return;
    }

    int k = n - round(sqrt(2*n+1)) + 1;
    if (k<1)
    {
        k = 1;
    }

    hanoi4(n - k, from, wieza4, wieza3, to);
    hanoi3(k, from, to, wieza3);
    hanoi4(n - k, wieza4, to, from, wieza3);
}

void hanoi5(int n, int from, int to, int wieza3, int wieza4, int wieza5)
{
    if (n == 0)
    {
        return;
    }
    if (n == 1)
    {
        ruch(1, from, to);
        return;
    }

    int k = n - round(sqrt(2*n+1)) + 1;
    if (k<1) k = 1;

    hanoi5(n - k, from, wieza5, wieza3, wieza4, to);
    hanoi4(k, from, to, wieza3, wieza4);
    hanoi5(n - k, wieza5, to, from, wieza3, wieza4);
}

void hanoi_uniwersalne(int n, int from, int to, int* wieze, int ilosc)
{
    if (n == 0) {
        return; }
    if (n == 1) {
        ruch(1, from, to);
        return; }

    int r = ilosc + 2;    // r - calkowita liczba wiez

    if (r == 3) {
        hanoi3(n, from, to, wieze[0]);
        return; }

    int k = n - round(sqrt(2*n+1)) + 1;
    if (k<1) k = 1;
    if (k>=n) k = n-1;

    int wieza_x = wieze[ilosc - 1];  // wieza_x - bufor dla "gornej czesci"

    int* nowe_wieze = new int[ilosc];
    for (int i=0; i<ilosc-1; i++)
    {
        nowe_wieze[i] = wieze[i];
    }

    nowe_wieze[ilosc - 1] = to;

    hanoi_uniwersalne(n-k, from, wieza_x, nowe_wieze, ilosc);
    hanoi_uniwersalne(k, from, to, wieze, ilosc-1);

    nowe_wieze[ilosc - 1] = from;

    hanoi_uniwersalne(n-k, wieza_x, to, nowe_wieze, ilosc);

    delete[] nowe_wieze;
}

int main()
{
    int n; cout << "Liczba krazkow: "; cin >> n;
    int wieze; cout  << "Liczba wiez: "; cin >> wieze;
    cout << endl << n << endl << wieze << endl;

    int rozmiar_tablicy = wieze-2;
    int* wieze_tablica = new int[rozmiar_tablicy];

    for (int i=0; i<rozmiar_tablicy; i++)
    {
        wieze_tablica[i] = i + 3;
    }

    hanoi_uniwersalne(n, 1, 2, wieze_tablica, rozmiar_tablicy);

    delete[] wieze_tablica;

    return 0;
}
