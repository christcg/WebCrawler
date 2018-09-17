#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <sstream>

#include "webpage.h"
#include "config.h"

using namespace std;

//Intersect webpage between candidate set and links.
set<WebPage*> intersectWebPage(vector<WebPage*> links, set<WebPage*> candSet){
	set<WebPage*> result;
	for(vector<WebPage*>::iterator it = links.begin(); it < links.end(); it++){
		if(candSet.find(*it) != candSet.end()){
			result.insert(*it);
		}
	}

	return result;

}

//Passes into intersectString and unionString by reference for result set.
//Similar to old program except here it's been modified so the 
//results is an array fo vectors.
void intersectString(map<string, WebPage> &parsedPages, vector<string> query, 
	set<WebPage*> &result){

	for(map<string, WebPage>::iterator it = parsedPages.begin(); 
		it!= parsedPages.end(); it++){

		//Cyles through each parased page and then cycles through each word in the
		//query page. If any word is found, breaks out of the for loop
		//and adds Page to the  result set.
		bool hasWords = true;
		for(vector<string>::iterator qit = query.begin(); 
			qit != query.end(); qit++){
			if(!(it->second.getWords().count(*qit))){
				hasWords = false;
				break;
			}
		}

		if(hasWords){
			if(result.find(&(it->second)) == result.end()){
				result.insert(&(it->second));
			}

			//vector<WebPage> pages = it->second.getOLinks();
			for(unsigned x = 0; x < it->second.getOLinks().size(); x++){
				if(result.find(it->second.getOLinks()[x]) == result.end())
					result.insert(it->second.getOLinks()[x]);
			}

			//pages = it->second.getILinks();
			for(unsigned x = 0; x < it->second.getILinks().size(); x++){
				if(result.find(it->second.getILinks()[x]) == result.end())
					result.insert(it->second.getILinks()[x]);
			}
		}
	}


}

void unionString(map<string, WebPage> &parsedPages, vector<string> query, 
	set<WebPage*> &result){
	for(map<string, WebPage>::iterator it = parsedPages.begin(); 
		it!= parsedPages.end(); it++){

		//Cyles through each parased page and then cycles through each word in the
		//query page. If any word is found, breaks out of the for loop
		//and adds Page to the  result set.
		bool hasWord = false;
		for(vector<string>::iterator qit = query.begin(); 
			qit != query.end(); qit++){
			if(it->second.getWords().count(*qit)){
				hasWord = true;
				break;
			}
		}

		if(hasWord){
			if(result.find(&(it->second)) == result.end()){
				result.insert(&(it->second));
			}

			//vector<WebPage> pages = it->second.getOLinks();
			for(unsigned x = 0; x < it->second.getOLinks().size(); x++){
				if(result.find(it->second.getOLinks()[x]) == result.end())
					result.insert(it->second.getOLinks()[x]);
			}

			//pages = it->second.getILinks();
			for(unsigned x = 0; x < it->second.getILinks().size(); x++){
				if(result.find(it->second.getILinks()[x]) == result.end())
					result.insert(it->second.getILinks()[x]);
			}
		}
	}
}

//Changed so that the links are now vector of pointers.
//This way, when links are added to pages that are linked to,
//they are updated as well.
void addLinks(map<string, WebPage> &parsedPages){

	//Add all the outgoing links
	for(map<string, WebPage>::iterator it = parsedPages.begin(); 
		it!= parsedPages.end(); it++){
		set<string> links = it->second.getSLinks();

		//Cycles through vector of links in string form and finds them in the 
		//map to add to the WebPage vector.
		for(set<string>::iterator linkit = links.begin(); 
			linkit != links.end(); linkit++){
			if((parsedPages.find(*linkit)) != parsedPages.end())
			it->second.addoutLink((parsedPages.find(*linkit))->second);
		}
	}

	//Add all incoming links.
	for(map<string, WebPage>::iterator it = parsedPages.begin(); 
		it!= parsedPages.end(); it++){
		vector<WebPage*> obj = it->second.getOLinks();

		//Cycles through vector of links in string form and finds them in the 
		//map to add to the WebPage vector.
		for(vector<WebPage*>::iterator pageit = obj.begin(); 
			pageit != obj.end(); pageit++){
			parsedPages.find((*pageit)->getLink())->second.addinLink(it->second);
		}
	}

}

