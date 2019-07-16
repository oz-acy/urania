/**************************************************************************
 *
 *  scroll-h.cpp
 *  by oZ/acy
 *  (c) 2009-2014 oZ/acy. ALL RIGHTS RESERVED.
 *
 *  last update: 25 Jan 2014
 *************************************************************************/

#include "win.h"


/*==========================================================
 *  WMHandler::onScroll_()
 *  スクロール系の定型處理を行ひ、コールバックを呼び出す
 */
void urania::WMHandler::onScroll_(
  urania::Window* w, UINT msg, WPARAM wparam, LPARAM lparam)
{
  int code = LOWORD(wparam);
  int pos = HIWORD(wparam);
  HWND hw = (HWND)(lparam);

  int bar;
  int id;
  SCROLLINFO si;
  si.cbSize = sizeof(si);
  si.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
  if (hw) {
    bar = SB_CTL;
    id = ::GetDlgCtrlID(hw);
  }
  else if (msg == WM_VSCROLL)
  {
    hw = WndBase::getHWND(w);
    bar = SB_VERT;
    id = ID_SBV;
  }
  else
  {
    hw = WndBase::getHWND(w);
    bar = SB_HORZ;
    id = ID_SBH;
  }
  ::GetScrollInfo(hw, bar, &si);

  switch (code)
  {
  case SB_TOP:
    si.nPos = si.nMin;
    break;

  case SB_BOTTOM:
    si.nPos = si.nMax;
    break;

  case SB_LINEUP: 
    si.nPos--;
    break;
  case SB_PAGEUP:
    si.nPos -= si.nPage;
    break;
  case SB_LINEDOWN:
    si.nPos++;
    break;

  case SB_PAGEDOWN:
    si.nPos += si.nPage;
    break;

  case SB_THUMBPOSITION:
  case SB_THUMBTRACK:
    si.nPos = pos;
    break;
  }

  si.fMask = SIF_POS | SIF_DISABLENOSCROLL;
  SetScrollInfo(hw, bar, &si, TRUE);

  si.fMask = SIF_POS;
  GetScrollInfo(hw, bar, &si);

  onScroll(w, id, si.nPos);
}


//eof
