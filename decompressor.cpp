#include<string.h>
#include<fstream>
#include<iostream>
#include<vector>
#include<queue>


using namespace std;

struct Node {
    char data;
    unsigned freq;
    string code;
    Node *left, *right;

    Node() {
        left = right = NULL;
    }
};

vector <Node*> arr;

fstream inFile, outFile;

string inFileName, outFileName;
        
Node *root;
       
class Compare {
       public:
            bool operator() (Node* l, Node* r)
                {
                    return l->freq > r->freq;
                }
        };

priority_queue <Node*, vector<Node*>, Compare> minHeap;


void buildTree(char a_code, string& path) {
    Node* curr = root;
    for (int i = 0; i < path.length(); i++) {
        if (path[i] == '0') {
            if (curr->left == NULL) {
                curr->left = new Node();
            }
            curr = curr->left;
        }
        else if (path[i] == '1') {
            if (curr->right == NULL) {
                curr->right = new Node();
            }
            curr = curr->right;
        }
    }
    curr->data = a_code;
}

string decToBin(int inNum) {
    string temp = "", res = "";
    while (inNum > 0) {
        temp += (inNum % 2 + '0');
        inNum /= 2;
    }
    res.append(8 - temp.length(), '0');
    for (int i = temp.length() - 1; i >= 0; i--) {
        res += temp[i];
    }
    return res;
}

void gettree(){
    inFile.open(inFileName, ios::in | ios::binary);

    unsigned char size;
    inFile.read(reinterpret_cast<char*>(&size), 1);
    root=new Node();
    for(int i=0;i<size;i++){
        char aCode;
        unsigned char hCodeC[16];
        inFile.read(&aCode, 1);
        inFile.read(reinterpret_cast<char*>(hCodeC), 16);
        string hCodeStr = "";
        for (int i = 0; i < 16; i++) {
            hCodeStr += decToBin(hCodeC[i]);
    }
    int j = 0;
        while (hCodeStr[j] == '0') {
            j++;
        }
        hCodeStr = hCodeStr.substr(j+1);
        buildTree(aCode, hCodeStr);
    }
    inFile.close();
}

void saveDecodedFile() {
    inFile.open(inFileName, ios::in | ios::binary);
    outFile.open(outFileName, ios::out);
    unsigned char size;
    inFile.read(reinterpret_cast<char*>(&size), 1);
    inFile.seekg(-1, ios::end);
    char count0;
    inFile.read(&count0, 1);
    inFile.seekg(1 + 17 * size, ios::beg);

    vector<unsigned char> text;
    unsigned char textseg;
    inFile.read(reinterpret_cast<char*>(&textseg), 1);
    while (!inFile.eof()) {
        text.push_back(textseg);
        inFile.read(reinterpret_cast<char*>(&textseg), 1);
    }

    Node *curr = root;
    string path;
    for(int i = 0; i < text.size() - 1; i++) {
        path = decToBin(text[i]);
        if (i == text.size() - 2) {
            path = path.substr(0, 8 - count0);
        }
        for (int j = 0; j < path.size(); j++) {
        
            if (path[j] == '0') {
                curr = curr->left;
            }
            else {
                curr = curr->right;
            }

            if (curr->left == NULL && curr->right == NULL) {
                outFile.put(curr->data);
                curr = root;
            }
        }
    }
    inFile.close();
    outFile.close();
}

void decompress() {
    gettree();
    saveDecodedFile();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Failed" << endl;
        exit(1);
    }

    inFileName = argv[1];
    outFileName = argv[2];

    decompress();

    cout << "File decompressed successfully"<< endl;
    return 0;
}

