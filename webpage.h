#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <string>
#include <set>
#include <vector>

using namespace std;


class WebPage{
 public:
 	//CLASS DEFINITIONS
 	WebPage(string fileName);
 	~WebPage();

 	//Standard functions
 	void parseFile(string fileName);
 	void print(ofstream &outfile);
 	void addinLink(WebPage &page) const;
 	void addoutLink(WebPage &page) const;
 	void popoutLink() const;
 	void popinLink() const;

 	//Get functions for private members.
 	set<string> getWords();
 	vector<WebPage*>  getILinks() const;
 	vector<WebPage*>  getOLinks() const;
 	set<string> getSLinks() const;


 	mutable double pRank;
 	mutable double pRankO;


 	string getLink() const;

 	//Prints to console for debug purposes.
 	void printWords();

 	//Overload < in order to allow set creation.
 	bool operator<(const WebPage& rhs) const;

 private:
 	//set for easy searching.
 	set<string> words;

 	//Vector because pushing is used often but
 	//isn't searched beyond being iterated through to print.
 	mutable vector<WebPage*> outLinks;
 	mutable vector<WebPage*> inLinks;
 	vector<string> printLines;
 	set<string> stringLinks;

 	string pageLink;

 };

#endif
