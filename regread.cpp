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
/*
 * @file regread.cpp
 * @date oZ/acy
 * @brief レジストリ讀み込み操作クラス
 *
 * @date 22 Feb 2004  作成
 * @date 28 Dec 2005  修正
 * @date 24 Apr 2008  修正
 * @date  4 Apr 2009  修正 for UNICODE對應
 * @date  8 Sep 2011  修正 for C++11對應
 * @date 29 Feb 2016  修正
 *
 */
#include <memory>
#include <cstdlib>
#include "registry.h"


/*==========================================================
 *  RegistryReader::RegistryReader()
 *
 */
urania::RegistryReader::RegistryReader(
  urania::RegRootKey root, const std::wstring& path)
{
  HKEY rootkey = getRoot_(root);
  if (!rootkey)
    return;

  HKEY key;
  RegOpenKeyEx(rootkey, path.c_str(), 0, KEY_QUERY_VALUE, &key);
  setKey_(key);
}



/*==========================================================
 *  RegistryReader::getDwordData()
 *
 */
DWORD urania::RegistryReader::getDwordData(const std::wstring& name)
{
  DWORD typ;
  DWORD sz = 0;

  if (::RegQueryValueEx(getKey_(), name.c_str(), nullptr, &typ, nullptr, &sz))
    return 0;

  if (typ != REG_DWORD)
    return 0;

  // データ格納領域を確保
  auto data = std::make_unique<unsigned char[]>(sz);
  //std::unique_ptr<unsigned char[]> data(new unsigned char[sz]);

  // データを取得する
  if (::RegQueryValueEx(
          getKey_(), name.c_str(), nullptr, nullptr,
          (BYTE *)(data.get()), &sz)) {
    return 0;
  }

  return *(DWORD*)(data.get());
}


/*==========================================================
 *  RegistryReader::getStringData()
 *
 */
std::wstring urania::RegistryReader::getStringData(const std::wstring& name)
{
  DWORD typ;
  DWORD sz = 0;

  if (::RegQueryValueEx(getKey_(), name.c_str(), nullptr, &typ, nullptr, &sz))
    return L"";

  if (typ != REG_SZ)
    return L"";

  // データ格納領域を確保
  auto data = std::make_unique<unsigned char[]>(sz);
  //std::unique_ptr<unsigned char[]> data(new unsigned char[sz]);

  // データを取得する
  if (::RegQueryValueEx(
          getKey_(), name.c_str(), nullptr, nullptr,
          (BYTE*)(data.get()), &sz)) {
    return L"";
  }

  return std::wstring(reinterpret_cast<wchar_t*>(data.get()));
}




//eof
