/*
1. Write a function that receives a string containing a 16 - bit binary
integer.The function must return the string's integer value.
2. Write a function that receives a string containing a 32-bit
hexadecimal integer. The function must return the string's integer
value.
3. Write a function that receives an integer. The function must
return a string containing the binary representation of the integer.
4. Write a function that receives an integer. The function must
return a string containing the hexadecimal representation of the
integer.
5. Write a function that adds two digit strings in base b, where
2 <= b <= 10. Each string may contain as many as 1,000 digits.
6. Write a function that adds two hexadecimal strings, each as long
as 1,000 digits. Return a hexadecimal string that represents the
sum of the inputs.
7. Write a function that multiplies a single hexadecimal digit by a
hexadecimal digit string as long as 1,000 digits. Return a
hexadecimal string that represents the product.
9. Devise a way of subtracting unsigned binary integers. Test your
technique by subtracting binary 00000101 from binary 10001000,
producing 10000011. Test your technique with at least two other
sets of integers, in which a smaller value is always subtracted
from a larger one.
*/

#include <iostream>
#include <string>

using namespace std;

// ========== 1. 16-bit binary string → integer ==========
int bintodec(string s)
{
    int dec = 0;
    for (int i = 0; i < (int)s.size(); i++)
    {
        dec = dec * 2 + (s[i] - '0');
    }
    return dec;
}

// ========== 2. 32-bit hexadecimal string → integer ==========
int hextodec(string s)
{
    int dec = 0;
    for (int i = 0; i < (int)s.size(); i++)
    {
        int digit;
        if (s[i] >= '0' && s[i] <= '9')
            digit = s[i] - '0';
        else if (s[i] >= 'A' && s[i] <= 'F')
            digit = s[i] - 'A' + 10;
        else if (s[i] >= 'a' && s[i] <= 'f')   // 支持小写
            digit = s[i] - 'a' + 10;
        else
            digit = 0;
        dec = dec * 16 + digit;
    }
    return dec;
}

// ========== 3. integer → binary string ==========
string dectobin(int n)
{
    if (n == 0) return "0";
    string s = "";
    while (n > 0)
    {
        s = (n % 2 == 0 ? "0" : "1") + s;
        n /= 2;
    }
    return s;
}

// ========== 4. integer → hexadecimal string ==========
string dectohex(int n)
{
    if (n == 0) return "0";
    string s = "";
    while (n > 0)
    {
        int rem = n % 16;
        if (rem < 10)
        {
            s = char(rem + '0') + s;
        }
        else
        {
            s = char(rem - 10 + 'A') + s;
        }
        n /= 16;
    }
    return s;
}

// ========== 5. 两数字串相加 (base b, 2 ≤ b ≤ 10) ==========
string addBase(string a, string b, int base)
{
    // 补齐前导零使长度相等
    while (a.size() < b.size()) a = '0' + a;
    while (b.size() < a.size()) b = '0' + b;

    string result = "";
    int carry = 0;
    for (int i = (int)a.size() - 1; i >= 0; i--)
    {
        int sum = (a[i] - '0') + (b[i] - '0') + carry;
        carry = sum / base;
        sum = sum % base;
        result = char(sum + '0') + result;
    }
    if (carry > 0)
    {
        result = char(carry + '0') + result;
    }
    return result;
}

// ========== 6. 两十六进制串相加 (最长 1000 位) ==========
string addHex(string a, string b)
{
    // 补齐前导零使长度相等
    while (a.size() < b.size()) a = '0' + a;
    while (b.size() < a.size()) b = '0' + b;

    string result = "";
    int carry = 0;
    for (int i = (int)a.size() - 1; i >= 0; i--)
    {
        // 将两个十六进制字符转为数值
        auto hexVal = [](char c) -> int {
            if (c >= '0' && c <= '9') return c - '0';
            if (c >= 'A' && c <= 'F') return c - 'A' + 10;
            if (c >= 'a' && c <= 'f') return c - 'a' + 10;
            return 0;
            };
        int sum = hexVal(a[i]) + hexVal(b[i]) + carry;
        carry = sum / 16;
        sum = sum % 16;

        if (sum < 10)
            result = char(sum + '0') + result;
        else
            result = char(sum - 10 + 'A') + result;
    }
    if (carry > 0)
    {
        if (carry < 10)
            result = char(carry + '0') + result;
        else
            result = char(carry - 10 + 'A') + result;
    }
    return result;
}

