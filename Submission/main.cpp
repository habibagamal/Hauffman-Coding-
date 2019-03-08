#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <bitset>
#include <math.h>
using namespace std;

int prob [256];

int binaryToDecimal(long long n)
{
    unsigned long long num = n;
    unsigned int dec_value = 0;

    // Initializing base value to 1, i.e 2^0
    int base = 1;

    unsigned long long temp = num;
    while (temp)
    {
        int last_digit = temp % 10;
        temp = temp/10;

        dec_value += last_digit*base;

        base = base*2;
    }

    return dec_value;
}

string decToBinary(int n)
{
    // array to store binary number
    int binaryNum[1000];
    string temp = "";
    // counter for binary array
    int i = 0;
    while (n > 0) {

        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }

    // printing binary array in reverse order
    for (int j = i - 1; j >= 0; j--)
    {
        stringstream ss;
        ss << binaryNum[j];
        string str = ss.str();
        temp = temp + str;
    }
    return temp;
}

struct character { char c; int p; character* left; character* right; bool leaf; string code=""; };
vector <character> characters;
bool operator < (const character& a, const character& b)
{
    return a.p > b.p;
}
priority_queue <character> heap;
struct codes { char c; string code;int p;};
vector <codes> hauffmanCoding;
vector <codes> decompression;

void initilaizeProbability()
{
    for (int i=0; i <256; i++)
        prob[i] =0;
}
void readFile (string filename);
void removeZeros ();
void makeHeap ();
void makeHauffman ();
void printCodes(character* root, int store[], int top);
void printHauffman (character* root);
void InorderTraversal(character* node, char x, string prevCode);
void outputFile (string inp, string outp);
void decompress (string inp, string outp);
void computations ();
int main()
{
    initilaizeProbability();
    string infile, outfile;
    bool compress;
    cout << "Please enter input file path\n";
    cin >> infile;
    cout << "Please enter output file path\n";
    cin >> outfile;
    cout << "Please enter 1 if used for compression. 0 for decompression\n";
    cin >> compress;
    if (compress)
    {
        readFile(infile);
        removeZeros();
        makeHeap ();
        makeHauffman();
        character* top = new character;
        *top = heap.top();
        InorderTraversal(top, 'o', "");
        outputFile (infile, outfile);
        computations();
    }
    else
    {
        decompress(infile, outfile);
    }

}

void readFile (string filename)
{
    ifstream inp;
    inp.open(filename);
    if (inp.is_open())
    {
        string temp;
        getline(inp,temp);
        while (!inp.eof())
        {
            for (int i = 0; i < temp.length(); i++)
            {
                prob[temp[i]] ++;
            }
            getline(inp, temp);
            prob[10]++;
        }
        prob[10]--;
        inp.close();
    }
    else
        cout << "Error Opening File" << endl;
}

void removeZeros ()
{
    for (int i=0; i <256; i++)
    {
        if (prob[i] != 0)
        {
            character temp;
            temp.c = char (i);
            temp.p = prob[i];
            temp.left = nullptr;
            temp.right = nullptr;
            temp.leaf = true;
            characters.push_back(temp);
        }
    }
}

void makeHeap ()
{
    for (int i=0; i < characters.size(); i++)
    {
        heap.push(characters[i]);
    }
}

void makeHauffman()
{
    while(heap.size() != 1)
    {
        character* left = new character;
        character* right = new character;
        character temp;
        temp.p = 0;

        *left = heap.top();
        temp.p += heap.top().p;
        heap.pop();

        *right = heap.top();
        temp.p += heap.top().p;
        heap.pop();

        temp.c = '@';
        temp.left  = left;
        temp.right = right;
        temp.leaf = false;
        heap.push(temp);
    }
}

void InorderTraversal(character* node, char x, string prevCode)
{
    if (node == NULL)
        return;

    if (x == 'r')
        node -> code = prevCode + '1';
    else if (x=='l')
        node -> code = prevCode + '0';


    InorderTraversal(node->left, 'l', node->code);

    if (node->leaf == true)
    {
        codes temp;
        temp.c = node -> c;
        temp.code = node -> code;
        temp.p = node -> p;
        hauffmanCoding.push_back(temp);
    }

    InorderTraversal(node->right, 'r', node -> code);
}

