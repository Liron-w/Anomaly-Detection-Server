/*
 * SimpleAnomalyDetector.h
 *
 * Author: Liron Weizman 206505588
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_
#include <string>
#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <math.h>
#include <string>
#include "minCircle.h" 

using namespace std;

struct correlatedFeatures
{
	string feature1, feature2; // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
	float x_center;
	float y_center;
};

class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector
{
protected:
	vector<correlatedFeatures> cf;
	float my_threshold;

public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();

	virtual void learnNormal(const TimeSeries &ts);
	virtual vector<AnomalyReport> detect(const TimeSeries &ts);

	vector<correlatedFeatures> getNormalModel()
	{
		return cf;
	}
	float find_threshold(Point **points, Line reg_line, int size);

	
	virtual void learn_normal_helper(string feature1, string feature2, 
				float max_corelation, float *col1, float *col2, int size);
	virtual bool detect_helper(correlatedFeatures cf, Point point);
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
