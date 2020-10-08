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

#include "writer.hpp"

#include "sexpr_writer_impl.hpp"
#include "json_writer_impl.hpp"
#include "jsonpretty_writer_impl.hpp"

namespace prio {

Writer
Writer::fastjson(std::ostream& out)
{
  return Writer(std::make_unique<JsonWriterImpl>(out));
}

Writer
Writer::json(std::ostream& out)
{
  return Writer(std::make_unique<JsonPrettyWriterImpl>(out));
}

Writer
Writer::sexpr(std::ostream& out)
{
  return Writer(std::make_unique<SExprWriterImpl>(out));
}

Writer::Writer(std::ostream& out) :
  m_impl(std::make_unique<SExprWriterImpl>(out))
{
}

Writer::Writer(std::unique_ptr<WriterImpl> impl) :
  m_impl(std::move(impl))
{
}

Writer::~Writer()
{
}

void
Writer::begin_collection(const char* name)
{
  m_impl->begin_collection(name);
}

void
Writer::end_collection()
{
  m_impl->end_collection();
}

void
Writer::begin_object(const char* type)
{
  m_impl->begin_object(type);
}

void
Writer::end_object()
{
  m_impl->end_object();
}

void
Writer::begin_mapping(const char* name)
{
  m_impl->begin_mapping(name);
}

void
Writer::end_mapping()
{
  m_impl->end_mapping();
}

void
Writer::write(const char* name, bool value)
{
  m_impl->write(name, value);
}

void
Writer::write(const char* name, int value)
{
  m_impl->write(name, value);
}

void
Writer::write(const char* name, float value)
{
  m_impl->write(name, value);
}

void
Writer::write(const char* name, const std::string& value)
{
  m_impl->write(name, value);
}

void
Writer::write(const char* name, std::vector<bool> const& values)
{
  m_impl->write(name, values);
}

void
Writer::write(const char* name, std::vector<int> const& values)
{
  m_impl->write(name, values);
}

void
Writer::write(const char* name, std::vector<float> const& values)
{
  m_impl->write(name, values);
}

void
Writer::write(const char* name, std::vector<std::string> const& values)
{
  m_impl->write(name, values);
}

} // namespace prio

/* EOF */
