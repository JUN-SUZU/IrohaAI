#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

// ハイパーパラメータ
const int VECTOR_SIZE = 100; // 単語ベクトルの次元数
const double LEARNING_RATE = 0.01;
const int EPOCHS = 10000;

// 単語辞書とIDマッピング
unordered_map<string, vector<int>> word_to_ids;
unordered_map<int, string> id_to_word;
vector<vector<double>> word_vectors;

// シグモイド関数
double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

// 乱数初期化
double random_weight()
{
    return ((double)rand() / RAND_MAX) * 2 - 1; // -1.0 から 1.0
}

// 初期化関数
void initialize_vectors(int vocab_size)
{
    word_vectors.resize(vocab_size, vector<double>(VECTOR_SIZE));
    for (int i = 0; i < vocab_size; i++)
    {
        for (int j = 0; j < VECTOR_SIZE; j++)
        {
            word_vectors[i][j] = random_weight();
        }
    }
}

// 学習関数（Skip-gram）
void train(vector<pair<int, int>> &training_data)
{
    for (int epoch = 0; epoch < EPOCHS; epoch++)
    {
        double loss = 0.0;
        for (auto &pair : training_data)
        {
            int input_word = pair.first;
            int output_word = pair.second;

            // 内積計算
            double dot = 0.0;
            for (int i = 0; i < VECTOR_SIZE; i++)
            {
                dot += word_vectors[input_word][i] * word_vectors[output_word][i];
            }

            double prediction = sigmoid(dot);
            double error = 1.0 - prediction;
            loss += error * error;

            // 勾配更新
            for (int i = 0; i < VECTOR_SIZE; i++)
            {
                double grad = LEARNING_RATE * error * word_vectors[output_word][i];
                word_vectors[input_word][i] += grad;
                word_vectors[output_word][i] += grad;
            }
        }
        if (epoch % 1000 == 0)
        {
            cout << "Epoch: " << epoch << " Loss: " << loss << endl;
        }
    }
}

int main()
{
    srand(time(0));

    // サンプルデータ（同じ単語でも品詞・読みが異なる場合は別ID）
    vector<pair<string, string>> corpus = {{"猫", "ねこ"}, {"が", "が"}, {"魚", "さかな"}, {"を", "を"}, {"食べる", "たべる"}, {"猫", "びょう"}, {"食べる", "くらう"}};

    int id_counter = 0;
    for (auto &word : corpus)
    {
        if (word_to_ids.find(word.first) == word_to_ids.end())
        {
            word_to_ids[word.first] = {};
        }
        word_to_ids[word.first].push_back(id_counter);
        id_to_word[id_counter] = word.first + "(" + word.second + ")";
        id_counter++;
    }

    int vocab_size = id_counter;
    initialize_vectors(vocab_size);

    // 学習データ作成（隣の単語ペア）
    vector<pair<int, int>> training_data;
    for (int i = 0; i < corpus.size() - 1; i++)
    {
        for (int id1 : word_to_ids[corpus[i].first])
        {
            for (int id2 : word_to_ids[corpus[i + 1].first])
            {
                training_data.push_back({id1, id2});
            }
        }
    }

    // 学習
    train(training_data);

    // 結果表示
    for (int i = 0; i < vocab_size; i++)
    {
        cout << "Word: " << id_to_word[i] << " Vector: ";
        for (double v : word_vectors[i])
        {
            cout << v << " ";
        }
        cout << endl;
    }

    return 0;
}
