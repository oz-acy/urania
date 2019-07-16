/**************************************************************************
 *
 *  sys-mbs2ws.cpp
 *  by oZ/acy
 *  (c) 2009-2016 oZ/acy. ALL RIGHTS RESERVED.
 *
 *  履歴
 *    9 Nov MMIX  作成
 *    8 Sep MMXI  修正
 *    27 Feb MMXVI  メソッド名變更
 */
#include <memory>
#include "system.h"

std::wstring urania::System::strcpyMultiByteToWide(const std::string& s)
{
  using namespace std;

  int len
    = MultiByteToWideChar(
        CP_THREAD_ACP, 0, s.c_str(), -1, nullptr, 0);

  unique_ptr<wchar_t[]> buf(new wchar_t[len]);
  MultiByteToWideChar(CP_THREAD_ACP, 0, s.c_str(), -1, buf.get(), len);
  return wstring(buf.get());
}


//eof
