#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
using namespace std;

struct Node
{
    char character;
    int frequency;
    Node *left;
    Node *right;
};

struct comparison
{
    bool operator()(Node *left, Node *right)
    {
        return left->frequency > right->frequency;
    }
};

Node *addNode(char character, int frequency, Node *left, Node *right);
void encode(Node *root, string sourceString, unordered_map<char, string> &huffmanTable);
void decode(Node *root, int &index, string sourceString, string &decodedString);
void buildHuffmanCode(string sourceString);
void printString(string title, string str);
void printHaffmanTable(unordered_map<char, string> huffmanTable);
void printTree(Node *root, string prefix = "", bool isLeft = true);

int main()
{
    buildHuffmanCode("The grass is always greener on the other side");
    return 0;
}

Node *addNode(char character, int frequency, Node *left, Node *right)
{
    Node *newNode = new Node();

    newNode->character = character;
    newNode->frequency = frequency;
    newNode->left = left;
    newNode->right = right;

    return newNode;
}

void encode(Node *root, string binaryCode, unordered_map<char, string> &huffmanTable)
{
    if (!root)
        return;

    if (!root->left && !root->right)
        huffmanTable[root->character] = binaryCode;

    encode(root->left, binaryCode + "0", huffmanTable);
    encode(root->right, binaryCode + "1", huffmanTable);
}

void decode(Node *root, int &index, string binaryCode, string &decodedString)
{
    if (!root)
        return;

    if (!root->left && !root->right)
    {
        decodedString += root->character;
        return;
    }

    index++;

    if (binaryCode[index] == '0')
        decode(root->left, index, binaryCode, decodedString);
    else
        decode(root->right, index, binaryCode, decodedString);
}

void buildHuffmanCode(string sourceString)
{
    unordered_map<char, int> frequencyMap;
    for (char ch : sourceString)
        frequencyMap[ch]++;

    priority_queue<Node *, vector<Node *>, comparison> queue;

    for (auto pair : frequencyMap)
        queue.push(addNode(pair.first, pair.second, nullptr, nullptr));

    while (queue.size() > 1)
    {
        Node *left = queue.top();
        queue.pop();
        Node *right = queue.top();
        queue.pop();

        queue.push(addNode('\0', left->frequency + right->frequency, left, right));
    }

    Node *root = queue.top();

    unordered_map<char, string> huffmanTable;
    encode(root, "", huffmanTable);

    string encodedString = "";
    for (char c : sourceString)
        encodedString += huffmanTable[c];

    string decodedString = "";
    int index = -1;

    while (index < (int)encodedString.size() - 1)
        decode(root, index, encodedString, decodedString);

    printString("\nOriginal string:\n", sourceString);
    cout << '\n';
    printTree(root);
    cout << '\n';
    printHaffmanTable(huffmanTable);
    printString("\nEncoded string:\n", encodedString);
    printString("\nDecoded string:\n", decodedString);
}

void printString(string title, string str)
{
    cout << title << str << '\n';
}

void printHaffmanTable(unordered_map<char, string> huffmanTable)
{
    cout << '\n'
         << "Huffman Table:\n";

    for (auto pair : huffmanTable)
        cout << "[" << pair.first << "]" << " - " << pair.second << '\n';
}

void printTree(Node *root, string prefix, bool isLeft)
{
    if (root == nullptr)
        return;

    cout << prefix;
    cout << (isLeft ? "├── " : "└── ");

    if (root->character == '\0')
        cout << "[" << root->frequency << "]" << '\n';
    else
        cout << "[" << root->character << ":" << root->frequency << "]" << '\n';

    printTree(root->left, prefix + (isLeft ? "│   " : "    "), true);
    printTree(root->right, prefix + (isLeft ? "│   " : "    "), false);
}
