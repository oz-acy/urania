/**************************************************************************
 *
 *  ctrl_sb.cpp
 *  by oZ/acy
 *  (c) 2012-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  WndBase スクロールバー制禦關係
 *
 *  履歴
 *    2016.2.27  修正
 *************************************************************************/

#include "wbase.h"

/*================================================
 *  スクロール位置を設定
 */
int urania::WndBase::getPosSB(int id)
{
  HWND w;
  int bar;
  if (id == ID_SBV)
  {
    w = hw_;
    bar = SB_VERT;
  }
  else if (id == ID_SBH)
  {
    w = hw_;
    bar = SB_HORZ;
  }
  else
  {
    w = ::GetDlgItem(hw_, id);
    bar = SB_CTL;
  }

  if (!w)
    return 0;

  SCROLLINFO si;
  si.cbSize = sizeof(si);
  si.fMask = SIF_POS;
  ::GetScrollInfo(w, bar, &si);
  return si.nPos;
}

/*================================================
 *  スクロール位置を取得
 */
void urania::WndBase::setPosSB(int id, int pos)
{
  HWND w;
  int bar;
  if (id == ID_SBV)
  {
    w = hw_;
    bar = SB_VERT;
  }
  else if (id == ID_SBH)
  {
    w = hw_;
    bar = SB_HORZ;
  }
  else
  {
    w = ::GetDlgItem(hw_, id);
    bar = SB_CTL;
  }

  if (!w)
    return;

  SCROLLINFO si;
  si.cbSize = sizeof(si);
  si.fMask = SIF_POS;
  si.nPos = pos;
  ::SetScrollInfo(w, bar, &si, TRUE);
}


/*================================================
 *  レンジを設定
 */
void urania::WndBase::setRangeSB(int id, int min, int max, int page)
{
  HWND w;
  int bar;
  if (id == ID_SBV)
  {
    w = hw_;
    bar = SB_VERT;
  }
  else if (id == ID_SBH)
  {
    w = hw_;
    bar = SB_HORZ;
  }
  else
  {
    w = ::GetDlgItem(hw_, id);
    bar = SB_CTL;
  }

  if (!w)
    return;

  SCROLLINFO si;
  si.cbSize = sizeof(si);
  si.fMask = SIF_PAGE | SIF_RANGE | SIF_DISABLENOSCROLL;
  si.nMin = min;
  si.nMax = max;
  si.nPage = page;
  ::SetScrollInfo(w, bar, &si, TRUE);
}



//eof