void outputFile (string inp, string outp)
{
    ofstream out;
    out.open(outp, ios::out|ios::binary);
    ifstream in;
    in.open (inp);

    int index = 0;
    for (int i=0; i<hauffmanCoding.size(); i++)
    {
        if (hauffmanCoding[i].c == 10)
        {
            index = i;
        }
    }

    if (out.is_open())
    {
        string yadyelnila = "";
        if (in.is_open())
        {
            string temp;
            string compression = "";
            getline(in,temp);
            while (!in.eof())
            {
                for (int x = 0; x < temp.length(); x++)
                {
                    bool flag = false;
                    for (int i = 0; (i < hauffmanCoding.size()) && !flag; i++)
                    {
                        if (hauffmanCoding[i].c == temp[x])
                        {
                            compression = compression + hauffmanCoding[i].code;
                            flag = true;
                        }
                    }
                }
                getline(in, temp);
                if (!in.eof())
                {
                    compression = compression + hauffmanCoding[index].code;
                }
            }
            in.close();
            int x = 8 - (compression.length() % 8);
            out << x << endl;
            for (int i=0; i<compression.length()+x; i+=8)
            {
                string shit="";
                for (int j= 0; j<8; j++)
                {
                    if (i+j < compression.length())
                        shit = shit + compression[i+j];
                }
                if (shit!="")
                {
                    uint8_t z = binaryToDecimal(stol(shit));
                    out << hex <<(uint8_t) z;
                }
            }
            out << endl <<"T" <<endl;
            for (int i = 0; i < hauffmanCoding.size(); i++)
            {
                out << hauffmanCoding[i].c << hauffmanCoding[i].code << endl;
//                out << hauffmanCoding[i].c;
//
//                if (hauffmanCoding[i].code.length()<=8)
//                {
//                    uint8_t hab = binaryToDecimal(stol(hauffmanCoding[i].code));
//                    out << hex <<(uint8_t) hab << endl;
//                    cout << hex <<(uint8_t) hab << endl;
//                }
            }

        }
        else
        {
            cout << "error opening input file" << endl;
        }


        out.close();
    }
    else
    {
        cout << "error opening output file" << endl;
    }
}

void decompress (string inp, string outp)
{
    fstream out;
    out.open(outp);
    ifstream in;
    in.open (inp, ios::in|ios::binary);

    if (out.is_open())
    {
        string huge="";
        string junk;
        int min;
        int max;
        if (in.is_open())
        {
            max = 0;
            min = 100000000;
            codes temp;
            
            string k;
            getline (in, k);
            int x = stoi(k);
            getline(in, junk);
            
            while (junk!="T")
            {
                int i=0;
                while (i < junk.length())
                {
                    uint8_t u = (uint8_t)junk[i];
                    string mini = decToBinary(u);
                    
                    char c;
                    c = in.peek();
                    while (mini.length() < 8)
                    {
                        if (i != junk.length()-1)
                            mini = "0" + mini;
                        else if (c != 'T')
                                mini = "0" + mini;
                        else
                            if (x == 8)
                                 mini = "0" + mini;
                            else
                            {
                                while (mini.length() < 8-x)
                                       mini =  "0" + mini;
                                break;
                            }
                    }
                    
                    huge = huge + mini;
                    i++;
                }
                getline(in, junk);
            }
            string trial;
            getline(in, trial);
            while (!in.eof())
            {
                if(trial=="")
                {
                    temp.c = 10;
                    getline(in, trial);
                    temp.code="";
                    
                    for (int i=0; i < trial.length(); i++)
                        temp.code = temp.code + trial[i];

                }
                else
                {
                    temp.c = trial[0];
                    temp.code = "";
                    
                    for (int i=1; i < trial.length(); i++)
                        temp.code = temp.code + trial[i];
                }
                if (temp.code.length() > max)
                {
                    max = temp.code.length();
                }

                if (temp.code.length() < min)
                {
                    min = temp.code.length();
                }
                decompression.push_back(temp);


                getline(in, trial);
            }
            in.close();
        }
        else
        {
            cout << "error opening input file" << endl;
        }

        string temp = "";
        for (int i=0; i<huge.length(); i++)
        {
            temp = temp + huge[i];
            while (temp.length() > max)
            {
                temp.erase(temp.begin());
            }

            for (int j=0; (j<decompression.size()) && (temp!= "") && (temp.length() >= min); j++)
            {
                if (temp == decompression[j].code)
                {
                    out << decompression[j].c;
                    temp = "";
                }
            }
        }
        out.close();
    }
    else
    {
        cout << "error opening output file" << endl;
    }
}

void computations ()
{
    int pTotal = 0;
    double l = 0;
    double h=0;
    for (int i=0; i<hauffmanCoding.size(); i++)
    {
        pTotal += hauffmanCoding[i].p;
    }
    for (int i=0; i< hauffmanCoding.size(); i++)
    {
        double probability = (hauffmanCoding[i].p/double(pTotal));
        l += (hauffmanCoding[i].code.length() * probability);
        h += (probability * log2(1.0/probability));
    }
    double efficiency = h / l;
    cout << "<L> = " << l << endl;
    cout << "Compression Ratio = " << l / 8.0 << endl;
    cout << "H = " << h << endl;
    cout << "efficiency = "<<efficiency << endl;
}
