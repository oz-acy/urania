/*
 * Copyright 2012-2021 oZ/acy (名賀月晃嗣)
 * Redistribution and use in source and binary forms, 
 *     with or without modification, 
 *   are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*
 * @file ctrl_sb.cpp
 * @author oZ/acy
 * @brief WndBaseのスクロールバー制御處理
 *
 * @date 2016.2.27   修正
 * @date 2018.12.24  修正
 *
 */
#include "wbase.h"

/*================================================
 *  スクロール位置を設定
 */
int urania::WndBase::getPosSB(int id)
{
  if (!hw_)
    return 0;

  HWND w;
  int bar;
  if (id == ID_SBV) {
    w = hw_;
    bar = SB_VERT;
  }
  else if (id == ID_SBH) {
    w = hw_;
    bar = SB_HORZ;
  }
  else {
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
  if (!hw_)
    return;

  HWND w;
  int bar;
  if (id == ID_SBV) {
    w = hw_;
    bar = SB_VERT;
  }
  else if (id == ID_SBH) {
    w = hw_;
    bar = SB_HORZ;
  }
  else {
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
 *  レンジを取得
 */
void urania::WndBase::getRangeSB(int id, int& min, int& max, int& page)
{
  if (!hw_)
    return;

  HWND w;
  int bar;
  if (id == ID_SBV) {
    w = hw_;
    bar = SB_VERT;
  }
  else if (id == ID_SBH) {
    w = hw_;
    bar = SB_HORZ;
  }
  else {
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
 *  レンジを設定
 */
void urania::WndBase::setRangeSB(int id, int min, int max, int page)
{
  if (!hw_)
    return;

  HWND w;
  int bar;
  if (id == ID_SBV) {
    w = hw_;
    bar = SB_VERT;
  }
  else if (id == ID_SBH) {
    w = hw_;
    bar = SB_HORZ;
  }
  else {
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
