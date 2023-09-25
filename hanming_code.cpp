
#include <iostream>
#include <vector>
#include <cstring>
#include <math.h>

class HanmingCode {
private:

    bool isPowerOfTwo(int n) {
        return (n > 0) && ((n & (n - 1)) == 0);
    }

    std::vector<int> getGroupIdx(const size_t& range, const size_t& position) {
        int targetValue = std::pow(2, position - 1);
        std::vector<int> idVector;
        for (int ii = 1; ii <= range; ++ii) {
            if ((ii & targetValue) == targetValue) {
                idVector.push_back(ii);
            }
        }
        return idVector;
    }

    inline int binaryToInt(const char& charVal) {
        return charVal - '0';
    }

    // 计算需要多少校验位
    size_t caluParityBitLen(const size_t& sourceCodeLen) {
        size_t parityBitLen = 1;
        while (std::pow(2, parityBitLen) - parityBitLen < sourceCodeLen - 1) {
            parityBitLen++;
        }
        return parityBitLen;
    }

public:
    // 将校验位和数据位拼在一起，采用偶校验
    std::string getHanmingCode(const std::string& sourceCode)
    {
        size_t sourCodeIdx = 0;
        size_t parityBitLen = caluParityBitLen(sourceCode.size());
        size_t targetCodeLen = sourceCode.size() + parityBitLen;
        std::string targetCode;

        // 填充校验位
        for (size_t ii = 0; ii < targetCodeLen; ii++) {
            if (isPowerOfTwo(ii + 1)) { // 是否是校验位
                targetCode.push_back('0');
            }
            else targetCode.push_back(sourceCode[sourCodeIdx++]);
        }

        // 遍历每个校验小组，有多少个校验位，就有多少个检验小组
        for (size_t ii = 1; ii <= parityBitLen; ii++) {
            std::vector<int> currentParityGroupIdxVector = getGroupIdx(targetCodeLen, ii);
            size_t len = currentParityGroupIdxVector.size();
            int result = 0;

            for (size_t idx = 1; idx < len; idx++) { // 跳过第一个
                result = result ^ binaryToInt(targetCode[currentParityGroupIdxVector[idx] - 1]);
            }
            if (result == 1) {
                targetCode[currentParityGroupIdxVector[0] - 1] = '1';
            }
            else targetCode[currentParityGroupIdxVector[0] - 1] = '0';
        }
        return targetCode;
    }

    // 校验汉明编码
    bool hanmingCorrect(std::string& sourceCode) {
        size_t sourceCodeLen = sourceCode.size();
        std::vector<int> groupParityResultVector;
        for (size_t ii = 1; std::pow(2, ii - 1) < sourceCodeLen; ii++) {
            std::vector<int> currentParityGroupIdxVector = getGroupIdx(sourceCodeLen, ii);
            size_t len = currentParityGroupIdxVector.size();
            int result = 0;

            for (size_t idx = 0; idx < len; idx++) { // 不跳过第一个
                result = result ^ binaryToInt(sourceCode[currentParityGroupIdxVector[idx] - 1]);
            }
            groupParityResultVector.push_back(result);
        }

        int hanmingResult = 0;
        for (size_t ii = 0; ii < groupParityResultVector.size(); ii++) {
            hanmingResult += groupParityResultVector[ii] * std::pow(2, ii);
        }
        if (hanmingResult == 0) {
            return true;
        }
        sourceCode[hanmingResult - 1] = (sourceCode[hanmingResult - 1] == '0') ? '1' : '0';
        return false;
    }
};

int main()
{
    HanmingCode hanmingAgent;

    std::string sourceCode = "001101001";
    std::cout << "接受到的二进制串:" << sourceCode << std::endl;
    if (hanmingAgent.hanmingCorrect(sourceCode)) {
        std::cout << "无差错" << std::endl;
    }
    else {
        std::cout << "修正后的二进制为:" << sourceCode << std::endl;
    }

    sourceCode = "0011";
    std::cout << "源二进制串:" << sourceCode << std::endl;
    std::cout << "编码后的二进制串:" << hanmingAgent.getHanmingCode(sourceCode);

    return 0;
}
