//====================================================
//     File data
//====================================================
/**
 * @file output_redirector.cpp
 * @author Joel Thomas (joelthomas.e@gmail.com)
 * @date 2022-10-5
 * @copyright Copyright (c) 2022 Gianluca Bianco under the MIT license.
 */

//====================================================
//     Headers
//====================================================

// My headers
#include "../../include/utility/output_redirector.hpp"
#include "../../include/utility/iostream.hpp"
#include "../../include/utility/strings.hpp"

// External headers
#include <arsenalgear/utils.hpp>

// STD headers
#include <stdexcept>
#include <mutex>
#include <filesystem>

namespace osm
{

  //====================================================
  //     Constructors and destructors
  //====================================================

  // Default constructor
  /**
   * @brief Construct a new OutputRedirector object with the default filename, or as provided by the user. The filename is relative to the current working directory.
   *
   * @param filename name of the output file.
   *
   */
  OutputRedirector::OutputRedirector( const std::string & filename ):
   enabled_( false ),
   file_dir_( fs::current_path() ),
   last_ansi_str_size_( 0 ),
   last_ansi_str_index_( 0 )
  {
    this->rdbuf( this );

    std::string cwd = fs::current_path().filename();
    std::string output_dir = ( cwd == "bin" || cwd == "build" ) ? "../output" : "output";
    setFileDirectory( output_dir );
    setFilename( filename );
  }

  // Destructor
  /**
   * @brief Destructs OutputRedirector object. If redirection is still enabled, the buffer is flushed before being destroyed.
   *
   */
  OutputRedirector::~OutputRedirector()
  {
    if( enabled_ )
    {
      this->pubsync();
      enabled_ = false;
    }
  }

  //====================================================
  //     Setters
  //====================================================

  // setFilename
  /**
   * @brief Sets the filename of the output file relative to the current working directory and resets internal data.
   *
   * @param filename the filename of the output file.
   *
   */
  void OutputRedirector::setFilename( const std::string & filename )
  {
    std::scoped_lock<std::mutex> slock { this->getMutex() };
    filename_.clear();
    filename_ = filename;
    create_filepath();

    output_str_.clear();
    last_ansi_str_index_ = 0;
    last_ansi_str_size_ = 0;
  }

  // setFileDirectory
  /**
   * @brief Sets the file directory of the output file. The file directory is relative to the current working directory.
   *
   * @param file_dir the file directory of the output file.
   *
   */
  void OutputRedirector::setFileDirectory( const std::string & file_dir )
  {
    std::scoped_lock<std::mutex> slock { this->getMutex() };
    file_dir_ = ( file_dir_ / file_dir ).lexically_normal();
  }

  //====================================================
  //     Getters
  //====================================================

  // getFilename
  /**
   * @brief Get the filename of the output file.
   *
   * @return string containing the filename of the output file.
   *
   */
  std::string & OutputRedirector::getFilename()
  {
    std::scoped_lock<std::mutex> slock { this->getMutex() };
    return filename_;
  }

  // getFilepath
  /**
   * @brief Get the absolute path of the output file.
   *
   * @return fs::path containing the absolute path of the output file.
   *
   */
  fs::path OutputRedirector::getFilepath()
  {
    std::scoped_lock<std::mutex> slock { this->getMutex() };
    return filepath_;
  }

  //====================================================
  //     Methods
  //====================================================

  // begin
  /**
   * @brief Enables output redirection
   *
   * @throws std::runtime_error if redirection is already enabled.
   *
   */
  void OutputRedirector::begin()
  {
    sanity_check( "begin" );
    enabled_ = true;
  }

  // end
  /**
   * @brief Flushes the buffer and disables output redirection.
   *
   * @throws std::runtime_error if redirection is not enabled.
   *
   */
  void OutputRedirector::end()
  {
    sanity_check( "end" );
    cout.flush();
    enabled_ = false;
  }

