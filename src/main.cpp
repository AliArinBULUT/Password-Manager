#include <iostream>
#include <string>
#include <fstream>
using std::getline, std::cin, std::cout, std::string;

string path {""};
std::fstream file;
string encryption_key {"secret"};

bool Access_File();
bool Check_Master_Key_Presence();
void Update_File (string, string);
string xorEncryption(string&);

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

    cout << "Enter the Master Key: ";

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

void Update_File (string label, string unencrypted_text){
    file.close();
    file.open(path);
    if (label == "Master Key:"){
        file << label << " " << xorEncryption(unencrypted_text) << "\n";
    }
}

string xorEncryption(string& unencrypted_text){
    string result;
    for (size_t i=0; i < unencrypted_text.size(); ++i){
        result += unencrypted_text[i] ^ encryption_key[i % encryption_key.size()];
    }
    return result;
}
