/*
 * Copyright 2002-2021 oZ/acy (名賀月晃嗣)
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
 * @file pdev2.cpp
 * @author oZ/acy (名賀月晃嗣)
 *
 * class urania::PaintDevice の実装定義(2)
 * 圖形文字描畫及びそのヘルパ
 *
 * @date 2016.2.27  修正
 * @date 2019.8.29  修正
 */
#include <cstring>
#include "paintdev.h"


/*===============================================================
 *  PaintDevice::changeBrush_()
 *  描画用ブラシの交換
 *   引数 :  col : 新たに指定するブラシの色
 */
void urania::PaintDevice::changeBrush_(const urania::Color& col)
{
  HBRUSH br = CreateSolidBrush(col.getColorref());
  HBRUSH ob = (HBRUSH)SelectObject(hdc_, br);
  DeleteObject(ob);
}


/*==========================================================
 *  PaintDevice::changePen_()
 *  描画用ペンの交換
 *   引数 :  col : 新たに指定するペンの色
 */
void urania::PaintDevice::changePen_(const urania::Color& col)
{
  HPEN tp = CreatePen(PS_SOLID, 0, col.getColorref());
  HPEN op = (HPEN)SelectObject(hdc_, tp);
  DeleteObject(op);
}


/*===========================================
 *  PaintDevice::box()
 *  長方形を描画
 */
void
urania::PaintDevice::box(
  int x1, int y1, int x2, int y2, const urania::Color& col, bool f)
{
  if (f) {
    HBRUSH fb = CreateSolidBrush(col.getColorref());
    RECT rc = {x1, y1, x2, y2};
    FillRect(hdc_, &rc, fb);
    DeleteObject(fb);
  }
  else {
    POINT pt[5];
    pt[0].x = x1;
    pt[0].y = y1;
    pt[1].x = x2;
    pt[1].y = y1;
    pt[2].x = x2;
    pt[2].y = y2;
    pt[3].x = x1;
    pt[3].y = y2;
    pt[4].x = x1;
    pt[4].y = y1;
    changePen_(col);
    Polyline(hdc_, pt, 5);
  }
}


/*======================================
 *  PaintDevice::line()
 *  直線を描画
 */
void
urania::PaintDevice::line(
  int x1, int y1, int x2, int y2, const urania::Color& col)
{
  changePen_(col);
  MoveToEx(hdc_, x1, y1, nullptr);
  LineTo(hdc_, x2, y2);
}


/*======================================
 *  PaintDevice::ellipse()
 *  楕円を描画
 */
void
urania::PaintDevice::ellipse(
  int x, int y, int a, int b, const urania::Color& col, bool f)
{
  changePen_(col);

  if (f) {
    changeBrush_(col);
    Ellipse(hdc_, x-a, y-b, x+a, y+b);
  }
  else {
    Arc(hdc_, x-a, y-b, x+a, y+b, x, y-1, x, y+1);
    Arc(hdc_, x-a, y-b, x+a, y+b, x, y+1, x, y-1);
  }
}


/*======================================
 *  PaintDevice::dotset()
 *  点を描画
 */
void urania::PaintDevice::dotset(int x, int y, const urania::Color& col)
{
  ::SetPixelV(hdc_, x, y, col.getColorref());
}


/*===========================================
 *  PaintDevice::text()
 *  テキストを描画
 *   返値 : 描画長方形の右下角座標
 */
eunomia::Point
urania::PaintDevice::text(
  int x, int y, const std::wstring& str, const urania::Color& col)
{
  ::SetTextColor(hdc_, col.getColorref());
  auto ptxt = std::make_unique<wchar_t[]>(str.length());
  std::wcscpy(ptxt.get(), str.c_str());

  RECT rc = {x, y, x, y};
  DRAWTEXTPARAMS dtp;
  dtp.cbSize = sizeof(dtp);
  dtp.iTabLength = 4;
  dtp.iLeftMargin = 0;
  dtp.iRightMargin = 0;

  ::DrawTextEx(
    hdc_, ptxt.get(), -1, &rc,
    DT_CALCRECT | DT_NOPREFIX | DT_TABSTOP | DT_EXPANDTABS, &dtp);

  ::DrawTextEx(
    hdc_, ptxt.get(), -1, &rc,
    DT_NOCLIP | DT_NOPREFIX | DT_TABSTOP | DT_EXPANDTABS, &dtp);

  return eunomia::Point(rc.right, rc.bottom);
}


/*===========================================
 *  PaintDevice::text()
 *  テキストを描画(幅指定バージョン)
 *   返値 : 描画長方形の右下角座標
 */
eunomia::Point
urania::PaintDevice::text(
  int x, int y, int ww, const std::wstring& str, const urania::Color& col)
{
  ::SetTextColor(hdc_, col.getColorref());
  auto ptxt = std::make_unique<wchar_t>(str.length());
  std::wcscpy(ptxt.get(), str.c_str());

  RECT rc = {x, y, x + ww, y};
  DRAWTEXTPARAMS dtp;
  dtp.cbSize = sizeof(dtp);
  dtp.iTabLength = 4;
  dtp.iLeftMargin = 0;
  dtp.iRightMargin = 0;

  ::DrawTextEx(
    hdc_, ptxt.get(), -1, &rc,
    DT_CALCRECT | DT_NOPREFIX | DT_WORDBREAK | DT_EDITCONTROL
    | DT_TABSTOP | DT_EXPANDTABS,
    &dtp);

  ::DrawTextEx(
    hdc_, ptxt.get(), -1, &rc,
    DT_NOCLIP | DT_NOPREFIX | DT_WORDBREAK | DT_EDITCONTROL
    | DT_TABSTOP | DT_EXPANDTABS,
    &dtp);

  return eunomia::Point(rc.right, rc.bottom);
}


/*===========================================
 *  PaintDevice::changeFont()
 *  フォントの変更
 */
void
urania::PaintDevice::changeFont(
  int size, const std::wstring& fn,
  bool ro, bool fx, bool bo, bool itl, bool ul, bool sk)
{
  const wchar_t *face = fn.length() ? fn.c_str() : nullptr;
  DWORD paf = fx ? FIXED_PITCH : DEFAULT_PITCH;
  int wgt = bo ? FW_BOLD : FW_NORMAL;
  BYTE italic = (BYTE)(itl ? 1 : 0);
  BYTE uline = (BYTE)(ul ? 1 : 0);
  BYTE strike = (BYTE)(sk ? 1 : 0);

  paf |= ro ? FF_ROMAN : FF_MODERN;
  int wf = fx ? (size / 2) : 0;

  HFONT tf
   = CreateFont(
       size, wf, 0, 0, wgt, italic, uline, strike, SHIFTJIS_CHARSET, 0, 0, 0,
       paf, face);

  HFONT of = (HFONT)SelectObject(hdc_, tf);
  DeleteObject(of);
}


//eof
