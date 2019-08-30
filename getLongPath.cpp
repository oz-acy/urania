/**************************************************************************
 *
 *  getLongPath.cpp
 *  by oZ/acy (名賀月晃嗣)
 *
 *  履歴
 *    2016.2.27  修正
 *    2019.8.30  修正
 */
#include <cstring>
#include <memory>
#include "system.h"

/*==============================================
 *  getLongFileName()
 *  "長いファイル名"を得る
 *============================================*/
std::wstring urania::System::getLongPathName(const std::wstring& path)
{
  using namespace std;

  int len = 256;

  for (;;) {
    auto buf = std::make_unique<wchar_t[]>(len);
    int r = ::GetLongPathName(path.c_str(), buf.get(), len);
    if (r == 0) {
      return L"";
    }
    else if (r < len) {
      return std::wstring(buf.get());
    }
    else {
      len = r;
    }
  }
}


//eof
