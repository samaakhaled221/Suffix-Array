#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <algorithm>

using namespace std;

#define DIRECTION "D:/files"

int N = 1e6;
int *suffix = new int[N];
char *arr = new char[N];
int sz=100;  //==x
void readsequence(int x)  // function to read from a file
{
    char *buff = new char[N]; // array to store the reading data
    FILE *file = fopen("D:/files/genome.fasta", "r");
    buff[0] = 0;
    fscanf(file, "%[^\n\r] ", buff); // Ignore this line
    int num_reads = 0;
    while (num_reads < x) // while loop will stop when the data size is equal to the size passed to the function.
    {
        buff[0] = 0;
        fscanf(file, "%[^\n\r] ", buff);
        if (buff[0] == '>')  //the reading will stop if it cames to the next id
        {
            break;
        }
        for (int i = 0; buff[i] != '\0'; i++)
        {
            arr[num_reads] = buff[i];
            num_reads++;
            if (num_reads == x)
            {
                break;
            }
        }
    }
    sz=num_reads;

    fclose(file);
    fflush(NULL);
    delete[] buff;
}

void write(const char name[]) // function to write in a file which is already created.
{
    FILE *file = fopen(name, "w");

    for (int i = 0; i < sz; i++)
    {
        fprintf(file, " %d\n", suffix[i]);
    }

    fclose(file);
}

void print()
{
    for (int i = 0; i < sz; i++) //  print sorted suffix array
    {
        cout << suffix[i] << "  " ;
    }
    cout<<endl;
}


//function  to compare between characters and return the character
//with the smallest lexographically minimum between 2 strings , it takes to integers of the indeces
//which the function will compare strated from them to the end of the char array.
bool comparechar(int i, int j)
{
    for (; i < sz && j < sz; i++, j++)
    {
        if (arr[i] > arr[j])   //return 0 if the first char is greater than the second one
        {
            return 0;
        }
        else if (arr[i] < arr[j]) //return 1 if the first char is smaller than the second one
        {
            return 1;
        }
        else
        {
            continue;     //continue if the 2 characters is equal to each other
        }
    }
    return i == sz ? 1 : 0;   //return 1 if the first character equal to the size which mean
}                             //it comes to the end of the array and return 0 if not.

void suffixArrayNaive()
{

    for (int i = 0; i < sz; i++) // for loop to store indecies to suffix array before sorting .
    {
        suffix[i] = i;
    }

    sort(suffix, suffix + sz, comparechar);  //sort the array alphabitcally from the smallest
                                             // to the greatest based on the compare function.

    cout << "NAIVE SUFFIX ARRAY:" << endl;

}


//--------------------------PREFIX DOUBLING-----------------------


struct SA
{
    int index;
    int order[2];
};

//function to compare between 2 suffixes and return the smallest lexographically minimum.
bool compare(SA i, SA j)
{
    if (i.order[0] == j.order[0])
    {

        return i.order[1] < j.order[1];
    }
    else
    {
        return i.order[0] < j.order[0];
    }

}

class SuffixArr
{
    struct SA *sufarr = new SA[sz];


public:
    //constructor taking char arr and store the suffixes in an array with thier indecies
    //in an alphabitcal way
    SuffixArr(char arr[])
    {

        for (int i = 0; i < sz; i++)
        {
            sufarr[i].index = i;
            sufarr[i].order[0] = arr[i] - 97;
            sufarr[i].order[1] = ((i + 1) < sz) ? (arr[i + 1] - 97) : -1;
        }
    }


