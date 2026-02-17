#pragma once

#include "hittable.h"
#include "material.h"

class camera {
  public:
    float aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 50;

    float vfov = 90; // Vertical view angle (field of view)
    point3 lookfrom = point3(0,0,0); // Point camera is looking from
    point3 lookat = point3(0,0,-1); // Point cammera is looking at
    vec3 vup = vec3(0,1,0); // Camera-relative "up" ray_direction

    float defocus_angle = 0;
    float focus_dist = 10;

    void render(const hittable& world) {
        initialize();

        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

        for(int y = 0; y < image_height; y++) {
            std::clog << "\rScanlines remaining: " << (image_height - y) << ' ' << std::flush;

            for(int x = 0; x < image_width; x++) {
                color pixel_color(0, 0, 0); // accumulate the rays here
                for(int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(x, y);
                    pixel_color += ray_color(r, max_depth, world);
                }

                write_color(std::cout, pixel_samples_scale*pixel_color);
            }
        }

        std::clog << "\rFinished rendering.             \n";
    }

  private:
    int image_height;
    float pixel_samples_scale;
    point3 center, pixel00_loc;
    vec3 pixel_delta_u, pixel_delta_v;
    vec3 u, v, w; // Camera frame basis vectors
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        pixel_samples_scale = 1./samples_per_pixel;
        
        // Camera, dimensions are not int-quantized, but in real space
        center = lookfrom;

        float theta = degrees_to_radians(vfov);
        float h = std::tan(theta / 2);
        float viewport_height = 2*h*focus_dist;
        float viewport_width = viewport_height * (float(image_width) / image_height);
        // Calculate the u,v,w unit basis vectors for the camera coordinate frame
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        point3 viewport_upper_left = center - (focus_dist*w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        float defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    ray get_ray(int x, int y) const {
        vec3 offset = sample_square();
        point3 pixel_sample = pixel00_loc + ((x+offset.x())*pixel_delta_u) + ((y+offset.y())*pixel_delta_v);

        point3 ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        vec3 ray_direction = pixel_sample - ray_origin;
        return ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const {
        return vec3(random_float() - 0.5, random_float() - 0.5, 0);
    }

    point3 defocus_disk_sample() const {
        vec3 p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    color ray_color(const ray& r, int depth, const hittable& world) const {
        if(depth <= 0)
            return color(0,0,0);

        // Check if the ray intersects any geometry
        hit_record rec;
        if(world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if(rec.mat->scatter(r, rec, attenuation, scattered)) {
                return attenuation * ray_color(scattered, depth - 1, world);
            }
            return color(0., 0., 0.);
        }

        // Background coloring
        vec3 unit_direction = unit_vector(r.direction());
        float a = 0.5 * (unit_direction.y() + 1.);
        return (1.-a)*color(1., 1., 1.) + a*color(0.5, 0.7, 1.);
    }
};
