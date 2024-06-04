#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;


string shiftCipherDecrypt(const string &cipherText, int key) {
    string decryptedText = cipherText;

    for (char &ch : decryptedText)
        ch = 'a' + ((ch - 'a' + key) % 26);

    return decryptedText;
}


double calculateIoC(string &text) {
    map<char, int> frequency;
    double total = 0;

    for (char ch : text) {
        if (ch >= 'a' && ch <= 'z') {
            frequency[ch]++;
            total++;
        }
    }

    vector<double> expectedFreq = {
        0.0817, 0.0149, 0.0278, 0.0425, 0.1270, 0.0223, 0.0202, 0.0609,
        0.0697, 0.0015, 0.0077, 0.0403, 0.0241, 0.0675, 0.0751, 0.0193,
        0.0009, 0.0599, 0.0633, 0.0906, 0.0276, 0.0098, 0.0236, 0.0015,
        0.0197, 0.0007
    };

    double ioc = 0.0;
    int index = 0;

    for (char ch = 'a'; ch <= 'z'; ch++) {
        if (frequency[ch] == 0) {
            index++;
            continue;
        }
        ioc += (frequency[ch] / total) * expectedFreq[index++];
    }

    return ioc;
}

int main() {
    ifstream file("input_for_shift_cipher.txt");
    if (!file) {
        cerr << "Error: Unable to open the input file." << endl;
        return 1;
    }

    string cipherText;
    getline(file, cipherText);
    file.close();

    transform(cipherText.begin(), cipherText.end(), cipherText.begin(), ::tolower);

    double expectedIoC = 0.067;
    int optimalKey = -1;
    double minDifference = 1.0;

    for (int key = 0; key <= 25; ++key) {
        string decryptedText = shiftCipherDecrypt(cipherText, key);
        double ioc = calculateIoC(decryptedText);

        if (abs(ioc - expectedIoC) < abs(minDifference - expectedIoC)) {
            minDifference = ioc;
            optimalKey = key;
        }
    }

    cout << "Best Key : " << optimalKey << endl;
    cout << "Decrypted Text: " << endl;
    cout << shiftCipherDecrypt(cipherText, optimalKey) << endl;

    return 0;
}