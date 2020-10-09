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

#ifndef HEADER_PRIO_FILE_WRITER_HPP
#define HEADER_PRIO_FILE_WRITER_HPP

#include <string>
#include <vector>
#include <memory>

namespace prio {

class WriterImpl;

class Writer final
{
public:
  static Writer json(std::ostream& out);
  static Writer fastjson(std::ostream& out);
  static Writer sexpr(std::ostream& out);

public:
  Writer(std::ostream& out);
  Writer(std::unique_ptr<WriterImpl> impl);
  Writer(Writer&& other) noexcept = default;
  ~Writer();

  /** collections contain an ordered sequence of objects */
  void begin_collection(std::string_view key);
  void end_collection();

  /** write an object into a collection, objects start a mapping for
      object properties */
  void begin_object(std::string_view type);
  void end_object();

  /** mappings contain name/value pairs */
  void begin_mapping(std::string_view key);
  void end_mapping();

  void begin_keyvalue(std::string_view key);
  void end_keyvalue();

  /** write a name/value pair inside a mapping */
  void write(std::string_view key, bool value);
  void write(std::string_view key, int value);
  void write(std::string_view key, float value);
  void write(std::string_view key, char const* value);
  void write(std::string_view key, std::string_view value);
  void write(std::string_view key, std::string const& value);

  void write(std::string_view key, std::vector<bool> const&);
  void write(std::string_view key, std::vector<int> const&);
  void write(std::string_view key, std::vector<float> const&);
  void write(std::string_view key, std::vector<std::string> const&);

  template<class E, class T>
  void write_enum(std::string_view key, E& value, T string2enum) {
    write_string(key, string2enum(value));
  }

  template<typename T>
  void write(std::string_view key, T const& value) {
    write_custom(*this, key, value);
  }

  template<typename Enum, typename Enum2String>
  void write(std::string_view key, Enum const& value, Enum2String enum2string)
  {
    write(key, enum2string(value));
  }

private:
  std::unique_ptr<WriterImpl> m_impl;
};

} // namespace prio

#endif

/* EOF */
