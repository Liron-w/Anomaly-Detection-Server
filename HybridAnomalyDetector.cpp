/*
 * HybridAnomalyDetector.h
 *
 * Author: Liron Weizman 206505588
 */
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
}
HybridAnomalyDetector::~HybridAnomalyDetector() {
}
void HybridAnomalyDetector::learn_normal_helper(string feature1, string feature2, 
				float max_corelation, float *col1, float *col2, int size)
{	
	if(max_corelation >= this->my_threshold){
		SimpleAnomalyDetector::learn_normal_helper(feature1, feature2, max_corelation, col1, col2, size);
	}
		else if (max_corelation >= 0.5 && max_corelation < this->my_threshold){
		Point **points = get_points(col1, col2, size);
		Circle min_c = findMinCircle(points,size);
		correlatedFeatures c;
		c.feature1 = feature1;
		c.feature2 = feature2;
		c.corrlation = max_corelation;
		c.x_center = min_c.center.x;
		c.y_center = min_c.center.y;
		c.threshold = 1.1 * min_c.radius; // threshold is radius
		this->cf.push_back(c);	
		for (int k = 0; k < size; k++)
		{
			delete points[k];
		}
		delete[] points;		
	}
}


bool HybridAnomalyDetector::detect_helper(correlatedFeatures cf, Point point)
{
	if(cf.corrlation >= this->my_threshold){
		return SimpleAnomalyDetector::detect_helper(cf, point);
	}
	else{ // 0.5<=correlation<0.9
		return (!pointIsInCircle(point, Circle(Point(cf.x_center, cf.y_center), cf.threshold)));
	}
}


void HybridAnomalyDetector::set_threshold(float threshold){
	this->my_threshold = threshold;
	
}

float HybridAnomalyDetector::get_threshold(){
	return this->my_threshold;
}


