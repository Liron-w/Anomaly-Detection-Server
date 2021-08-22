/*
 * commands.h
 *
 * Author: Liron Weizman 206505588
 */
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include <algorithm>

using namespace std;

class DefaultIO
{
public:
	virtual string read() = 0;
	virtual void write(string text) = 0;
	virtual void write(float f) = 0;
	virtual void read(float *f) = 0;
	void upload_file(const string name_file)
	{
		ofstream file;
		file.open(name_file);
		string line = this->read();
		while (line != "done")
		{
			file << line << endl;
			line = this->read();
		}
		file.close();
	}
	
	virtual ~DefaultIO() {}

	// you may add additional methods here
};

class StandardIO : public DefaultIO
{
public:
	string read()
	{
		string str;
		cin >> str;
		return str;
	}
	void read(float *f)
	{
		cin >> *f;
	}
	void write(string text)
	{
		cout << text << endl;
	}
	void write(float f)
	{
		cout << f << endl;
	}
};

struct Info
{
	TimeSeries *ts_train;
	TimeSeries *ts_test;
	HybridAnomalyDetector *hybrid_detector = new HybridAnomalyDetector();
	vector<AnomalyReport> ar;
};

class Command
{
protected:
	DefaultIO *dio;
	Info *info;
	string description;

public:
	Command(DefaultIO *dio, Info *info) : dio(dio), info(info)
	{
	}
	virtual void execute() = 0;
	virtual ~Command() {}
	virtual void print_description()
	{
		this->dio->write(this->description);
	}
};

//first command:
class UploadFile : public Command
{
private:
	string up_file = "Please upload your local train CSV file.\n";
	string second_description = "Please upload your local test CSV file.\n";
	string upload_complete = "Upload complete.\n";

public:
	//constructor
	UploadFile(DefaultIO *dio, Info *info) : Command(dio, info)
	{
		this->description = "1.upload a time series csv file\n";
	}

	void execute()
	{

		dio->write(this->up_file);									//print to user the discription
		this->dio->upload_file("anomalyTrain.csv"); 				//upload the file

		(info->ts_train) = new TimeSeries("anomalyTrain.csv"); 		//save the time series in the info struct
		dio->write(this->upload_complete);

		dio->write(this->second_description); 						//the same - with test file
		this->dio->upload_file("anomalyTest.csv");

		(info->ts_test) = new TimeSeries("anomalyTest.csv");
		dio->write(this->upload_complete);

	
	}
	~UploadFile()
	{
	}
};

class CurrentCoralation : public Command
{
private:
	string type_threshold = "Type a new threshold\n";
	string new_valid_threshold = "please choose a value between 0 and 1.\n";

public:
	CurrentCoralation(DefaultIO *dio, Info *info) : Command(dio, info)
	{
		this->description = "2.algorithm settings\n";
	}

	void execute()
	{
		float t = info->hybrid_detector->get_threshold();
		string t_sting = to_string(t);
		t_sting.erase(t_sting.find_last_not_of('0') + 1, std::string::npos); 		//delete '0' at the end
		string current_corelaion = "The current correlation threshold is " + t_sting + "\n";
		float new_threshold;
		do
		{
			dio->write(current_corelaion);
			dio->write(this->type_threshold);
			dio->read(&new_threshold);
			if (new_threshold > 1 || new_threshold < 0) 							//if the threshold is invalid
			{
				dio->write(this->new_valid_threshold); 								//ask the user to valid threshold
			}
			else
			{
				info->hybrid_detector->set_threshold(new_threshold); 				//change the threshold
			}
		} while (new_threshold > 1 || new_threshold < 0);
	}
};

class DetectAnomalies : public Command
{
private:
	string des = "anomaly detection complete.\n";

public:
	DetectAnomalies(DefaultIO *dio, Info *info) : Command(dio, info)
	{
		this->description = "3.detect anomalies\n";
	}
	void execute()
	{
		//the server will run the algorithm on the previously uploaded CSV files:
		info->hybrid_detector->learnNormal(*info->ts_train);
		info->ar = info->hybrid_detector->detect(*info->ts_test);
		this->dio->write(this->des);
	}
};

class DisplayResults : public Command
{
public:
	DisplayResults(DefaultIO *dio, Info *info) : Command(dio, info)
	{
		this->description = "4.display results\n";
	}
	void execute()
	{
		for (AnomalyReport a : this->info->ar)
		{
			string report = to_string(a.timeStep) + "	" + a.description + "\n";
			dio->write(report);
		}
		dio->write("Done.\n");
	}
};

class Uploads : public Command
{
private:
	string up_anomalies = "Please upload your local anomalies file.\n";
	string upload_complete = "Upload complete.\n";

public:
	Uploads(DefaultIO *dio, Info *info) : Command(dio, info)
	{
		this->description = "5.upload anomalies and analyze results\n";
	}

	//sort the vector by time step
	static bool sort_by_timestep(const pair<bool, pair<long, string>> &a,
								 const pair<bool, pair<long, string>> &b)
	{
		return (a.second.first < b.second.first);
	}

