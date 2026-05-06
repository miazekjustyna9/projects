#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

using namespace std;
using namespace std::chrono;

long long l_porownan = 0;
long long l_zamian = 0;

void zamien(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void kopia_tablicy(int cel[], int zrodlo[], int n) {
    for (int i = 0; i < n; i++) {
        cel[i] = zrodlo[i];
    }
}

// SHELLSORT tryb: 0 = Shell, 1 = Hibbard, 2 = Tokuda

void shell_sort(int* t, int n, int tryb) {
    int przyrosty[100];
    int ilosc = 0;

    if (tryb == 0) {
        // Sekwencja Shella
        for (int h = n / 2; h > 0; h /= 2) {
            przyrosty[ilosc++] = h;
        }
    }
    else if (tryb == 1) {
        // Sekwencja Hibbarda
        int k = 1;
        while (true) {
            int h = (int)pow(2, k) - 1;
            if (h >= n) break;
            przyrosty[ilosc++] = h;
            k++;
        }
        for(int i=0; i<ilosc/2; i++) zamien(przyrosty[i], przyrosty[ilosc-1-i]);
    }
    else if (tryb == 2) {
        // Sekwencja Tokudy
        int k = 1;
        while (true) {
            double val = (pow(2.25, k) - 1.0) / 1.25;
            int h = (int)ceil(val);
            if (h >= n) break;
            przyrosty[ilosc++] = h;
            k++;
        }
        for(int i=0; i<ilosc/2; i++) zamien(przyrosty[i], przyrosty[ilosc-1-i]);
    }

    for (int p = 0; p < ilosc; p++) {
        int h = przyrosty[p];
        for (int i = h; i < n; i++) {
            int temp = t[i];
            int j;
            for (j = i; j >= h; j -= h) {
                l_porownan++;
                if (t[j - h] > temp) {
                    t[j] = t[j - h];
                    l_zamian++;
                } else {
                    break;
                }
            }
            t[j] = temp;
        }
    }
}

// QUICKSORT
int podziel(int* t, int lewy, int prawy) {
    int pivot = t[prawy];
    int granica = (lewy - 1);

    for (int j = lewy; j <= prawy - 1; j++) {
        l_porownan++;
        if (t[j] < pivot) {
            granica++;
            zamien(t[granica], t[j]);
            l_zamian++;
        }
    }
    zamien(t[granica + 1], t[prawy]);
    l_zamian++;
    return (granica + 1);
}

void wybor_pivota(int* t, int lewy, int prawy, int tryb) {
    if (tryb == 0) {
        // Right - nic nie robie bo pivot = t[prawy]
    }
    else if (tryb == 1) {
        // Random
        int losowy = lewy + rand() % (prawy - lewy + 1);
        zamien(t[losowy], t[prawy]);
        l_zamian++;
    }
    else if (tryb == 2) {
        // Mediana z trzech
        int srodek = lewy + (prawy - lewy) / 2;

        if (t[srodek] < t[lewy]) { zamien(t[srodek], t[lewy]); l_zamian++; }
        if (t[prawy] < t[lewy]) { zamien(t[prawy], t[lewy]); l_zamian++; }
        if (t[prawy] < t[srodek]) { zamien(t[prawy], t[srodek]); l_zamian++; }

        zamien(t[srodek], t[prawy]);
        l_zamian++;
    }
}

void quick_sort(int* t, int lewy, int prawy, int tryb_pivota) {
    if (lewy < prawy) {
        wybor_pivota(t, lewy, prawy, tryb_pivota);

        int pp = podziel(t, lewy, prawy);

        quick_sort(t, lewy, pp - 1, tryb_pivota);
        quick_sort(t, pp + 1, prawy, tryb_pivota);
    }
}

int main() {
    srand(time(NULL));

    string nazwaPliku = string("wyniki") + string(".csv");
    ofstream plik(nazwaPliku.c_str());

    if (!plik.is_open()) {
        cout << "Blad otwarcia pliku" << endl;
        return 1;
    }

    plik << "Algorytm;TypDanych;Rozmiar;Czas[us];Porownania;Zamiany\n";

    for (int n = 10000; n <= 250000; n += 10000)
    {
        cout << "Przetwarzanie rozmiaru: " << n << "\n";

        int* t_wzor = new int[n];
        int* t1 = new int[n];

        for (int typ = 0; typ < 3; typ++)
        {
            string nazwa_typu;

            if (typ == 0) {
                nazwa_typu = "Losowe";
                for (int i = 0; i < n; i++) t_wzor[i] = rand() % 1000000;
            }
            else if (typ == 1) {
                nazwa_typu = "Rosnace";
                for (int i = 0; i < n; i++) t_wzor[i] = i;
            }
            else {
                nazwa_typu = "Duplikaty";
                for (int i = 0; i < n; i++) t_wzor[i] = rand() % 6;
            }

            for (int algo = 0; algo < 6; algo++) {
                // zabezpieczenie
                bool problem_right_rosnace = (algo == 3 && typ == 1);
                bool problem_duplikaty = (algo >= 3 && typ == 2);

                if ((problem_right_rosnace || problem_duplikaty) && n > 20000) {
                     string nazwa_obecnego_algo;
                     if (algo == 3) nazwa_obecnego_algo = "QuickSort (Right)";
                     else if (algo == 4) nazwa_obecnego_algo = "QuickSort (Random)";
                     else nazwa_obecnego_algo = "QuickSort (Median3)";

                     plik << nazwa_obecnego_algo << ";" << nazwa_typu << ";" << n << ";-1;-1;-1" << endl;
                     continue;
                }

                long long suma_czas = 0;
                long long suma_porownan = 0;
                long long suma_zamian = 0;
                int powtorzenia = 3;

                for (int r = 0; r < powtorzenia; r++) {
                    l_porownan = 0;
                    l_zamian = 0;

                    kopia_tablicy(t1, t_wzor, n);

                    auto start = steady_clock::now();

                    if (algo == 0) shell_sort(t1, n, 0);
                    else if (algo == 1) shell_sort(t1, n, 1);
                    else if (algo == 2) shell_sort(t1, n, 2);
                    else if (algo == 3) quick_sort(t1, 0, n-1, 0);
                    else if (algo == 4) quick_sort(t1, 0, n-1, 1);
                    else if (algo == 5) quick_sort(t1, 0, n-1, 2);

                    auto end = steady_clock::now();
                    auto elapsed_us = duration_cast<microseconds>(end - start).count();

                    suma_czas += elapsed_us;
                    suma_porownan += l_porownan;
                    suma_zamian += l_zamian;
                }

                string nazwa_algo;
                if (algo == 0) nazwa_algo = "ShellSort (Shell)";
                else if (algo == 1) nazwa_algo = "ShellSort (Hibbard)";
                else if (algo == 2) nazwa_algo = "ShellSort (Tokuda)";
                else if (algo == 3) nazwa_algo = "QuickSort (Right)";
                else if (algo == 4) nazwa_algo = "QuickSort (Random)";
                else if (algo == 5) nazwa_algo = "QuickSort (Median3)";

                plik << nazwa_algo << ";" << nazwa_typu << ";" << n << ";"
                     << (suma_czas / powtorzenia) << ";"
                     << (suma_porownan / powtorzenia) << ";"
                     << (suma_zamian / powtorzenia) << "\n";
            }
        }
        delete[] t_wzor;
        delete[] t1;
    }

    plik.close();
    cout << "Gotowe" << endl;

    return 0;
}
