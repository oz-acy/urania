/**************************************************************************
 *
 *  ctrl_sb.cpp
 *  by oZ/acy
 *  (c) 2012-2018 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  WndBase �X�N���[���o�[����萌W
 *
 *  ����
 *    2016.2.27   �C��
 *    2018.12.24  �C��
 */
#include "wbase.h"

/*================================================
 *  �X�N���[���ʒu��ݒ�
 */
int urania::WndBase::getPosSB(int id)
{
  if (!hw_)
    return 0;

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
 *  �X�N���[���ʒu���擾
 */
void urania::WndBase::setPosSB(int id, int pos)
{
  if (!hw_)
    return;

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
 *  �����W���擾
 */
void urania::WndBase::getRangeSB(int id, int& min, int& max, int& page)
{
  if (!hw_)
    return;

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
  ZeroMemory(&si, sizeof(SCROLLINFO));
  si.cbSize = sizeof(si);
  si.fMask = SIF_PAGE | SIF_RANGE;
  ::GetScrollInfo(w, bar, &si);
  min = si.nMin;
  max = si.nMax;
  page = si.nPage;
}


/*================================================
 *  �����W��ݒ�
 */
void urania::WndBase::setRangeSB(int id, int min, int max, int page)
{
  if (!hw_)
    return;

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
