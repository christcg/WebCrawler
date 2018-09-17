#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <stack>

#include "webpage.h"
#include "config.h"

using namespace std;

void linkDFS(map<string, WebPage>  &seedPages, ofstream &index){
	stack<WebPage> pages;
	set<string> visited;
	for(map<string, WebPage>::iterator it = seedPages.begin(); 
		it!= seedPages.end(); it++){
		pages.push(it->second);

		while(!pages.empty()){
			WebPage curr = pages.top();
			pages.pop();

			if(visited.find(curr.getLink()) == visited.end()){
				index << curr.getLink() << endl;
				visited.insert(curr.getLink());
			}

			set<string> links = curr.getSLinks();
			for(set<string>::iterator pageit = links.begin(); 
			pageit != links.end(); pageit++){
				if(!(visited.count(*pageit))){
					ifstream test(*pageit);
					if(test.fail()){
						continue;
					}

					WebPage newPage(*pageit);
					pages.push(newPage);
					visited.insert(*pageit);
					index << *pageit << endl;
				}
			}
		}
	}
}

int main(int argc, char* argv[]){
	Config conf;
	if (argc < 2){
		conf = Config("config.txt");
	}else {
		conf = Config(argv[1]);
	}

	string input = conf.fetch("INDEX_FILE");
	string output = conf.fetch("OUTPUT_FILE");

	ifstream seed(input);
	ofstream ofile(output);

	map<string, WebPage> indexedPages;
	string curr;

	//Parses each webage and stores into map using link location
	//as the key and the WebPage class as the value.
	while(getline(seed, curr)){
		//If file doesn't exist, don't bother opening.
		ifstream test(curr);
		if(test.fail()){
			continue;
		}

		WebPage currPage(curr);
		indexedPages.insert(pair<string, WebPage>(curr, currPage));
	}
	
	linkDFS(indexedPages, ofile);

	return 0;
}