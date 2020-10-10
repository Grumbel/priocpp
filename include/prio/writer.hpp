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

#include <filesystem>
#include <memory>
#include <span>
#include <string>
#include <vector>

#include "format.hpp"

namespace prio {

class Writer;
class WriterImpl;

template<typename T>
void write_custom(Writer& writer, std::string_view key, T const& value)
{
  static_assert(!std::is_same<T, T>::value,
                "no write_custom() specialisation found for the given type");
}

class Writer final
{
public:
  static Writer from_file(std::filesystem::path const& filename, Format format = Format::AUTO);
  static Writer from_stream(std::ostream& out, Format format = Format::AUTO);
  static Writer from_stream(std::unique_ptr<std::ostream> out, Format format = Format::AUTO);

public:
  Writer(std::ostream& out);
  Writer(std::unique_ptr<WriterImpl> impl);
  Writer(Writer&& other) noexcept = default;
  ~Writer();

  void write_comment(std::string_view text);

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

  void write(std::string_view key, std::span<bool const> values);
  void write(std::string_view key, std::span<int const> values);
  void write(std::string_view key, std::span<float const> values);
  void write(std::string_view key, std::span<std::string const> values);

  void write(std::string_view key, std::vector<bool> const& values);
  void write(std::string_view key, std::vector<int> const& values);
  void write(std::string_view key, std::vector<float> const& values);
  void write(std::string_view key, std::vector<std::string> const& values);

  template<typename T>
  void write(std::string_view key, T const& value) {
    write_custom<T>(*this, key, value);
  }

  template<typename Enum, typename Enum2String,
           std::enable_if_t<std::is_enum<Enum>::value> = 0>
  void write(std::string_view key, Enum const& value, Enum2String enum2string)
  {
    write(key, enum2string(value));
  }

private:
  std::unique_ptr<WriterImpl> m_impl;
  std::unique_ptr<std::ostream> m_owned;
};

} // namespace prio

#endif

/* EOF */
