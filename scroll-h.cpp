/*
 * Copyright 2009-2021 oZ/acy (名賀月晃嗣)
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
 * @file scroll-h.cpp
 * @author oZ/acy (名賀月晃嗣)
 *
 */
#include "window.h"


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
  else if (msg == WM_VSCROLL) {
    hw = WndBase::getHWND(w);
    bar = SB_VERT;
    id = ID_SBV;
  }
  else {
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
