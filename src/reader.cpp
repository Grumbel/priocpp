// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#include "reader.hpp"

#include <sstream>
#include <fstream>
#include <json/reader.h>
#include <utility>

#include <logmich/log.hpp>
#include <sexp/parser.hpp>

#include "json_reader_impl.hpp"
#include "reader_impl.hpp"
#include "sexpr_reader_impl.hpp"
#include "reader_collection.hpp"
#include "reader_mapping.hpp"

namespace prio {

ReaderMapping::ReaderMapping(std::shared_ptr<ReaderMappingImpl> impl_) :
  m_impl(std::move(impl_))
{
}

ReaderMapping::ReaderMapping() :
  m_impl()
{
}

ReaderCollection::ReaderCollection(std::shared_ptr<ReaderCollectionImpl> impl) :
  m_impl(std::move(impl))
{
}

ReaderCollection::ReaderCollection() :
  m_impl()
{
}

std::vector<ReaderObject>
ReaderCollection::get_objects() const
{
  if (m_impl)
    return m_impl->get_objects();
  else
    return {};
}

ReaderObject::ReaderObject(std::shared_ptr<ReaderObjectImpl> impl) :
  m_impl(std::move(impl))
{
}

ReaderObject::ReaderObject() :
  m_impl()
{
}

std::string
ReaderObject::get_name() const
{
  if (m_impl)
    return m_impl->get_name();
  else
    return {};
}

ReaderMapping
ReaderObject::get_mapping() const
{
  if (m_impl)
    return m_impl->get_mapping();
  else
    return {};
}

bool
ReaderMapping::read_int(const char* key, int& value) const
{
  if (m_impl)
    return m_impl->read_int(key, value);
  else
    return false;
}

bool
ReaderMapping::read_float (const char* key, float& value) const
{
  if (m_impl)
    return m_impl->read_float(key, value);
  else
    return false;
}

bool
ReaderMapping::read_bool  (const char* key, bool& value) const
{
  if (m_impl)
    return m_impl->read_bool(key, value);
  else
    return false;
}

bool
ReaderMapping::read_string(const char* key, std::string& value) const
{
  if (m_impl)
    return m_impl->read_string(key, value);
  else
    return false;
}

bool
ReaderMapping::read_object(const char* key, ReaderObject& object) const
{
  if (m_impl)
    return m_impl->read_object(key, object);
  else
    return false;
}

bool
ReaderMapping::read_mapping(const char* key, ReaderMapping& mapping) const
{
  if (m_impl)
    return m_impl->read_mapping(key, mapping);
  else
    return false;
}

bool
ReaderMapping::read_collection(const char* key, ReaderCollection& collection) const
{
  if (m_impl)
    return m_impl->read_collection(key, collection);
  else
    return false;
}


ReaderMapping
ReaderMapping::read_mapping(const char* key) const
{
  if (!m_impl)
  {
    return {};
  }
  {
    ReaderMapping result;
    read_mapping(key, result);
    return result;
  }
}

ReaderCollection
ReaderMapping::read_collection(const char* key) const
{
  if (!m_impl)
  {
    return {};
  }
  {
    ReaderCollection result;
    read_collection(key, result);
    return result;
  }
}

ReaderObject
ReaderMapping::read_object(const char* key) const
{
  if (!m_impl)
  {
    return {};
  }
  {
    ReaderObject result;
    read_object(key, result);
    return result;
  }
}

std::vector<std::string>
ReaderMapping::get_keys() const
{
  if (m_impl)
    return m_impl->get_keys();
  else
    return {};
}

ReaderObject
Reader::parse_string(std::string const& text)
{
  std::istringstream in(text);
  return Reader::parse(in);
}

ReaderObject
Reader::parse(std::istream& stream)
{
  int c = stream.get();
  stream.unget();
  if (c == '{')
  {
    Json::CharReaderBuilder builder;
    std::string errs;
    Json::Value root;
    if (Json::parseFromStream(builder, stream, &root, &errs))
    {
      return ReaderObject(std::make_shared<JsonReaderObjectImpl>(root));
    }
    else
    {
      log_error("json parse error: {}", errs);
      return ReaderObject();
    }
  }
  else
  {
    try
    {
      auto sx = sexp::Parser::from_stream(stream);
      return ReaderObject(std::make_shared<SExprReaderObjectImpl>(std::move(sx)));
    }
    catch(std::exception const& err)
    {
      log_error("sexp parse error: {}", err.what());
      return ReaderObject();
    }
  }
}

ReaderObject
Reader::parse(const std::string& filename)
{
  std::ifstream fin(filename);
  if (!fin)
  {
    return ReaderObject();
  }
  else
  {
    return parse(fin);
  }
}

std::vector<ReaderObject>
Reader::parse_many(const std::string& pathname)
{
  return {};
#if 0
  std::shared_ptr<lisp::Lisp> sexpr = lisp::Parser::parse(pathname.get_sys_path());
  if (sexpr)
  {
    std::vector<Reader> sections;
    for(size_t i = 0; i < sexpr->get_list_size(); ++i)
    {
      sections.push_back(Reader(std::make_shared<SExprReaderImpl>(sexpr->get_list_elem(i))));
    }

    return sections;
  }
  else
  {
    return std::vector<Reader>();
  }
#endif
}

} // namespace prio

/* EOF */
