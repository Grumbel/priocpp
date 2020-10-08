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

#include "jsonpretty_writer_impl.hpp"

#include <sstream>
#include <ostream>
#include <assert.h>

namespace prio {

JsonPrettyWriterImpl::JsonPrettyWriterImpl(std::ostream& out) :
  m_out(out),
  m_depth(0),
  m_write_seperator( { false } ),
  m_context( { Context::Collection } )
{
}

JsonPrettyWriterImpl::~JsonPrettyWriterImpl()
{
}

void
JsonPrettyWriterImpl::begin_collection(const char* name)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(name);
  m_out << ": [";

  m_context.push_back(Context::Collection);
  m_write_seperator.push_back(false);
  m_depth += 1;
}

void
JsonPrettyWriterImpl::end_collection()
{
  assert(m_context.back() == Context::Collection);

  m_write_seperator.back() = false;
  m_depth -= 1;

  write_indent();
  m_out << "]";

  m_context.pop_back();
  m_write_seperator.pop_back();

  write_separator();
}

void
JsonPrettyWriterImpl::begin_object(const char* type)
{
  assert(m_context.back() == Context::Collection);

  if (m_depth != 0)
  {
    write_indent();
  }

  m_out << "{";
  m_depth += 1;
  write_indent();
  write_quoted_string(type);
  m_out << ": {";

  m_context.push_back(Context::Mapping);
  m_write_seperator.push_back(false);
  m_depth += 1;
}

void
JsonPrettyWriterImpl::end_object()
{
  m_write_seperator.back() = false;
  m_depth -= 1;

  write_indent();
  m_out << "}";
  m_depth -= 1;
  write_indent();
  m_out << "}";

  if (m_depth == 0)
  {
    m_out << "\n";
  }

  m_context.pop_back();
  m_write_seperator.pop_back();

  write_separator();
}

void
JsonPrettyWriterImpl::begin_mapping(const char* name)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(name);
  m_out << ": {";

  m_context.push_back(Context::Mapping);
  m_write_seperator.push_back(false);
  m_depth += 1;
}

void
JsonPrettyWriterImpl::end_mapping()
{
  assert(m_context.back() == Context::Mapping);

  m_write_seperator.back() = false;
  m_depth -= 1;

  write_indent();
  m_out << "}";

  m_context.pop_back();
  m_write_seperator.pop_back();

  write_separator();
}

void
JsonPrettyWriterImpl::write(const char* name, bool value)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(name);
  m_out << ": " << (value ? "true" : "false");
  write_separator();
}

void
JsonPrettyWriterImpl::write(const char* name, int value)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(name);
  m_out << ": " << value;
  write_separator();
}

void
JsonPrettyWriterImpl::write(const char* name, float value)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(name);
  m_out << ": " << value;
  write_separator();
}

void
JsonPrettyWriterImpl::write(const char* name, char const* text)
{
  write(name, std::string_view(text));
}

void
JsonPrettyWriterImpl::write(const char* name, std::string_view value)
{
  assert(m_context.back() == Context::Mapping);

  write_indent();
  write_quoted_string(name);
  m_out << ": ";
  write_quoted_string(value);
  write_separator();
}

void
JsonPrettyWriterImpl::write(const char* name, std::vector<bool> const& values)
{
  write_indent();
  write_quoted_string(name);
  m_out << ": [";
  size_t index = 0;
  for(auto const& value : values) {
    m_out << (value ? "true" : "false");
    if (index != values.size() - 1) {
      m_out << ", ";
    }
    index += 1;
  }
  m_out << "]";
  write_separator();
}

void
JsonPrettyWriterImpl::write(const char* name, std::vector<int> const& values)
{
  write_indent();
  write_quoted_string(name);
  m_out << ": [";
  for(auto const& value : values) {
    m_out << value;
    if (&value != &values.back()) {
      m_out << ", ";
    }
  }
  m_out << "]";
  write_separator();
}

void
JsonPrettyWriterImpl::write(const char* name, std::vector<float> const& values)
{
  write_indent();
  write_quoted_string(name);
  m_out << ": [";
  for(auto const& value : values) {
    m_out << value;
    if (&value != &values.back()) {
      m_out << ", ";
    }
  }
  m_out << "]";
  write_separator();
}

void
JsonPrettyWriterImpl::write(const char* name, std::vector<std::string> const& values)
{
  write_indent();
  write_quoted_string(name);
  m_out << ": [";
  for(auto const& value : values) {
    write_quoted_string(value);
    if (&value != &values.back()) {
      m_out << ", ";
    }
  }
  m_out << "]";
  write_separator();
}

void
JsonPrettyWriterImpl::write_indent()
{
  if (m_write_seperator.back())
  {
    m_out.write(",\n", 2);
    m_write_seperator.back() = false;
  }
  else
  {
    m_out.write("\n", 1);
  }

  for(int i = 0; i < m_depth; ++i)
  {
    m_out.write("  ", 2);
  }
}

void
JsonPrettyWriterImpl::write_separator()
{
  m_write_seperator.back() = true;
}

void
JsonPrettyWriterImpl::write_quoted_string(const char* text)
{
  write_quoted_string(std::string_view(text));
}

void
JsonPrettyWriterImpl::write_quoted_string(std::string_view text)
{
  // FIXME: obviously evil
  m_out << '"' << text << '"';
}

} // namespace prio

/* EOF */
