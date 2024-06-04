#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;


int calculateGCD(const vector<int> &numbers) {
    if (numbers.empty()) {
        return 0;
    }

    int gcd = numbers[0];
    for (size_t i = 1; i < numbers.size(); ++i) {
        int b = numbers[i];
        while (b != 0) {
            int temp = b;
            b = gcd % b;
            gcd = temp;
        }
    }

    return gcd;
}


string decryptShiftCipher(const string &cipherText, int shift) {
    string plainText = cipherText;
    for (char &c : plainText)
        c = 'a' + ((c - 'a' + shift) % 26);
    return plainText;
}


double calculateIOC(string &decryptedText) {
    map<char, int> letterFrequency;
    double count = 0;
    for (char c : decryptedText) {
        if (c >= 'a' && c <= 'z') {
            letterFrequency[c]++;
            count++;
        }
    }

    vector<double> expectedFrequencies = {
        0.0817, 0.0149, 0.0278, 0.0425, 0.1270, 0.0223, 0.0202, 0.0609,
        0.0697, 0.0015, 0.0077, 0.0403, 0.0241, 0.0675, 0.0751, 0.0193,
        0.0009, 0.0599, 0.0633, 0.0906, 0.0276, 0.0098, 0.0236, 0.0015,
        0.0197, 0.0007};

    double ioc = 0.0;
    int z = 0;
    for (char i = 'a'; i <= 'z'; i++) {
        if (letterFrequency[i] == 0) {
            z++;
            continue;
        }
        ioc = ioc + (letterFrequency[i] / count) * (expectedFrequencies[z++]);
    }

    return ioc;
}


string readInputFile(const string &filename) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Unable to open the input file." << endl;
        exit(1);
    }

    string input;
    getline(inputFile, input);
    inputFile.close();

    transform(input.begin(), input.end(), input.begin(), ::tolower);
    return input;
}


vector<int> findDifferences(const string &input, int minBigramLength, int maxBigramLength) {
    map<string, vector<int>> bigrams;
    for (int len = minBigramLength; len <= maxBigramLength; len++) {
        for (int i = 0; i < input.length() - len + 1; i++) {
            string bigram = input.substr(i, len);
            bigrams[bigram].push_back(i);
        }
    }

    vector<int> differences;
    for (const auto &pair : bigrams) {
        const vector<int> &indexes = pair.second;
        if (indexes.size() >= 2) {
            int diff = indexes[1] - indexes[0];
            differences.push_back(diff);
        }
    }

    return differences;
}

int main() {
    string input = readInputFile("input.txt");

    int minBigramLength = 3;
    int maxBigramLength = 8;
    vector<int> differences = findDifferences(input, minBigramLength, maxBigramLength);

    vector<int> d;
    for (int diff : differences) {
        if (diff <= 20)
            d.push_back(diff);
    }

    int keySize = calculateGCD(d);
    cout << "Possible key length is: " << keySize << endl;

    vector<string> keyLengthStrings;
    for (size_t i = 0; i < keySize; ++i) {
        string indexString;
        for (size_t j = i; j < input.length(); j += keySize) {
            indexString.push_back(input[j]);
        }
        keyLengthStrings.push_back(indexString);
    }

    vector<int> shifts;
    string key;
    for (string str : keyLengthStrings) {
        double expectedIoC = 0.067;
        int bestShift = -1;
        double closestIoC = 1.0;

        for (int shift = 0; shift <= 25; ++shift) {
            string decryptedText = decryptShiftCipher(str, shift);
            double ioc = calculateIOC(decryptedText);

            if (abs(ioc - expectedIoC) < abs(closestIoC - expectedIoC)) {
                closestIoC = ioc;
                bestShift = shift;
            }
        }

        shifts.push_back(bestShift);
        key.push_back('a' + ((26 - bestShift) % 26));
    }

    for (int i = 0; i < keyLengthStrings.size(); i++) {
        keyLengthStrings[i] = decryptShiftCipher(keyLengthStrings[i], shifts[i]);
    }

    cout << "Key : " << key << endl;

    size_t maxLength = 0;
    for (const string &str : keyLengthStrings) {
        if (str.length() > maxLength) {
            maxLength = str.length();
        }
    }

    cout << "PlainText: \n\n";
    for (size_t i = 0; i < maxLength; ++i) {
        for (const string &str : keyLengthStrings) {
            if (i < str.length()) {
                cout << str[i];
            } else {
                cout << ' ';
            }
        }
    }

    cout << endl;

    return 0;
}