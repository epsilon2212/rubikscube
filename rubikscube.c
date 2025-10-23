
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef N
#define N 5
#endif
#define POCZATKOWY_ROZMIAR_DANYCH 20 // sizeof(char) * 20 = 4 * 20 = 80, zatem na dobry poczatek mozemy trzymac 20 znakow.
#define LICZBA_SCIANEK 6

void drukuj_kostke(int Kostka[LICZBA_SCIANEK][N][N])
{
    printf("\n");
    for (int i = 0; i < N; i++) //pierwsza partycja
    {
        for (int j = 0; j <= N; j++)
        {
            printf(" ");
        }
        for (int j = 0; j < N; j++)
        {
            printf("%d", Kostka[0][i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < N; i++) //druga partycja
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d", Kostka[1][i][j]);
        }
        printf("|");
        for (int j = 0; j < N; j++)
        {
            printf("%d", Kostka[2][i][j]);
        }
        printf("|");
        for (int j = 0; j < N; j++)
        {
            printf("%d", Kostka[3][i][j]);
        }
        printf("|");
        for (int j = 0; j < N; j++)
        {
            printf("%d", Kostka[4][i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < N; i++) //trzecia partycja
    {
        for (int j = 0; j <= N; j++)
        {
            printf(" ");
        }
        for (int j = 0; j < N; j++)
        {
            printf("%d", Kostka[5][i][j]);
        }
        printf("\n");
    }
}

//dynamicznie alokuje tablice i zapisuje w niej komendy od uzytkownika
char* zczytaj_komendy(size_t *indeks_wyjscia)
{
    //alokujemy tablice na komendy
    char* dane = malloc(POCZATKOWY_ROZMIAR_DANYCH * sizeof(char));
    if (dane == NULL)
    {
        return NULL;
    }

    int znak;
    size_t indeks = 0; //kazdy indeks to jeden znak w tablicy dane[] (w tym nowe linie, gdzie nowa linia jest repreentowana przez 'x')

    size_t max_dlugosc = POCZATKOWY_ROZMIAR_DANYCH; // narazie trzymamy 20 znakow.

    int kontynuuj_czytanie  = 1;

    while (kontynuuj_czytanie)
    {
        while ((znak = getchar()) != '\n' && znak != EOF && znak != '.') //wczytujemy wszystkie dozwolone znaki
        {
            indeks++;
            if (indeks + 2 >= max_dlugosc) //powiekszamy rozmiar tablicy jesli zaraz zabraknie miejsca.
            {
                max_dlugosc *= 2;
                char *tymczasowy = realloc(dane, (size_t)max_dlugosc * sizeof(char));

                if (tymczasowy == NULL) {
                    free(dane);
                    return NULL;
                }
                dane = tymczasowy;
            }

            dane[indeks-1] = (char) znak; //zapisuje w tablicy

        }
        if (znak == '.')
        {
            dane[indeks] = '\0';
            kontynuuj_czytanie = 0;
        }
        else
        {
            dane[indeks] = 'x'; // rownowaznosc z '\n'
            indeks++;
        }
    }
    *indeks_wyjscia = indeks;

    return dane;
}

int potega(int k) //zwraca 10^k
{
    int wynik = 1;
    for (int i = 0; i < k; i++)
    {
        wynik = wynik * 10;
    }
    return wynik;
}

//obraca sciane na ktora sie patrzymy przy obracaniu
void obrot_frontowej_sciany(int numer_sciany, int kat_obrotu, int Kostka[LICZBA_SCIANEK][N][N])
{
    int tymczasowa[N][N];

    for (int numer_obrotu = 0; numer_obrotu < kat_obrotu; numer_obrotu++)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                tymczasowa[j][N - i - 1] = Kostka[numer_sciany][i][j];
            }
        }
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                Kostka[numer_sciany][i][j] = tymczasowa[i][j];
            }
        }
    }
}

