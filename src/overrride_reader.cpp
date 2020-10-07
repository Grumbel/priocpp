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

#include "overrride_reader.hpp"

#include <set>

#include "reader_mapping.hpp"
#include "reader_impl.hpp"

namespace prio {

class OverrideReaderMappingImpl : public ReaderMappingImpl
{
private:
  ReaderMapping m_reader;
  ReaderMapping m_overrides;

public:
  OverrideReaderMappingImpl(const ReaderMapping& reader,
                            const ReaderMapping& overrides) :
    m_reader(reader),
    m_overrides(overrides)
  {
  }

  std::vector<std::string> get_keys() const override
  {
    std::set<std::string> result;

    auto lst = m_reader.get_keys();
    result.insert(lst.begin(), lst.end());
    lst = m_overrides.get_keys();
    result.insert(lst.begin(), lst.end());

    return std::vector<std::string>(result.begin(), result.end());
  }

  bool read(const char* name, int& v) const override
  {
    if (m_overrides.read(name, v))
    {
      return true;
    }
    else
    {
      return m_reader.read(name, v);
    }
  }

  bool read(const char* name, float& v) const override
  {
    if (m_overrides.read(name, v))
    {
      return true;
    }
    else
    {
      return m_reader.read(name, v);
    }
  }

  bool read(const char* name, bool& v) const override
  {
    if (m_overrides.read(name, v))
    {
      return true;
    }
    else
    {
      return m_reader.read(name, v);
    }
  }

  bool read(const char* name, std::string& str) const override
  {
    if (m_overrides.read(name, str))
    {
      return true;
    }
    else
    {
      return m_reader.read(name, str);
    }
  }

  bool read(const char* name, ReaderMapping& result) const override
  {
    ReaderMapping overwrite_result;
    if (m_overrides.read(name, overwrite_result))
    {
      if (m_reader.read(name, result))
      {
        result = make_override_mapping(result, overwrite_result);
        return true;
      }
      else
      {
        result = overwrite_result;
        return true;
      }
    }
    else
    {
      return m_reader.read(name, result);
    }
  }

  bool read(const char* key, ReaderCollection& result) const override
  {
    if (m_overrides.read(key, result))
    {
      return true;
    }
    else
    {
      return m_reader.read(key, result);
    }
  }

  bool read(const char* key, ReaderObject& result) const override
  {
    if (m_overrides.read(key, result))
    {
      return true;
    }
    else
    {
      return m_reader.read(key, result);
    }
  }
};

ReaderMapping
make_override_mapping(const ReaderMapping& reader, const ReaderMapping& overrides)
{
  return ReaderMapping(std::make_shared<OverrideReaderMappingImpl>(reader, overrides));
}

} // namespace prio

/* EOF */
