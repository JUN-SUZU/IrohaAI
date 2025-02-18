#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <limits>
#include <cmath>
#include "lib/UTF8ConvUtils/util.hpp"
#include "lib/LoadMatrix/util.hpp"
using namespace std;

struct WordInfo
{
    wstring surface;        // 表層形
    string pos;             // 品詞(名詞、動詞、形容詞、副詞、助詞、助動詞、接続詞、感動詞)
    string conjugationForm; // 活用形(未然形、連用形、終止形、連体形、已然形、命令形)
    string conjugationType; // 活用型(五段、カ変、サ変)
    string pronunciation;   // 読み(カタカナ)
    int leftConnectionId;   // 左文脈ID
    int rightConnectionId;  // 右文脈ID
    int vitalCost;          // 使用コスト(大きいほど使われにくい)
};
// 辞書の単語を表す構造体
vector<WordInfo> dictionary;
// 単語の最初の1文字を集めた1つの文字列を作成
wstring runTimeAllWords = L"";

void loadDictionary(const string &filename)
{
    ifstream file(filename);
    string line;
    int count = 0;
    while (getline(file, line))
    {
        WordInfo word;
        vector<string> items;
        stringstream ss(line);
        string item;
        while (getline(ss, item, '\t'))
        {
            items.push_back(item);
        }
        wstring wordSurface = Utf8ConvUtil::utf8ToWstring(items[0]);
        word.surface = wordSurface;
        word.leftConnectionId = stoi(items[1]);
        word.rightConnectionId = stoi(items[2]);
        word.vitalCost = stoi(items[3]);
        word.pos = items[4];
        word.conjugationForm = items[5];
        word.conjugationType = items[6];
        word.pronunciation = items[7];
        dictionary.push_back(word);
        runTimeAllWords += wordSurface[0]; // 検索用の文字列を作成
        count++;
    }
    cout << "辞書読み込み完了: " << count << "単語" << endl;
}

string matrixPath = "tools/matrix/matrix.bin";
vector<vector<int>> connectionMatrix = LoadMatrixUtil::loadMatrix(matrixPath);

vector<int> findWord(string input)
{
    wstring sentence = Utf8ConvUtil::utf8ToWstring(input);
    int n = sentence.size();
    vector<int> samePrefixIndexes;
    for (int j = 0; j < runTimeAllWords.size(); j++)
    {
        if (runTimeAllWords[j] == sentence[0])
        {
            samePrefixIndexes.push_back(j);
        }
    }
    if (!samePrefixIndexes.size()) return vector<int>();
    int maxIndex = -1;
    cout << "samePrefixIndexes: " << samePrefixIndexes.size() << endl;
    vector<int> result;
    for (const int index : samePrefixIndexes)
    {
        wstring word = dictionary[index].surface;
        if (sentence == word)
        {
            result.push_back(index);
        }
    }
    return result;
}

int main()
{
    string dictPath = "tools/dictionary/sdictionary.txt";
    loadDictionary(dictPath);
    while (true)
    {
        string input;
        cout << "最初の単語を入力してください: " << endl;
        cin >> input;
        vector<int> inputs = findWord(input);
        if (inputs.size() == 0)
        {
            cout << "単語が見つかりませんでした" << endl;
            continue;
        }
        int prevWordIndex;
        for (int inputIndex: inputs)
        {
            cout << inputIndex << endl;
            cout << "単語: " << Utf8ConvUtil::wstringToUtf8(dictionary[inputIndex].surface) << endl;
            cout << "品詞: " << dictionary[inputIndex].pos << endl;
            cout << "読み: " << dictionary[inputIndex].pronunciation << endl;
        }
        cout << "どれにしますか?" << endl;
        cin >> prevWordIndex;
        cout << "洗濯機の生成を開始します" << endl;
        int n;
        cout << "洗濯機の数を入力してください: " << endl;
        cin >> n;
        vector<int> washingMachines(n, 0);
        int type = 0;
        for (int i = 0; i < n; i++)
        {
            int bestWordIndex = -1;
            int bestCost = numeric_limits<int>::max() - 30000;
            string pos = "";
            type += rand() % 14 + 1;
            if (type > 20) type = 0;
            if (type < 5) pos = "名詞";
            else if (type < 8) pos = "動詞";
            else if (type < 10) pos = "形容詞";
            else if (type < 12) pos = "副詞";
            else if (type < 13) pos = "助動詞";
            else if (type < 14) pos = "接続詞";
            else if (type < 15) pos = "感動詞";
            else pos = "助詞";
            cout << "品詞: " << type << pos << endl;
            for (int j = 0; j < dictionary.size(); j++)
            {
                int randIndex = rand() % 30000;
                if (dictionary[j].pos == pos && connectionMatrix[dictionary[prevWordIndex].rightConnectionId][dictionary[j].leftConnectionId] + dictionary[j].vitalCost < bestCost + randIndex && find(washingMachines.begin(), washingMachines.end(), j) == washingMachines.end())
                {
                    bestCost = connectionMatrix[dictionary[prevWordIndex].rightConnectionId][dictionary[j].leftConnectionId] + dictionary[j].vitalCost;
                    bestWordIndex = j;
                }
            }
            washingMachines[i] = bestWordIndex;
            prevWordIndex = bestWordIndex;
            cout << bestWordIndex << endl;
        }
        cout << "洗濯機の生成が完了しました" << endl;
        cout << "洗濯機の結果を表示します" << endl;
        for (int i = 0; i < n; i++)
        {
            cout << Utf8ConvUtil::wstringToUtf8(dictionary[washingMachines[i]].surface);
        }
        cout << endl;
        for (int i = 0; i < n; i++)
        {
            cout << dictionary[washingMachines[i]].pronunciation;
        }
        cout << endl;
    }
    return 0;
}