//wykonuje komende na kostce.
void przeksztalc_kostke(int liczba_warstw, int numer_sciany, int kat_obrotu, int Kostka[LICZBA_SCIANEK][N][N])
{
    //obraca frontowa scianke
    obrot_frontowej_sciany(numer_sciany, kat_obrotu, Kostka);

    //jezeli licczawarstw == N, to jest to jedyny przypadek gdy cala kostka zmieni swoja orientacje wzgledem stałej osi, zatem przeciwna scianka do wybranej tez sie zmieni
    if (liczba_warstw == N)
    {
        int numer_sciany_przeciwnej;

        if (numer_sciany == 0)
        {
            numer_sciany_przeciwnej = 5;
        }
        else if (numer_sciany == 1)
        {
            numer_sciany_przeciwnej = 3;
        }
        else if (numer_sciany == 2)
        {
            numer_sciany_przeciwnej = 4;
        }
        else if (numer_sciany == 3)
        {
            numer_sciany_przeciwnej = 1;
        }
        else if (numer_sciany == 4)
        {
            numer_sciany_przeciwnej = 2;
        }
        else //5
        {
            numer_sciany_przeciwnej = 0;
        }
        //(4 - kat obrotu) to -(kat obrotu)
        obrot_frontowej_sciany(numer_sciany_przeciwnej, 4-kat_obrotu, Kostka);
    }
    for (int w = 0; w < liczba_warstw; w++) //obracanie scianek sasiadujacych z frotnowa scianka (kazda warstwa osobno)
    {
        for (int j= 0; j < kat_obrotu; j++) // kazdy obrot o 90 stopni osobno.
        {
            int tymczasowa_kolumna[N];

            // mamy "zamkniete"  niezalezne algorytmy zamiany kolumn/wierszy w sciankach, co gwarantuje nam poprawnosc zamiany elementow dla danej scianki frontowej.
            if (numer_sciany == 0)
            {
                for (int i = 0; i < N; i++)
                {
                    tymczasowa_kolumna[i] = Kostka[2][w][i];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[2][w][i] = Kostka[3][w][i];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[3][w][i] = Kostka[4][w][i];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[4][w][i] = Kostka[1][w][i];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[1][w][i] = tymczasowa_kolumna[i];
                }
            }
            else if (numer_sciany == 1)
            {
                for (int i = 0; i < N; i++)
                {
                    tymczasowa_kolumna[i] = Kostka[0][i][w];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[0][i][w] = Kostka[4][N-i-1][N-w-1];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[4][N-i-1][N-w-1] = Kostka[5][i][w];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[5][i][w] = Kostka[2][i][w];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[2][i][w] = tymczasowa_kolumna[i];
                }
            }
            else if (numer_sciany == 2)
            {
                for (int i = 0; i < N; i++)
                {
                    tymczasowa_kolumna[i] = Kostka[0][N-w-1][i];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[0][N-w-1][i] = Kostka[1][N-i-1][N-w-1];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[1][i][N-w-1] = Kostka[5][w][i];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[5][w][i] = Kostka[3][N-i-1][w];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[3][i][w] = tymczasowa_kolumna[i];
                }
            }
            else if (numer_sciany == 3)
            {
                for (int i = 0; i < N; i++)
                {
                    tymczasowa_kolumna[i] = Kostka[0][i][N-w-1];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[0][i][N-w-1] = Kostka[2][i][N-w-1];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[2][i][N-w-1] = Kostka[5][i][N-w-1];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[5][i][N-w-1] = Kostka[4][N-i-1][w];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[4][N-i-1][w] = tymczasowa_kolumna[i];
                }
            }
            else if (numer_sciany == 4)
            {
                for (int i = 0; i < N; i++)
                {
                    tymczasowa_kolumna[i] = Kostka[0][w][i];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[0][w][i] = Kostka[3][i][N-w-1];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[3][i][N-w-1] = Kostka[5][N-w-1][N-i-1];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[5][N-w-1][i] = Kostka[1][i][w];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[1][N-i-1][w] = tymczasowa_kolumna[i];
                }
            }
            else //5
            {
                for (int i = 0; i < N; i++)
                {
                    tymczasowa_kolumna[i] = Kostka[2][N-w-1][i];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[2][N-w-1][i] = Kostka[1][N-w-1][i];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[1][N-w-1][i] = Kostka[4][N-w-1][i];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[4][N-w-1][i] = Kostka[3][N-w-1][i];
                }
                for (int i = 0; i < N; i++)
                {
                    Kostka[3][N-w-1][i] = tymczasowa_kolumna[i];
                }
            }
        }
    }
}

