#include "config.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    string filePath("../cfg.conf");
    Config config(filePath);

    cout << "arduinoName=" << config.GetString("arduinoName","") << endl;
    cout << "serverIP=" << config.GetString("serverIP", "") << endl;
    cout << "serverPort=" << config.GetInt("serverPort", 0) << endl;
}