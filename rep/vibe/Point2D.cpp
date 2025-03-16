#include "Point2D.h"

Point2D::Point2D() : x(0.0), y(0.0) {}

Point2D::Point2D(double x, double y) : x(x), y(y) {}

Point2D Point2D::operator+(const Point2D& other) const {
    return Point2D(x + other.x, y + other.y);
}

Point2D Point2D::operator-(const Point2D& other) const {
    return Point2D(x - other.x, y - other.y);
}

Point2D Point2D::operator*(const Point2D& other) const {
    return Point2D(x * other.x, y * other.y);
}

Point2D Point2D::operator/(const Point2D& other) const {
    if ((other.x <= 0) || (other.y <= 0)) return Point2D(0, 0);
    return Point2D(x / other.x, y / other.y);
}

Point2D Point2D::operator*(double scalar) const {
    return Point2D(x * scalar, y * scalar);
}

Point2D Point2D::operator/(double scalar) const {
    if (scalar == 0.0) {
        throw std::runtime_error("Division by zero in Point2D::operator/");
    }
    return Point2D(x / scalar, y / scalar);
}

Point2D& Point2D::operator+=(const Point2D& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Point2D& Point2D::operator-=(const Point2D& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Point2D& Point2D::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Point2D& Point2D::operator/=(double scalar) {
    if (scalar == 0.0) {
        throw std::runtime_error("Division by zero in Point2D::operator/=");
    }
    x /= scalar;
    y /= scalar;
    return *this;
}

double Point2D::Length() const {
    return std::sqrt(x * x + y * y);
}

Point2D Point2D::Normalized() const {
    double len = Length();
    if (len == 0.0) {
        return Point2D(0.0, 0.0);
    }
    return *this / len;
}

void Point2D::Normalize() {
    double len = Length();
    if (len != 0.0) {
        *this /= len;
    }
}

void Point2D::Y_inversion() {
    y = -y;
}

bool Point2D::operator==(const Point2D& other) const {
    return x == other.x && y == other.y;
}

bool Point2D::operator!=(const Point2D& other) const {
    return !(*this == other);
}
