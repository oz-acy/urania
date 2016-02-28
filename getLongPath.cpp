/**************************************************************************
 *
 *  getLongPath.cpp
 *  by oZ/acy
 *  (c) 2004-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  óöó
 *    2016.2.27  èCê≥
 *************************************************************************/
#include <cstring>
#include <memory>
#include "system.h"

/*==============================================
 *  getLongFileName()
 *  "í∑Ç¢ÉtÉ@ÉCÉãñº"ÇìæÇÈ
 *============================================*/
std::wstring urania::System::getLongPathName(const std::wstring& path)
{
  using namespace std;

  int len = 256;
  std::unique_ptr<wchar_t[]> buf;

  for (;;) {
    buf.reset(new wchar_t[len]);
    int r = ::GetLongPathName(path.c_str(), buf.get(), len);
    if (r == 0)
    {
      return L"";
    }
    else if (r < len)
    {
      return std::wstring(buf.get());
    }
    else
    {
      len = r;
    }
  }
}


//eof
