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
/**
 * @file pmdev.cpp
 * @author oZ/acy
 * @brief class urania::PaintMemDevice の實裝
 *
 * @date 2016.2.27 修正
 * @date 2016.3.2  修正
 * @date 2019.8.29 修正
 * @date 2021.3.26 修正
 * @date 2021.4.1  修正
 *
 */
#include <algorithm>
#include "paintdev.h"


urania::PaintMemDevice::PaintMemDevice(unsigned w, unsigned h)
  : polymnia::ImageBuffer<urania::Color>(w, h, 0), hdc_(NULL), oldbmp_(NULL)
{
  int oo = w_ * sizeof(Color);
  offset_
    = (oo % 4) ? (oo / (4 * sizeof(Color)) + 1) * 4  :  oo / sizeof(Color);

  HBITMAP hBitmapNew;
  HDC hTmpDC;
  struct
  {
    BITMAPINFOHEADER Header;
    RGBQUAD ColorTable[1];
  } BmpInfo;

  BmpInfo.Header.biSize = sizeof(BITMAPINFOHEADER);
  BmpInfo.Header.biWidth = offset_;
  BmpInfo.Header.biHeight = -h_;
  BmpInfo.Header.biPlanes = 1;
  BmpInfo.Header.biBitCount = 24;
  BmpInfo.Header.biCompression = BI_RGB;
  BmpInfo.Header.biSizeImage = 0;
  BmpInfo.Header.biXPelsPerMeter = 0;
  BmpInfo.Header.biYPelsPerMeter = 0;
  BmpInfo.Header.biClrUsed = 0;
  BmpInfo.Header.biClrImportant = 0;

  hTmpDC = GetDC(NULL);
  if (!hTmpDC)
    return;

  hBitmapNew
   = CreateDIBSection(
       hTmpDC, (BITMAPINFO*)&BmpInfo, DIB_RGB_COLORS, (void**)(&buf_), NULL, 0);

  if (!hBitmapNew || !buf_) {
    //初期化に失敗
    ReleaseDC(NULL, hTmpDC);
    return;
  }

  hdc_ = CreateCompatibleDC(hTmpDC);
  ReleaseDC(NULL, hTmpDC);

  oldbmp_ = (HBITMAP)SelectObject(hdc_, hBitmapNew);

  //バッファ内容の初期化
  //memset(buf_, 0, sizeof(Color) * offset_ * h_); // 今更流石にこれは無い
  std::fill_n(buf_, offset_ * h_, Color(0, 0, 0));
  
}


urania::PaintMemDevice::~PaintMemDevice()
{
  HBITMAP hbmp;
  if (hdc_ && oldbmp_) {
    // hdc_が確保されている場合には開放する
    hbmp = (HBITMAP)SelectObject(hdc_, oldbmp_);
    DeleteObject(hbmp);
    DeleteDC(hdc_);
  }
}


std::unique_ptr<urania::PaintMemDevice>
urania::PaintMemDevice::create(unsigned w, unsigned h)
{
  try {
    std::unique_ptr<PaintMemDevice> dv(new PaintMemDevice(w, h));
    if (dv->buf_)
      return dv;
    else
      return nullptr;
  }
  catch (std::bad_alloc&) {
    return nullptr;
  }
}


std::unique_ptr<urania::PaintMemDevice>
urania::PaintMemDevice::duplicate(const polymnia::Picture* pct)
{
  int ww = pct->width();
  int hh = pct->height();

  auto vd = create(ww, hh);
  if (!vd)
    return nullptr;

  const polymnia::RgbColor* src = pct->buffer();
  Color* res = vd->buffer();

  int o = pct->offset();
  int oo = vd->offset_;
  for (int j = 0, p = 0, q = 0; j < hh; j++, p += o, q += oo)
    for (int i = 0; i < ww; i++)
      res[q + i] = src[p + i];

  return vd;
}


std::unique_ptr<polymnia::Picture>
urania::PaintMemDevice::duplicatePicture() const
{
  using namespace polymnia;

  auto pct = Picture::create(w_, h_);
  if (!pct)
    return nullptr;

  RgbColor* res = pct->buffer();
  int oo = pct->offset();
  for (int j = 0, p = 0, q = 0; j < h_; j++, p += offset_, q += oo)
    //for (int i = 0; i < w_; i++)
    //  res[q + i] = buf_[p + i];
    std::copy_n(buf_ + p, w_, res + q);

  return pct;
}


std::unique_ptr<urania::PaintMemDevice> urania::PaintMemDevice::clone() const
{
  auto res = create(w_, h_);
  if (!res)
    return nullptr;
  std::copy(buf_, buf_ + h_ * offset_, res->buf_);
  return res;
}




//eof
