/**************************************************************************
 *
 *  sys-ws2mbs.cpp
 *  by oZ/acy
 *  (c) 2009-2016 oZ/acy. ALL RIGHTS RESERVED.
 *
 *  履歴
 *    4 Apr MMIX  作成
 *    7 Nov MMIX  修正
 *    8 Sep MMXI  修正
 *    27 Feb MMXVI  メソッド名變更
 */
#include <memory>
#include "system.h"

std::string urania::System::strcpyWideToMultiByte(const std::wstring& ws)
{
  using namespace std;

  int len
    = WideCharToMultiByte(
        CP_THREAD_ACP, 0, ws.c_str(), -1, nullptr, 0, nullptr, nullptr);

  unique_ptr<char[]> buf(new char[len]);

  WideCharToMultiByte(
    CP_THREAD_ACP, 0, ws.c_str(), -1, buf.get(), len, nullptr, nullptr);

  return string(buf.get());
}


//eof
