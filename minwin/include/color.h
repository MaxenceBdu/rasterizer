//!
//! \brief     Declares class Color and related elements.
//!
//! \file      color.h
//! \copyright (c) 2021 Université d'Artois
//! \author    Tiago de Lima <tiago.delima@univ-artois.fr>
//!

#ifndef _MINWIN_COLOR_H_
#define _MINWIN_COLOR_H_

#include <sstream>
#include "SDL2/SDL.h"

namespace minwin {

class Color 
{
  public:
    Uint8 r;  //!< red
    Uint8 g;  //!< green
    Uint8 b;  //!< blue
    Uint8 a;  //!< alpha
};

bool operator==( const Color &, const Color &);
std::string to_string( const Color & );

// Some basic color names
static constexpr Color BLACK        {   0,  0,  0, 0 };
static constexpr Color WHITE        { 255,255,255, 0 };
static constexpr Color RED          { 255,  0,  0, 0 };
static constexpr Color GREEN        {   0,255,  0, 0 };
static constexpr Color BLUE         {   0,  0,255, 0 };
static constexpr Color CYAN         {   0,255,255, 0 };
static constexpr Color YELLOW       { 255,255,  0, 0 };
static constexpr Color MAGENTA      { 255,255,  0, 0 };
static constexpr Color GREY         { 128,128,128, 0 };
//static constexpr Color DARKGREY     { 219,219,219, 0 };

} // end of namespace minwin

#endif // _MINWIN_COLOR_H_

