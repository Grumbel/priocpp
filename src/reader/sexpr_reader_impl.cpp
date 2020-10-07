// Pingus - A free Lemmings clone
// Copyright (C) 2005 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "reader/sexpr_reader_impl.hpp"

#include <sstream>

#include <logmich/log.hpp>
#include <sexp/util.hpp>
#include <sexp/io.hpp>

#include "reader/reader.hpp"
#include "reader/reader_impl.hpp"

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
  if (sexp::list_length(m_sx) < 1)
  {
    throw std::runtime_error("invalid syntax");
  }
  else
  {
    return m_sx.get_car().as_string();
  }
}

ReaderMapping
SExprReaderObjectImpl::get_mapping() const
{
  return ReaderMapping(std::make_shared<SExprReaderMappingImpl>(m_sx.get_cdr()));
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
  for(auto const& cur : sexp::ListAdapter(m_sx))
  {
    lst.push_back(ReaderObject(std::make_shared<SExprReaderObjectImpl>(cur)));
  }
  return lst;
}


SExprReaderMappingImpl::SExprReaderMappingImpl(sexp::Value const& sx) :
  m_sx(sx)
{
  if (!sexp::is_list(m_sx))
  {
    std::ostringstream msg;
    msg << "SExprReaderMapping s-expression must be a list: " << sx;
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

  for(auto const& cur : sexp::ListAdapter(m_sx))
  {
    // assert if (cur.is_cons())
    lst.push_back(cur.get_car().as_string());
  }

  return lst;
}

bool
SExprReaderMappingImpl::read_int(const char* key, int& value) const
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
SExprReaderMappingImpl::read_float(const char* key, float& value) const
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
SExprReaderMappingImpl::read_bool(const char* key, bool& value) const
{
  sexp::Value const* item = get_subsection_item(key);
  if (item && item->is_boolean())
  {
    value = item->as_bool();
    return true;
  }
  else if (item && item->is_integer())
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
SExprReaderMappingImpl::read_string(const char* key, std::string& value) const
{
  sexp::Value const* sub = get_subsection(key);
  if (sub)
  {
    value = "";
    for(auto const& item : sexp::ListAdapter(*sub))
    {
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
SExprReaderMappingImpl::read_object(const char* key, ReaderObject& value) const
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
SExprReaderMappingImpl::read_collection(const char* key, ReaderCollection& value) const
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
SExprReaderMappingImpl::read_mapping(const char* key, ReaderMapping& value) const
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
  if (sub && sexp::list_length(*sub) >= 1)
  {
    if (sexp::list_length(*sub) > 1)
    {
      log_error("invalid items in section: {}", key);
    }

    return &sub->get_car();
  }
  else
  {
    return nullptr;
  }
}

sexp::Value const*
SExprReaderMappingImpl::get_subsection(const char* key) const
{
  sexp::Value const* result = nullptr;

  int count = 0;
  for(auto const& cur : sexp::ListAdapter(m_sx))
  {
    if (cur.get_car().as_string() == key)
    {
      count += 1;
      result = &cur.get_cdr();
    }
  }

  if (count > 1)
  {
    log_error("duplicate key value '{}'", key);
  }

  return result;
}

/* EOF */
