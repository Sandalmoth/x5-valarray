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


  class LocationTypeException : public std::runtime_error { };


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
    std::valarray<T> fetch() {
      std::valarray<T> v;
      H5O_info_t info;
      H5Oget_info(location->getId(), &info);
      if (info.type == H5O_TYPE_DATASET) {
        std::cout << "opening dataset" << std::endl;
      } else {
        throw LocationTypeException(std::string(location) + " is not a dataset");
      }
    }


    template <typename T>
    void store() { }


    Location operator[](std::string path) {
      std::string next = partial_split(path, "/");
      // Does the next step exist
      if (H5Oexists_by_name(location->getId(), next.c_str(), H5P_DEFAULT) > 0) {
        H5O_info_t info;
        H5Oget_info_by_name(location->getId(), next.c_str(), &info, H5P_DEFAULT);
        std::shared_ptr<H5::H5Location> next_location;
        switch (info.type) {
        case H5O_TYPE_DATASET:
          next_location = std::static_pointer_cast<H5::H5Location>(std::make_shared<H5::DataSet>(std::dynamic_pointer_cast<H5::CommonFG>(location)->openDataSet(next)));
          break;
        case H5O_TYPE_GROUP:
          next_location = std::static_pointer_cast<H5::H5Location>(std::make_shared<H5::Group>(std::dynamic_pointer_cast<H5::CommonFG>(location)->openGroup(next)));
          break;
        default:
          std::cerr << "Unsupported datatype " << info.type << std::endl;
        }
      } else {
        return nowhere;
      }
      // if (H5Oexists_by_name(id_location, next.c_str(), H5P_DEFAULT) > 0) {
        // return Location(file, H5Oopen(location, next.c_str(), H5P_DEFAULT));
      // } else {
        // return nowhere;
      // }
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
