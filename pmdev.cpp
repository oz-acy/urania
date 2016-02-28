/**************************************************************************
 *
 *  pmdev.cpp
 *  by oZ/acy
 *  (c) 2002-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  class urania::PaintMemDevice の実装定義
 *
 *  履歴
 *    2016.2.27  修正
 **************************************************************************/

#include <algorithm>
#include "paintdev.h"

/*================================================
 *  PaintMemDevice::PaintMemDevice()
 *  DIBSection生成他初期化
 *==============================================*/
urania::PaintMemDevice::PaintMemDevice(unsigned w, unsigned h)
  : polymnia::ImageBuffer<urania::Color>(w, h, 0), hdc_(NULL), oldbmp_(NULL)
{
  using namespace std;

  int s = sizeof(Color);
  int oo = w_ * sizeof(Color);
  offset_ = (oo % (4 * s)) ? (oo / (4 * s) + 1) * 4  :  oo / s;

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

  if (!hBitmapNew || !buf_)
  {
    //初期化に失敗
    ReleaseDC(NULL, hTmpDC);
    return;
  }

  hdc_ = CreateCompatibleDC(hTmpDC);
  ReleaseDC(NULL, hTmpDC);

  oldbmp_ = (HBITMAP)SelectObject(hdc_, hBitmapNew);

  //メモリ領域のクリア
  memset(buf_, 0, sizeof(polymnia::RgbColor) * offset_ * h_);
}


/*===================================================
 *  PaintMemDevice::~PaintMemDevice()
 *  割り当てデバイスの解放
 *=================================================*/
urania::PaintMemDevice::~PaintMemDevice()
{
  HBITMAP hbmp;
  if (hdc_ && oldbmp_)
  {
    //hdcが確保されている場合だけ開放する
    hbmp = (HBITMAP)SelectObject(hdc_, oldbmp_);
    DeleteObject(hbmp);
    DeleteDC(hdc_);
  }
}


/*===============================================================
 *  PaintMemDevice::create()
 *  Object生成
 *  引數:  unsigned w : デバイス幅
 *         unsigned h : デバイス高さ
 *  返値:  生成したオブジェクトへのポインタ
 *=============================================================*/
urania::PaintMemDevice* urania::PaintMemDevice::create(
  unsigned w, unsigned h) throw()
{
  try
  {
    PaintMemDevice* dv = new PaintMemDevice(w, h);
    if (dv->buf_)
      return dv;
    else
      return nullptr;
  }
  catch (std::bad_alloc)
  {
    return nullptr;
  }
}


/*=========================================================
 *  PaintMemDevice::create()
 *  Object生成 (既存 Picture Object の "複製")
 *  引數: Picture* pct : コピー元 Picture
 *  返値: オブジェクトへのポインタ
 *=======================================================*/
urania::PaintMemDevice*
urania::PaintMemDevice::create(const polymnia::Picture* pct)
{
  int ww = pct->width();
  int hh = pct->height();

  PaintMemDevice* vd = create(ww, hh);
  if (!vd)
    return nullptr;

  const polymnia::RgbColor* src = pct->buffer();
  Color* res = vd->buffer();

  int p = 0;
  int q = 0;
  int o = pct->offset();
  int oo = vd->offset_;
  for (int j = 0; j < hh; j++, p += o, q += oo)
    for (int i = 0; i < ww; i++)
      res[q + i] = src[p + i];

  return vd;
}


/*====================================================================
 *  PaintMemDevice::createPicture()
 *  同内容の Picture インスタンスの生成
 *  返値: 生成されたインスタンスへのポインタ
 *==================================================================*/
polymnia::Picture* urania::PaintMemDevice::createPicture() const
{
  using namespace polymnia;

  Picture* pct = Picture::create(w_, h_);
  if (!pct)
    return nullptr;

  RgbColor* res = pct->buffer();

  int p = 0;
  int q = 0;
  int oo = pct->offset();
  for (int j = 0; j < h_; j++, p += offset_, q += oo)
    for (int i = 0; i < w_; i++)
      res[q + i] = buf_[p + i];

  return pct;
}


/*=====================================================
 *  PaintMemDevice::clone()
 *  同内容の Object の生成
 *  返値: 生成されたオブジェクトへのポインタ
 *===================================================*/
urania::PaintMemDevice* urania::PaintMemDevice::clone() const
{
  using namespace std;

  PaintMemDevice* res = create(w_, h_);
  if (!res)
    return nullptr;

  copy(buf_, buf_ + h_ * offset_, res->buf_);
  return res;
}




//eof
