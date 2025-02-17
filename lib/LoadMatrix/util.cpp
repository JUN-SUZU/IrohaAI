#include "util.hpp"

std::vector<std::vector<int>> LoadMatrixUtil::loadMatrix(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    short n, m;
    file.read((char *)&n, sizeof(short));
    file.read((char *)&m, sizeof(short));
    std::vector<std::vector<int>> matrix(n, std::vector<int>(m));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            short x;
            file.read((char *)&x, sizeof(short));
            matrix[i][j] = x;
        }
    }
    return matrix;
}
