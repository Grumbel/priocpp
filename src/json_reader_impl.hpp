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

#ifndef HEADER_PRIO_JSON_READER_IMPL_HPP
#define HEADER_PRIO_JSON_READER_IMPL_HPP

#include "reader_impl.hpp"

#include <json/value.h>

namespace prio {

class JsonReaderDocumentImpl final : public ReaderDocumentImpl
{
public:
  JsonReaderDocumentImpl(Json::Value value);

  virtual ReaderObject get_root() const;

private:
  Json::Value m_value;
};

class JsonReaderObjectImpl final : public ReaderObjectImpl
{
public:
  JsonReaderObjectImpl(Json::Value const& value);
  ~JsonReaderObjectImpl() override;

  std::string get_name() const override;
  ReaderMapping get_mapping() const override;

private:
  Json::Value const& m_json;
};

class JsonReaderCollectionImpl final : public ReaderCollectionImpl
{
public:
  JsonReaderCollectionImpl(Json::Value const& value);
  ~JsonReaderCollectionImpl() override;

  std::vector<ReaderObject> get_objects() const override;

private:
  Json::Value const& m_json;
};

class JsonReaderMappingImpl final : public ReaderMappingImpl
{
public:
  JsonReaderMappingImpl(Json::Value const& value);
  ~JsonReaderMappingImpl() override;

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
  Json::Value const& m_json;
};

} // namespace prio

#endif

/* EOF */