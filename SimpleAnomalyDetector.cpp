/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Liron Weizman 206505588
 */
#include <string>
#include <vector>
#include "SimpleAnomalyDetector.h"
#include <string.h>
#include <string>
#include <math.h>
#include <iostream>
#include <iomanip>
using namespace std;


SimpleAnomalyDetector::SimpleAnomalyDetector()
{
	my_threshold = 0.9;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector()
{
}

// for array of points and regression line find the max devision and this will be the threshold
float SimpleAnomalyDetector::find_threshold(Point **points, Line reg_line, int size)
{
	float threshold = 0;
	for (int i = 0; i < size; i++)
	{
		float d = dev(*points[i], reg_line);
		if (d > threshold)
		{
			threshold = d;
		}
	}
	return 1.1 * threshold;
}

//We will take a file of a normal flight and check for each feature which of the other features
// is most correlative to it according to the pearson method.
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts)
{
	int j;
	int index_i, index_j;
	int size_of_features = ts.get_features().size();

	for (int i = 0; i < size_of_features -1; i++)
	{

		string it_i = ts.get_features().at(i);
		int size = ts.get_vector(it_i).size();
		vector<float> c_a = ts.get_vector(it_i);
		float* col1 = c_a.data();
		
		string feature1, feature2, it_j;
		float p = 0, max_corelation = 0, threshold;
		float* col2;
		float* best_col2;
		Line line;
		float *max_col2;

		for (j = i+1; j < size_of_features; j++)
		{
			it_j = ts.get_features().at(j);
			vector<float> c_b = ts.get_vector(it_j);
			col2 = c_b.data();
			p = fabs(pearson(col1, col2, size));

			//update the max corelation:
			if (p > max_corelation)
			{
				max_corelation = p;
				feature1 = it_i;
				feature2 = it_j;
				max_col2 = ts.get_vector(it_j).data();
				index_i = i;
				index_j = j;
			}
		} //end of inner for loop

		learn_normal_helper(feature1, feature2, max_corelation, ts.get_vector(feature1).data(),
										  ts.get_vector(feature2).data(), size);
	}
}

//add the corelated features to cf vector.
void SimpleAnomalyDetector::learn_normal_helper(string feature1, string feature2, 
				float max_corelation, float *col1, float *col2, int size)
{
	if(max_corelation >= this->my_threshold){ ////!!!!!!!
		Point **points = get_points(col1, col2, size);
		correlatedFeatures c;
		c.feature1 = feature1;
		c.feature2 = feature2;
		c.corrlation = max_corelation;
		c.lin_reg = linear_reg(points, size);
		c.threshold = find_threshold(points, c.lin_reg, size);
		cf.push_back(c);
		delete_points(points, size);
	}
}

/*
We will receive the input while flying - that is, line by line. 
For each two-dimensional point we will measure its distance from the regression line
we learned for the features that created it. If this distance is large enough than the
thrashold we saw for these features we will warn of an exception and include in the 
report the properties involved.
*/
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts)
{
	vector<AnomalyReport> reports;
	
	int size;
	if(!cf.empty()){
		size = ts.get_size_vector(cf.at(0).feature1);
	}
	else{
		size = 0;
	} 

	for (int i = 0; i < cf.size(); ++i)
	{
		for (long time_step = 0; time_step < size; time_step++)
		
		{
			string feature1 = cf.at(i).feature1;
			string feature2 = cf.at(i).feature2;
			float x_point = ts.find_value(feature1, time_step);
			float y_point = ts.find_value(feature2, time_step);
			Point point = Point(x_point,y_point);

			if (detect_helper(cf[i], point)){
				string descruption = feature1 + "-" + feature2;
				reports.push_back(AnomalyReport(descruption, time_step + 1));

			}
		}		
			
	}	
	return reports;
}

bool SimpleAnomalyDetector::detect_helper(correlatedFeatures cf, Point point){
	if ((dev(point, cf.lin_reg)) > cf.threshold){
		return true;
	}
	return false;
}


	
	


