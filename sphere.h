#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere: public hittable{
    public:
        sphere(const point3& static_center, double radius, shared_ptr<material> mat): center(static_center, vec3(0,0,0)), radius(std::fmax(0,radius)), mat(mat) {
            auto rvec=vec3(radius,radius,radius);
            bbox=aabb(static_center-rvec,static_center+rvec);
        }
        sphere(const point3& center1, const point3& center2, double radius,shared_ptr<material> mat): center(center1, center2 - center1), radius(std::fmax(0,radius)), mat(mat) {}

        bool hit(const ray& r,interval ray_t,hit_record& rec)const override{
            point3 current_center = center.at(r.time());
            vec3 oc = current_center - r.origin();  
            auto a=r.direction().length_squared();
            auto h=dot(r.direction(),oc);
            auto c=oc.length_squared()-radius*radius;

            auto discriminant=h*h-a*c;
            if(discriminant<0){
                return false;
            }

            auto sqrtd=std::sqrt(discriminant);

            auto root=(h-sqrtd)/a;
            if(!ray_t.surrounds(root)){
                root=(h+sqrtd)/a;
                if(!ray_t.surrounds(root))
                    return false;
            }

            rec.t=root;
            rec.p=r.at(rec.t);
            vec3 outward_normal = (rec.p - current_center) / radius;
            rec.set_face_normal(r,outward_normal);
            rec.mat=mat;

            return true;

        }

        aabb bounding_box()const override{return bbox;}
    
    private:
        ray center;
        double radius;
        shared_ptr<material>mat;
        aabb bbox;
};

#endif