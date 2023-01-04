#include "matrix.h"

#define DEFAULT_MOVE_SPEED 0.1
#define DEFAULT_ROT_SPEED 1

class Camera {
    aline::real aspect_ratio, focal_dist, move_speed, rot_speed;
    //, zoom_speed;
    aline::Vec4r position;
    aline::Vec3r orientation, translation, rotation;
    
public:
    Camera(){
        aspect_ratio = 1.0;
        focal_dist = 2.0;
        orientation = {0.0, 0.0, 0.0};
        position = {0.0, 0.0, 0.0, 1.0};
        move_speed = 0;
        rot_speed = 0; 
    }

    Camera(aline::real aspect_ratio) : aspect_ratio(aspect_ratio)
    {
        focal_dist = 2.0;
        orientation = {0.0, 0.0, 0.0};
        position = {0.0, 0.0, 0.0, 1.0};
        move_speed = 0;
        rot_speed = 0;
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

    aline::Vec4r get_position(){
        return position;
    }

    aline::Vec3r get_orientation(){
        return orientation;
    }

    void move_forward(uint axis){
        move_speed = DEFAULT_MOVE_SPEED;
        if(axis == 0)
            translation= {1.0, 0.0, 0.0};
        else if(axis == 1)
            translation= {0.0, 1.0, 0.0};
        else if(axis == 2)
            translation= {0.0, 0.0, 1.0};
    }

    void move_backward(uint axis){
        move_speed = -DEFAULT_MOVE_SPEED;
        if(axis == 0)
            translation= {1.0, 0.0, 0.0};
        else if(axis == 1)
            translation= {0.0, 1.0, 0.0};
        else if(axis == 2)
            translation= {0.0, 0.0, 1.0};
    }

    void rotate_cw(uint axis){
    }

    void rotate_acw(uint axis){
    }

    void stop_movement(){
        move_speed = 0;
    }

    void stop_rotation(){
        rot_speed = 0;
    }

    aline::Mat44r transform() const {
        aline::Mat44r translation_matrix({
            {1.0, 0.0, 0.0, -position[0]},
            {0.0, 1.0, 0.0, -position[1]},
            {0.0, 0.0, 1.0, -position[2]},
            {0.0, 0.0, 0.0, 1.0}
        });

        /*aline::Mat44r rotation_matrix({
            {},
            {},
            {},
            {}
        });*/
        return translation_matrix;
    }

    void update(){
        aline::Vec3r temp = translation * move_speed;
        aline::Vec4r trans = {temp[0], temp[1], temp[2], 0.0};
        position = position + trans; 
    }
};