//====================================================
//     File data
//====================================================
/**
 * @file canvas.hpp
 * @author Miguel MJ (miguelmjvg@gmail.com)
 * @date 2022-06-06
 * @copyright Copyright (c) 2022 Gianluca Bianco under the MIT license.
 */

//====================================================
//     Preprocessor settings
//====================================================
#pragma once
#ifndef OSMANIP_CANVAS_HPP
#define OSMANIP_CANVAS_HPP

//====================================================
//     Headers
//====================================================

//STD headers
#include <vector>
#include <string>
#include <stdint.h>

namespace osm
 {
  //====================================================
  //     Enums
  //====================================================

  // FrameStyle
  /** 
   * @brief Enum class used to define the frame style.
   */
  typedef enum 
   {
    EMPTY = 0,
    ASCII = 1,
    BOX = 2
   } FrameStyle;

  //====================================================
  //     Classes
  //====================================================

  // Canvas
  /**
   * @brief Instances of this class are used to draw in a limited 2D space. All the functions that modify the canvas do it internally; to make the changes effective in the console, you must use the refresh method.
   * 
   */
  class Canvas
   {
    public:

     //====================================================
     //     Constructors
     //====================================================
     Canvas( uint32_t width, uint32_t height );

     //====================================================
     //     Setters
     //====================================================
     void enableFrame( bool frame_enabled );
     void setFrame( FrameStyle, const std::string& feat = "" );
     void setBackground( char c, const std::string& feat = "" );
     void setWidth( uint32_t width );
     void setHeight( uint32_t height );

     //====================================================
     //     Getters
     //====================================================
     char getBackground() const;
     std::string getBackgroundFeat() const;
     bool isFrameEnabled() const;
     std::string getFrameFeat() const;
     FrameStyle getFrameStyle() const;
     uint32_t getWidth() const;
     uint32_t getHeight() const;

     //====================================================
     //     Other methods
     //====================================================
     void clear();
     void put( uint32_t x, uint32_t y, char c, const std::string& feat = "" );
     void refresh();

    private:

     //====================================================
     //     Private attributes
     //====================================================
     bool frame_enabled_;
     FrameStyle frame_style_;
     std::string frame_feat_;
     char bg_char_;
     std::string bg_feat_;
     std::vector<char> char_buffer_;
     std::vector<std::string> feat_buffer_;
     bool already_drawn_;

     //====================================================
     //     Private methods
     //====================================================
     void resizeCanvas();

    protected:

     //====================================================
     //     Protected attributes
     //====================================================
     uint32_t width_, height_;
    };
 }

#endif
