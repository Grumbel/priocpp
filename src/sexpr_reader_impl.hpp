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

#include "error_handler.hpp"
#include "reader_impl.hpp"

namespace prio {

class SExprReaderDocumentImpl final : public ReaderDocumentImpl
{
public:
  SExprReaderDocumentImpl(sexp::Value sx, ErrorHandler error_handler,
                          std::optional<std::string> filename);

  ReaderObject get_root() const override;
  std::optional<std::string> get_filename() const override { return m_filename; }

  void error(sexp::Value const& sx, std::string_view message) const;
  void error(ErrorHandler error_handler, sexp::Value const& sx, std::string_view message) const;

private:
  sexp::Value m_sx;
  ErrorHandler m_error_handler;
  std::optional<std::string> m_filename;
};

class SExprReaderObjectImpl final : public ReaderObjectImpl
{
public:
  SExprReaderObjectImpl(SExprReaderDocumentImpl const& doc, sexp::Value const& sx);
  ~SExprReaderObjectImpl() override;

  std::string get_name() const override;
  ReaderMapping get_mapping() const override;

private:
  SExprReaderDocumentImpl const& m_doc;
  sexp::Value const& m_sx;
};

class SExprReaderCollectionImpl final : public ReaderCollectionImpl
{
public:
  SExprReaderCollectionImpl(SExprReaderDocumentImpl const& doc, sexp::Value const& m_sx);
  ~SExprReaderCollectionImpl() override;

  std::vector<ReaderObject> get_objects() const override;

private:
  SExprReaderDocumentImpl const& m_doc;
  sexp::Value const& m_sx;
};

class SExprReaderMappingImpl final : public ReaderMappingImpl
{
public:
  SExprReaderMappingImpl(SExprReaderDocumentImpl const& doc, sexp::Value const& m_sx);
  ~SExprReaderMappingImpl() override;

  std::vector<std::string> get_keys() const override;

  bool read(std::string_view key, bool&) const override;
  bool read(std::string_view key, int&) const override;
  bool read(std::string_view key, float&) const override;
  bool read(std::string_view key, std::string&) const override;

  bool read(std::string_view key, std::vector<bool>& v) const override;
  bool read(std::string_view key, std::vector<int>& v) const override;
  bool read(std::string_view key, std::vector<float>& v) const override;
  bool read(std::string_view key, std::vector<std::string>& v) const override;

  bool read(std::string_view key, ReaderMapping&) const override;
  bool read(std::string_view key, ReaderCollection&) const override;
  bool read(std::string_view key, ReaderObject&) const override;

  void error(std::string_view key, std::string_view message) const override;
  void missing_key_error(std::string_view key) const override;

private:
  sexp::Value const* get_subsection_item(std::string_view key) const;
  sexp::Value const* get_subsection_items(std::string_view key) const;
  sexp::Value const* get_subsection(std::string_view key) const;

private:
  SExprReaderDocumentImpl const& m_doc;
  sexp::Value const& m_sx;
};

} // namespace prio

#endif

/* EOF */
