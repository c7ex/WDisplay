#ifndef POINT2D_H
#define POINT2D_H

#include <cmath>
#include <stdexcept>

class Vec2d {
public:
    double x, y;

    Vec2d();
    Vec2d(double x, double y);

    Vec2d operator+(const Vec2d& other) const;
    Vec2d operator-(const Vec2d& other) const;
    Vec2d operator*(const Vec2d& other) const;
    Vec2d operator/(const Vec2d& other) const;

    Vec2d operator*(double scalar) const;
    Vec2d operator/(double scalar) const;

    Vec2d& operator+=(const Vec2d& other);
    Vec2d& operator-=(const Vec2d& other);
    Vec2d& operator*=(double scalar);
    Vec2d& operator/=(double scalar);

    void invert_y();

    bool operator==(const Vec2d& other) const;
    bool operator!=(const Vec2d& other) const;
};

typedef Vec2d Size2d;
typedef Vec2d Scale2d;
typedef Vec2d Offset2d;
typedef Vec2d Position2d;

#endif // POINT2D_H