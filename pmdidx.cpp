/**************************************************************************
 *
 *  pmdidx.cpp
 *  by oZ/acy (名賀月晃嗣)
 *
 *  class urania::PaintMemDeviceIndexed の實裝
 *
 *  履歴
 *    2016.3.2   修正
 *    2019.8.29  修正
 */
#include <cstring>
#include <algorithm>
#include "paintdev.h"


namespace
{


/*===================================================================
 *  createPalHandle_()
 *  パレットハンドルの生成
 *  引數 : col[] : パレットで指定する256色の配列
 *  返値 : パレットのハンドル(HPALETTE)
 *  備考 : 渡す配列が 256 個の要素を持たない場合は
 *         不正メモリアクセスが出ても責に任ぜず
 */
HPALETTE createPalHandle_(const urania::Color col[])
{
  int i;
  struct
  {
    WORD Version;
    WORD NumberOfEntries;
    PALETTEENTRY aEntries[256];
  } PAL = { 0x300, 256 };

  for (i=0; i<256; i++)
  {
    PAL.aEntries[i].peRed = col[i].r;
    PAL.aEntries[i].peGreen = col[i].g;
    PAL.aEntries[i].peBlue = col[i].b;
    PAL.aEntries[i].peFlags = PC_RESERVED;
  }

  return CreatePalette((LOGPALETTE*)&PAL);
}

}//end of namespace




urania::PaintMemDeviceIndexed::PaintMemDeviceIndexed(unsigned w, unsigned h)
  : polymnia::ImageBuffer<themis::UByte>(w, h, 0),
    hdc_(NULL), hpal_(NULL), oldbmp_(NULL)
{
  using namespace themis;
  using namespace polymnia;
  using namespace std;

  int oo = w_ * sizeof(UByte);
  offset_ = (oo % 4) ? (oo / 4 + 1) * 4 / sizeof(UByte) : oo / sizeof(UByte);

  HBITMAP hBitmapNew;
  HDC hTmpDC;
  struct
  {
    BITMAPINFOHEADER Header;
    RGBQUAD ColorTable[256];
  }BmpInfo;


  //パレットの初期化
  for (int i = 0; i < 256; i++) {
    pal_[i].r = (UByte)( ((i>>5) & 7)*255/7 );
    pal_[i].g = (UByte)( ((i>>2) & 7)*255/7 );
    pal_[i].b = (UByte)( (i & 3)*255/3 );
  }
  hpal_ = createPalHandle_(pal_);

  ////
  BmpInfo.Header.biSize = sizeof(BITMAPINFOHEADER);
  BmpInfo.Header.biWidth = offset_;
  BmpInfo.Header.biHeight = -h_;
  BmpInfo.Header.biPlanes = 1;
  BmpInfo.Header.biBitCount = 8;
  BmpInfo.Header.biCompression = BI_RGB;
  BmpInfo.Header.biSizeImage = 0;
  BmpInfo.Header.biXPelsPerMeter = 0;
  BmpInfo.Header.biYPelsPerMeter = 0;
  BmpInfo.Header.biClrUsed = 0;
  BmpInfo.Header.biClrImportant = 0;

  //パレットをカラーテーブルにコピー
  for (int i=0; i < 256; i++) {
    BmpInfo.ColorTable[i].rgbRed = pal_[i].r;
    BmpInfo.ColorTable[i].rgbGreen = pal_[i].g;
    BmpInfo.ColorTable[i].rgbBlue = pal_[i].b;
    BmpInfo.ColorTable[i].rgbReserved = 0;
  }

  hTmpDC = GetDC(NULL);
  if (!hTmpDC)
    return;

  hBitmapNew
   = CreateDIBSection(
       hTmpDC, (BITMAPINFO*)&BmpInfo, DIB_RGB_COLORS, (void**)(&buf_), NULL, 0);

  if (!hBitmapNew || !buf_) {
    //初期化に失敗  パレットを解放
    if (hpal_)
      DeleteObject(hpal_);
    hpal_ = NULL;
    ReleaseDC(NULL, hTmpDC);
    return;
  }

  hdc_ = CreateCompatibleDC(hTmpDC);
  ReleaseDC(NULL, hTmpDC);

  oldbmp_ = (HBITMAP)SelectObject(hdc_, hBitmapNew);

  //メモリ領域のクリア
  memset(buf_, 0, sizeof(UByte) * offset_ * h_);
}




