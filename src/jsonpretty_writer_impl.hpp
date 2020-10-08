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

#ifndef HEADER_PRIO_JSONPRETTY_FILE_WRITER_HPP
#define HEADER_PRIO_JSONPRETTY_FILE_WRITER_HPP

#include <functional>
#include <json/json.h>
#include <iosfwd>

#include "writer_impl.hpp"

namespace prio {

class JsonPrettyWriterImpl final : public WriterImpl
{
private:
  enum class Context { Mapping, Collection };

  std::ostream& m_out;
  int m_depth;
  std::vector<bool> m_write_seperator;
  std::vector<Context> m_context;

public:
  JsonPrettyWriterImpl(std::ostream& out);
  ~JsonPrettyWriterImpl() override;

  void begin_collection(const char* name) override;
  void end_collection() override;

  void begin_object(const char* type) override;
  void end_object() override;

  void begin_mapping(const char* name) override;
  void end_mapping() override;

  void write_bool(const char* name, bool) override;
  void write_int(const char* name, int) override;
  void write_float(const char* name, float) override;
  void write_string(const char* name, const std::string&) override;

  void write_bools(const char* name, std::vector<bool> const&) override;
  void write_ints(const char* name, std::vector<int> const&) override;
  void write_floats(const char* name, std::vector<float> const&) override;
  void write_strings(const char* name, std::vector<std::string> const&) override;

private:
  inline void write_indent();
  inline void write_separator();
  inline void write_quoted_string(const char* str);
  inline void write_quoted_string(const std::string& str);

private:
  JsonPrettyWriterImpl(const JsonPrettyWriterImpl&) = delete;
  JsonPrettyWriterImpl& operator=(const JsonPrettyWriterImpl&) = delete;
};

} // namespace prio

#endif

/* EOF */