    void prefixDoubling()
    {
        //sort the suffix according to the first 2 characters based on the compare function
        sort(sufarr, sufarr + sz, compare);
        int *indeces = new int[sz]; // array to store the index of suffixes
        for (int i = 4; i < 2 * sz; i *= 2)
        {
            int order = 0;
            int previous = sufarr[0].order[0];
            sufarr[0].order[0] = order;
            indeces[sufarr[0].index] = 0;
            int j = 1, z = 0;

            while (j < sz)
            {
                //f the current and next order is equal to the prev one then don't change the order
                if (sufarr[j].order[0] == previous && sufarr[j].order[1] == sufarr[j - 1].order[1])
                {
                    previous = sufarr[j].order[0];
                    sufarr[j].order[0] = order;
                }
                else
                {
                    //increase the order by one
                    previous = sufarr[j].order[0];
                    sufarr[j].order[0] = ++order;
                }
                indeces[sufarr[j].index] = j;
                j++;
            }
            while (z < sz) // store and assign the next order in every suffix of the suffixes.
            {
                int next_index = sufarr[z].index + i / 2;
                if (next_index < sz)
                {
                    sufarr[z].order[1] = sufarr[indeces[next_index]].order[0];
                }
                else
                {
                    sufarr[z].order[1] = -1;
                }
                z++;
            }
            // sort the suffix accordint to the first 4,8 ,... i characters.
            sort(sufarr, sufarr + sz, compare);
        }

        //loop to store the sorted indecies of the suffixes in auffix array
        for (int i = 0; i < sz; i++)
        {
            suffix[i] = sufarr[i].index;
        }
        cout << "PREFIX DOUBLING:" << endl;
        delete[] indeces;

    }
    ~SuffixArr()
    {
        delete[] sufarr;
    }

};


