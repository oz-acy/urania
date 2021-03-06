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
 * @file pdev.cpp
 * @author oZ/acy
 * 
 * class urania::PaintDevice の實裝定義(1)
 * コンストラクタ、デストラクタ、システムカラー系、クリア及び轉送處理
 *
 * @date 2016.3.2  修正
 * @date 2021.3.23
 *   PaintDevice::blt()の一部の仕樣を變更。PaintDevice::stretchBlt()を追加。
 * @date 2021.6.11
 *   依據ライブラリをthemis+polymniaからeunomiaに切り替へるための修正
 *
 */
#include "paintdev.h"
#include <cstring>


urania::PaintDevice::PaintDevice(HDC dc, DestProc dp, void* ap, int w, int h)
  : hdc_(dc), width_(w), height_(h), app_(ap), dst_(dp)
{
  // DCに設定するオブジェクトを生成する。
  HBRUSH tb = CreateSolidBrush(RGB(255, 255, 255));
  HPEN tp = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
  HFONT tf
   = CreateFont(
       16, 0, 0, 0, 0, 0, 0, 0, SHIFTJIS_CHARSET, 0, 0, 0, FIXED_PITCH,
       nullptr);

  // 生成したオブジェクトをDCに設定するとともに、原狀を保存しておく。
  obr_ = (HBRUSH)SelectObject(hdc_, tb);
  opn_ = (HPEN)SelectObject(hdc_, tp);
  oft_ = (HFONT)SelectObject(hdc_, tf);
  obm_ = SetBkMode(hdc_, TRANSPARENT);
}


urania::PaintDevice::~PaintDevice()
{
  // DCの各オブジェクトを原狀復歸する。
  HBRUSH tb = (HBRUSH)SelectObject(hdc_, obr_);
  HPEN tp = (HPEN)SelectObject(hdc_, opn_);
  HFONT tf = (HFONT)SelectObject(hdc_, oft_);
  SetBkMode(hdc_, obm_);

  // 自分で作成、設定してゐたオブジェクトを破棄する。
  DeleteObject(tb);
  DeleteObject(tp);
  DeleteObject(tf);

  // 解放用コールバックがあるなら呼び出す。
  if (dst_)
    dst_(hdc_, app_);
}


std::unique_ptr<urania::PaintDevice>
urania::PaintDevice::create(HDC dc, DestProc dp, void* a, int w, int h)
{
  try {
    return std::unique_ptr<PaintDevice>(new PaintDevice(dc, dp, a, w, h));
  }
  catch (std::bad_alloc&) {
    return nullptr;
  }
}


void urania::PaintDevice::setSysColor(int id, const urania::Color& col)
{
  int i[1] = { id };
  COLORREF c[1] = { col.getColorref() };
  SetSysColors(1, i, c);
}


void urania::PaintDevice::clear(const urania::Color& col)
{
  HBRUSH br = CreateSolidBrush(col.getColorref());
  RECT rc = {0, 0, width_, height_};
  FillRect(hdc_, &rc, br);
  DeleteObject(br);
}




void
urania::PaintDevice::blt(
  const urania::PaintMemDevice& src, int sx, int sy, int w, int h,
  int dx, int dy, const std::optional<eunomia::Rect>& cliprect)
{
  eunomia::implement_::Clipper_
    clipper(
      sx, sy, src.width(), src.height(), w, h,
      dx, dy, width(), height(), cliprect);

  if (clipper) {
    BitBlt(
      hdc_, clipper.dx, clipper.dy, clipper.w, clipper.h,
      src.hdc_, clipper.sx, clipper.sy, SRCCOPY);
  }

  //polymnia::imp_::Clip_ clip(sx, sy, w, h, dx, dy, mask);
  //if (clip)
  //  BitBlt(
  //    hdc_, clip.dx, clip.dy, clip.w, clip.h, src->hdc_, clip.sx, clip.sy,
  //    SRCCOPY);
}

