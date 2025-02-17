# CMakeのバージョンを設定
cmake_minimum_required(VERSION 3.13)


# プロジェクト名と使用する言語を設定
project(MorphologicalAnalysis CXX)
# 実行ファイルを生成
add_executable(MorphologicalAnalysis divideSentence.cpp lib/UTF8ConvUtils/util.cpp lib/LoadMatrix/util.cpp)
