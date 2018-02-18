#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

struct maxStorage{
    string key;
    string input;
    string output;
    int max;
};

class Crypt{
    public:
        void hexToBase64(string input);
        void fixedXOR(string input1, string input2);
        void singleByteXORCipher(string input);
        void findTheCipherfromFile(string filename);
        void repeatingkeyXOR(string input, string key);
        void breakRepeatingXOR(string input, int from, int to);
        void testing(string input, string input2);
    private:
        //binary transitions
        void NumToBinaryByte(string &binary, int input);
        void hexToBinary(string &binary, string input);
        void StringTobinary(string &binary, string input);
        char BinaryByteToChar(string binary);
        string BinaryByteToHex(string input);
        string numToHex(int input);
        void binaryToBase64(string &base64, string binary);
        void Base64ToBinary(string &binary, string base64);
        void BinaryToXOR(string &Final, string binary1, string binary2);
        string singleByteXOR(string binary1, string key);
        int englishTextScoring(string input);
        string repeatingkeyXORp(string binary1, string binary2);
        int DifferingBits(string one, string two);
};

//public functions
void Crypt::testing(string input, string input2){
    int key;
    Base64ToBinary(input, input2);
//    key = DifferingBits;
    cout <<input  << endl;
}
void Crypt::hexToBase64(string input){
    string binary, base64;
    hexToBinary(binary, input);
    binaryToBase64(base64, binary);
    cout << base64 << endl;
}

void Crypt::fixedXOR(string input1,string input2){
    string binary1, binary2, Final;
    hexToBinary(binary1, input1);
    hexToBinary(binary2, input2);
    BinaryToXOR(Final, binary1, binary2);
    cout << Final << endl;
}

void Crypt::singleByteXORCipher(string input){
    vector<string> output;
    int scores[256] = {0};
    string tmp, binary;
//    cout << input << endl;
//    StringTobinary(binary, input);
    hexToBinary(binary, input);
//    cout << binary << endl;
    for(int i = 0; i < 256; i++){
        NumToBinaryByte(tmp, i);
        output.push_back(singleByteXOR(binary, tmp));
//        cout << output[i] << endl;
        tmp.erase(tmp.begin(), tmp.end());
        scores[i] = englishTextScoring(output[i]);
//        if(scores[i] != 0)
  //          cout << output[i] << endl << scores[i] << endl << endl;
    }
    maxStorage max;
    max.max = -1;
    for(int i = 0; i < 256; i++){
        if(scores[i] > max.max){
            max.max = scores[i];
            max.key = numToHex(i);
            max.output = output[i];
        }
    }

    cout << "Output: " << max.output << endl << "Key: 0x" << max.key << endl << endl;
}

void Crypt::findTheCipherfromFile(string filename){
    ifstream file;
    file.open("file");
    maxStorage max;
    max.max = -1;
    string tmp,binary,binaryNum;
    while(file >> tmp){
        hexToBinary(binary, tmp);
        vector<string> output;
        int scores[256] = {0};
        for(int i = 0; i < 256; i++){
            NumToBinaryByte(binaryNum, i);
            output.push_back(singleByteXOR(binary, binaryNum));
            binaryNum.erase(binaryNum.begin(),binaryNum.end());

            scores[i] = englishTextScoring(output[i]);
            if(scores[i] > max.max){
                max.max = scores[i];
                max.output = output[i];
                max.input = tmp;
                max.key = numToHex(i);
            }
        }
    
        tmp.erase(tmp.begin(), tmp.end());
        binary.erase(binary.begin(), binary.end());
    }
    cout << max.output << endl << max.key << endl << endl << max.input << endl << endl;
}

void Crypt::repeatingkeyXOR(string input, string key){
    string inputBinary, keyBinary;
    StringTobinary(inputBinary, input);
    StringTobinary(keyBinary, key);
    cout << repeatingkeyXORp(inputBinary, keyBinary) << endl;
}

void Crypt::breakRepeatingXOR(string input, int from, int to){
    int keysize;
    string first, second;
    int normalize[to];
    vector<int> least;
    int min = 99999;

    for(keysize = from; keysize <= to; keysize++){
        normalize[keysize] = 0;
        for(int i = 0; i < keysize; i++){
            if(i < input.length())
                first.push_back(input[i]);
        }
        for(int x = 0; x < 3; x++){
           for(int i = keysize; i < keysize*2; i++){
               if(i < input.length())
                   second.push_back(input[i + (x * keysize)]);
           }
           normalize[keysize] = (normalize[keysize] + (DifferingBits(first, second) / keysize)) / 2;
           if(normalize[keysize] < min){
               least.erase(least.begin(),least.end());
               least.push_back(keysize);
               min = normalize[keysize];
           }
           else if(normalize[keysize] == min){
               least.push_back(keysize);
           }
        }

        //cout <<keysize << ": "<< normalize[keysize] << endl;
        first.erase(first.begin(), first.end());
        second.erase(second.begin(), second.end());
    }
    string binaryHex;
    string baseToBinary;
    Base64ToBinary(baseToBinary, input);
    for(int i = 0; i < least.size(); i++){
        binaryHex = numToHex(least[i]);
        cout << "KEY: " << least[i] << endl << endl;
        repeatingkeyXOR(baseToBinary, binaryHex);
    }
//    keysize = DifferingBits(one, two);
//    cout << "KEYSIZE: "<< keysize <<endl;
}

