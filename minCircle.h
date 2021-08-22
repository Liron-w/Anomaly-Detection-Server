// 206505588	208943555

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <math.h>
#include "anomaly_detection_util.h"

using namespace std;

// ------------ DO NOT CHANGE -----------


class Circle
{
public:
	Point center;
	float radius;
	Circle(Point c, float r) : center(c), radius(r) {}
};
// --------------------------------------

// calculate the distance between two points
float my_distance(Point p1, Point p2);

// return the circle with one point or two points or three points
Circle basicCircle(vector<Point> points_circle);

// checking if the last_point inside the circle
bool pointIsInCircle(Point last_point, Circle rec_circle);

// the function caluculate recursivly the the minimun circle
Circle recursiveFindCircle(Point **points, vector<Point> points_circle, size_t size);

//the function call to recursive method that return the minimum circle
Circle findMinCircle(Point **points, size_t size);

// you may add helper functions here

#endif /* MINCIRCLE_H_ */
