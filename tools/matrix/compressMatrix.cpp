#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    ifstream fin("matrix.def");
    ofstream fout("matrix.bin", ios::binary);
    short n, m, row, col, x;
    fin >> n >> m;
    fout.write((char*)&n, sizeof(short));
    fout.write((char*)&m, sizeof(short));
    for (int i = 0; i < n * m; i++) {
        fin >> row >> col >> x;
        fout.write((char*)&x, sizeof(short));
    }

    return 0;
}
