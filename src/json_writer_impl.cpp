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

#include "json_writer_impl.hpp"

#include <assert.h>

#include <memory>
#include <ostream>
#include <sstream>

namespace prio {

JsonWriterImpl::JsonWriterImpl(std::ostream& out) :
  m_out(out),
  m_root(Json::objectValue),
  m_stack()
{
  // the root element is always an anonymous mapping
  m_stack.push_back(m_root);
}

JsonWriterImpl::~JsonWriterImpl()
{
  assert(m_stack.size() == 1);
}

void
JsonWriterImpl::begin_collection(const char* name)
{
  assert(!m_stack.empty());
  assert(m_stack.back().get().type() == Json::objectValue);

  m_stack.push_back((m_stack.back().get()[name] = Json::Value(Json::arrayValue)));
}

void
JsonWriterImpl::end_collection()
{
  assert(!m_stack.empty());
  m_stack.pop_back();
}

void
JsonWriterImpl::begin_object(const char* type)
{
  // FIXME: hack
  begin_mapping(type);
}

void
JsonWriterImpl::end_object()
{
  // FIXME: hack
  end_mapping();
}

void
JsonWriterImpl::begin_mapping(const char* name)
{
  assert(!m_stack.empty());

  if (m_stack.back().get().type() == Json::objectValue)
  {
    m_stack.push_back((m_stack.back().get()[name] = Json::Value(Json::objectValue)));
  }
  else if (m_stack.back().get().type() == Json::arrayValue)
  {
    m_stack.back().get().append(Json::Value(Json::objectValue));
    m_stack.push_back(m_stack.back().get()[m_stack.back().get().size()-1][name] = Json::Value(Json::objectValue));
  }
  else
  {
    assert(false && "never reached");
  }
}

void
JsonWriterImpl::end_mapping()
{
  assert(!m_stack.empty());
  m_stack.pop_back();

  if (m_stack.size() == 1)
  {
    flush();
  }
}

void
JsonWriterImpl::write(const char* name, bool value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().get().type() == Json::objectValue);

  m_stack.back().get()[name] = Json::Value(value);
}

void
JsonWriterImpl::write(const char* name, int value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().get().type() == Json::objectValue);

  m_stack.back().get()[name] = Json::Value(value);
}

void
JsonWriterImpl::write(const char* name, float value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().get().type() == Json::objectValue);

  m_stack.back().get()[name] = Json::Value(value);
}

void
JsonWriterImpl::write(const char* name, const std::string& value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().get().type() == Json::objectValue);

  m_stack.back().get()[name] = Json::Value(value);
}

void
JsonWriterImpl::write(const char* name, std::vector<bool> const& values)
{
  assert(!m_stack.empty());
  assert(m_stack.back().get().type() == Json::objectValue);

  auto& arr = m_stack.back().get()[name] = Json::Value(Json::arrayValue);
  for(int i = 0; i < static_cast<int>(values.size()); ++i) {
    arr[i] = Json::Value(values[i]);
  }
}

void
JsonWriterImpl::write(const char* name, std::vector<int> const& values)
{
  assert(!m_stack.empty());
  assert(m_stack.back().get().type() == Json::objectValue);

  auto& arr = m_stack.back().get()[name] = Json::Value(Json::arrayValue);
  for(int i = 0; i < static_cast<int>(values.size()); ++i) {
    arr[i] = Json::Value(values[i]);
  }
}

void
JsonWriterImpl::write(const char* name, std::vector<float> const& values)
{
  assert(!m_stack.empty());
  assert(m_stack.back().get().type() == Json::objectValue);

  auto& arr = m_stack.back().get()[name] = Json::Value(Json::arrayValue);
  for(int i = 0; i < static_cast<int>(values.size()); ++i) {
    arr[i] = Json::Value(values[i]);
  }
}

void
JsonWriterImpl::write(const char* name, std::vector<std::string> const& values)
{
  assert(!m_stack.empty());
  assert(m_stack.back().get().type() == Json::objectValue);

  auto& arr = m_stack.back().get()[name] = Json::Value(Json::arrayValue);
  for(int i = 0; i < static_cast<int>(values.size()); ++i) {
    arr[i] = Json::Value(values[i]);
  }
}

namespace {

void strip_trailing_whitespace(std::ostream& out, std::istream& in)
{
  std::string line;
  while(std::getline(in, line))
  {
    const auto p = line.find_last_not_of(' ');
    if (p == std::string::npos)
    {
      if (line.empty())
      {
        out << line << '\n';
      }
    }
    else
    {
      out.write(line.data(), static_cast<std::streamsize>(p + 1));
      out << '\n';
    }
  }
}

} // namespace

void
JsonWriterImpl::flush()
{
  assert(m_stack.size() == 1);
  std::stringstream out;
  if ((false))
  {
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "   ";
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(m_stack.back(), &out);
    strip_trailing_whitespace(m_out, out);
  }
  else
  {
    Json::StreamWriterBuilder builder;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(m_stack.back(), &m_out);
  }
}

} // namespace prio

/* EOF */
