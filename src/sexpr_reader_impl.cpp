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

#include "sexpr_reader_impl.hpp"

#include <sstream>

#include <logmich/log.hpp>
#include <sexp/util.hpp>
#include <sexp/io.hpp>

#include "reader_collection.hpp"
#include "reader_impl.hpp"
#include "reader_mapping.hpp"
#include "reader_object.hpp"

namespace prio {

SExprReaderObjectImpl::SExprReaderObjectImpl(sexp::Value const& sx) :
  m_sx(sx) // FIXME: all this copying is unnecessary
{
  // Expects data in the format:
  // (objectname
  //   (property1 45)
  //   (property2 45))
}

SExprReaderObjectImpl::~SExprReaderObjectImpl()
{
}

std::string
SExprReaderObjectImpl::get_name() const
{
  if (m_sx.as_array().size() < 1)
  {
    throw std::runtime_error("invalid syntax");
  }
  else
  {
    return m_sx.as_array()[0].as_string();
  }
}

ReaderMapping
SExprReaderObjectImpl::get_mapping() const
{
  return ReaderMapping(std::make_shared<SExprReaderMappingImpl>(m_sx));
}


SExprReaderCollectionImpl::SExprReaderCollectionImpl(sexp::Value const& sx) :
  m_sx(sx)
{
}

SExprReaderCollectionImpl::~SExprReaderCollectionImpl()
{
}

std::vector<ReaderObject>
SExprReaderCollectionImpl::get_objects() const
{
  std::vector<ReaderObject> lst;
  for (size_t i = 1; i < m_sx.as_array().size(); ++i) {
    lst.push_back(ReaderObject(std::make_shared<SExprReaderObjectImpl>(m_sx.as_array()[i])));
  }
  return lst;
}


SExprReaderMappingImpl::SExprReaderMappingImpl(sexp::Value const& sx) :
  m_sx(sx)
{
  if (!m_sx.is_array())
  {
    std::ostringstream msg;
    msg << "SExprReaderMapping s-expression must be a array: " << sx;
    throw std::runtime_error(msg.str());
  }

  // Expects data in this format:
  // (objectname
  //   (property1 45)
  //   (property2 45))
  // 'objectname' is ignored
}

SExprReaderMappingImpl::~SExprReaderMappingImpl()
{
}

std::vector<std::string>
SExprReaderMappingImpl::get_keys() const
{
  std::vector<std::string> lst;

  for(auto const& cur : m_sx.as_array())
  {
    // assert if (cur.is_cons())
    lst.push_back(cur.as_array()[0].as_string());
  }

  return lst;
}

bool
SExprReaderMappingImpl::read(const char* key, bool& value) const
{
  sexp::Value const* item = get_subsection_item(key);
  if (item && item->is_boolean())
  {
    value = item->as_bool();
    return true;
  }
  else if (item && item->is_integer()) // FIXME: remove this, be explicit
  {
    value = item->as_int();
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read(const char* key, int& value) const
{
  sexp::Value const* item = get_subsection_item(key);
  if (item && item->is_integer())
  {
    value = item->as_int();
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read(const char* key, float& value) const
{
  sexp::Value const* item = get_subsection_item(key);
  if (item)
  {
    if (item->is_real())
    {
      value = item->as_float();
      return true;
    }
    else if (item->is_integer())
    {
      value = static_cast<float>(item->as_int());
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read(const char* key, std::string& value) const
{
  sexp::Value const* sub = get_subsection(key);
  if (sub)
  {
    value = "";
    for (size_t i = 1; i < sub->as_array().size(); ++i)
    {
      auto const& item = sub->as_array()[i];

      if (item.is_string())
      {
        value += item.as_string();
      }
      else if (item.is_symbol())
      {
        value += item.as_string();
      }
    }
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read(const char* key, std::vector<bool>& values) const
{
#if 1
  sexp::Value const* item = get_subsection_items(key);
  if (!item || !item->is_array()) return false;

  values.resize(item->as_array().size() - 1);
  for (size_t i = 0; i < values.size(); ++i) {
    values[i] = item->as_array()[i + 1].as_bool();
  }
  return true;
#else
  sexp::Value const* item = get_subsection_items(key);
  if (!item || !item->is_cons()) return false;

  values.resize(sexp::list_length(*item));
  int i = 0;
  for(auto const& sx : sexp::ListAdapter(*item)) {
    values[i] = sx.as_bool();
    i += 1;
  }
  return true;
#endif
}

bool
SExprReaderMappingImpl::read(const char* key, std::vector<int>& values) const
{
#if 1
  sexp::Value const* item = get_subsection_items(key);
  if (!item || !item->is_array()) return false;

  values.resize(item->as_array().size() - 1);
  for (size_t i = 0; i < values.size(); ++i) {
    values[i] = item->as_array()[i + 1].as_int();
  }
  return true;
#else
  sexp::Value const* item = get_subsection_items(key);
  if (!item || !item->is_cons()) return false;

  values.resize(sexp::list_length(*item));
  int i = 0;
  for(auto const& sx : sexp::ListAdapter(*item)) {
    values[i] = sx.as_int();
    i += 1;
  }
  return true;
#endif
}

bool
SExprReaderMappingImpl::read(const char* key, std::vector<float>& values) const
{
#if 1
  sexp::Value const* item = get_subsection_items(key);
  if (!item || !item->is_array()) return false;

  values.resize(item->as_array().size() - 1);
  for (size_t i = 0; i < values.size(); ++i) {
    values[i] = item->as_array()[i + 1].as_float();
  }
  return true;
#else
  sexp::Value const* item = get_subsection_items(key);
  if (!item || !item->is_cons()) return false;

  values.resize(sexp::list_length(*item));
  int i = 0;
  for(auto const& sx : sexp::ListAdapter(*item)) {
    values[i] = sx.as_float();
    i += 1;
  }
  return true;
#endif
}

bool
SExprReaderMappingImpl::read(const char* key, std::vector<std::string>& values) const
{
#if 1
  sexp::Value const* item = get_subsection_items(key);
  if (!item || !item->is_array()) return false;

  values.resize(item->as_array().size() - 1);
  for (size_t i = 0; i < values.size(); ++i) {
    values[i] = item->as_array()[i + 1].as_string();
  }
  return true;
#else
  sexp::Value const* item = get_subsection_items(key);
  if (!item || !item->is_cons()) return false;

  values.resize(sexp::list_length(*item));
  int i = 0;
  for(auto const& sx : sexp::ListAdapter(*item)) {
    values[i] = sx.as_string();
    i += 1;
  }
  return true;
#endif
}

bool
SExprReaderMappingImpl::read(const char* key, ReaderObject& value) const
{
  sexp::Value const* cur = get_subsection_item(key);
  if (cur)
  {
    value = ReaderObject(std::make_shared<SExprReaderObjectImpl>(*cur));
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read(const char* key, ReaderCollection& value) const
{
  sexp::Value const* cur = get_subsection(key);
  if (cur)
  {
    value = ReaderCollection(std::make_shared<SExprReaderCollectionImpl>(*cur));
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read(const char* key, ReaderMapping& value) const
{
  sexp::Value const* cur = get_subsection(key);
  if (cur)
  {
    value = ReaderMapping(std::make_shared<SExprReaderMappingImpl>(*cur));
    return true;
  }
  else
  {
    return false;
  }
}

sexp::Value const*
SExprReaderMappingImpl::get_subsection_item(const char* key) const
{
  sexp::Value const* sub = get_subsection(key);
  if (sub && sub->as_array().size() >= 2)
  {
    if (sub->as_array().size() > 2)
    {
      log_error("invalid items in section: {}", key);
    }

    return &sub->as_array()[1];
  }
  else
  {
    return nullptr;
  }
}

sexp::Value const*
SExprReaderMappingImpl::get_subsection_items(const char* key) const
{
  sexp::Value const* sub = get_subsection(key);
  if (sub && sub->as_array().size() >= 1) {
    return sub;
  } else {
    return nullptr;
  }
}

sexp::Value const*
SExprReaderMappingImpl::get_subsection(const char* key) const
{
  sexp::Value const* result = nullptr;

  int count = 0;
  for (size_t i = 1; i < m_sx.as_array().size(); ++i)
  {
    if (m_sx.as_array()[i].as_array()[0].as_string() == key)
    {
      count += 1;
      result = &m_sx.as_array()[i];
    }
  }

  if (count > 1)
  {
    log_error("duplicate key value '{}'", key);
  }

  return result;
}

} // namespace prio

/* EOF */
