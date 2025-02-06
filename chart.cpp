#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <random>
#include <sstream>
using namespace std;
using namespace filesystem;

const string searchStr = "holdTime";

// 统计字符串出现次数
int countOccurrences(const string& filename, const string& searchStr) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "无法打开文件: " << filename << endl;
        return 0;
    }

    string content((istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>());
    file.close();

    int count = 0;
    size_t pos = 0;
    while ((pos = content.find(searchStr, pos)) != string::npos) {
        count++;
        pos += searchStr.length();
    }
    return count;
}

//随机数
int generateRandomNumber() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1000, 9999);
    return dis(gen);
}

int main(int argc, char* argv[]) {
    // 检查参数
    if (argc != 2) {
        string exeName = path(argv[0]).filename().string();
        cout << "使用方法：\n在命令行运行" << exeName << " <工作目录路径>\n示例: " << exeName << " C:\\Documents\\工作目录\n\n按回车退出...";
        cin.get();
        return 1;
    }
    
    string workDir = argv[1];
    string exePath = absolute(argv[0]).string();

    // 检查目录
    if (!exists(workDir)) {
        cout << "目录不存在!" << endl;
        return 2;
    }

    try {
        for (const auto& entry : directory_iterator(workDir)) {
            if (entry.is_regular_file()) {
                string filename = entry.path().string();

                // 跳过程序和已重命名文件
                if ((filename.find("-") != string::npos &&
                    filename.substr(filename.length() - 5) == ".json") || 
                    (filename == absolute(argv[0]).string())) {
                    continue;
                }

                // 生成文件名
                string newFilename = workDir + "\\" +
                    to_string(countOccurrences(filename, searchStr)) + "-" +
                    to_string(generateRandomNumber()) +
                    ".json";

                // 重命名
                try {
                    rename(filename, newFilename);
                    cout << "已重命名: " << filename << " -> " << newFilename << endl;
                }catch (const exception& e) {
                    cout << "重命名失败: " << filename << " - " << e.what() << endl;
                }
            }
        }
    }catch (const exception& e) {
        cout << "发生错误: " << e.what() << endl;
        return 3;
    }
    return 0;
}