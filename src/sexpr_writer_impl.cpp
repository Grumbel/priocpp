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

#include "sexpr_writer_impl.hpp"

#include <map>

namespace prio {

namespace {

void write_escaped(std::ostream& os, std::string_view text)
{
  os << '"';
  for(char const c : text) {
    switch(c) {
      case '"':
        os << "\\\"";
        break;

      case '\\':
        os << "\\\\";
        break;

      default:
        os << c;
        break;
    }
  }
  os << '"';
}

} // namespace

SExprWriterImpl::SExprWriterImpl(std::ostream& out_) :
  out(&out_),
  level(0)
{
}

SExprWriterImpl::~SExprWriterImpl()
{
}

std::string
SExprWriterImpl::indent() const
{
  return std::string(level*2, ' ');
}

void
SExprWriterImpl::begin_collection(const char* name)
{
  begin_mapping(name);
}

void
SExprWriterImpl::end_collection()
{
  end_mapping();
}

void
SExprWriterImpl::begin_object(const char* type)
{
  begin_mapping(type);
}

void
SExprWriterImpl::end_object()
{
  end_mapping();
}

void
SExprWriterImpl::begin_mapping(const char* name)
{
  if (level != 0)
    (*out) << std::endl;

  (*out) << indent() << "(" << name;
  ++level;
}

void
SExprWriterImpl::end_mapping()
{
  --level;
  (*out) << ")";

  // insert trailing newline and EOF marker at end of file
  if (level == 0)
  {
    (*out) << "\n\n;; EOF ;;\n";
  }
}

void
SExprWriterImpl::write(const char* name, bool value)
{
  (*out) << "\n" << indent() << "(" << name << " " << (value ? "#t" : "#f") << ")";
}

void
SExprWriterImpl::write(const char* name, int value)
{
  (*out) << "\n" << indent() << "(" << name << " " << value << ")";
}

void
SExprWriterImpl::write(const char* name, float value)
{
  (*out) << "\n" << indent() << "(" << name << " " << value << ")";
}

void
SExprWriterImpl::write(const char* name, char const* text)
{
  write(name, std::string_view(text));
}

void
SExprWriterImpl::write(const char* name, std::string_view value)
{
  (*out) << "\n" << indent() << "(" << name << " ";
  write_escaped(*out, value);
  (*out) << ")";
}

void
SExprWriterImpl::write(const char* name, std::vector<bool> const& values)
{
  (*out) << "\n" << indent() << "(" << name;
  for (bool const value : values) {
    (*out) << ' ' << (value ? "#t" : "#f");
  }
  (*out) << ")";
}

void
SExprWriterImpl::write(const char* name, std::vector<int> const& values)
{
  (*out) << "\n" << indent() << "(" << name;
  for (int const value : values) {
    (*out) << ' ' << value;
  }
  (*out) << ")";
}

void
SExprWriterImpl::write(const char* name, std::vector<float> const& values)
{
  (*out) << "\n" << indent() << "(" << name;
  for (float const value : values) {
    (*out) << ' ' << value;
  }
  (*out) << ")";
}

void
SExprWriterImpl::write(const char* name, std::vector<std::string> const& values)
{
  (*out) << "\n" << indent() << "(" << name;
  for (std::string const& value : values) {
    (*out) << ' ';
    write_escaped(*out, value);
  }
  (*out) << ")";
}

} // namespace prio

/* EOF */
