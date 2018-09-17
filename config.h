#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>
#include <vector>

class Config{
	//Custom configuration class to parse.
	public:
		Config();
		Config(const char* filename);
		~Config();

		void parse(const char* filename);

		std::string fetch(std::string key);
		std::vector<std::string> parse(std::string input, char delim);

	private:
		std::map<std::string, std::string> parsedVal;

};

#endif