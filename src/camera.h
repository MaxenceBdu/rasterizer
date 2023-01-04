#include "matrix.h"

#define DEFAULT_MOVE_SPEED 0.5
#define DEFAULT_ROT_SPEED 1

class Frustum {

    aline::Vec4r near, dist, left, right, top, bot;

public:
    Frustum(aline::real near, aline::real dist){
        this->near = aline::Vec4r({0.0, 0.0, -1.0, -near});

        this->dist = aline::Vec4r({0.0, 0.0, 1.0, dist});

        this->left = aline::Vec4r({2.0, 0.0, -1.0, 0.0});

        this->right = aline::Vec4r({-2.0, 0.0, -1.0, 0.0});

        this->top = aline::Vec4r({0.0, 2.0, -1.0, 0});

        this->bot = aline::Vec4r({0.0, -2.0, -1.0, 0.0});
    }

    aline::Vec4r get_near(){
        return near;
    }

    aline::Vec4r get_dist(){
        return dist;
    }

    aline::Vec4r get_left(){
        return left;
    }

    aline::Vec4r get_right(){
        return right;
    }

    aline::Vec4r get_top(){
        return top;
    }

    aline::Vec4r get_bot(){
        return bot;
    }
};

class Camera {
    aline::real aspect_ratio, focal_dist, move_speed, rot_speed;
    //, zoom_speed;
    aline::Vec4r position;
    aline::Vec3r orientation, translation, rotation;
    Frustum frustum;
    
public:

    Camera(aline::real aspect_ratio) : aspect_ratio(aspect_ratio), frustum(Frustum(0.1, 5.0))
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
        translation[axis] = 1.0;
    }

    void move_backward(uint axis){
        move_speed = -DEFAULT_MOVE_SPEED;
        translation[axis] = 1.0;
    }

    void rotate_cw(uint axis){
        rot_speed = DEFAULT_ROT_SPEED;
        rotation[axis] = 1.0;
    }

    void rotate_acw(uint axis){
        rot_speed = -DEFAULT_ROT_SPEED;
        rotation[axis] = 1.0;
    }

    void stop_movement(uint axis){
        move_speed = 0;
        translation[axis] = 0.0;
    }

    void stop_rotation(uint axis){
        rot_speed = 0;
        rotation[axis] = 0.0;
    }

    aline::Mat44r transform() const {
        aline::Mat44r translation_matrix({
            {1.0, 0.0, 0.0, -position[0]},
            {0.0, 1.0, 0.0, -position[1]},
            {0.0, 0.0, 1.0, -position[2]},
            {0.0, 0.0, 0.0, 1.0}
        });

        aline::real alpha = degrees_to_radians(orientation[0]);
        aline::real beta = degrees_to_radians(orientation[1]);
        aline::real gamma = degrees_to_radians(orientation[2]);
        aline::Mat44r rotation_matrix({
            {cos(beta)*cos(gamma), cos(beta)*sin(gamma), -sin(beta), 0.0},
            {sin(alpha)*sin(beta)*cos(gamma)-cos(alpha)*sin(gamma), sin(alpha)*sin(beta)*sin(gamma)+cos(alpha)*cos(gamma), sin(alpha)*cos(beta), 0.0},
            {cos(alpha)*sin(beta)*cos(gamma)+sin(alpha)*sin(gamma), cos(alpha)*sin(beta)*sin(gamma)-sin(alpha)*cos(gamma), cos(alpha)*cos(beta), 0.0},
            {0.0, 0.0, 0.0, 1.0}
        });

        return translation_matrix * inverse(rotation_matrix);
    }

    void update(){
        aline::Vec3r temp = translation * move_speed;
        aline::Vec4r trans = {temp[0], temp[1], temp[2], 0.0};
        position = position + trans; 

        orientation += (rotation * rot_speed);
    }

private:
    aline::real degrees_to_radians(aline::real x) const{
        return x * (M_PI / 180);
    }
};