	void execute()
	{
		dio->write(this->up_anomalies);
		
		//read the file and insert the pairs to vector
		string line, first_word, second_word;
		vector<pair<int, int>> anomalies_vec;
		int sum_of_exseptions = 0;
		ofstream ex_file;
		ex_file.open("exeptions.csv");
		line = dio->read();
		while (line != "done")
		{
			stringstream t(line);
			getline(t, first_word, ',');
			getline(t, second_word, ',');
			anomalies_vec.push_back(make_pair(stoi(first_word), stoi(second_word)));
			sum_of_exseptions += stoi(second_word) - stoi(first_word) + 1;
			line = dio->read();
		}
		ex_file.close();
		dio->write(this->upload_complete);

		//the amount of exceptions in the exceptions file
		int P = anomalies_vec.size();
		
		//the number of lines in the input
		int n;
		vector<string> features = this->info->ts_test->get_features();
		if (features.empty())
		{
			n = 0;
		}
		else
		{
			n = this->info->ts_test->get_size_vector(features.at(0));
		}

		//int n = this->info->ts_test->get_size_vector(this->info->ts_test->get_features().at(0));

		//The amount of time steps in which there was no exception in the exceptions file
		int N = n - sum_of_exseptions;

		//vector of bool, time step and description of the report
		vector<pair<bool, pair<long, string>>> pairs_of_ar;
		for (AnomalyReport a : this->info->ar)
		{
			pairs_of_ar.push_back(make_pair(false, make_pair(a.timeStep, a.description)));
		}
		//sort the vector by time step
		sort(pairs_of_ar.begin(), pairs_of_ar.end(), sort_by_timestep);

		//union detector reports that have the same description and continuity in timestep.
		vector<pair<int, int>> continues_reports = union_continues_reports(pairs_of_ar);

		//check the false positive snd true positive:
		float FP = 0, TP = 0, FN = 0, TN = 0;
		vector<pair<int, int>> check; // red_vec
		int green_start, green_end, red_start, red_end;

		for (int i = 0; i < continues_reports.size(); i++)
		{ // green
			bool bool_check = false;
			green_start = continues_reports.at(i).first;
			green_end = continues_reports.at(i).second;
			for (int j = 0; j < P; j++)
			{ // red
				red_start = anomalies_vec.at(j).first;
				red_end = anomalies_vec.at(j).second;

				if ((red_start <= green_start && green_start <= red_end) //cutting right
					|| (red_start <= green_end && green_end <= red_end)	 // cutting end
					|| (green_start < red_start && red_end < green_end)) // contain
				{
					bool_check = true;
					if (!count(check.begin(), check.end(), anomalies_vec.at(j)))
					{ 
						{
							TP++;
							check.push_back(anomalies_vec.at(j)); 
						}
					}
				}
			}
			if (!bool_check)
			{
				FP++;
			}
		}

		//any time frame in which there was an exception, and there is no cut with no reporting
		FN = P - check.size();

		vector<pair<int, int>> union_vec = anomalies_vec;
		union_vec.insert(union_vec.end(), continues_reports.begin(), continues_reports.end());
		TN = n - segment_union_length(union_vec);
		float tpr = TP / P;
		float true_positive_rate = (floor(tpr * 1000)) / 1000;
		float far = FP / N;
		float false_alarm_rate = (floor(far * 1000)) / 1000;

		dio->write("True Positive Rate: ");
		dio->write(true_positive_rate);
		dio->write("\n");

		dio->write("False Positive Rate: ");
		dio->write(false_alarm_rate);
		dio->write("\n");
	}

	// Returns sum of lengths covered by union of given segments
	int segment_union_length(const vector<pair<int, int>> &segment_vec)
	{
		int n = segment_vec.size();

		// Create a vector to store starting and ending points
		vector<pair<int, bool>> points_vec(n * 2);
		for (int i = 0; i < n; i++)
		{
			points_vec[i * 2] = make_pair(segment_vec[i].first, false);
			points_vec[i * 2 + 1] = make_pair(segment_vec[i].second, true);
		}
		// Sorting all points by point value
		sort(points_vec.begin(), points_vec.end());
		// Initialize result
		int res = 0;
		// To keep track of counts of current open segments
		int counter = 0;
		// Trvaerse through all points
		for (unsigned i = 0; i < n * 2; i++)
		{
			// If there are open points, then we add the difference between previous and current point.
			if (counter)
				res += (points_vec[i].first - points_vec[i - 1].first);

			// If this is an ending point, reduce, count of open points.
			if (points_vec[i].second)
			{
				counter--;
			}
			else
			{
				counter++;
			}
		}
		return res;
	}
	vector<pair<int, int>> union_continues_reports(vector<pair<bool, pair<long, string>>> pairs_of_ar)
	{
		vector<pair<int, int>> continues_reports;
		int size;
		if (!pairs_of_ar.empty())
		{
			size = pairs_of_ar.size();
		}
		else
		{
			size = 0;
		}

		int start, end, i, j;
		bool is_checked;
		for (start = 0; start < size; ++start)
		{
			if (!pairs_of_ar.at(start).first)
			{ // if not checked
				i = start;
				end = start;
				j = start + 1;
				while (j < size && pairs_of_ar.at(j).second.first <= pairs_of_ar.at(i).second.first + 1)
				{
					if ((pairs_of_ar.at(j).second.second == (pairs_of_ar.at(i).second.second)) && (!pairs_of_ar.at(j).first))
					{
						i = j;
						pairs_of_ar.at(j).first = true;
						end = j;
					}

					j++;
				}
				continues_reports.push_back(make_pair(pairs_of_ar.at(start).second.first,
													  pairs_of_ar.at(end).second.first));
			}
		}
		return continues_reports;
	}

};

class ExitMenu : public Command
{
public:
	ExitMenu(DefaultIO *dio, Info *info) : Command(dio, info)
	{
		this->description = "6.exit\n";
	}
	void execute()
	{
		delete (this->info->ts_train);
		delete (this->info->ts_test);
		delete (this->info->hybrid_detector);
	}
};

#endif /* COMMANDS_H_ */
