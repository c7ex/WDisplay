#include "Vec2d.h"

Vec2d::Vec2d() : x(0.0), y(0.0) {}

Vec2d::Vec2d(double x, double y) : x(x), y(y) {}

Vec2d Vec2d::operator+(const Vec2d& other) const {
    return Vec2d(x + other.x, y + other.y);
}

Vec2d Vec2d::operator-(const Vec2d& other) const {
    return Vec2d(x - other.x, y - other.y);
}

Vec2d Vec2d::operator*(const Vec2d& other) const {
    return Vec2d(x * other.x, y * other.y);
}

Vec2d Vec2d::operator/(const Vec2d& other) const {
    if ((other.x <= 0) || (other.y <= 0)) return Vec2d(0, 0);
    return Vec2d(x / other.x, y / other.y);
}

Vec2d Vec2d::operator*(double scalar) const {
    return Vec2d(x * scalar, y * scalar);
}

Vec2d Vec2d::operator/(double scalar) const {
    if (scalar == 0.0) {
        throw std::runtime_error("Division by zero in Point2D::operator/");
    }
    return Vec2d(x / scalar, y / scalar);
}

Vec2d& Vec2d::operator+=(const Vec2d& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vec2d& Vec2d::operator-=(const Vec2d& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vec2d& Vec2d::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vec2d& Vec2d::operator/=(double scalar) {
    if (scalar == 0.0) {
        throw std::runtime_error("Division by zero in Point2D::operator/=");
    }
    x /= scalar;
    y /= scalar;
    return *this;
}

void Vec2d::invert_y() {
    y = -y;
}

bool Vec2d::operator==(const Vec2d& other) const {
    return x == other.x && y == other.y;
}

bool Vec2d::operator!=(const Vec2d& other) const {
    return !(*this == other);
}