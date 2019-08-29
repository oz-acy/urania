/**************************************************************************
 *
 *  pmdev.cpp
 *  by oZ/acy
 *
 *  class urania::PaintMemDevice の實裝
 *
 *  履歴
 *    2016.2.27  修正
 *    2016.3.2   修正
 *    2019.8.29  修正
 */
#include <algorithm>
#include "paintdev.h"


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

  if (!hBitmapNew || !buf_) {
    //初期化に失敗
    ReleaseDC(NULL, hTmpDC);
    return;
  }

  hdc_ = CreateCompatibleDC(hTmpDC);
  ReleaseDC(NULL, hTmpDC);

  oldbmp_ = (HBITMAP)SelectObject(hdc_, hBitmapNew);

  //メモリ領域のクリア
  memset(buf_, 0, sizeof(Color) * offset_ * h_);
}




urania::PaintMemDevice::~PaintMemDevice()
{
  HBITMAP hbmp;
  if (hdc_ && oldbmp_) {
    //hdcが確保されている場合だけ開放する
    hbmp = (HBITMAP)SelectObject(hdc_, oldbmp_);
    DeleteObject(hbmp);
    DeleteDC(hdc_);
  }
}




std::unique_ptr<urania::PaintMemDevice>
urania::PaintMemDevice::create(unsigned w, unsigned h)
{
  try
  {
    std::unique_ptr<PaintMemDevice> dv(new PaintMemDevice(w, h));
    if (dv->buf_)
      return dv;
    else
      return nullptr;
  }
  catch (std::bad_alloc&)
  {
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
    for (int i = 0; i < w_; i++)
      res[q + i] = buf_[p + i];

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
