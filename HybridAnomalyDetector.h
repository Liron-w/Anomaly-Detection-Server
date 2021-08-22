/*
 * HybridAnomalyDetector.cpp
 *
 * Author: Liron Weizman 206505588
 */

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
protected:
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
	bool detect_helper(correlatedFeatures cf, Point point) override;
	void learn_normal_helper(string feature1, string feature2, 
				float max_corelation, float *col1, float *col2, int size) override;
	void set_threshold(float threshold);
	float get_threshold();

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
