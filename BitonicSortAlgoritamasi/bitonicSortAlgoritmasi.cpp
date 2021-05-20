#include <iostream> 
#include <cstdlib>
#include <time.h>
#include <omp.h>
using namespace std;// Bu satýrda using kodu herhangi bir komutu tanýmlamak için namespace ise bu gibi komutlarýn hepsini birlikte tanýmlamaya imkan verir.

void artanSiralama(int index1, int index2, int* array) //Dizide artan sýrada görünecek þekilde iki deðeri deðiþtirin
{
    if (array[index2] < array[index1])
    {
        int temp = array[index2];
        array[index2] = array[index1];
        array[index1] = temp;
    }
}
void azalanSiralama(int index1, int index2, int* array) // Dizide azalan sýrada görünecek þekilde iki deðeri deðiþtirin
{
    if (array[index1] < array[index2])
    {
        int temp = array[index2];
        array[index2] = array[index1];
        array[index1] = temp;
    }
}
//Sýralanacak sýra, alt dizin konumundan baþlar.
void bitonikDizidenbitonikSiralama(int startIndex, int lastIndex, int dir, int* array) //Ýþleve bitonik girdi verildiðinde artan veya azalan bir dizi oluþtur
{
    if (dir == 1) //Artan Sýralama
    {
        int counter = 0; // deðiþtirilmiþ öðeleri takip etmek için sayaç
        int noOfElements = lastIndex - startIndex + 1; //lastIndex = 7 ise ve startIndex = 0 ise noOfElements = 8
        for (int j = noOfElements / 2; j > 0; j = j / 2) //(j=8/2 = 4; j>0; j = j/2)
        {
            counter = 0;
            for (int i = startIndex; i + j <= lastIndex; i++)
            {
                if (counter < j)
                {
                    artanSiralama(i, i + j, array); //artan sýralama yaptýysak
                    counter++; //counter 1 arttýr
                }
                else //Artan sýralama yapmadýysak
                {
                    counter = 0; //counter 0'dýr
                    i = i + j - 1; // i = 0+4-1 = 3
                }
            }
        }
    }
    else // Azalan Sýralama
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
                    azalanSiralama(i, i + j, array); //Azalan sýralama yaptýysak
                    counter++; //counter 1 arttýr
                }
                else //Azalan sýralama yapmadýysak
                {
                    counter = 0; //counter 0'dýr
                    i = i + j - 1;
                }
            }
        }
    }
}

void bitonicSiralamaSeri(int startIndex, int lastIndex, int* array) //Rastgele bir düzenden bitonik bir dizi oluþturun
{
    int noOfElements = lastIndex - startIndex + 1;
    for (int j = 2; j <= noOfElements; j = j * 2) // (j = 2 ise j yeni deðer 4; j = 3 ise j yeni deðer 6)
    {

        for (int i = 0; i < noOfElements; i = i + j)
        {
            if (((i / j) % 2) == 0)
            {
                bitonikDizidenbitonikSiralama(i, i + j - 1, 1, array); //Artan yönde sýralama
            }
            else
            {
                bitonikDizidenbitonikSiralama(i, i + j - 1, 0, array); //Azalan yönde sýralama
            }
        }
    }
}
//Bitonic Sýra Oluþturucu
void bitonicSiralamaParallel(int startIndex, int lastIndex, int* array) //Rastgele bir düzenden bitonik bir dizi oluþturun
{
    int noOfElements = lastIndex - startIndex + 1;
    for (int j = 2; j <= noOfElements; j = j * 2) // (j = 2 ise j yeni deðer 4; j = 3 ise j yeni deðer 6)
    {
#pragma omp parallel for 
        for (int i = 0; i < noOfElements; i = i + j)
        {
            if (((i / j) % 2) == 0)
            {
                bitonikDizidenbitonikSiralama(i, i + j - 1, 1, array); //Artan yönde sýralama
            }
            else
            {
                bitonikDizidenbitonikSiralama(i, i + j - 1, 0, array); //Azalan yönde sýralama
            }
        }
    }
}
int main()
{
    omp_set_dynamic(0);                           // Ýþletim sisteminin iþ parçacýðý ayarlarýný geçersiz kýlmamasý için devre dýþý býrakýldý
    int maxNumberOfThreads = omp_get_num_procs(); // Mantýksal çekirdek sayýsý verir
    omp_set_num_threads(maxNumberOfThreads);      // Ýþ parçacýðý sayýsýný ayarlayýn

    int n;
    cout << "Siralanacak ogelerin sayisini girin (2^n seklinde olmalidir) : ";
    cin >> n;

    int* array = new int[n];

    srand(time(NULL)); //Farklý deðerlerin gelmesi için
    for (int i = 0; i < n; i++)
    {
        array[i] = i + rand() % 1000;
    }

    printf("\n");
    double start, end; //süre baþlangýcý ve bitiþi

    start = omp_get_wtime();
    //--- omp_get_wtime : Belirli bir noktadan geçen sürenin saniye cinsinden bir deðer döndürür---

    bitonicSiralamaSeri(0, n - 1, array);
    end = omp_get_wtime();
    printf("\n****************");
    cout << "\nSeri Hesaplama icin gecen sure: " << end - start << endl;
    printf("\n***************");

    start = omp_get_wtime();
    //--- omp_get_wtime : Belirli bir noktadan geçen sürenin saniye cinsinden bir deðer döndürür---

    bitonicSiralamaParallel(0, n - 1, array);
    end = omp_get_wtime();
    printf("\n****************");
    cout << "\nParalel Hesaplama icin gecen sure: " << end - start << endl;
    printf("\n****************");
}