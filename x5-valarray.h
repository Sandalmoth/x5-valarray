#ifndef __X5_VALARRAY_H__
#define __X5_VALARRAY_H__


#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <valarray>
#include <vector>

#include <H5Cpp.h>


namespace X5 {


  std::vector<std::string> split(const std::string &s, const std::string &d) {
    // Split a string by every occurrence of a delimiter (discarding the delimiter)
    std::vector<std::string> ss;
    size_t pos = 0;
    size_t oldpos = 0;
    while ((pos = s.find(d, pos)) != std::string::npos) {
      ss.emplace_back(s.substr(oldpos, pos - oldpos));
      pos += d.size();
      oldpos = pos;
    }
    ss.emplace_back(s.substr(oldpos, pos - oldpos));
    return ss;
  }


  std::string partial_split(std::string &s, const std::string &d) {
    if (s.size() == 0) // Cant split an empty string
      return std::string();
    size_t pos = s.find(d, 0);
    std::string ss = s.substr(0, pos);
    s.erase(0, ss.size() + d.size());
    return ss;
  }


  enum class FILE_MODE {
    READ
  };


  class Location {
  public:
    Location() { }
    Location(std::shared_ptr<H5::H5File> file, std::shared_ptr<H5::H5Location> location)
      : file(file)
      , location(location) { }


    static Location nowhere;


    void open(std::string filename, FILE_MODE mode) {
      unsigned int flags = 0;
      switch (mode) {
      case FILE_MODE::READ:
        flags = H5F_ACC_RDONLY;
        break;
      }
      file = std::make_shared<H5::H5File>(H5::H5File(filename, flags));
      location = std::static_pointer_cast<H5::H5Location>(file);
    }


    template <typename T>
    std::valarray<T> fetch() { }


    template <typename T>
    void store() { }


    Location operator[](std::string path) {
      std::string next = partial_split(path, "/");
      try {
        // Open or something
      } catch (...) {
        return nowhere;
      }
      Location l;
      if (path.size() == 0)
        return l;
      else
        return l[path];
    }


  private:
    std::shared_ptr<H5::H5File> file;
    std::shared_ptr<H5::H5Location> location;
  };


} // end namespace X5


#endif
