#include <iostream>
#include <set>


template <class T>
std::set<T> union(std::set<T>& lhs, std::set<T>& rhs){
	std::set<T> result = rhs;

	for(std::set<T>::iterator it = lhs.begin(); 
		it!= lhs.end(); it++){
		if(result.find(*it) == result.end())
			result.insert(*it);

	}

	return result;
}


template <class T>
std::set<T> intersect(std::set<T>& lhs, std::set<T>& rhs){
	std::set<T> result;

	for(std::set<T>::iterator it = rhs.begin(); 
		it!= rhs.end(); it++){
		if(lhs.find(*it) != lhs.end())
			result.insert(*it);
	}

	return result;
	
}