//private functions
void Crypt::NumToBinaryByte(string &binary, int input){
    stack<char> tmp;
    for(int i = 0; i < 8; i++){
        if(input % 2){
            tmp.push('1');
        }
        else
            tmp.push('0');
        input = input / 2;
    }
    for(int i = 0;i < 8; i++){
            binary.push_back(tmp.top());
            tmp.pop();
    }
}

string Crypt::BinaryByteToHex(string tmp){
    int power = 128;
    int num = 0;
    string hex;
    for(int i = 0; i < 8; i++){
        if(tmp[i] == '1')
            num += power;
        power = power / 2;
    }
    return numToHex(num);
}

void Crypt::hexToBinary(string &binary, string input){
    int tmp;
    for(int i = 0; i < input.length(); i++){
        if(input[i] >= '0' && input[i] <= '9'){
            tmp = input[i] - '0';
        }
        else
            tmp = (input[i] - 'A') + 10;
        stack<char> binStack;
        for(int x = 0; x < 4; x++){
            if(tmp == 0){
                binStack.push('0');
            }
            else{
                if(tmp % 2 == 1)
                    binStack.push('1');
                else
                    binStack.push('0');
                tmp = tmp / 2;
            }
        }
        for(int x = 0; x < 4; x++){
            binary.push_back(binStack.top());
            binStack.pop();
        }
    }
}

void Crypt::StringTobinary(string &binary, string input){
        stack<char> tmp;
        int character;
        for(int i = 0; i < input.size(); i++){
            character = (int)input[i];
          //  cout << character << endl;
            for(int x = 0; x < 8; x++){
                if(character % 2)
                    tmp.push('1');
                else    
                    tmp.push('0');
                character = character / 2;
            }
            for(int x = 0; x < 8; x++){
                binary.push_back(tmp.top());
                tmp.pop();
            }
        }
    }

string Crypt::numToHex(int input){
    stack<char> tmp;
    string hex;
    for(int i = 0; i < 2; i++){
        int x = input % 16;
        if(x >= 0 && x <= 9){
            tmp.push(x + '0');
        }
        else
            tmp.push(x - 10 + 'A');
        input = input / 16;
    }
    for(int i = 0; i < 2; i++){
        hex.push_back(tmp.top());
        tmp.pop();
    }
    return hex;
}

char Crypt::BinaryByteToChar(string binary){
    int power = 128;
    int num = 0;
    for(int i = 0; i < 8; i++){
        if(binary[i] == '1')
            num += power;
        power = power / 2;
    }

    char r = num;
    return r;
}

void Crypt::binaryToBase64(string &base64, string binary){
    int power, tmp;
    for(int i = 0; i < binary.length(); i += 6){
        power = 32;
        tmp = 0;

        for(int x = 0; x < 6; x++){
            if(i+x >= binary.length()){
                break;
            }
            else{
                if(binary[i+x] == '1')
                    tmp = tmp + power;
            }
            power = power / 2;
        }

        if(tmp >= 0 && tmp <= 25){
            base64.push_back(tmp + 'A');
        }
        else if(tmp >= 26 && tmp <= 51){
            base64.push_back((tmp - 26) + 'a');
        }
        else if(tmp >= 52 && tmp <= 61){
            base64.push_back((tmp - 52) + '0');
        }
        else if(tmp == 62){
            base64.push_back('+');
        }
        else if(tmp == 63){
            base64.push_back('\\');
        }
    }
}

void Crypt::Base64ToBinary(string &binary, string base64){
    int num;
    stack<int> bin;
    for(int i = 0; i < base64.length(); i++){
        if(base64[i] >= 'A' && base64[i] <= 'Z'){
            num = base64[i] - 'A';
        }
        else if(base64[i] >= 'a' && base64[i] <= 'z'){
            num = base64[i] - 'a' + 26;
        }
        else if(base64[i] >= '0' && base64[i] <= '9'){
            num = base64[i] - '0' + 52;
        }
        else if(base64[i] == '+')
            num = 62;
        else
            num = 63;

    
        for(int x = 0; x < 6; x++){
            bin.push(num%2);
            num = num / 2;
        }
        for(int x = 0; x < 6; x++){
            binary.push_back(bin.top() + '0');
            bin.pop();
        }
    }
}

