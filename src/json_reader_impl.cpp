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

#include "json_reader_impl.hpp"

#include <logmich/log.hpp>

#include "reader_collection.hpp"
#include "reader_impl.hpp"
#include "reader_mapping.hpp"
#include "reader_object.hpp"

namespace {

void syntax_error(Json::Value const& value, const char* message)
{
  log_error("syntax error: {}", message);
}

} // namespace

namespace prio {

JsonReaderDocumentImpl::JsonReaderDocumentImpl(Json::Value value) :
  m_value(std::move(value))
{
}

ReaderObject
JsonReaderDocumentImpl::get_root() const
{
  return ReaderObject(std::make_shared<JsonReaderObjectImpl>(m_value));
}

JsonReaderObjectImpl::JsonReaderObjectImpl(Json::Value const& json) :
  m_json(json)
{
  if (!m_json.isObject() || m_json.size() != 1)
  {
    syntax_error(json, "expected hash with one element");
  }
}

JsonReaderObjectImpl::~JsonReaderObjectImpl()
{
}

std::string
JsonReaderObjectImpl::get_name() const
{
  auto it = m_json.begin();
  return it.key().asString();
}

ReaderMapping
JsonReaderObjectImpl::get_mapping() const
{
  auto it = m_json.begin();
  return ReaderMapping(std::make_shared<JsonReaderMappingImpl>(*it));
}


JsonReaderCollectionImpl::JsonReaderCollectionImpl(Json::Value const& json) :
  m_json(json)
{
  if (!m_json.isArray())
  {
    syntax_error(json, "expected array");
  }
}

JsonReaderCollectionImpl::~JsonReaderCollectionImpl()
{
}

std::vector<ReaderObject>
JsonReaderCollectionImpl::get_objects() const
{
  std::vector<ReaderObject> result;
  for(Json::ArrayIndex i = 0; i < m_json.size(); ++i)
  {
    result.push_back(ReaderObject(std::make_shared<JsonReaderObjectImpl>(m_json[i])));
  }
  return result;
}


JsonReaderMappingImpl::JsonReaderMappingImpl(Json::Value const& json) :
  m_json(json)
{
}

JsonReaderMappingImpl::~JsonReaderMappingImpl()
{
}

std::vector<std::string>
JsonReaderMappingImpl::get_keys() const
{
  std::vector<std::string> result;
  for(auto it = m_json.begin(); it != m_json.end(); ++it)
  {
    result.push_back(it.key().asString());
  }
  return result;
}

bool
JsonReaderMappingImpl::read(const char* key, bool& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isBool())
  {
    value = element.asBool();
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read(const char* key, int& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isInt())
  {
    value = element.asInt();
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read(const char* key, float& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isDouble())
  {
    value = element.asFloat();
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read(const char* key, std::string& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isString())
  {
    value = element.asString();
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read(const char* key, std::vector<bool>& values) const
{
  const Json::Value& element = m_json[key];
  if (!element.isArray()) return false;

  values.resize(element.size());
  for(int i = 0; i < element.size(); ++i) {
    values[i] = element[i].asBool();
  }
  return true;
}

bool
JsonReaderMappingImpl::read(const char* key, std::vector<int>& values) const
{
  const Json::Value& element = m_json[key];
  if (!element.isArray()) return false;

  values.resize(element.size());
  for(int i = 0; i < element.size(); ++i) {
    values[i] = element[i].asInt();
  }
  return true;
}

bool
JsonReaderMappingImpl::read(const char* key, std::vector<float>& values) const
{
  const Json::Value& element = m_json[key];
  if (!element.isArray()) return false;

  values.resize(element.size());
  for(int i = 0; i < element.size(); ++i) {
    values[i] = element[i].asFloat();
  }
  return true;
}

bool
JsonReaderMappingImpl::read(const char* key, std::vector<std::string>& values) const
{
  const Json::Value& element = m_json[key];
  if (!element.isArray()) return false;

  values.resize(element.size());
  for(int i = 0; i < element.size(); ++i) {
    values[i] = element[i].asString();
  }
  return true;
}

bool
JsonReaderMappingImpl::read(const char* key, ReaderMapping& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isObject())
  {
    value = ReaderMapping(std::make_shared<JsonReaderMappingImpl>(element));
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read(const char* key, ReaderCollection& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isArray())
  {
    value = ReaderCollection(std::make_shared<JsonReaderCollectionImpl>(element));
    return true;
  }
  else
  {
    return false;
  }
}

bool
JsonReaderMappingImpl::read(const char* key, ReaderObject& value) const
{
  const Json::Value& element = m_json[key];
  if (element.isObject())
  {
    value = ReaderObject(std::make_shared<JsonReaderObjectImpl>(element));
    return true;
  }
  else
  {
    return false;
  }
}

} // namespace prio

/* EOF */
