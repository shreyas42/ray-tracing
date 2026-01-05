#include "rt_weekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if(world.hit(r, interval(0, infinity), rec)) {
        return 0.5 * (rec.normal + color(1,1,1));
    }

    vec3 unit_direction = unit_vector(r.direction());
    float a = 0.5*(unit_direction.y() + 1.0);
    return (1.-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

int main(void) {
    // Image dimensions
    float aspect_ratio =  16. / 9.;
    int image_width = 400;
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera, dimensions are not int-quantized, but in real space
    float focal_length = 1.0;
    float viewport_height = 2.0;
    float viewport_width = viewport_height * (float(image_width) / image_height);
    point3 camera_center = point3(0, 0, 0);

    // Vectors along the viewport width and height
    vec3 viewport_u = vec3(viewport_width, 0., 0.);
    vec3 viewport_v = vec3(0, -viewport_height, 0.);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    vec3 pixel_delta_u = viewport_u / image_width;
    vec3 pixel_delta_v = viewport_v / image_height;

    // Calculate the position of the upper left pixel
    point3 viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    
    // Calculate the location of the 0,0 pixel
    point3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render the image
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    for(int y = 0; y < image_height; y++)
    {
        std::clog << "\rScanlines remaining: " << (image_height - y) << ' ' << std::flush;
        for(int x = 0; x < image_width; x++)
        {
            point3 pixel_center = pixel00_loc + (pixel_delta_u * x) + (pixel_delta_v * y);
            vec3 ray_direction = pixel_center - camera_center; // not a unit vector
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }
    
    std::clog << "\rFinished rendering.             \n";
    return 0;
}
