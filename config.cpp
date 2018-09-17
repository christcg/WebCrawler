#include "config.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

Config::Config(){

}
Config::Config(const char* filename){
	parse(filename);
}
Config::~Config(){

}

//Splits the string into multiple strings by the 
//delimiter, and pushes to a vector.
vector<string> Config::parse(string input, char delim){
	vector<string> result;
	stringstream ss(input);
	string parsed;

	while(getline(ss, parsed, delim)){
		result.push_back(parsed);
	}

	return result;
}

//Main parsing function.
void Config::parse(const char* filename){
	ifstream file(filename);

	if(file.fail()){
		//Error
		cerr << "Failed to open" << endl;
		return;
	}

	string line;
	while(getline(file, line)){
		string key = "", value = "";
		auto lineSplit = parse(line, '#');

		line = lineSplit[0];

		bool switched = false;
		for(unsigned x = 0; x < line.size(); x++){
			string curr(1, line[x]);

			if(line[x] == '='){
				switched = true;
				continue;
			}

			if(line[x] == ' '){
				continue;
			}

			if(!switched){
				key += curr;
			}else if (switched){
				value += curr;
			}
		}

		//Pushes to map, where the value is the value and the
		//key is the name in the file.
		parsedVal.insert(pair<string, string>(key, value));
	}
}

//Returns a value by the key in the configuration file.
string Config::fetch(string key){
	string res = "";
	if(parsedVal.find(key) != parsedVal.end()){
		res = parsedVal.at(key);
	}

	return res;
}