//Generic mergesort (modified from the one used in HW5)
void merge(vector<WebPage> &input, int lo, int mid, int hi){
	int a = 0, b = 0, c = lo;
	int x = mid - lo + 1, y = hi - mid;

	vector<WebPage> mLeft, mRight;
	for(int i = 0; i < x; i++){
		mLeft.push_back(input[lo +i ]);
	}
	for(int j = 0; j < y; j++){
		mRight.push_back(input[(mid+1) + j]);
	}

	// Depending on if increasing or decreasing sort
	// the comparators.
	while(a < x && b < y){
		if(mLeft[a].pRankO >= mRight[b].pRankO){
			input[c] = mLeft[a];
			a++;
		}
		else{
			input[c] = mRight[b];
			b++;
		}

		c++;
	}

	for(int i = a; i < x; i++){
		input[c] = mLeft[i];
		c++;
	}


	for(int j = b; j < y; j++){
		input[c] = mRight[j];
		c++;
	}
}

// This is a standard merge sort.
void mergeSort(vector<WebPage> &input, int lo, int hi){
	int mid;
	if(lo < hi){
		mid = lo + (hi - lo)/2;
		mergeSort(input, lo, mid);
		mergeSort(input, mid+1, hi);

		merge(input, lo, mid, hi);

	} 
}

void mSort(vector<WebPage>& input){
	mergeSort(input, 0, (int)input.size()-1);
}

//Page rank
vector<WebPage> pageRanked(set<WebPage*> candSet, Config conf){
	//READ FOR CONFIG FILE
	int steps = stoi(conf.fetch("STEP_NUMBER"));
	double e = stod(conf.fetch("RESTART_PROBABILITY"));
	double n = (double) candSet.size();

	//Goes through and adds self loop as well as sets initial page rank.
	for(set<WebPage*>::iterator vit = candSet.begin(); vit != candSet.end(); vit++){
		(*vit)->pRankO = (1.0/n);
		set<string> slinks = (*vit)->getSLinks();
		if(slinks.find((*vit)->getLink()) == slinks.end()){
			(*vit)->addoutLink(**vit);
			(*vit)->addinLink(**vit);
		}

	}

	//Iteratively completes the algorithm for pagerank.
	for(int x = 0; x < steps; x++){
		for(set<WebPage*>::iterator it = candSet.begin(); it != candSet.end(); it++){
			double sum = 0;
			set<WebPage*> lSet = intersectWebPage((*it)->getILinks(), candSet);
			for(set<WebPage*>::iterator lit = lSet.begin(); lit != lSet.end(); lit++){
				set<WebPage*> sSet = intersectWebPage((*lit)->getOLinks(), candSet);
				sum += ((*lit)->pRankO)/(double)sSet.size();

			}

			(*it)->pRank = ((1.0-e)*sum + (e/n));

		}

		for(set<WebPage*>::iterator reit = candSet.begin(); reit != candSet.end(); reit++){
			(*reit)->pRankO = (*reit)->pRank;
		}
	}


	//Pass into vector and sort through mergesort.
	vector<WebPage> sortLinks;
	for(set<WebPage*>::iterator sit = candSet.begin(); sit != candSet.end(); sit++){
		sortLinks.push_back(**sit);
	}

	mSort(sortLinks);

	//Delete the self loop if wasn't present before;
	//The vector is returned but this is so in future iterations there isn't multiple
	//self loops added.
	for(set<WebPage*>::iterator vit = candSet.begin(); vit != candSet.end(); vit++){
		set<string> slinks = (*vit)->getSLinks();
		if(slinks.find((*vit)->getLink()) == slinks.end()){
			(*vit)->popoutLink();
			(*vit)->popinLink();
		}

	}
	return sortLinks;

}


