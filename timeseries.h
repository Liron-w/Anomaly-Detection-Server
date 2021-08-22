

#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <string>
//#include "string.h"

using namespace std;
#include <string>

class TimeSeries
{
	vector<std::string> m_features;
	map<string, vector<float>> m_myFile;

public:
	TimeSeries(const char *CSVfileName);

	float find_value(string feacher, int time) const;

	vector<string> get_features() const;

	map<string, vector<float>> get_map() const;

	vector<float> get_vector(string feature) const;

	int get_size_vector(string feature) const;
};

#endif /* TIMESERIES_H_ */
