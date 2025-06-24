#include <iostream>
#include <string>
#include <fstream>
using std::getline, std::cin, std::cout, std::string;

string path {""};
std::fstream file;

bool Access_File(){
    return file.is_open();
}

bool Check_Master_Key(){
    file.clear();
    file.seekg(0);

    string first_line;
    if (getline(file, first_line))
        return first_line.find("Master Key:") != string::npos;
    else
        return false;
}

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

}