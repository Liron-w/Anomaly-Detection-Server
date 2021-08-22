/*
 * animaly_detection_util.cpp
 *
 * Author: Liron Weizman 206505588
 */

#include <math.h>
#include "anomaly_detection_util.h"

float avg(float *x, int size)
{

    float sum = 0.0;
    for (int i = 0; i < size; i++)
    {
        sum = sum + x[i];
    }
    return (sum / (float)size);
}

// returns the variance of X and Y
float var(float *x, int size)
{
    float sum = 0.0;
    for (int i = 0; i < size; i++)
    {
        sum = sum + pow(x[i], 2);
    }
    float var = sum / (float)size - powf(avg(x, size), 2);
    return var;
}

// returns the covariance of X and Y
float cov(float *x, float *y, int size)
{
    float sum = 0.0;
    float avgX = avg(x, size);
    float avgY = avg(y, size);
    for (int i = 0; i < size; i++)
    {
        sum = sum + (x[i] - avgX) * (y[i] - avgY);
    }
    return sum / (float)size;
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float *x, float *y, int size)
{
    float p = (cov(x, y, size) / (sqrt(var(x, size)) * (sqrt(var(y, size))))); //i did (float) after return
    if (p > 1)
    {
        return 1;
    }

    return p;
}

// performs a linear regression and returns the line equation
Line linear_reg(Point **points, int size)
{

    float *x = new float[size];
    float *y = new float[size];
    for (int i = 0; i < size; i++)
    {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    float a = cov(x, y, size) / var(x, size);
    float b = avg(y, size) - a * avg(x, size);
    delete[] x;
    delete[] y;

    return Line(a, b);
}

Line linear_reg_from_arrays(float *x, float *y, int size)
{
    float a = cov(x, y, size) / var(x, size);
    float b = avg(y, size) - a * avg(x, size);

    return Line(a, b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size)
{
    Line l = linear_reg(points, size);
    float fx = l.a * p.x + l.b;
    float myY = p.y;
    return fabs(fx - myY);
}

// returns the deviation between point p and the line
float dev(Point p, Line l)
{
    float fx = l.a * p.x + l.b;
    float myY = p.y;
    return fabs(fx - myY);
}

Point **get_points(float *x, float *y, int size)
{

    Point **points = new Point *[size];
    for (int i = 0; i < size; i++)
    {

        points[i] = new Point(x[i], y[i]);
    }
    return points;
}

void delete_points(Point **points, int size)
{
    for (int k = 0; k < size; k++)
    {
        delete points[k];
    }
    delete[] points;
}
