#pragma once

struct vec3 {
    float e[3];

    vec3(): e{0, 0, 0} {}
    vec3(float e0, float e1, float e2): e{e0, e1, e2} {}

    static vec3 random() {
        return vec3(random_float(), random_float(), random_float());
    }

    static vec3 random(float min, float max) {
        return vec3(random_float(min, max), random_float(min, max), random_float(min, max));
    }

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

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions
        float s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
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

inline vec3 random_in_unit_disk() {
    while(true) {
        vec3 p = vec3(random_float(-1, 1), random_float(-1, 1), 0);
        if(p.length_squared() < 1) {
            return p;
        }
    }
}

inline vec3 random_unit_vector() {
    while(true) {
        vec3 random_vector = vec3::random(-1, 1);
        float lensq = random_vector.length_squared();
        if(1e-38 < lensq && lensq <= 1)
            return random_vector / sqrt(lensq);
    }
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if(dot(on_unit_sphere, normal) > 0.) { // Same hemisphere as normal
        return on_unit_sphere;
    }
    else {
        return -on_unit_sphere;
    }
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v, n)*n;
}

inline vec3 refract(const vec3& uv, const vec3& n, float etai_over_etat) {
    float cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}
