//
// Created by hz on 16-11-13.
//

#ifndef SSC_CONFIG_H
#define SSC_CONFIG_H

#include <string>
#include <map>
using namespace std;

class Config {
public:
    Config(string fileName);
    ~Config();

    int GetInt(string key, int defaultVal);
    bool GetBool(string key, bool defaultVal);
    string GetString(string key, string defaultVal);
    double GetDouble(string key, double defaultVal);
private:
    string getValue(string key);
    string fileName_;
    map<string,string> cfgData_;
};


#endif //SSC_CONFIG_H
