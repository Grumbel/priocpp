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
  m_stack()
{
}

JsonWriterImpl::~JsonWriterImpl()
{
  assert(m_stack.size() == 1);
}

void
JsonWriterImpl::begin_collection(std::string_view key)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  m_stack.emplace_back(std::string(key));
  m_stack.emplace_back(Json::arrayValue);
}

void
JsonWriterImpl::end_collection()
{
  assert(!m_stack.empty());

  Json::Value value = std::move(m_stack.back());
  m_stack.pop_back();

  Json::Value key = std::move(m_stack.back());
  m_stack.pop_back();

  m_stack.back()[key.asString()] = std::move(value);
}

void
JsonWriterImpl::begin_object(std::string_view type)
{
  assert(m_stack.empty() || // root
         m_stack.back().type() == Json::stringValue || // keyvalue
         m_stack.back().type() == Json::arrayValue); // collection

  m_stack.emplace_back(std::string(type));
  m_stack.emplace_back(Json::objectValue);
}

void
JsonWriterImpl::end_object()
{
  assert(m_stack.size() >= 2);

  Json::Value mapping = std::move(m_stack.back());
  m_stack.pop_back();

  Json::Value type = std::move(m_stack.back());
  m_stack.pop_back();

  Json::Value object(Json::objectValue);
  object[type.asString()] = std::move(mapping);

  if (m_stack.empty()) {
    m_stack.emplace_back(std::move(object));
    flush();
  } else if (m_stack.back().type() == Json::stringValue) {
    m_stack.emplace_back(std::move(object));
  } else if (m_stack.back().type() == Json::arrayValue) {
    m_stack.back().append(std::move(object));
  }
}

void
JsonWriterImpl::begin_mapping(std::string_view key)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  m_stack.emplace_back(std::string(key));
  m_stack.emplace_back(Json::objectValue);
}

void
JsonWriterImpl::end_mapping()
{
  assert(m_stack.size() >= 2);

  Json::Value value = std::move(m_stack.back());
  m_stack.pop_back();

  Json::Value key = std::move(m_stack.back());
  m_stack.pop_back();

  m_stack.back()[key.asString()] = std::move(value);
}

void
JsonWriterImpl::begin_keyvalue(std::string_view key)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  m_stack.emplace_back(std::string(key));
}

void
JsonWriterImpl::end_keyvalue()
{
  assert(m_stack.size() >= 2);
  assert(m_stack.back().type() == Json::stringValue);

  Json::Value value = m_stack.back();
  m_stack.pop_back();

  Json::Value key = m_stack.back();
  m_stack.pop_back();

  m_stack.back()[key.asString()] = value;
}

void
JsonWriterImpl::write(std::string_view key, bool value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  m_stack.back()[std::string(key)] = Json::Value(value);
}

void
JsonWriterImpl::write(std::string_view key, int value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  m_stack.back()[std::string(key)] = Json::Value(value);
}

void
JsonWriterImpl::write(std::string_view key, float value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  m_stack.back()[std::string(key)] = Json::Value(value);
}

void
JsonWriterImpl::write(std::string_view key, char const* text)
{
  write(key, std::string_view(text));
}

void
JsonWriterImpl::write(std::string_view key, std::string_view value)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  m_stack.back()[std::string(key)] = Json::Value(value.begin(), value.end());
}

void
JsonWriterImpl::write(std::string_view key, std::vector<bool> const& values)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  auto& arr = m_stack.back()[std::string(key)] = Json::Value(Json::arrayValue);
  for(int i = 0; i < static_cast<int>(values.size()); ++i) {
    arr[i] = Json::Value(values[i]);
  }
}

void
JsonWriterImpl::write(std::string_view key, std::vector<int> const& values)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  auto& arr = m_stack.back()[std::string(key)] = Json::Value(Json::arrayValue);
  for(int i = 0; i < static_cast<int>(values.size()); ++i) {
    arr[i] = Json::Value(values[i]);
  }
}

void
JsonWriterImpl::write(std::string_view key, std::vector<float> const& values)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  auto& arr = m_stack.back()[std::string(key)] = Json::Value(Json::arrayValue);
  for(int i = 0; i < static_cast<int>(values.size()); ++i) {
    arr[i] = Json::Value(values[i]);
  }
}

void
JsonWriterImpl::write(std::string_view key, std::vector<std::string> const& values)
{
  assert(!m_stack.empty());
  assert(m_stack.back().type() == Json::objectValue);

  auto& arr = m_stack.back()[std::string(key)] = Json::Value(Json::arrayValue);
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
