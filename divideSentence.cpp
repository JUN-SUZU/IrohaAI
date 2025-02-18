#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
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

vector<WordInfo> viterbiSegment(const string &sentenceUTF8)
{
    wstring sentence = Utf8ConvUtil::utf8ToWstring(sentenceUTF8);
    int n = sentence.size();
    vector<vector<int>> beginNodes(n);
    vector<vector<int>> endNodes(n);
    for (int i = 0; i < n; i++)
    {
        vector<int> samePrefixIndexes;
        for (int j = 0; j < runTimeAllWords.size(); j++)
        {
            if (runTimeAllWords[j] == sentence[i])
            {
                samePrefixIndexes.push_back(j);
            }
        }
        if (!samePrefixIndexes.size())
            continue;
        int maxIndex = -1;
        cout << "samePrefixIndexes: " << samePrefixIndexes.size() << endl;
        for (const int index : samePrefixIndexes)
        {
            wstring word = dictionary[index].surface;
            if (i + word.size() <= n && sentence.substr(i, word.size()) == word)
            {
                beginNodes[i].push_back(index);
            }
        }
    }
    struct bestPath
    {
        int n = 0;                    // number of words
        vector<int> dictionaryIndex;  // each word's index in dictionary
        vector<int> cost;             // each word's cost
        vector<int> prevIndex;        // previous word's index
        vector<vector<int>> nextList; // next word's indexes list
        vector<int> totalCost;        // total cost to reach the word
        void findBestPath(int index, int currentCost, int previousIndex)
        {
            if (index == n)
            {
                if (currentCost < totalCost[index])
                {
                    totalCost[index] = currentCost;
                    prevIndex[index] = previousIndex;
                }
                return;
            }
            currentCost += cost[index] + connectionMatrix[dictionary[dictionaryIndex[previousIndex]].rightConnectionId][dictionary[dictionaryIndex[index]].leftConnectionId];
            if (currentCost >= totalCost[index])
                return;
            totalCost[index] = currentCost;
            prevIndex[index] = previousIndex;
            if (nextList[index].size() == 0)
            {
                return;
            }
            for (int nextNode : nextList[index])
            {
                findBestPath(nextNode, currentCost, index);
            }
        }
    };
    vector<vector<int>> beginNextList(n);
    bestPath bp;
    for (int i = 0; i < n; i++)
    {
        vector<int> beginNode = beginNodes[i];
        for (int j = 0; j < beginNode.size(); j++)
        {
            int index = beginNode[j];
            cout << bp.n << Utf8ConvUtil::wstringToUtf8(dictionary[index].surface) << " ";
            bp.dictionaryIndex.push_back(index);
            bp.cost.push_back(dictionary[index].vitalCost);
            beginNextList[i].push_back(bp.n);
            bp.n++;
        }
    }
    bp.nextList = vector<vector<int>>(bp.n);
    int nextIndex = 0;
    for (int i = 0; i < n; i++)
    {
        vector<int> beginNode = beginNodes[i];
        for (int j = 0; j < beginNode.size(); j++)
        {
            if (i + dictionary[beginNode[j]].surface.size() == n)
            {
                bp.nextList[nextIndex].push_back(bp.n);
                continue;
            }
            else
            {
                vector<int> nextNodeList = beginNextList[i + dictionary[beginNode[j]].surface.size()];
                for (int k = 0; k < nextNodeList.size(); k++)
                {
                    bp.nextList[nextIndex].push_back(nextNodeList[k]);
                }
            }
            nextIndex++;
        }
    }
    bp.prevIndex = vector<int>(bp.n + 1, -1);
    bp.totalCost = vector<int>(bp.n + 1, numeric_limits<int>::max());
    for (int i : beginNextList[0])
    {
        bp.findBestPath(i, 0, -1);
    }
    vector<WordInfo> result;
    vector<int> reverseIndex;
    int index = bp.n;
    while (true)
    {
        cout << index << " << ";
        if (index!=bp.n) reverseIndex.push_back(index);
        if (bp.prevIndex[index] == -1)
            break;
        index = bp.prevIndex[index];
    }
    cout << endl;
    for (int i = reverseIndex.size() - 1; i >= 0; i--)
    {
        int index = reverseIndex[i];
        result.push_back(dictionary[bp.dictionaryIndex[index]]);
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
        cout << "文章を入力してください: " << endl;
        getline(cin, input);
        vector<WordInfo> segmented = viterbiSegment(input);

        cout << "分割結果: " << endl;
        for (const auto &word : segmented)
        {
            cout << Utf8ConvUtil::wstringToUtf8(word.surface) << "\t" << word.pos << "\t" << word.pronunciation << endl;
        }
        cout << endl;
    }

    return 0;
}
