/*
 * Copyright 2004-2021 oZ/acy (名賀月晃嗣)
 * Redistribution and use in source and binary forms, 
 *     with or without modification, 
 *   are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/**
 *  @file registry.h
 *  @author oZ/acy (名賀月晃嗣)
 *  @brief レジストリ操作クラス
 *
 *  @date  15 Feb 2004  作成
 *  @date  24 Apr 2008  修正
 *  @date   4 Apr 2009  修正 for UNICODE對應
 *  @date  29 Feb 2016  修正
 *
 */
#ifndef INCLUDE_GUARD_URANIA_REGISTRY_H
#define INCLUDE_GUARD_URANIA_REGISTRY_H

#include <windows.h>
#include <string>


namespace urania {

enum RegRootKey {
  REGKEY_CLASSES_ROOT,
  REGKEY_CURRENT_USER,
  REGKEY_LOCAL_MACHINE,
  REGKEY_USERS,
  REGKEY_CURRENT_CONFIG,
};


/*----------------------------------------------------------
 *  class RegistryBase
 *  レジストリ操作クラス基底
 */
class RegistryBase
{
private:
  HKEY key_;

protected:
  RegistryBase() : key_(NULL) {}

  void setKey_(HKEY k) { key_ = k; }
  HKEY getKey_() const { return key_; }

public:
  ~RegistryBase()
  {
    if (key_)
      ::RegCloseKey(key_);
  }

protected:
  static HKEY getRoot_(RegRootKey k);
};


/*-----------------------------------------------------------
 *  class RegistryReader
 *  レジストリ讀込クラス
 */
class RegistryReader : public RegistryBase
{
public:
  RegistryReader(RegRootKey root, const std::wstring& path);

  operator bool() const { return getKey_() != NULL; }
  bool operator!() const { return getKey_() == NULL; }

  DWORD getDwordData(const std::wstring& name);
  std::wstring getStringData(const std::wstring& name);
};


/*-----------------------------------------------------------
 *  class RegistryWriter
 *  レジストリ書込クラス
 */
class RegistryWriter : public RegistryBase
{
public:
  RegistryWriter(RegRootKey root, const std::wstring& path);

  operator bool() const { return getKey_() != NULL; }
  bool operator!() const { return getKey_() == NULL; }

  void setDwordData(const std::wstring& name, DWORD val);
  void setStringData(const std::wstring& name, const std::wstring& val);
};


}// end of namespace urania


#endif // INCLUDE_GUARD_URANIA_REGISTRY_H
