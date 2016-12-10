//
// Created by hz on 16-11-13.
//

#include <fstream>
#include <assert.h>
#include <iostream>
#include "config.h"


Config::Config(string fileName) {
    this->fileName_ = fileName;
    fstream cfgFile(fileName);

    if (!cfgFile.is_open()){
        cout << "read configuration file error!" << endl;
        exit(1);
    }
    char line[1000];
    while (!cfgFile.eof()) {
        cfgFile.getline(line, 1000);
        string content(line);
        size_t pos = content.find('=');

        if(pos == string::npos) {
            continue;
        }
        string key = content.substr(0, pos);
        if(key.empty()) {
            continue;
        }

        string value = content.substr(pos + 1);
        if(value.empty()){
            continue;
        }

        pair<string, string> p(key, value);
        cfgData_.insert(p);
    }

    cfgFile.close();
}

Config::~Config() {

}

int
Config::GetInt(string key, int defaultVal) {
    string val = getValue(key);
    if (val.empty()) {
        return defaultVal;
    }
    int value = static_cast<int> (atoi(val.c_str()));
    return value;
}

bool
Config::GetBool(string key, bool defaultVal) {
    string val = getValue(key);
    if (val.empty()) {
        return defaultVal;
    }
    if (val == "true"){
        return true;
    } else if (val == "false"){
        return false;
    }
    return defaultVal;
}

string
Config::GetString(string key, string defaultVal) {
    string val = getValue(key);
    if (val.empty() || val.size() < 3) {
        return defaultVal;
    }
    size_t len = val.size();
    if (val[0] != '"' || val[len - 1] != '"') {
        return defaultVal;
    }
    string content = val.substr(1,len - 2);
    return content;
}

double
Config::GetDouble(string key, double defaultVal) {
    string val = getValue(key);
    if (val.empty()) {
        return defaultVal;
    }
    double value = static_cast<double> (atof(val.c_str()));
    return value;
}

string
Config::getValue(string key) {
    string val("");
    if (key.empty()) {
        return val;
    }
    map<string,string>::iterator it;
    it = cfgData_.find(key);
    if (it == cfgData_.end()) {
        return val;
    }
    return it->second;
}