/**************************************************************************
 *
 *  sys-mbs2ws.cpp
 *  by oZ/acy
 *  (c) 2009-2011 oZ/acy. ALL RIGHTS RESERVED.
 *
 *  óöó
 *    9 Nov MMIX  çÏê¨
 *    8 Sep MMXI  èCê≥
 */
#include "sys.h"

std::wstring urania::System::cnvMBStr2WStr(const std::string& s)
{
  using namespace std;

  int len = MultiByteToWideChar(
              CP_THREAD_ACP, 0, s.c_str(), -1, nullptr, 0);

  wchar_t* buf = new wchar_t[len];

  MultiByteToWideChar(CP_THREAD_ACP, 0, s.c_str(), -1, buf, len);

  wstring r = buf;
  delete[] buf;
  return r;
}


//eof