int main(int argc, char* argv[]){
	//Custom config class that parses the config files.
	Config conf;
	if (argc < 2){
		conf = Config("config.txt");
	}else {
		conf = Config(argv[1]);
	}

	string ind = conf.fetch("INDEX_FILE");
	string que = conf.fetch("QUERY_FILE");
	string out = conf.fetch("OUTPUT_FILE");

	ifstream index(ind);
	ifstream query(que);
	ofstream output(out);

	map<string, WebPage> indexedPages;
	string curr;

	//Parses each webage and stores into map using link location
	//as the key and the WebPage class as the value.
	while(getline(index, curr)){
		//If file doesn't exist, don't bother opening.
		ifstream test(curr);
		if(test.fail()){
			continue;
		}

		WebPage currPage(curr);
		indexedPages.insert(pair<string, WebPage>(curr, currPage));
	}

	addLinks(indexedPages);

	while(getline(query, curr)){

		//Gets each line and parses through.
		istringstream iss(curr);
		string cmd, search;

		//skip blank lines.
		if(curr.empty())
			continue;

		//Creates a string stream because can assume
		//queries are stopped at whitespaces (according to
		//assignment specifications)
		iss >> cmd;
		vector<string> queryWords;
		if(iss >> search){


			//AND queries.
			if(cmd == "AND"){

				//Converts to all lower case.
				for(string::size_type i = 0; i < search.length(); i++){
					if(isalpha(search[i]))
						search[i] = (char)tolower(search[i]);
				}
				set<WebPage*> results;
				queryWords.push_back(search);
				while(iss >> search){
					for(unsigned int i = 0; i < search.length(); i++){
						if(isalpha(search[i]))
							search[i] = (char)tolower(search[i]);
					}	
					queryWords.push_back(search);
				}

				intersectString(indexedPages, queryWords, results);

				vector<WebPage> sortedRes = pageRanked(results, conf);
				output << sortedRes.size() << endl;

				for(vector<WebPage>::iterator resit = sortedRes.begin(); 
					resit != sortedRes.end(); resit++){
					output << resit->getLink() << endl;
				}
			}

			//OR queries
			else if(cmd == "OR"){
				set<WebPage*> results;

				for(string::size_type i = 0; i < search.length(); i++){
					if(isalpha(search[i]))
						search[i] = (char)tolower(search[i]);
				}
				queryWords.push_back(search);
				while(iss >> search){
					for(unsigned int i = 0; i < search.length(); i++){
						if(isalpha(search[i]))
							search[i] = (char)tolower(search[i]);
					}
					queryWords.push_back(search);
				}

				unionString(indexedPages, queryWords, results);
				vector<WebPage> sortedRes = pageRanked(results, conf);
				output << sortedRes.size() << endl;

				for(vector<WebPage>::iterator resit = sortedRes.begin(); 
					resit != sortedRes.end(); resit++){
					output << resit->getLink() << endl;
				}
			}

			//INCOMING queries
			else if(cmd == "INCOMING"){
				//Checks for existenc eof the page.
				if(indexedPages.find(search) == indexedPages.end()){
					output << "Invalid query" << endl;
					continue;
				}

				//Finds the Webpage mentioned if prints as needed.
				WebPage temp = indexedPages.find(search)->second;

				vector<WebPage*> iset = temp.getILinks();

				output << iset.size() << endl;

				for(vector<WebPage*>::iterator it = iset.begin(); 
					it != iset.end(); it++){
					output << (*it)->getLink() << endl;
				}
			}

			//OUTGOING queries.
			else if(cmd == "OUTGOING"){	
				if(indexedPages.find(search) == indexedPages.end()){
					output << "Invalid query" << endl;
					continue;
				}

				WebPage temp = indexedPages.find(search)->second;
				vector<WebPage*> oset = temp.getOLinks();

				output << oset.size() << endl;

				for(vector<WebPage*>::iterator it = oset.begin(); 
					it != oset.end(); it++){
					output << (*it)->getLink() << endl;
				}

			}

			//PRINT queries.
			else if(cmd == "PRINT"){
				if(indexedPages.find(search) == indexedPages.end()){
					output << "Invalid query" << endl;
					continue;
				}

				output << search << endl;

				WebPage temp = indexedPages.find(search)->second;
				temp.print(output);
			}

			else{
				output << "Invalid query" << endl;
			}	

		}

		//If there is no second word, treat the command as a search query.
		else if(!(iss >> search)){
			search = cmd; 
			for(string::size_type i = 0; i < search.length(); i++){
				if(isalpha(search[i]))
					search[i] = (char)tolower(search[i]);
			}
			queryWords.push_back(search);
			set<WebPage*> results;
			intersectString(indexedPages, queryWords, results);

			vector<WebPage> sortedRes = pageRanked(results, conf);
			output << sortedRes.size() << endl;

			for(vector<WebPage>::iterator resit = sortedRes.begin(); 
				resit != sortedRes.end(); resit++){
				output << resit->getLink() << endl;
			}
		}

		else{
			output << "Invalid query" << endl;
		}

	}



	return 0;
}