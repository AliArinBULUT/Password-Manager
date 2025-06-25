#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
using std::getline, std::cin, std::cout, std::string;

string path {""};
std::fstream file;
string encryption_key {"secret"};
bool exit_program {false};

bool Access_File();
bool Check_Master_Key_Presence();
bool Master_Key_Match (string&);
void Update_File (string, string);
string xorEncryption(string&);
string toHex(const string&);
string fromHex(const string&);
void Menu();
void Update_Password(string);

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
        Update_File("Master Key", Master_Key);
        if (file.is_open())
            file.close();
        file.open(path);
    }

    string Input_Key;
    size_t tries {0};
    while (!Master_Key_Match(Input_Key) && (tries < 3)){
        cout << "Enter the Master Key: ";
        getline(cin, Input_Key);
        tries += 1;
    }

    if (tries == 3){
        cout << "Wrong Master Key";
        exit_program = true;
    }

    while(!exit_program){
        Menu();
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
        return first_line.find("Master Key") != string::npos;
    else
        return false;
}

bool Master_Key_Match(string& Input_Key){
    file.clear();
    file.seekg(0);
    string first_line;
    getline(file, first_line);
    string find_text {"Master Key"};
    size_t pos = first_line.find(find_text);
    string Master_Key = first_line.substr(pos + find_text.length() + 1);
    if (Master_Key == toHex(xorEncryption(Input_Key)))
        return true;
    else
        return false;
}

void Menu(){
    cout << "1) Add Password\n2)Display Passwords\n3)Change Passwords\n4)Change Master Key\n9)Exit\nSelection: ";
    size_t selection {0};
    cin >> selection;
    std::cin.ignore();
    file.clear();
    file.seekg(0);
    switch (selection) {
        case 1: {
            cout << "Enter a label for the password: ";
            string label {""};
            getline(cin, label);
            cout << "Enter your password: ";
            string password;
            getline(cin, password);
            Update_File(label, password);
            file.close();
            file.open(path);
            break;
        }
        case 2: {
            cout << "***********************\nPasswords\n***********************\n";
            file.clear();
            file.seekg(0);
            string line;
            getline(file, line);
            getline(file, line);
            while(getline(file, line)){
                size_t pos = line.find(' ');
                if (pos != string::npos){
                    string label = line.substr(0, pos);
                    string password = fromHex(line.substr(pos + 1));

                    cout << label << ": " << xorEncryption(password) << "\n";
                }
            }
            cout << "***********************"<< std::endl;
            break;
        }
        case 3: {
            cout << "Enter the label: ";
            string label {""};
            getline(cin, label);
            Update_Password(label);
            file.close();
            file.open(path);
            break;
        }
        case 4: {
            Update_Password("Master Key");
            file.close();
            file.open(path);
            break;
        }
        case 9: {
            exit_program = true;
            break;
        }
        default:
            cout << "NOT A VALID SELECTION\n";
    }
}

void Update_File (string label, string unencrypted_text){
    file.close();
    file.open(path);
    if (label == "Master Key"){
        file << label << " " << toHex(xorEncryption(unencrypted_text)) << "\n\n";
    }
    else{
        file.seekp(0, std::ios::end);
        file << label << " " << toHex(xorEncryption(unencrypted_text)) << "\n";
    }
    file.close();
}

void Update_Password(string label){
    file.close();
    file.open(path);
    std::vector <string> lines;
    string line;
    bool found = false;

    while (getline(file, line))
        lines.push_back(line);

    for (size_t i = 0; i < lines.size(); ++i){
        if (lines[i].find(label) == 0){
            cout << "Enter the new password: ";
            string new_password;
            getline(cin, new_password);
            lines[i] = label + " " + toHex(xorEncryption(new_password));
            found = true;
            file.close();
            file.open(path);
            break;
        }
    }

    if (!found){
        cout << "Could not find the label\n";
        return;
    }

    for (const auto& line: lines)
        file << line << "\n";
    file.close();
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