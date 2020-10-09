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

#include "reader_mapping.hpp"

#include "reader_collection.hpp"
#include "reader_object.hpp"
#include "reader_impl.hpp"

namespace prio {

ReaderMapping::ReaderMapping(std::unique_ptr<ReaderMappingImpl> impl) :
  m_impl(std::move(impl))
{
}

ReaderMapping::ReaderMapping() :
  m_impl()
{
}

ReaderMapping::~ReaderMapping()
{
}

ReaderMapping&
ReaderMapping::operator=(ReaderMapping&&) = default;

bool
ReaderMapping::read(std::string_view key, bool& value) const
{
  if (m_impl)
    return m_impl->read(key, value);
  else
    return false;
}

bool
ReaderMapping::read(std::string_view key, int& value) const
{
  if (m_impl)
    return m_impl->read(key, value);
  else
    return false;
}

bool
ReaderMapping::read (std::string_view key, float& value) const
{
  if (m_impl)
    return m_impl->read(key, value);
  else
    return false;
}

bool
ReaderMapping::read(std::string_view key, std::string& value) const
{
  if (m_impl)
    return m_impl->read(key, value);
  else
    return false;
}

bool
ReaderMapping::read  (std::string_view key, std::vector<bool>& values) const
{
  if (m_impl)
    return m_impl->read(key, values);
  else
    return false;
}

bool
ReaderMapping::read(std::string_view key, std::vector<int>& values) const
{
  if (m_impl)
    return m_impl->read(key, values);
  else
    return false;
}

bool
ReaderMapping::read(std::string_view key, std::vector<float>& values) const
{
  if (m_impl)
    return m_impl->read(key, values);
  else
    return false;
}

bool
ReaderMapping::read(std::string_view key, std::vector<std::string>& values) const
{
  if (m_impl)
    return m_impl->read(key, values);
  else
    return false;
}

bool
ReaderMapping::read(std::string_view key, ReaderObject& object) const
{
  if (m_impl)
    return m_impl->read(key, object);
  else
    return false;
}

bool
ReaderMapping::read(std::string_view key, ReaderMapping& mapping) const
{
  if (m_impl)
    return m_impl->read(key, mapping);
  else
    return false;
}

bool
ReaderMapping::read(std::string_view key, ReaderCollection& collection) const
{
  if (m_impl)
    return m_impl->read(key, collection);
  else
    return false;
}

std::vector<std::string>
ReaderMapping::get_keys() const
{
  if (m_impl)
    return m_impl->get_keys();
  else
    return {};
}

} // namespace prio

/* EOF */
