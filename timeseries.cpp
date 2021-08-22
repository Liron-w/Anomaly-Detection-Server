#include <string>
#include "timeseries.h"
#include <iostream>
#include <vector>
#include <fstream>
#include "string.h"
#include <map>
#include <sstream>
#include <string>

using namespace std;

TimeSeries::TimeSeries(const char *CSVfileName)
{
	//find the features:
	fstream myFile;
	myFile.open(CSVfileName);
	string line, word, first_line;

	//created the paurs of feature and vector in the map:
	getline(myFile, first_line);
	stringstream t(first_line);
	while (getline(t, word, ','))
	{
		m_features.push_back(word);
		vector<float> col;
		m_myFile.insert(pair<string, vector<float>>(word, col));
	}

	//initializes the values in each column vector:

	while (getline(myFile, line)) //  && line != "done"///////////add!
	{
		stringstream t(line);
		for (vector<string>::iterator iter = m_features.begin(); iter != m_features.end(); ++iter)
		{
			getline(t, word, ',');
			m_myFile[*iter].push_back(stof(word));
		}
	}

	myFile.close();
}

//find the vector of "feature" and return the value in the "time" line
float TimeSeries::find_value(string feature, int time) const
{
	if (m_myFile.count(feature))
	{
		map<string, vector<float>>::const_iterator it = m_myFile.find(feature);
		return it->second.at(time);
	}
	return 0; //change to infinty
}

//return vector of the features
vector<string> TimeSeries::get_features() const
{
	return m_features;
}

//return the map
map<string, vector<float>> TimeSeries::get_map() const
{
	return m_myFile;
}

//return the vector values of the feature key
vector<float> TimeSeries::get_vector(string feature) const
{
	map<string, vector<float>>::const_iterator it = m_myFile.find(feature);
	return it->second;
}

//return the vector size of the the feature
int TimeSeries::get_size_vector(string feature) const
{
	return get_vector(feature).size();
	/*
	map<string, vector<float>>::const_iterator it = m_myFile.find(feature);
	return it->second.size();*/
}
