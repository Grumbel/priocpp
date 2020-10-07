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

#ifndef HEADER_PRIO_SEXPR_READER_HPP
#define HEADER_PRIO_SEXPR_READER_HPP

#include <sexp/value.hpp>

#include "reader_impl.hpp"

namespace prio {

class SExprReaderDocumentImpl final : public ReaderDocumentImpl
{
public:
  SExprReaderDocumentImpl(sexp::Value sx);

  ReaderObject get_root() const;

private:
  sexp::Value m_sx;
};

class SExprReaderObjectImpl final : public ReaderObjectImpl
{
public:
  SExprReaderObjectImpl(sexp::Value const& sx);
  ~SExprReaderObjectImpl() override;

  std::string get_name() const override;
  ReaderMapping get_mapping() const override;

private:
  sexp::Value const& m_sx;
};

class SExprReaderCollectionImpl final : public ReaderCollectionImpl
{
public:
  SExprReaderCollectionImpl(sexp::Value const& m_sx);
  ~SExprReaderCollectionImpl() override;

  std::vector<ReaderObject> get_objects() const override;

private:
  sexp::Value const& m_sx;
};

class SExprReaderMappingImpl final : public ReaderMappingImpl
{
public:
  SExprReaderMappingImpl(sexp::Value const& m_sx);
  ~SExprReaderMappingImpl() override;

  std::vector<std::string> get_keys() const override;

  bool read(const char* key, bool&) const override;
  bool read(const char* key, int&) const override;
  bool read(const char* key, float&) const override;
  bool read(const char* key, std::string&) const override;

  bool read(const char* key, std::vector<bool>& v) const override;
  bool read(const char* key, std::vector<int>& v) const override;
  bool read(const char* key, std::vector<float>& v) const override;
  bool read(const char* key, std::vector<std::string>& v) const override;

  bool read(const char* key, ReaderMapping&) const override;
  bool read(const char* key, ReaderCollection&) const override;
  bool read(const char* key, ReaderObject&) const override;

private:
  sexp::Value const* get_subsection_item(const char* name) const;
  sexp::Value const* get_subsection_items(const char* name) const;
  sexp::Value const* get_subsection(const char* name) const;

private:
  sexp::Value const& m_sx;
};

} // namespace prio

#endif

/* EOF */
