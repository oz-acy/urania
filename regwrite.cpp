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
 * @file regwrite.cpp
 * @author by oZ/acy
 * @brief レジストリ書き込み操作クラス
 *
 * @date 22 Feb 2004  作成
 * @date 17 Mar 2006  修正
 * @date 24 Apr 2008  修正
 * @date  4 Apr 2009  修正 for UNICODE對應
 * @date  8 Sep 2011  修正 for C++11對應等
 * @date 29 Feb 2016  修正
 *
 */

#include "registry.h"


/*=====================================================
 *  RegistryWriter::RegistryWriter()
 *
 */
urania::RegistryWriter::RegistryWriter(
  urania::RegRootKey root, const std::wstring& path)
{
  HKEY rootkey = getRoot_(root);
  if (!rootkey)
    return;

  HKEY key;
  RegCreateKeyEx(
    rootkey, path.c_str(), 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, &key,
    nullptr);
  setKey_(key);
}


/*==========================================================
 *  RegistryWriter::setDwordData()
 *
 */
void
urania::RegistryWriter::setDwordData(const std::wstring& name, DWORD val)
{
  ::RegSetValueEx(
    getKey_(), name.c_str(), 0, REG_DWORD, reinterpret_cast<BYTE*>(&val),
    sizeof(val));
}


/*==========================================================
 *  RegistryWriter::setStringData()
 *
 */
void
urania::RegistryWriter::setStringData(
  const std::wstring& name, const std::wstring& val)
{
  ::RegSetValueEx(
    getKey_(), name.c_str(), 0, REG_SZ,
    reinterpret_cast<const BYTE*>(val.c_str()),
    (val.size() + 1) * sizeof(wchar_t));
}




//eof
