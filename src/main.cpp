#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
using std::getline, std::cin, std::cout, std::string;

string path {""};
std::fstream file;
string encryption_key {"secret"};

bool Access_File();
bool Check_Master_Key_Presence();
bool Master_Key_Match (string&);
void Update_File (string, string);
string xorEncryption(string&);
string toHex(const string&);
string fromHex(const string&);

int main(){
    while(!Access_File()){
        cout << "Please enter the path to the password file: ";
        getline(cin, path);
        if (file.is_open())
            file.close();
        file.open(path);
        if (!Access_File())
            cout << "Wrong File Path!\n";
    }

    while(!Check_Master_Key_Presence()){
        cout << "Enter a Master Key: ";
        string Master_Key;
        getline(cin, Master_Key);
        Update_File("Master Key:", Master_Key);
    }

    string Input_Key;
    short tries {0};
    while (!Master_Key_Match(Input_Key) && (tries < 3)){
        cout << "Enter the Master Key: ";
        getline(cin, Input_Key);
        tries += 1;
    }


}

bool Access_File(){
    return file.is_open();
}

bool Check_Master_Key_Presence(){
    file.clear();
    file.seekg(0);

    string first_line;
    if (getline(file, first_line))
        return first_line.find("Master Key:") != string::npos;
    else
        return false;
}

bool Master_Key_Match(string& Input_Key){
    file.clear();
    file.seekg(0);
    string first_line;
    getline(file, first_line);
    string find_text {"Master Key:"};
    size_t pos = first_line.find(find_text);
    string Master_Key = first_line.substr(pos + find_text.length() + 1);
    if (Master_Key == toHex(xorEncryption(Input_Key)))
        return true;
    else
        return false;
}

void Update_File (string label, string unencrypted_text){
    file.close();
    file.open(path);
    if (label == "Master Key:"){
        file << label << " " << toHex(xorEncryption(unencrypted_text)) << "\n";
    }
}

string xorEncryption(string& text){
    string result;
    for (size_t i=0; i < text.size(); ++i){
        result += text[i] ^ encryption_key[i % encryption_key.size()];
    }
    return result;
}

string toHex(const string& input){
    std::ostringstream oss;
    for (unsigned char c : input)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    return oss.str();
}

string fromHex(const string& hex){
    string output;
    for (size_t i = 0; i < hex.length(); i+=2){
        string byteString = hex.substr(i, 2);
        char byte = (char) strtol(byteString.c_str(), nullptr, 16);
        output += byte;
    }
    return output;
}