int main()
{

    /*readsequence(1000);
    suffixArrayNaive();
    write("D:/files/output1");

    readsequence(10000);
    SuffixArr zz(arr);
    zz.prefixDoubling();
    write("D:/files/pdSuffix");


    readsequence(N);
    SuffixArr cc(arr);
    cc.prefixDoubling();
    write("D:/files/wholegenome");*/



    //------ TEST CASES----------------

    SuffixArr ar("GAACGCCTAT$");     // out1:10  1  2  8  5  3  6  0  4  9  7
    sz = strlen("GAACGCCTAT$");
    ar.prefixDoubling();
    print();

    /* arr = "GACGATATT$";     //out2: 9  1  4  6  2  0  3  8  5  7
     sz = strlen(arr);
     suffixArrayNaive();
     print();


     SuffixArr z("CAACTCATGAT$");     // out3:11  1  2  9  6  0  5  3  8  10  4  7
     sz = strlen("CAACTCATGAT$");
     z.prefixDoubling();
     print();


     arr = "CAGGATACCT$"; // out4:10  6  1  4  0  7  8  3  2  9  5
     sz = strlen(arr);
     suffixArrayNaive();
     print();


     SuffixArr r("TTGTGGATAA$");     // out5:10  9  8  6  5  4  2  7  3  1  0
     sz = strlen("TTGTGGATAA$");
     r.prefixDoubling();
     print();


     SuffixArr b("ACACAGGACC$");     // out6:10  0  2  7  4  9  1  3  8  6  5
     sz = strlen("ACACAGGACC$");
     b.prefixDoubling();
     print();


     arr = "ACACAGGACC$";    //  out7:10  0  2  7  4  9  1  3  8  6  5
     sz = strlen(arr);
     suffixArrayNaive();
     print();

     SuffixArr d("CAACAAGAGGG"); //out8:1  4  2  5  7  0  3  10  6  9  8
     sz = strlen("CAACAAGAGGG");
     d.prefixDoubling();
     print();

     SuffixArr tt("TCATCAGTTGTAAA$");   // out9:14  13  12  11  5  2  4  1  9  6  10  3  0  8  7
     sz = strlen("TCATCAGTTGTAAA$");
     tt.prefixDoubling();
     print();

     SuffixArr m("TAAATAGTTATG");     // out10:1  2  5  3  9  11  6  0  4  8  10  7
     sz = strlen("TAAATAGTTATG");
     m.prefixDoubling();
     print();

     arr = "CAATAGATGAAGTC"; // out11:9  1  4  10  2  6  13  0  8  5  11  3  12  7
     sz = strlen(arr);
     suffixArrayNaive();
     print();

     arr = "GGTTTTGGGTTATTT"; // out12:11  6  7  0  8  1  14  10  5  13  9  4  12  3  2
     sz = strlen(arr);
     suffixArrayNaive();
     print();

     SuffixArr q("GAAAGAAACAAA");    // out13:11  10  9  5  1  6  2  7  3  8  4  0
     sz = strlen("GAAAGAAACAAA");
     q.prefixDoubling();
     print();

     arr = "AAAAGGATAGTGACG"; // out14:0  1  2  12  3  8  6  13  14  11  5  4  9  7  10
     sz = strlen(arr);
     suffixArrayNaive();
     print();

     SuffixArr w("GCAGATGAAAAGGT");     //out15:7  8  9  2  10  4  1  6  3  0  11  12  13  5
     sz = strlen("GCAGATGAAAAGGT");
     w.prefixDoubling();
     print();

     arr = "TTTCATTTTT";   //out16: 4  3  9  2  8  1  7  0  6  5
     sz = strlen(arr);
     suffixArrayNaive();
     print();


     SuffixArr zz("TTTTTTAAAAACCCC");     // out17:6  7  8  9  10  14  13  12  11  5  4  3  2  1  0
     sz = strlen("TTTTTTAAAAACCCC");
     zz.prefixDoubling();
     print();

     arr = "GACAAGATATAGATATT";     //out18: 3  1  4  10  8  6  12  14  2  0  5  11  16  9  7  13  15
     sz = strlen(arr);
     suffixArrayNaive();
     print();

     SuffixArr f("TCCCGTCTAATAAT");  // out19:11  8  12  9  1  2  3  6  4  13  10  7  0  5
     sz = strlen("TCCCGTCTAATAAT");
     f.prefixDoubling();
     print();

     SuffixArr g("GCTGAGATGGAAAGAC");     // out20:10  11  14  12  4  6  15  1  9  13  3  5  0  8  2  7
     sz = strlen("GCTGAGATGGAAAGAC");
     g.prefixDoubling();
     print();

     SuffixArr h("GAGCTTGCAGA");     // out21:10  8  1  7  3  9  0  6  2  5  4
     sz = strlen("GAGCTTGCAGA");
     h.prefixDoubling();
     print();

     arr = "TAAAGATGATTGCTTTTCT";     //out22:1  2  3  5  8  17  12  4  7  11  18  0  16  6  10  15  9  14  13
     sz = strlen(arr);
     suffixArrayNaive();
     print();

     arr = "TTTTAACGGCCGGC";     //out23:4  5  13  9  10  6  12  8  11  7  3  2  1  0
     sz = strlen(arr);
     suffixArrayNaive();
     print();

     SuffixArr kk("TTTGCCGTAACTG");   // out24:8  9  4  5  10  12  3  6  7  11  2  1  0
     sz = strlen("TTTGCCGTAACTG");
     kk.prefixDoubling();
     print();

     arr = "AAAAAGAAGA";     //out25: 9  0  1  2  6  3  7  4  8  5
     sz = strlen(arr);
     suffixArrayNaive();
     print();

     SuffixArr nn("CAAATTAGTGAGG");    // out26:1  2  10  6  3  0  12  9  11  7  5  8  4
     sz = strlen("CAAATTAGTGAGG");
     nn.prefixDoubling();
     print();

     arr = "AACTCCTATTTATTT";     //out27: 0  1  11  7  4  5  2  14  10  6  3  13  9  12  8
     sz = strlen(arr);
     suffixArrayNaive();
     print();


     arr = "TTTTTGTTATTT";     //out28:8  5  11  7  4  10  6  3  9  2  1  0
     sz = strlen(arr);
     suffixArrayNaive();
     print();

     SuffixArr y("TTGTTACTAAAACAA"); // out29:14  13  8  9  10  11  5  12  6  2  7  4  1  3  0
     sz = strlen("TTGTTACTAAAACAA");
     y.prefixDoubling();
     print();


     SuffixArr e("AAAAAAATGT");   // out30:0  1  2  3  4  5  6  8  9  7
     sz = strlen("AAAAAAATGT");
     e.prefixDoubling();
     print();

     SuffixArr j("GAATTGACAACA");   // out31:11  8  1  9  6  2  10  7  0  5  4  3
     sz = strlen("GAATTGACAACA");
     j.prefixDoubling();
     print();

     SuffixArr ff("CAAATAAAAT");   // out32:5  6  1  7  2  8  3  0  9  4
     sz = strlen("CAAATAAAAT");
     ff.prefixDoubling();
     print();*/

    delete[] suffix;  // delete the dynamic arrays to free the memory storage.
    delete[] arr;



    return 0;
}
//time of the NAIVE function = 0.091 s
//time of prefix doubling function= 0.014 s