urania::PaintMemDeviceIndexed::~PaintMemDeviceIndexed()
{
  HBITMAP hbmp;

  if (hdc_ && oldbmp_) {
    // hdcが確保されている場合だけ開放する
    hbmp = (HBITMAP)SelectObject(hdc_, oldbmp_);
    DeleteObject(hbmp);
    DeleteDC(hdc_);
  }

  // パレットの解放
  if (hpal_)
    DeleteObject(hpal_);
}




void urania::PaintMemDeviceIndexed::updatePalette()
{
  using namespace std;

  HPALETTE h_old, h_neu;

  h_old = hpal_;
  h_neu = createPalHandle_(pal_);

  if (!h_neu)
    return;
  hpal_ = h_neu;

  if (h_old)
    DeleteObject(h_old);

  RGBQUAD rgbq[256];
  for (int i = 0; i < 256; i++)
  {
    rgbq[i].rgbRed = pal_[i].r;
    rgbq[i].rgbGreen = pal_[i].g;
    rgbq[i].rgbBlue = pal_[i].b;
    rgbq[i].rgbReserved = 0;
  }

  SetDIBColorTable(hdc_, 0, 256, rgbq);
  copy(pal_, pal_ + 256, oldpal_);
}




std::unique_ptr<urania::PaintMemDeviceIndexed>
urania::PaintMemDeviceIndexed::create(unsigned w, unsigned h)
{
  try
  {
    std::unique_ptr<PaintMemDeviceIndexed> pvd(new PaintMemDeviceIndexed(w, h));
    if (pvd->buf_)
      return pvd;
    else
      return nullptr;
  }
  catch (std::bad_alloc&)
  {
    return nullptr;
  }
}




std::unique_ptr<urania::PaintMemDeviceIndexed>
urania::PaintMemDeviceIndexed::duplicate(const polymnia::PictureIndexed* pct)
{
  using namespace themis;
  using namespace polymnia;

  int ww = pct->width();
  int hh = pct->height();

  auto vd = create(ww,hh);
  if (!vd)
    return nullptr;

  const UByte* src = pct->buffer();
  UByte* res = vd->buffer();

  int o = pct->offset();
  int oo = vd->offset_;
  for (int i=0, p = 0, q = 0; i < hh; ++i, p += o, q += oo)
    std::copy(src + p, src + p + ww, res + q);

  const RgbColor* sp = pct->paletteBuffer();
  Color* rp = vd->paletteBuffer();
  std::copy(sp, sp + 256, rp);
  vd->updatePalette();

  return vd;
}




std::unique_ptr<polymnia::PictureIndexed>
urania::PaintMemDeviceIndexed::duplicatePictureIndexed() const
{
  using namespace themis;
  using namespace polymnia;

  auto pct = PictureIndexed::create(w_, h_);
  if (!pct)
    return nullptr;

  UByte* res = pct->buffer();
  int oo = pct->offset();
  for (int i = 0, p = 0, q = 0; i < h_; i++, p += offset_, q += oo)
    std::copy(buf_ + p, buf_ + p + w_, res + q);

  RgbColor* rp = pct->paletteBuffer();
  std::copy(pal_, pal_ + 256, rp);

  return pct;
}


std::unique_ptr<urania::PaintMemDeviceIndexed>
urania::PaintMemDeviceIndexed::clone() const
{
  auto res = create(w_, h_);
  if (!res)
    return nullptr;
  std::copy(buf_, buf_ + h_ * offset_, res->buf_);
  std::copy(pal_, pal_ + 256, res->pal_);
  return res;
}


//eof