// ========== 7. 单个十六进制数字 × 十六进制数字串 ==========
string multiplyHexDigit(char digit, string hexStr)
{
    // 将单个十六进制数字转为数值
    int multiplier;
    if (digit >= '0' && digit <= '9')
        multiplier = digit - '0';
    else if (digit >= 'A' && digit <= 'F')
        multiplier = digit - 'A' + 10;
    else if (digit >= 'a' && digit <= 'f')
        multiplier = digit - 'a' + 10;
    else
        return "0";

    if (multiplier == 0) return "0";

    string result = "";
    int carry = 0;
    for (int i = (int)hexStr.size() - 1; i >= 0; i--)
    {
        int val;
        char c = hexStr[i];
        if (c >= '0' && c <= '9')
            val = c - '0';
        else if (c >= 'A' && c <= 'F')
            val = c - 'A' + 10;
        else if (c >= 'a' && c <= 'f')
            val = c - 'a' + 10;
        else
            val = 0;

        int product = val * multiplier + carry;
        carry = product / 16;
        product = product % 16;

        if (product < 10)
            result = char(product + '0') + result;
        else
            result = char(product - 10 + 'A') + result;
    }
    // 处理剩余进位
    while (carry > 0)
    {
        int rem = carry % 16;
        if (rem < 10)
            result = char(rem + '0') + result;
        else
            result = char(rem - 10 + 'A') + result;
        carry /= 16;
    }
    return result;
}

// ========== 9. 无符号二进制减法 (a ≥ b) ==========
string subtractBin(string a, string b)
{
    // 补齐前导零
    while (a.size() < b.size()) a = '0' + a;
    while (b.size() < a.size()) b = '0' + b;

    string result = "";
    int borrow = 0;
    for (int i = (int)a.size() - 1; i >= 0; i--)
    {
        int diff = (a[i] - '0') - (b[i] - '0') - borrow;
        if (diff < 0)
        {
            diff += 2;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }
        result = char(diff + '0') + result;
    }
    // 去除前导零（保留至少一位）
    while (result.size() > 1 && result[0] == '0')
    {
        result.erase(0, 1);
    }
    return result;
}

// ========== 主函数 ==========
int main(int argc, char *argv[], char *envp[])
{
    string s1, s2;
    int n;

    // 1. 二进制 → 十进制
    cout << "Enter a 16-bit binary number: ";
    cin >> s1;
    cout << "The decimal number is: " << bintodec(s1) << endl;

    // 2. 十六进制 → 十进制
    cout << "Enter a 32-bit hexadecimal number: ";
    cin >> s1;
    cout << "The decimal number is: " << hextodec(s1) << endl;

    // 3. 十进制 → 二进制
    cout << "Enter a decimal number: ";
    cin >> n;
    cout << "The binary number is: " << dectobin(n) << endl;

    // 4. 十进制 → 十六进制
    cout << "Enter a decimal number: ";
    cin >> n;
    cout << "The hexadecimal number is: " << dectohex(n) << endl;

    // 5. 两个二进制数相加 (base 2)
    cout << "Enter two binary numbers: ";
    cin >> s1 >> s2;
    cout << "The sum is: " << addBase(s1, s2, 2) << endl;

    // 6. 两个十六进制数相加
    cout << "Enter two hexadecimal numbers: ";
    cin >> s1 >> s2;
    cout << "The sum is: " << addHex(s1, s2) << endl;

    // 7. 单个十六进制数字 × 十六进制数字串
    char hexDigit;
    cout << "Enter a single hexadecimal digit: ";
    cin >> hexDigit;
    cout << "Enter a hexadecimal string: ";
    cin >> s1;
    cout << "The product is: " << multiplyHexDigit(hexDigit, s1) << endl;

    // 9. 二进制减法
    cout << "Enter two binary numbers (larger first): ";
    cin >> s1 >> s2;
    cout << "The difference is: " << subtractBin(s1, s2) << endl;

    // 验证题目要求的测试用例
    cout << "\n=== Verification ===" << endl;
    cout << "10001000 - 00000101 = " << subtractBin("10001000", "00000101")
        << " (expected: 10000011)" << endl;
    cout << "11111111 - 01010101 = " << subtractBin("11111111", "01010101")
        << " (expected: 10101010)" << endl;
    cout << "00010000 - 00000001 = " << subtractBin("00010000", "00000001")
        << " (expected: 00001111)" << endl;

    system("pause");
    return 0;
}
