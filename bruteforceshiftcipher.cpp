#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<string> loadDictionary(const string &filename)
{
    vector<string> words;
    ifstream file(filename);
    if (file.is_open())
    {
        string word;
        while (getline(file, word))
        {
            words.push_back(word);
        }
        file.close();
    }
    return words;
}

string decryptShiftCipher(const string &ciphertext, int shift)
{
    string plaintext = ciphertext;
    for (char &c : plaintext)
    {
        if (isalpha(c))
        {
            char base = islower(c) ? 'a' : 'A';
            c = ((c - base - shift + 26) % 26) + base;
        }
    }
    return plaintext;
}

bool isEnglish(const string &text, const vector<string> &dictionary)
{
    string lowercaseText = text;
    transform(lowercaseText.begin(), lowercaseText.end(), lowercaseText.begin(), ::tolower);
    return find(dictionary.begin(), dictionary.end(), lowercaseText) != dictionary.end();
}

int main()
{
    string ciphertext;
    cout << "Enter the ciphertext: ";
    getline(cin, ciphertext);

    vector<string> englishWords = loadDictionary("dictionary.txt");

    bool breakable = false;

    for (int shift = 1; shift <= 25; shift++)
    {
        string decryptedText = decryptShiftCipher(ciphertext, shift);
        if (isEnglish(decryptedText, englishWords))
        {
            cout << "Decryption with Shift " << shift << ": " << decryptedText << endl;
            breakable = true;
        }
    }

    if (!breakable)
    {
        for (int shift = 1; shift <= 25; shift++)
        {
            string decryptedText = decryptShiftCipher(ciphertext, shift);
            cout << "Shift " << shift << " decryption: " << decryptedText << endl;
        }
    }

    return 0;
}
