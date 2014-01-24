/*********************************************************************
*
*  askYNCBox.cpp
*  by oZ/acy
*  (c) 2001-2009. ALL RIGHTS RESERVED.
*
*  —š—ð
*    14 Feb 2004  sys.cpp ‚©‚ç•ªŠ„
*     4 Apr 2009  UNICODE›”œä string -> wstring
*********************************************************************/

#include "sys.h"


int urania::System::askYNCBox(const std::wstring& ttl, const std::wstring& msg)
{
  int res = ::MessageBox(NULL, msg.c_str(), ttl.c_str(), MB_YESNOCANCEL);
  switch(res)
  {
  case IDYES:
    return 1;
  case IDNO:
    return 0;
  case IDCANCEL:
    return 2;
  default:
    return 2;  // ƒLƒƒƒ“ƒZƒ‹
  }
}


//eof
