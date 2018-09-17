#include "webpage.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <locale>

using namespace std;

WebPage::WebPage(string file){
	parseFile(file);
	pageLink = file;
 	pRank = 0;
 	pRankO = 0;
}
WebPage::~WebPage(){

}

bool WebPage::operator<(const WebPage& rhs) const 
{
   return pageLink < rhs.pageLink;  //assume that you compare the record based on a
}

//Function to parse the file.
void WebPage::parseFile(string fileName){
	ifstream input(fileName);
	string curr = "";

	while(getline(input, curr)){
		string toPrint = "", word = "";
		bool inLink = false;

		for(unsigned int i = 0; i < curr.size(); i++){
			//Converts to a string.
			string s(1, curr[i]);

			//if ( --> program knows it's in a link.
			if(curr[i] == '(')
				inLink = true;

			//Push the link to link lists and start pushing
			//as normal again.
			else if(curr[i] == ')'){
				stringLinks.insert(word);
				word = "";

				inLink = false;

			}

			else if(isalnum(curr[i])){
				if(!inLink)
					toPrint += s;
				if(isalpha(curr[i]) && !inLink)
					s[0] = (char)tolower(s[0]);

				word += s;
			}
			
			else if(curr[i] == '.'){
				if(inLink)
					word += s;
				else if(!inLink)
					toPrint += s;
			}

			//When encountering a non alphanumeric symbol
			//when not parsing a link, push the word, and
			//start a new one.
			else{
				if(!inLink){
					if(!word.empty())
						words.insert(word);
					word = "";
					toPrint += s;
				}
				else if(inLink){
					word += s;
				}
			}



		}

		if(!word.empty())
			words.insert(word);

		printLines.push_back(toPrint);
	}


}

void WebPage::print(ofstream &outfile){
	for(vector<string>::iterator it = printLines.begin(); it != printLines.end(); it++){
		outfile << *it << endl;
	}

}

//ADDING AND POPPING LINK FUNCITONS.
void WebPage::addinLink(WebPage &page) const{
	inLinks.push_back(&page);

}

void WebPage::popoutLink() const{
	outLinks.pop_back();
}

void WebPage::popinLink() const{
	inLinks.pop_back();

}

void WebPage::addoutLink(WebPage &page) const{
	outLinks.push_back(&page);
}


//GET FUNCTIOKNS.
set<string> WebPage::getWords(){
	return words;

}
vector<WebPage*>  WebPage::getILinks() const{
	return inLinks;
}

vector<WebPage*>  WebPage::getOLinks() const{
	return outLinks;
}

set<string> WebPage::getSLinks() const{
	return stringLinks;
}

string WebPage::getLink() const{
	return pageLink;
}



//Print function for testing.
void WebPage::printWords() {
	for(set<string>::iterator it = words.begin(); it != words.end(); it++){
		cout << *it << endl;
	}
}