  // touch
  /**
   * @brief Opens the file, if present. Otherwise, creates the directory and file as needed.
   *
   * @throws std::invalid_argument if the file cannot be opened.
   *
   */
  void OutputRedirector::touch()
  {
    std::scoped_lock<std::mutex> slock { this->getMutex() };

    if( !fs::exists( file_dir_ ) )
    {
      fs::create_directories( file_dir_ );
    }

    if( fstream_.open( filepath_, std::fstream::in ); !fstream_.is_open() )
    {
      if( fstream_.open( filepath_, std::fstream::trunc | std::fstream::out ); !fstream_.is_open() )
      {
        exception_file_not_found();
      }
    }

    fstream_.close();
  }

  // isEnabled
  /**
   * @brief Returns the current state of the output redirection object.
   *
   * @return true if enabled. Otherwise, returns false.
   *
   */
  bool OutputRedirector::isEnabled()
  {
    return enabled_;
  }

  //====================================================
  //     Private methods
  //====================================================

  // sync
  /**
   * @brief Responsible for calling the appropriate output update functions and writing the result to the file.
   *
   * @return the error state of the stream buffer
   *
   */
  int32_t OutputRedirector::sync()
  {
    // Verify file is available
    touch();

    std::scoped_lock<std::mutex> slock { this->getMutex() };

    read_file();
    prepare_output();
    write_output();

    return fstream_.rdstate();
  }

  // setFileDirectory
  /**
   * @brief  the filepath of the output file by concatenating the file directory and filename.
   *
   */
  void OutputRedirector::create_filepath()
  {
    filepath_ = file_dir_ / filename_;
  }

  // prepare_output
  /**
   * @brief Updates and formats the output string and clears the buffer.
   *
   */
  void OutputRedirector::prepare_output()
  {
    output_str_ << this << std::flush;

    if( agr::is_escape( this->str(), agr::ANSI::generic ) )
    {
      output_str_.str( get_formatted_from_ansi( output_str_.str(), &last_ansi_str_index_, &last_ansi_str_size_ ) );
    }
    this->str( "" );
  }

  // write_output
  /**
   * @brief Writes the output string to the file. The file will be cleared before the string is written.
   *
   * @throws std::invalid_argument if there is a problem opening the file.
   *
   */
  void OutputRedirector::write_output()
  {
    if( fstream_.open( filepath_, std::fstream::trunc | std::fstream::out ); !fstream_.is_open() )
    {
      exception_file_not_found();
      return;
    }

    fstream_ << output_str_.rdbuf() << std::flush;
    fstream_.close();
  }

  // read_file
  /**
   * @brief Reads the file and stores its entire contents.
   *
   * @throws std::invalid_argument if there is a problem opening the file.
   *
   */
  void OutputRedirector::read_file()
  {
    if( fstream_.open( filepath_, std::fstream::in ); !fstream_.is_open() )
    {
      exception_file_not_found();
      return;
    }
    else
    {
      output_str_.str( "" );
      fstream_ >> output_str_.rdbuf();
      fstream_.close();
    }
  }

  // sanity_check
  /**
   * @brief Asserts the redirection flag is correct for the specified function.
   *
   * @throws std::runtime_error if the flag is incorrect.
   * @throws std::invalid_argument if an unknown function name is used.
   *
   */
  void OutputRedirector::sanity_check( const std::string & func_name )
  {
    if( func_name == "begin" )
    {
      if( enabled_ )
      {
        throw std::runtime_error( "Did you forget to call 'end()'?" );
      }
    }
    else if( func_name == "end" )
    {
      if( !enabled_ )
      {
        throw std::runtime_error( "Did you forget to call 'begin()'?" );
      }
    }
  }

  // exception_file_not_found
  /**
   * @brief Throws an invalid_argument exception if the file was not found or could not be opened. It is important to note that the calling thread is excepted to own the mutex. Calling this function will retain ownership of the mutex and release it before throwing the exception.
   *
   * @throws std::invalid_argument
   *
   */
  void OutputRedirector::exception_file_not_found()
  {
    std::string filename;
    {
      std::scoped_lock<std::mutex> slock { std::adopt_lock, this->getMutex() };
      filename = filepath_;
    }

    throw std::invalid_argument( std::string( "Could not open file " ) + "'" + filename + "'" );
  }

}      // namespace osm