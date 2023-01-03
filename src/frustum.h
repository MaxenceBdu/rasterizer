#include "vector.h"

class Frustum {

    aline::Vec4r near, dist, left, right, top, bot;

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