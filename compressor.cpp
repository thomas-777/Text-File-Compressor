#include<string>
#include<vector>
#include <queue>
#include<fstream>
#include<iostream>

using namespace std;

struct Node
{
    char data;
    unsigned freq;
    Node* left;
    Node* right;
    string code;
    Node(){
        left = right = NULL;
    }
};

struct compare
{
    bool operator()(Node* l, Node* r)
    {
        return l->freq > r->freq;
    }
};

vector<Node*> arr;

fstream inFile,outFile;

string inFileName,outFileName;

Node* root=nullptr;

priority_queue<Node*, vector<Node*>, compare> minHeap;

void createarr(){
    for(int i = 0; i < 128; i++){
        arr.push_back(new Node());
        arr[i]->data = i;
        arr[i]->freq = 0;
    }
}
int binToDec(string inStr){
    int res=0;
    for(auto c:inStr){
        res=res*2 +c -'0';
    }
    return res;
}


void traverse(Node* root, string str){
    if(root->left == NULL && root->right == NULL){
        root->code=str;
        return;
    }
    traverse(root->left, str + '0');
    traverse(root->right, str + '1');
}

void createminheap(){
    char id;
    inFile.open(inFileName,ios::in);
    inFile.get(id);
    while(!inFile.eof()){
        arr[id]->freq++;
        inFile.get(id);
    }
    inFile.close();

    for(int i = 0; i < 128; i++){
        if(arr[i]->freq > 0){
            minHeap.push(arr[i]);
        }
    }

}

void createtree(){
    Node *left,*right;

    priority_queue<Node*, vector<Node*>, compare> temp (minHeap);
    while(temp.size() > 1){
        left = temp.top();
        temp.pop();
        right = temp.top();
        temp.pop();

        Node* top = new Node();
        top->freq = left->freq + right->freq;
        top->left = left;
        top->right = right;
        temp.push(top);
    }
    root=temp.top();
}

void createcode(){
    traverse(root,"");
}

void saveencodedfile(){
    char id;
    inFile.open(inFileName,ios::in);
    outFile.open(outFileName,ios::out | ios::binary);
    string in="";
    string s="";
    
    in += (char)minHeap.size();
    priority_queue <Node* ,vector<Node*>,compare> tempPQ(minHeap);
    while(!tempPQ.empty()){
        Node *curr=tempPQ.top();
        in += curr->data;
        s.assign(127-curr->code.length(),'0');
        s+='1';
        s += curr->code;

        in+=(char)binToDec(s.substr(0,8));
        for(int i=0;i<15;i++){
            s=s.substr(8);
            in+=(char)binToDec(s.substr(0,8));
        }
        tempPQ.pop();
    }
    s.clear();

    inFile.get(id);
    while(!inFile.eof()){
        s+=arr[id]->code;
        while(s.length()>8){
            in+=(char)binToDec(s.substr(0,8));
            s=s.substr(8);
        }
        inFile.get(id);
    }

    int count = 8 - s.length();
	if (s.length() < 8) {
		s.append(count, '0');
	}
	in += (char)binToDec(s);	
    in += (char)count;

    outFile.write(in.c_str(),in.size());
    inFile.close();
    outFile.close();
}



void compress(){
    createminheap();
    createtree();
    createcode();
    saveencodedfile();
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Failed" << endl;
        exit(1);
    }

    inFileName = argv[1];
    outFileName = argv[2];
    createarr();

    compress();

    cout << "File compressed successfully"<< endl;
    return 0;
}

