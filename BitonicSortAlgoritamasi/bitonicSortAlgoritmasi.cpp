#include <iostream> 
#include <cstdlib>
#include <time.h>
#include <omp.h>
using namespace std;// Bu sat�rda using kodu herhangi bir komutu tan�mlamak i�in namespace ise bu gibi komutlar�n hepsini birlikte tan�mlamaya imkan verir.

void artanSiralama(int index1, int index2, int* array) //Dizide artan s�rada g�r�necek �ekilde iki de�eri de�i�tirin
{
    if (array[index2] < array[index1])
    {
        int temp = array[index2];
        array[index2] = array[index1];
        array[index1] = temp;
    }
}
void azalanSiralama(int index1, int index2, int* array) // Dizide azalan s�rada g�r�necek �ekilde iki de�eri de�i�tirin
{
    if (array[index1] < array[index2])
    {
        int temp = array[index2];
        array[index2] = array[index1];
        array[index1] = temp;
    }
}
//S�ralanacak s�ra, alt dizin konumundan ba�lar.
void bitonikDizidenbitonikSiralama(int startIndex, int lastIndex, int dir, int* array) //��leve bitonik girdi verildi�inde artan veya azalan bir dizi olu�tur
{
    if (dir == 1) //Artan S�ralama
    {
        int counter = 0; // de�i�tirilmi� ��eleri takip etmek i�in saya�
        int noOfElements = lastIndex - startIndex + 1; //lastIndex = 7 ise ve startIndex = 0 ise noOfElements = 8
        for (int j = noOfElements / 2; j > 0; j = j / 2) //(j=8/2 = 4; j>0; j = j/2)
        {
            counter = 0;
            for (int i = startIndex; i + j <= lastIndex; i++)
            {
                if (counter < j)
                {
                    artanSiralama(i, i + j, array); //artan s�ralama yapt�ysak
                    counter++; //counter 1 artt�r
                }
                else //Artan s�ralama yapmad�ysak
                {
                    counter = 0; //counter 0'd�r
                    i = i + j - 1; // i = 0+4-1 = 3
                }
            }
        }
    }
    else // Azalan S�ralama
    {
        int counter = 0;
        int noOfElements = lastIndex - startIndex + 1;
        for (int j = noOfElements / 2; j > 0; j = j / 2)
        {
            counter = 0;
            for (int i = startIndex; i <= (lastIndex - j); i++)
            {
                if (counter < j)
                {
                    azalanSiralama(i, i + j, array); //Azalan s�ralama yapt�ysak
                    counter++; //counter 1 artt�r
                }
                else //Azalan s�ralama yapmad�ysak
                {
                    counter = 0; //counter 0'd�r
                    i = i + j - 1;
                }
            }
        }
    }
}

void bitonicSiralamaSeri(int startIndex, int lastIndex, int* array) //Rastgele bir d�zenden bitonik bir dizi olu�turun
{
    int noOfElements = lastIndex - startIndex + 1;
    for (int j = 2; j <= noOfElements; j = j * 2) // (j = 2 ise j yeni de�er 4; j = 3 ise j yeni de�er 6)
    {

        for (int i = 0; i < noOfElements; i = i + j)
        {
            if (((i / j) % 2) == 0)
            {
                bitonikDizidenbitonikSiralama(i, i + j - 1, 1, array); //Artan y�nde s�ralama
            }
            else
            {
                bitonikDizidenbitonikSiralama(i, i + j - 1, 0, array); //Azalan y�nde s�ralama
            }
        }
    }
}
//Bitonic S�ra Olu�turucu
void bitonicSiralamaParallel(int startIndex, int lastIndex, int* array) //Rastgele bir d�zenden bitonik bir dizi olu�turun
{
    int noOfElements = lastIndex - startIndex + 1;
    for (int j = 2; j <= noOfElements; j = j * 2) // (j = 2 ise j yeni de�er 4; j = 3 ise j yeni de�er 6)
    {
#pragma omp parallel for 
        for (int i = 0; i < noOfElements; i = i + j)
        {
            if (((i / j) % 2) == 0)
            {
                bitonikDizidenbitonikSiralama(i, i + j - 1, 1, array); //Artan y�nde s�ralama
            }
            else
            {
                bitonikDizidenbitonikSiralama(i, i + j - 1, 0, array); //Azalan y�nde s�ralama
            }
        }
    }
}
int main()
{
    omp_set_dynamic(0);                           // ��letim sisteminin i� par�ac��� ayarlar�n� ge�ersiz k�lmamas� i�in devre d��� b�rak�ld�
    int maxNumberOfThreads = omp_get_num_procs(); // Mant�ksal �ekirdek say�s� verir
    omp_set_num_threads(maxNumberOfThreads);      // �� par�ac��� say�s�n� ayarlay�n

    int n;
    cout << "Siralanacak ogelerin sayisini girin (2^n seklinde olmalidir) : ";
    cin >> n;

    int* array = new int[n];

    srand(time(NULL)); //Farkl� de�erlerin gelmesi i�in
    for (int i = 0; i < n; i++)
    {
        array[i] = i + rand() % 1000;
    }

    printf("\n");
    double start, end; //s�re ba�lang�c� ve biti�i

    start = omp_get_wtime();
    //--- omp_get_wtime : Belirli bir noktadan ge�en s�renin saniye cinsinden bir de�er d�nd�r�r---

    bitonicSiralamaSeri(0, n - 1, array);
    end = omp_get_wtime();
    printf("\n****************");
    cout << "\nSeri Hesaplama icin gecen sure: " << end - start << endl;
    printf("\n***************");

    start = omp_get_wtime();
    //--- omp_get_wtime : Belirli bir noktadan ge�en s�renin saniye cinsinden bir de�er d�nd�r�r---

    bitonicSiralamaParallel(0, n - 1, array);
    end = omp_get_wtime();
    printf("\n****************");
    cout << "\nParalel Hesaplama icin gecen sure: " << end - start << endl;
    printf("\n****************");
}