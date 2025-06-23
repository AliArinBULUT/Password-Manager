#include <iostream>
#include <string>
#include <fstream>
using std::getline, std::cin, std::cout, std::string;

string path {""};
std::fstream file {path, std::ios::in};

bool Access_File(){
    if (file.is_open())
        return true;
    else
        return false;
}

int main(){
    while(!Access_File()){
        cout << "Please enter the path to the password file: ";
        getline(cin, path);
        file.open(path);
        if (!Access_File())
            cout << "Wrong File Path!\n";
    }

}