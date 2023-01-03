#include "matrix.h"

class Camera {
    aline::real aspect_ratio, focal_dist, move_speed, rot_speed;
    //, zoom_speed;
    aline::Vec4r position;
    aline::Vec3r orientation;

    Camera(aline::real aspect_ratio) : aspect_ratio(aspect_ratio)
    {
        focal_dist = 2.0;
        orientation = {0.0, 0.0, 0.0};
        position = {0.0, 0.0, 0.0, 1.0};
        move_speed = 0.1;
        rot_speed = 1;
        //zoom_speed = 0.0625;
    }

    aline::real get_aspect_ratio(){
        return aspect_ratio;
    }

    aline::real get_focal_dist(){
        return focal_dist;
    }

    aline::real get_move_speed(){
        return move_speed;
    }

    aline::real get_rot_speed(){
        return rot_speed;
    }

    //aline::real get_zoom_speed(){
    //    return zoom_speed;
    //}

    aline::Vec4r position(){
        return position;
    }

    aline::Vec3r orientation(){
        return orientation;
    }

    void move_forward(uint axis){
        if(axis < 3)
            position[axis]++;
    }

    void move_forward(uint axis){
        if(axis < 3)
            position[axis]--;
    }

    void rotate_cw(uint axis){
        aline::Vec3r a;
        if(axis == 0)
            a = {1.0, 0.0, 0.0};
        else if(axis == 1)
            a = {0.0, 1.0, 0.0};
        else if(axis == 2)
            a = {0.0, 0.0, 1.0};

        //aline::Vec2r r = {cos(rot_speed/2), a * sin(rot_speed/2)};
        if(axis < 3)
            orientation[axis]+=5;
    }

    void rotate_acw(uint axis){
        if(axis < 3)
            orientation[axis]-=5;
    }

    aline::Mat44r transform() const {
        aline::Mat44r translation_matrix({
            {1.0, 0.0, 0.0, -position[0]},
            {0.0, 1.0, 0.0, -position[1]},
            {0.0, 0.0, 1.0, -position[2]},
            {0.0, 0.0, 0.0, 1.0}
        });

        aline::Mat44r rotation_matrix({
            {},
            {},
            {},
            {}
        });
    }
};