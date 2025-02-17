#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

struct WordData
{
    string surface;          // 表層形
    string pos;              // 品詞(名詞、動詞、形容詞、副詞、助詞、助動詞、接続詞、感動詞)
    string conjugationForm;  // 活用形(未然形、連用形、終止形、連体形、已然形、命令形)
    string conjugationType;  // 活用型(五段、カ変、サ変)
    string pronunciation;    // 読み(カタカナ)
    int leftConnectionId;  // 左文脈ID
    int rightConnectionId; // 右文脈ID
    int vitalCost;           // 使用コスト(大きいほど使われにくい)
};

std::vector<WordData> parseFile(const std::string &filename)
{
    vector<WordData> words;
    ifstream file(filename);
    string line;

    while (getline(file, line))
    {
        WordData word;
        vector<string> items;
        stringstream ss(line);
        string item;
        while (getline(ss, item, ','))
        {
            if (item.find('"') != string::npos)
            {
                string nextItem;
                while (getline(ss, nextItem, ','))
                {
                    item += "," + nextItem;
                    if (nextItem.find('"') != string::npos)
                    {
                        break;
                    }
                }
            }
            items.push_back(item);
        }
        // Note: UniDic lex.csv format
        word.surface = items[0];
        word.leftConnectionId = stoi(items[1]);
        word.rightConnectionId = stoi(items[2]);
        word.vitalCost = stoi(items[3]);
        word.pos = items[4];
        word.conjugationForm = items[8];
        word.conjugationType = items[9];
        word.pronunciation = items[13];
        words.push_back(word);
    }
    return words;
}

void printWords(const vector<WordData> &words)
{
    for (const auto &word : words)
    {
        cout << "表層形: " << word.surface << endl;
        cout << "品詞: " << word.pos << endl;
        cout << "活用形: " << word.conjugationForm << endl;
        cout << "活用型: " << word.conjugationType << endl;
        cout << "読み: " << word.pronunciation << endl;
        cout << endl;
    }
}

void writeWords(const vector<WordData> &words)
{
    ofstream file("dictionary.txt");
    for (const auto &word : words)
    {
        file << word.surface << "\t" << word.leftConnectionId << "\t" << word.rightConnectionId << "\t" << word.vitalCost << "\t"
             << word.pos << "\t" << word.conjugationForm << "\t" << word.conjugationType << "\t" << word.pronunciation << endl;
    }
}

int main()
{
    string filename = "lex.csv"; // 入力ファイル名を指定
    vector<WordData> words = parseFile(filename);
    // printWords(words);
    writeWords(words);
    return 0;
}
