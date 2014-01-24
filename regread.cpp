/**************************************************************************
 *
 *  regread.cpp
 *  by oZ/acy
 *  (c) 2004-2011 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  レジストリ操作クラス (Reader)
 *
 *  履歴
 *    22 Feb 2004  作成
 *    28 Dec 2005  修正
 *    24 Apr 2008  修正
 *     4 Apr 2009  修正 for UNICODE對應
 *     8 Sep 2011  修正 for C++11對應
 *************************************************************************/
#include "registry.h"
#include <cstdlib>


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
themis::DWord urania::RegistryReader::getDwordData(const std::wstring& name)
{
  DWORD typ;
  DWORD sz = 0;

  if (::RegQueryValueEx(getKey_(), name.c_str(), nullptr, &typ, nullptr, &sz))
    return 0;

  if (typ != REG_DWORD)
    return 0;

  // データ格納領域を確保
  unsigned char* data = new unsigned char[sz];

  // データを取得する
  if (::RegQueryValueEx(
          getKey_(), name.c_str(), nullptr, nullptr, (BYTE *)data, &sz))
  {
    delete[] data;
    return 0;
  }

  themis::DWord res = *(themis::Dword*)(data);
  delete[] data;
  return res;
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
  unsigned char* data = new unsigned char[sz];

  // データを取得する
  if (::RegQueryValueEx(
          getKey_(), name.c_str(), nullptr, nullptr, (BYTE*)data, &sz))
  {
    delete[] data;
    return L"";
  }

  std::wstring res = reinterpret_cast<wchar_t*>(data);
  delete[] data;
  return res;
}


//eof