void Crypt::BinaryToXOR(string &Final, string binary1, string binary2){
    string tmp;
    for(int i = 0; i < binary1.length(); i++){
        if(binary1[i] != binary2[i])
            tmp.push_back('1');
        else
            tmp.push_back('0');
    }
    int power, x;
    for(int i = 0; i < tmp.length(); i+=4){
        power = 8;
        x = 0;
        for(int p = 0; p < 4; p++){
            if(tmp[i+p] == '1')
                x = x + power;
            power = power / 2;
        }
        if(x >= 0 && x <= 9){
            Final.push_back(x + '0');
        }
        else
            Final.push_back(((x-10) + 'A'));
    }
}

string Crypt::singleByteXOR(string binary1, string key){
    string tmp;
    string newString;
    string hex;
//    cout << binary1 << endl << key << endl << endl;
    for(int i = 0; i < binary1.size(); i += 8){
        for(int x = 0; x < 8; x++){
            if(binary1[i+x] != key[x])
                tmp.push_back('1');
            else
                tmp.push_back('0');
        }
//        cout << tmp << endl;
        newString.push_back(BinaryByteToChar(tmp));
        tmp.erase(tmp.begin(),tmp.end());
    }
    //cout << newString << endl;
    return newString;
}

int Crypt::englishTextScoring(string input){
    string keywords[100] = {"the", "be", "to", "of", "and", "hello", "in", "that", "have", "world", "it", "for", "not", "on", "with", "he", "as", "you", "do", "at", "this", "but", "his", "by", "from", "they", "we", "say", "her", "she", "or", "an", "will", "my", "one", "all", "would", "there", "their", "what", "so", "up", "out", "if", "about", "who", "get", "which", "go", "me", "when", "make", "can", "like", "time", "no", "just", "him", "know", "take", "people", "into", "year", "your", "good", "some", "could", "them", "see", "other", "than", "then", "now", "look", "only", "come", "its", "over", "think", "also", "back", "after", "use", "two", "how", "our", "work", "first", "well", "way", "even", "new", "want", "because", "any", "these", "give", "day", "most", "us"};
    int score = 0;
    for(int i = 0; i < 100; i++){
        for(int x = 0; x < input.size(); x++){
            if(input[x] == keywords[i][0] || input[x] == (keywords[i][0] - 'a') + 'A'){
                bool p = true;
                for(int q = 1; q < keywords[i].size(); q++){
                    if(input[x+q] != keywords[i][q]){
                        p = false;
                        break;
                    }
                }
                if(p){
                    score += keywords[i].size();
                    //cout << keywords[i] << endl;
                }
            }
        }
    }
    return score;    
}

string Crypt::repeatingkeyXORp(string binary1, string binary2){
    int numOfBytes = binary2.size() / 8;
    int counter = 0;
    string newString,tmp, hex;
    for(int i = 0; i < binary1.size(); i+=8){
        for(int x = 0; x < 8; x++){
            if(binary1[i+x] != binary2[x + (8 *(counter % numOfBytes))])
                tmp.push_back('1');
            else
                tmp.push_back('0');
        }
        hex = BinaryByteToHex(tmp);
        newString.push_back(hex[0]);
        newString.push_back(hex[1]);
        tmp.erase(tmp.begin(), tmp.end());
        counter++;
        counter = counter % numOfBytes;
    }
    return newString;
}

int Crypt::DifferingBits(string one, string two){
    string binaryOne, binaryTwo;
    int sum = 0;
    StringTobinary(binaryOne, one);
    StringTobinary(binaryTwo, two);

    for(int i = 0; i < binaryOne.length(); i++){
        if(i < binaryTwo.length()){
            if(binaryOne[i] != binaryTwo[i])
                sum++;
        }
        else
            if(binaryOne[i] == '1')
                sum++;
    }
    return sum;
}

int main(){
    Crypt x;
//    x.singleByteXORCipher("1B37373331363F78151B7F2B783431333D78397828372363C78373E783A393B3736");
//    x.findTheCipherfromFile("file");
//    x.repeatingkeyXOR("Burning 'em, if you ain't quick and nimble I go crazy when I hear a cymbal", "ICE");
//    x.testing("HELLO world");
    string y,q;
    while(cin >> y){
        for(int i = 0; i < y.length(); i++)
            q.push_back(y[i]);
    }
    string binary;
    x.breakRepeatingXOR(binary, 2,40);
//    cout << binary << endl;
//    x.breakRepeatingXOR(y, 2, 40);
    return 0;
}