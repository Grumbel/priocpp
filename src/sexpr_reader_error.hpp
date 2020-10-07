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

#ifndef HEADER_PRIO_SEXPR_READER_ERROR_HPP
#define HEADER_PRIO_SEXPR_READER_ERROR_HPP

#include <sexp/value.hpp>

#include <sstream>
#include <stdexcept>
#include <sexp/io.hpp>

#include "reader_document.hpp"

namespace prio {

[[noreturn]]
inline void
raise_exception(SExprReaderDocumentImpl const& doc, sexp::Value const& sx, const char* msg)
{
  std::ostringstream oss;
  oss << (doc.get_filename() ? *doc.get_filename() : "<unknown>")
      << ":" << sx.get_line() << ": "
      << msg << " in expression:"
      << "\n    " << sx;
  throw std::runtime_error(oss.str());
}

inline void assert_is_boolean(SExprReaderDocumentImpl const& doc, sexp::Value const& sx)
{
  if (!sx.is_boolean())
  {
    raise_exception(doc, sx, "expected boolean");
  }
}

inline void assert_is_integer(SExprReaderDocumentImpl const& doc, sexp::Value const& sx)
{
  if (!sx.is_integer())
  {
    raise_exception(doc, sx, "expected integer");
  }
}

inline void assert_is_real(SExprReaderDocumentImpl const& doc, sexp::Value const& sx)
{
  if (!sx.is_real())
  {
    raise_exception(doc, sx, "expected real");
  }
}

inline void assert_is_symbol(SExprReaderDocumentImpl const& doc, sexp::Value const& sx)
{
  if (!sx.is_symbol())
  {
    raise_exception(doc, sx, "expected symbol");
  }
}

inline void assert_is_string(SExprReaderDocumentImpl const& doc, sexp::Value const& sx)
{
  if (!sx.is_string())
  {
    raise_exception(doc, sx, "expected string");
  }
}

inline void assert_is_array(SExprReaderDocumentImpl const& doc, sexp::Value const& sx)
{
  if (!sx.is_array())
  {
    raise_exception(doc, sx, "expected array");
  }
}

inline void assert_array_size_ge(SExprReaderDocumentImpl const& doc, sexp::Value const& sx, int size)
{
  assert_is_array(doc, sx);

  if (!(static_cast<int>(sx.as_array().size()) >= size))
  {
    std::ostringstream msg;
    msg << "array should contain " << size << " elements or more";
    raise_exception(doc, sx, msg.str().c_str());
  }
}

inline void assert_array_size_eq(SExprReaderDocumentImpl const& doc, sexp::Value const& sx, int size)
{
  assert_is_array(doc, sx);

  if (static_cast<int>(sx.as_array().size()) != size)
  {
    std::ostringstream msg;
    msg << "array must have " << size << " elements, but has " << sx.as_array().size();
    raise_exception(doc, sx, msg.str().c_str());
  }
}

} // namespace prio

#endif

/* EOF */
