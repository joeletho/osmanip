#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <set>
#include "../include/progressbar.h"
#include "../include/csmanip.h"

namespace osm
 {
  //Definition of the "progress bar" class constructors and destructors:
  ProgressBar::ProgressBar(): 
   max_( 0 ), 
   min_( 0 ), 
   style_( "" ), 
   type_( "" ),
   message_( "" ), 
   time_count_( 0 ),
   brackets_open_( "" ), 
   brackets_close_( "" ), 
   color_( reset( "color" ) ) 
   {}

  ProgressBar::~ProgressBar() {}

  //ProgressBar static attributes definition:
  std::set <std::string> ProgressBar::set_p_ { "%", "/100" };
  
  std::set <std::string> ProgressBar::set_l_ { "#" };

  //ProgressBar setters definition:
  void ProgressBar::setMax( long long max )
   { 
    max_ = max; 
   }

  void ProgressBar::setMin( long long min )
   {
    min_ = min; 
   }

  void ProgressBar::setStyle( std::string type, std::string style )
   {
    error_style_ = "Inserted ProgressBar style" + static_cast <std::string>(" \"") + 
                   style + "\" is not supported for this type!\n",
    error_type_ = "Inserted ProgressBar type" + static_cast <std::string>(" \"") + 
                  type + "\" is not supported!\n";
    
    if( ( set_p_.find( style ) != set_p_.end() && type == "indicator" ) ||
        ( set_l_.find( style ) != set_l_.end() && type == "loader" ) )
     {
      style_ = style;
      type_ = type;
     }
    else if( ( set_p_.find( style ) == set_p_.end() && type == "indicator" ) ||
             ( set_l_.find( style ) == set_l_.end() && type == "loader" ) )
     {
      throw std::runtime_error( error_style_ );
     }
    else if( type == "complete" )
     {
      type_ = type;

      for( auto & element_p: set_p_ )
       {
        for( auto & element_l: set_l_ )
         {
          if( ( style.find( element_p ) != std::string::npos && style.find( element_l ) != std::string::npos ) && 
                style.length() == element_p.length() + element_l.length() )
           {
            style_ = style;
           }
         }
       }
      if( style_ != style )
       {
        throw std::runtime_error( error_style_ );
       }
     }
    else
     {
      throw std::runtime_error( error_type_ );
     }
   }

  void ProgressBar::setMessage( std::string message ) 
   { 
    message_ = message; 
   }

  void ProgressBar::setBegin() 
   { 
    begin = std::chrono::steady_clock::now();
   }

  void ProgressBar::setEnd()
   {
    end = std::chrono::steady_clock::now();
    time_count_ += std::chrono::duration_cast <std::chrono::milliseconds>( end - begin ).count();
   }
  
  void ProgressBar::setBrackets( std::string brackets_open, std::string brackets_close )
   { 
    brackets_open_ = brackets_open,
    brackets_close_ = brackets_close;
   }

  void ProgressBar::setColor( std::string color )
   { 
    color_ = feat( col, color );
   }

  //ProgressBar resetters definition:
  void ProgressBar::resetAll() 
   { 
    max_ = 0, 
    min_ = 0, 
    style_ = "", 
    type_ = "",
    message_ = "", 
    time_count_ = 0,
    brackets_open_ = "", 
    brackets_close_= "", 
    color_ = reset( "color" ); 
   }

  void ProgressBar::resetMax()
   { 
    max_ = 0;
   }

  void ProgressBar::resetMin()
   {
    min_ = 0;
   }

  void ProgressBar::resetStyle()
   {
    style_.clear();
    type_.clear();
   } 

  void ProgressBar::resetMessage()
   {
    message_.clear();
   } 

  void ProgressBar::resetTime()
   {
    time_count_ = 0;
   }

  void ProgressBar::resetBrackets() 
   {
    brackets_open_.clear(),
    brackets_close_.clear(); 
   }

  void ProgressBar::resetColor() 
   { 
    color_ = reset( "color" ); 
   }

  //ProgressBar getters definition:
  long long ProgressBar::getMax() const 
   { 
    return max_; 
   }

  long long ProgressBar::getMin() const 
   { 
    return min_;
   } 

  long long ProgressBar::getTime() const 
   {
    return time_count_;
   }

  long long ProgressBar::getIteratingVar() const 
   { 
    return iterating_var_; 
   }

  std::string ProgressBar::getStyle() const 
   { 
    return style_; 
   }

  std::string ProgressBar::getType() const 
   { 
    return type_; 
   }

  std::string ProgressBar::getMessage() const 
   { 
    return message_; 
   }

  std::string ProgressBar::getBrackets_open() const 
   {
    return brackets_open_; 
   }

  std::string ProgressBar::getBrackets_close() const 
   { 
    return brackets_close_; 
   }

  std::string ProgressBar::getColor() const 
   { 
    return color_; 
   }

  //Operator * redefinition definition to multiply strings by an integer:
  std::string operator * ( const std::string & generic_string, unsigned long long integer )
   {
    std::stringstream out;

    while ( integer-- ) 
     {
      out << generic_string; 
     }
     
    return out.str();
   }
 
  //ProgressBar update method definition:
  void ProgressBar::update( long long iterating_var )
   {
    iterating_var_ = 100 * ( iterating_var - min_ ) / ( max_ - min_ - 1 );
    width_ = ( iterating_var_ + 1 ) / 4;

    if( set_p_.find( style_ ) != set_p_.end() && type_ == "indicator" )
     {
      output_ = feat( crs, "left", 100 ) + 
                getColor() + 
                std::to_string( iterating_var_ ++ ) +
                reset( "color" ) + 
                getStyle();

      std::cout << output_ 
                << getColor() 
                << " " + message_ 
                << reset( "color" ) 
                << std::flush;
     }
    else if( set_l_.find( style_ ) != set_l_.end() && type_ == "loader" )
     {
      output_ = feat( crs, "left", 100 ) + 
                getBrackets_open() + 
                getColor() + 
                getStyle() * width_ + 
                static_cast <std::string>( " " ) * ( 25 - width_ ) + 
                reset( "color" ) + 
                getBrackets_close();  
                     
      std::cout << output_ 
                << getColor() 
                << " " + message_
                << reset( "color" ) 
                << std::flush;
     }
    else if ( type_ == "complete" )
     {
      for( auto & element_p: set_p_ )
       {
        for( auto & element_l: set_l_ )
         {
          if( style_.find( element_p ) != std::string::npos && style_.find( element_l ) != std::string::npos )
           {
            output_= feat( crs, "left", 100 ) + 
                     getBrackets_open() + 
                     getColor() + 
                     element_l * width_ + 
                     static_cast <std::string>( " " ) * ( 25 - width_ ) + 
                     reset( "color" ) +
                     getBrackets_close() + 
                     getColor() + 
                     " " + 
                     std::to_string( iterating_var_ ++ ) + 
                     reset( "color" ) + 
                     element_p; 

            std::cout << output_ 
                      << getColor() 
                      << " " + message_ 
                      << reset( "color" ) 
                      << std::flush;
           }
         }
       }
     }
    else
     {
      throw std::runtime_error( "ProgressBar style has not been set!" );
     }
   }

   //ProgressBar print method definition:
   void ProgressBar::print() const
    {
     std::cout << "Max: " << max_ << std::endl 
               << "Min: " << min_ << std::endl 
               << "Time counter: " << time_count_ << std::endl 
               << "Style: " << style_ << std::endl
               << "Message: " << message_ << std::endl
               << "Brackets style: " << brackets_open_ << brackets_close_<< std::endl
               << "Color: " << color_ << std::endl;
    }
 }