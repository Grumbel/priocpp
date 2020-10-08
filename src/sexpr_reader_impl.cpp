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
#include "reader_error.hpp"
#include "reader_impl.hpp"
#include "reader_mapping.hpp"
#include "reader_object.hpp"

namespace prio {

SExprReaderDocumentImpl::SExprReaderDocumentImpl(sexp::Value sx, bool pedantic, std::optional<std::string> filename) :
  m_sx(std::move(sx)),
  m_pedantic(pedantic),
  m_filename(std::move(filename))
{
}

ReaderObject
SExprReaderDocumentImpl::get_root() const
{
  return ReaderObject(std::make_unique<SExprReaderObjectImpl>(*this, m_sx));
}

void
SExprReaderDocumentImpl::error(sexp::Value const& sx, char const* message) const
{
  if (m_pedantic) {
    throw ReaderError(fmt::format("{}:{}: {}: {}", m_filename ? *m_filename : "<unknown>", sx.get_line(), sx, message));
  } else {
    log_error("{}:{}: {}: {}", m_filename ? *m_filename : "<unknown>", sx.get_line(), sx, message);
  }
}

SExprReaderObjectImpl::SExprReaderObjectImpl(SExprReaderDocumentImpl const& doc, sexp::Value const& sx) :
  m_doc(doc),
  m_sx(sx)
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
    throw ReaderError("invalid syntax");
  }
  else
  {
    return m_sx.as_array()[0].as_string();
  }
}

ReaderMapping
SExprReaderObjectImpl::get_mapping() const
{
  return ReaderMapping(std::make_unique<SExprReaderMappingImpl>(m_doc, m_sx));
}


SExprReaderCollectionImpl::SExprReaderCollectionImpl(SExprReaderDocumentImpl const& doc, sexp::Value const& sx) :
  m_doc(doc),
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
    lst.push_back(ReaderObject(std::make_unique<SExprReaderObjectImpl>(m_doc, m_sx.as_array()[i])));
  }
  return lst;
}


SExprReaderMappingImpl::SExprReaderMappingImpl(SExprReaderDocumentImpl const& doc, sexp::Value const& sx) :
  m_doc(doc),
  m_sx(sx)
{
  if (!m_sx.is_array())
  {
    std::ostringstream msg;
    msg << "SExprReaderMapping s-expression must be a array: " << sx;
    throw ReaderError(msg.str());
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
  std::vector<std::string> result;

  assert(m_sx.is_array());
  for (size_t i = 1; i < m_sx.as_array().size(); ++i) {
    sexp::Value const& keyvalue = m_sx.as_array()[i];
    if (!keyvalue.is_array() || keyvalue.as_array().size() < 1) {
      std::ostringstream oss;
      oss << keyvalue << ": malformed mapping, expected array";
      throw ReaderError(oss.str());
    }

    if (!keyvalue.as_array()[0].is_symbol()) {
      std::ostringstream oss;
      oss << keyvalue << ": malformed mapping, expected symbol";
      throw ReaderError(oss.str());
    }

    result.emplace_back(keyvalue.as_array()[0].as_string());
  }

  return result;
}

#define GET_VALUE_MACRO(type, checker, getter)         \
  sexp::Value const* item = get_subsection_item(key);  \
  if (!item) { return false; }                         \
  if (!item->checker()) {                              \
    m_doc.error(*item, "expected " type);              \
    return false;                                      \
  }                                                    \
  value = item->getter();                              \
  return true

bool
SExprReaderMappingImpl::read(const char* key, bool& value) const
{
  GET_VALUE_MACRO("bool", is_boolean, as_bool);
}

bool
SExprReaderMappingImpl::read(const char* key, int& value) const
{
  GET_VALUE_MACRO("int", is_integer, as_int);
}

bool
SExprReaderMappingImpl::read(const char* key, float& value) const
{
  GET_VALUE_MACRO("float", is_real, as_float);
}

bool
SExprReaderMappingImpl::read(const char* key, std::string& value) const
{
  GET_VALUE_MACRO("string", is_string, as_string);
}

#undef GET_VALUE_MACRO

#define GET_VALUES_MACRO(type, checker, getter)                 \
  sexp::Value const* item = get_subsection_items(key);          \
  if (!item) { return false; }                                  \
  if (!item->is_array()) {                                      \
    m_doc.error(*item, "expected array");                       \
    return false;                                               \
  }                                                             \
                                                                \
  for (size_t i = 1; i < item->as_array().size(); ++i) {        \
    if (!item->as_array()[i].checker()) {                       \
      m_doc.error(item->as_array()[i], "expected " type);       \
      return false;                                             \
    }                                                           \
  }                                                             \
                                                                \
  values.resize(item->as_array().size() - 1);                   \
  for (size_t i = 0; i < values.size(); ++i) {                  \
    values[i] = item->as_array()[i + 1].getter();               \
  }                                                             \
  return true

bool
SExprReaderMappingImpl::read(const char* key, std::vector<bool>& values) const
{
  GET_VALUES_MACRO("bool", is_boolean, as_bool);
}

bool
SExprReaderMappingImpl::read(const char* key, std::vector<int>& values) const
{
  GET_VALUES_MACRO("int", is_integer, as_int);
}

bool
SExprReaderMappingImpl::read(const char* key, std::vector<float>& values) const
{
  GET_VALUES_MACRO("float", is_real, as_float);
}

bool
SExprReaderMappingImpl::read(const char* key, std::vector<std::string>& values) const
{
  GET_VALUES_MACRO("string", is_string, as_string);
}

#undef GET_VALUES_MACRO

bool
SExprReaderMappingImpl::read(const char* key, ReaderObject& value) const
{
  sexp::Value const* cur = get_subsection_item(key);
  if (cur)
  {
    value = ReaderObject(std::make_unique<SExprReaderObjectImpl>(m_doc, *cur));
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
    value = ReaderCollection(std::make_unique<SExprReaderCollectionImpl>(m_doc, *cur));
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
    value = ReaderMapping(std::make_unique<SExprReaderMappingImpl>(m_doc, *cur));
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
  if (!sub) {
    return nullptr;
  }

  if (sub->as_array().size() > 2) {
    m_doc.error(*sub, "invalid items in section");
    return nullptr;
  }

  return &sub->as_array()[1];
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
