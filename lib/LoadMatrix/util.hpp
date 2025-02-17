#ifndef loadMatrixUtil_HPP
#define loadMatrixUtil_HPP

#include <fstream>
#include <vector>

class LoadMatrixUtil
{
public:
    static std::vector<std::vector<int>> loadMatrix(const std::string &filename);
};

#endif