//void
//urania::PaintDevice::blt(
//  int dx, int dy, const urania::PaintMemDevice* src, int sx, int sy,
//  int w, int h)
//{
//  polymnia::Rect rect(0, 0, width_, height_);
//  blt(dx, dy, src, sx, sy, w, h, rect);
//}

void
urania::PaintDevice::blt(
  urania::PaintMemDeviceIndexed& src, int sx, int sy, int w, int h,
  int dx, int dy, const std::optional<eunomia::Rect>& cliprect)
  //int dx, int dy, urania::PaintMemDeviceIndexed* src, int sx, int sy,
  //int w, int h, const polymnia::Rect& mask)
{
  eunomia::implement_::Clipper_
    clipper(
      sx, sy, src.width(), src.height(), w, h,
      dx, dy, width(), height(), cliprect);

  if (clipper) {
    src.updatePalette();
    BitBlt(
      hdc_, clipper.dx, clipper.dy, clipper.w, clipper.h,
      src.hdc_, clipper.sx, clipper.sy, SRCCOPY);
  }


  //polymnia::imp_::Clip_ clip(sx, sy, w, h, dx, dy, mask);
  //if (clip)
  //{
  //  src->updatePalette();
  //  BitBlt(
  //    hdc_, clip.dx, clip.dy, clip.w, clip.h, src->hdc_, clip.sx, clip.sy,
  //    SRCCOPY);
  //}
}

//void
//urania::PaintDevice::blt(
//  int dx, int dy, urania::PaintMemDeviceIndexed* src, int sx, int sy,
//  int w, int h)
//{
//  polymnia::Rect rect(0, 0, width_, height_);
//  blt(dx, dy, src, sx, sy, w, h, rect);
//}


void urania::PaintDevice::stretchBlt(const urania::PaintMemDevice& src)
{
  SetStretchBltMode(hdc_, COLORONCOLOR);
  StretchBlt(
    hdc_, 0, 0, width_, height_, src.hdc_, 0, 0, src.width(), src.height(),
    SRCCOPY);
}

void urania::PaintDevice::stretchBlt(urania::PaintMemDeviceIndexed& src)
{
  src.updatePalette();
  SetStretchBltMode(hdc_, COLORONCOLOR);
  StretchBlt(
    hdc_, 0, 0, width_, height_, src.hdc_, 0, 0, src.width(), src.height(),
    SRCCOPY);
}


/*=====================================================
 *  PaintDevice::blt()  × 2
 *  描畫内容の轉送。但しアスペクト比を保存しつつ擴大縮小する。
 *  轉送元とアスペクト比が異なる場合、餘白が生じる。
 *
 *  @param src 轉送元
 */
void urania::PaintDevice::blt(const urania::PaintMemDevice& src)
{
  auto w = width_;
  auto h = height_;

  if ((double)w / (double)src.width() > (double)h / (double)src.height())
    w = src.width() * h / src.height();
  else if ((double)w / (double)src.width() < (double)h / (double)src.height())
    h = src.height() * w / src.width();

  //clear(polymnia::RgbColor(255, 255, 255));
  SetStretchBltMode(hdc_, COLORONCOLOR);
  StretchBlt(
    hdc_, 0, 0, w, h, src.hdc_, 0, 0, src.width(), src.height(),
    SRCCOPY);
}

void urania::PaintDevice::blt(urania::PaintMemDeviceIndexed& src)
{
  auto w = width_;
  auto h = height_;

  if ((double)w / (double)src.width() > (double)h / (double)src.height())
    w = src.width() * h / src.height();
  else if ((double)w / (double)src.width() < (double)h / (double)src.height())
    h = src.height() * w / src.width();

  src.updatePalette();
  //clear(polymnia::RgbColor(255, 255, 255));
  SetStretchBltMode(hdc_, COLORONCOLOR);
  StretchBlt(
    hdc_, 0, 0, w, h, src.hdc_, 0, 0, src.width(), src.height(),
    SRCCOPY);
}




//eof
