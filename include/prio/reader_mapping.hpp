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

class ReaderMapping final
{
public:
  ReaderMapping();
  ReaderMapping(std::shared_ptr<ReaderMappingImpl> impl);

  std::vector<std::string> get_keys() const;

  bool read_bool(const char* key, bool& value) const;
  bool read_int(const char* key, int& value) const;
  bool read_float(const char* key, float& value) const;
  bool read_string(const char* key, std::string& value) const;

  bool read_mapping(const char* key, ReaderMapping&) const;
  bool read_collection(const char* key, ReaderCollection&) const;
  bool read_object(const char* key, ReaderObject&) const;

  ReaderMapping read_mapping(const char* key) const;
  ReaderCollection read_collection(const char* key) const;
  ReaderObject read_object(const char* key) const;

  template<class E, class T>
  bool read_enum (const char* key, E& value, T enum2string) const
  {
    std::string str;
    if(read_string(key, str))
    {
      value = enum2string(str);
      return true;
    }

    return false;
  }

private:
  std::shared_ptr<ReaderMappingImpl> m_impl;
};


#endif

/* EOF */
