#include "minCircle.h"


// calculate the distance between two points
float my_distance(Point p1, Point p2)
{
	float x1 = p1.x;
	float y1 = p1.y;
	float x2 = p2.x;
	float y2 = p2.y;
	return sqrtf(powf(x1 - x2, 2) + powf(y1 - y2, 2));
}

// return the circle with one point or two points or three points
Circle basicCircle(vector<Point> points_circle)
{
	// if we dont have border points
	if (points_circle.size() == 0)
	{
		//there is no points border
		return Circle(Point(0, 0), 0);
	}
	//if we have one border point
	else if (points_circle.size() == 1)
	{
		return Circle(points_circle[0], 0);
	}
	//if we have two border points - calculate the circle
	else if (points_circle.size() == 2)
	{
		float radius = (my_distance(points_circle[0], points_circle[1])) / 2;
		float x_circle = (points_circle[0].x + points_circle[1].x) / 2;
		float y_circle = (points_circle[0].y + points_circle[1].y) / 2;
		return (Circle(Point(x_circle, y_circle), radius));
	}
	else
	{ // if we have three border points

		//we found equation in the web to calculate the center of the circle when we have 3 points:
		float x1 = points_circle[0].x;
		float y1 = points_circle[0].y;
		float x2 = points_circle[1].x;
		float y2 = points_circle[1].y;
		float x3 = points_circle[2].x;
		float y3 = points_circle[2].y;

		float A = x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2;
		float B = (x1 * x1 + y1 * y1) * (y3 - y2) + (x2 * x2 + y2 * y2) * (y1 - y3) + (x3 * x3 + y3 * y3) * (y2 - y1);
		float C = (x1 * x1 + y1 * y1) * (x2 - x3) + (x2 * x2 + y2 * y2) * (x3 - x1) + (x3 * x3 + y3 * y3) * (x1 - x2);
		float D = (x1 * x1 + y1 * y1) * (x3 * y2 - x2 * y3) + (x2 * x2 + y2 * y2) * (x1 * y3 - x3 * y1) + (x3 * x3 + y3 * y3) * (x2 * y1 - x1 * y2);

		float x_center = -1 * B / (2 * A);
		float y_center = -1 * C / (2 * A);
		float radius = sqrtf(powf(x_center - x1, 2) + powf(y_center - y1, 2));
		return Circle(Point(x_center, y_center), radius);
	}
}

// checking if the last_point inside the circle
bool pointIsInCircle(Point last_point, Circle rec_circle)
{
	float dis = my_distance(last_point, rec_circle.center);
	if (dis > rec_circle.radius)
	{
		return false;
	}
	//else : distance <= radius
	return true;
}

// the function caluculate recursivly the the minimun circle
Circle recursiveFindCircle(Point **points, vector<Point> points_circle, size_t size)
{
	//base recusion
	if (size == 0 || points_circle.size() == 3)
	{
		return basicCircle(points_circle);
	}
	Point last_point = *points[size - 1];
	Circle rec_circle = recursiveFindCircle(points, points_circle, size - 1);
	//if the last point is inside the circle - we dont need to change the borders.
	if (pointIsInCircle(last_point, rec_circle))
	{
		return rec_circle;
	}
	//if the last point is not inside the circle - the point should be border point
	points_circle.push_back(last_point);
	//return in recrsion the circle with the new point border
	return recursiveFindCircle(points, points_circle, size - 1);
}

//the function call to recursive method that return the minimum circle
Circle findMinCircle(Point **points, size_t size)
{
	vector<Point> points_circle;
	Circle c = recursiveFindCircle(points, points_circle, size);
	return c;
}