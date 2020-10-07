// prio - Property I/O for C++
// Copyright (C) 2005-2020 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PRIO_READER_MAPPING_HPP
#define HEADER_PRIO_READER_MAPPING_HPP

#include <memory>
#include <vector>

namespace prio {

class ReaderCollection;
class ReaderMapping;
class ReaderMappingImpl;
class ReaderObject;

class ReaderMapping final
{
public:
  ReaderMapping();
  ReaderMapping(std::shared_ptr<ReaderMappingImpl> impl);

  std::vector<std::string> get_keys() const;

  bool read(const char* key, bool& value) const;
  bool read(const char* key, int& value) const;
  bool read(const char* key, float& value) const;
  bool read(const char* key, std::string& value) const;

  bool read(const char* key, ReaderMapping&) const;
  bool read(const char* key, ReaderCollection&) const;
  bool read(const char* key, ReaderObject&) const;

  ReaderMapping read_mapping(const char* key) const;
  ReaderCollection read_collection(const char* key) const;
  ReaderObject read_object(const char* key) const;

  template<class E, class T>
  bool read_enum (const char* key, E& value, T enum2string) const
  {
    std::string str;
    if(read(key, str))
    {
      value = enum2string(str);
      return true;
    }

    return false;
  }

  template<typename T>
  bool read(char const* name, T& value) const {
    return read_custom(*this, name, value);
  }

  template<typename T>
  T get(char const* name, T fallback) {
    read(name, fallback);
    return fallback;
  }

private:
  std::shared_ptr<ReaderMappingImpl> m_impl;
};

} // namespace prio

#endif

/* EOF */
