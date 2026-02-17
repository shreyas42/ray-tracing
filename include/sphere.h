#pragma once

#include "hittable.h"

class sphere : public hittable {
  public:
    sphere(const point3& center, float radius, shared_ptr<material> mat) : center(center), radius(std::fmax(radius, 0)), mat(mat) {} 

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 dir = center - r.origin();
        float a = r.direction().length_squared();
        float h = dot(r.direction(), dir);
        float c = dir.length_squared() - radius*radius;

        float discriminant = h*h - a*c;
        if(discriminant < 0.) {
            return false;
        }

        float sqrtd = std::sqrt(discriminant);
        float root = (h - sqrtd) / a;

        if(!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if(!ray_t.surrounds(root)) {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(root);
        rec.mat = mat;
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        return true;
    }

  private:
    point3 center;
    float radius;
    shared_ptr<material> mat;
};
