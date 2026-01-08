#pragma once

#include "hittable.h"

class camera {
  public:
    float aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;

    void render(const hittable& world) {
        initialize();

        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

        for(int y = 0; y < image_height; y++) {
            std::clog << "\rScanlines remaining: " << (image_height - y) << ' ' << std::flush;

            for(int x = 0; x < image_width; x++) {
                color pixel_color(0, 0, 0); // accumulate the rays here
                for(int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(x, y);
                    pixel_color += ray_color(r, world);
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

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        pixel_samples_scale = 1./samples_per_pixel;
        
        // Camera, dimensions are not int-quantized, but in real space
        center = point3(0, 0, 0);

        float focal_length = 1.0;
        float viewport_height = 2.0;
        float viewport_width = viewport_height * (float(image_width) / image_height);
        vec3 viewport_u = vec3(viewport_width, 0, 0);
        vec3 viewport_v = vec3(0, -viewport_height, 0);
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        point3 viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    ray get_ray(int x, int y) const {
        vec3 offset = sample_square();
        point3 pixel_sample = pixel00_loc + ((x+offset.x())*pixel_delta_u) + ((y+offset.y())*pixel_delta_v);
        vec3 ray_direction = pixel_sample - center;
        return ray(center, ray_direction);
    }

    vec3 sample_square() const {
        return vec3(random_float() - 0.5, random_float() - 0.5, 0);
    }

    color ray_color(const ray& r, const hittable& world) const {
        // Check if the ray intersects any geometry
        hit_record rec;
        if(world.hit(r, interval(0, infinity), rec)) {
            vec3 direction = random_on_hemisphere(rec.normal);
            return 0.5 * ray_color(ray(rec.p, direction), world);
        }

        // Background coloring
        vec3 unit_direction = unit_vector(r.direction());
        float a = 0.5 * (unit_direction.y() + 1.);
        return (1.-a)*color(1., 1., 1.) + a*color(0.5, 0.7, 1.);
    }
};
