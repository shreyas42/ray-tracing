#pragma once

struct vec3 {
    float e[3];

    vec3(): e{0, 0, 0} {}
    vec3(float e0, float e1, float e2): e{e0, e1, e2} {}

    float x() const { return e[0]; }
    float y() const { return e[1]; }
    float z() const { return e[2]; }
    
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    float operator[](int idx) const { return e[idx]; }
    float& operator[](int idx) { return e[idx]; }

    vec3& operator+=(const vec3& other) {
        e[0] += other.e[0];
        e[1] += other.e[1];
        e[2] += other.e[2];
        return *this;
    }

    vec3& operator*=(const vec3& other) {
        e[0] *= other.e[0];
        e[1] *= other.e[1];
        e[2] *= other.e[2];
        return *this;
    }

    vec3& operator/=(const vec3& other) {
        e[0] /= other.e[0];
        e[1] /= other.e[1];
        e[2] /= other.e[2];
        return *this;
    }

    float length() const {
        return std::sqrt(length_squared());
    }

    float length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
};

// vec3 is used to represent points in 3d geometric space as well as color values
// point3 is just a useful alias for readability
// onus is on caller not to do weird things like adding colors and points, etc.
using point3 = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline vec3 operator+(const vec3& a, const vec3& b) { return vec3(a.e[0] + b.e[0], a.e[1] + b.e[1], a.e[2] + b.e[2]); }
inline vec3 operator-(const vec3& a, const vec3& b) { return vec3(a.e[0] - b.e[0], a.e[1] - b.e[1], a.e[2] - b.e[2]); }
inline vec3 operator*(const vec3& a, const vec3& b) { return vec3(a.e[0] * b.e[0], a.e[1] * b.e[1], a.e[2] * b.e[2]); }
inline vec3 operator*(const vec3& a, float b) { return vec3(a.e[0] * b, a.e[1] * b, a.e[2] * b); }
inline vec3 operator*(float a, const vec3& b) { return b * a; }
inline vec3 operator/(const vec3& a, float b) { return vec3(a.e[0] / b, a.e[1] / b, a.e[2] / b); }

inline float dot(const vec3& a, const vec3& b) { return a.e[0]*b.e[0] + a.e[1]*b.e[1] + a.e[2]*b.e[2]; }
inline vec3 cross(const vec3& a, const vec3& b) { return vec3(a.e[1]*b.e[2] - a.e[2]*b.e[1], a.e[2]*b.e[0] - a.e[0]*b.e[2], a.e[0]*b.e[1] - a.e[1]*b.e[0]); }
inline vec3 unit_vector(const vec3& a) { return a / a.length(); }