//rozczytuje ciag znakow zezczytanej tablicy, rozbija na pojedyncze komendy i przekazuje do funkcji przeksztalcajacej kostke.
void wykonaj_komendy(const int rozmiar_tablicy_danych, char dane[], int Kostka[LICZBA_SCIANEK][N][N])
{
    int licznik = 0;
    while (licznik < rozmiar_tablicy_danych)
    {

        if (dane[licznik] == 'x') // nowa linia.
        {
            drukuj_kostke(Kostka);
            licznik++;
        }
        else
        {
            int liczba_warstw = 0;
            int ile_cyfr = 0;
            int numer_sciany = -1;
            int kat_obrotu = 1; //wartosc 1 to obrot o kat 90 w prawo, 2 to 180 w prawo itd...

            //zliczamy liczbe cyfr pod rzad. jezeli cyfry nie ma, to pomijamy petle i liczba_warstw ==1;
            while (licznik + ile_cyfr < rozmiar_tablicy_danych && isdigit(dane[licznik+ile_cyfr]))
            {
                ile_cyfr++;
            }
            for (int k = 0; k < ile_cyfr; k++) //obliczamy sume tych cyfr
            {
                liczba_warstw += (dane[licznik + k] - '0') * potega(ile_cyfr - k - 1);
            }

            if (liczba_warstw == 0 || liczba_warstw > N)
            {
                liczba_warstw = 1;
            }

            //wiecej cyfr pod rzad juz nie ma, czyli nastepny znak to na pewno litera oznaczajaca scianke kostki.
            if (ile_cyfr > 0)
            {
                licznik = licznik + ile_cyfr;
            }

            if (dane[licznik] == 'u') numer_sciany = 0;
            else if (dane[licznik] == 'l') numer_sciany = 1;
            else if (dane[licznik] == 'f') numer_sciany = 2;
            else if (dane[licznik] == 'r') numer_sciany = 3;
            else if (dane[licznik] == 'b') numer_sciany = 4;
            else if (dane[licznik] == 'd') numer_sciany = 5;

            //sprawdzamy, czy nastepny znak okresla nam kat obrotu
            if (licznik + 1 < rozmiar_tablicy_danych)
            {
                if (dane[licznik+1] == '"')
                {
                    kat_obrotu = 2;
                    licznik++;
                }
                else if (dane[licznik+1] == '\'')
                {
                    kat_obrotu = 3;
                    licznik++;
                }
            }

            if(numer_sciany > -1) przeksztalc_kostke(liczba_warstw, numer_sciany, kat_obrotu, Kostka);

            licznik++;
        }
    }
}

int main(void)
{
    size_t rozmiar_tablicy;

    char* dane = zczytaj_komendy(&rozmiar_tablicy); //najpierw czytamy dane.

    if (dane == NULL)
    {
        return 1;
    }
    const int rozmiar_tablicy_danych = (int) rozmiar_tablicy; //zamiana z unsigned int na int w celu wykonywania obliczeń

    int Kostka[LICZBA_SCIANEK][N][N]; //traktujemy jako 6 macierzy NxN, kazdy kolor [0...5] to pierwsza zmienna, pozostale to zadane wymiary.

    for (int i = 0; i < LICZBA_SCIANEK; i++) //wypelnia scianki kostki.
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                Kostka[i][j][k] = i;
            }
        }
    }

    wykonaj_komendy(rozmiar_tablicy_danych, dane, Kostka);

    free(dane);
    return 0;
}