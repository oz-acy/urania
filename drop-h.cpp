/**************************************************************************
*
*  drop-h.cpp
*  by oZ/acy
*  (c) 2009-2014 oZ/acy. ALL RIGHTS RESERVED.
*
*  Dropfiles Handler
*
*  last update: 25 Jan 2014
*/

#include "win.h"


/*===============================================================
 *  WMHandler::onDropFiles_()
 *  ドロップファイルの定型處理を行ひ、ユーザ實裝部を呼び出す
 */
void urania::WMHandler::onDropFiles_(urania::Window* win, WPARAM wp)
{
  HDROP hd = (HDROP)wp;
  POINT p;
  DragQueryPoint(hd, &p);

  std::vector<std::wstring> astr;
  wchar_t temp[256];
  int n = DragQueryFile(hd, 0xffffffff, nullptr, 0);

  for (int i = 0; i < n; i++)
  {
    DragQueryFile(hd, i, temp, 256);
    astr.push_back(temp);
  }

  DragFinish(hd);

  // ユーザがサブクラスでオーバーライドするハンドラを呼び出し
  onDropFiles(win, astr, p.x, p.y);
}




//eof
