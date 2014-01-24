/**************************************************************************
 *
 *  getLongPath.cpp
 *  by oZ/acy
 *  (c) 2004-2009 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  last update: 2009.4.4
 *
 *************************************************************************/
#include "sys.h"
#include <cstring>


/*==============================================
*  getLongFileName()
*  "’·‚¢ƒtƒ@ƒCƒ‹–¼"‚ð“¾‚é
*=============================================*/
std::wstring urania::System::getLongPathName(const std::wstring& path)
{
  using namespace std;

  int len = 256;
  wchar_t* buf;

  for (;;) {
    buf = new wchar_t[len];
    int r = ::GetLongPathName(path.c_str(), buf, len);
    if (r == 0)
    {
      delete[] buf;
      return L"";
    }
    else if (r < len)
    {
      wstring res = buf;
      delete[] buf;
      return res;
    }
    else
    {
      len = r;
      delete[] buf;
    }
  }
}


//eof
