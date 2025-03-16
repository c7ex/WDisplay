#ifndef POINT2D_H
#define POINT2D_H

#include <cmath>
#include <stdexcept>

class Point2D {
public:
    double x, y;

    Point2D();
    Point2D(double x, double y);

    Point2D operator+(const Point2D& other) const;
    Point2D operator-(const Point2D& other) const;
    Point2D operator*(const Point2D& other) const;
    Point2D operator/(const Point2D& other) const;

    Point2D operator*(double scalar) const;
    Point2D operator/(double scalar) const;

    Point2D& operator+=(const Point2D& other);
    Point2D& operator-=(const Point2D& other);
    Point2D& operator*=(double scalar);
    Point2D& operator/=(double scalar);

    double Length() const;
    Point2D Normalized() const;
    void Normalize();
    void Y_inversion();

    bool operator==(const Point2D& other) const;
    bool operator!=(const Point2D& other) const;
};

#endif // POINT2D_H
