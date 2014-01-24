/**************************************************************************
 *
 *  pmdidx.cpp
 *  by oZ/acy
 *  (c) 2002-2011 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  class urania::PaintMemDeviceIndexed の實裝定義
 *
 *  last update: 30 Oct 2011
 *************************************************************************/

#include <cstring>
#include <algorithm>
#include "pdev.h"


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




/*==============================================================
 *  PaintMemDeviceIndexed::PaintMemDeviceIndexed()
 *  DIBSection,パレットハンドル生成他初期化
 *============================================================*/
urania::PaintMemDeviceIndexed::PaintMemDeviceIndexed(unsigned w, unsigned h)
  : polymnia::ImageBuffer<themis::UByte>(w, h, 0), hdc_(NULL), oldbmp_(NULL),
    hpal_(NULL)
{
  using namespace themis;
  using namespace polymnia;
  using namespace std;

  int oo = w_ * sizeof(UByte);
  offset_ = (oo%4) ? (oo/4+1)*4/sizeof(UByte) : oo/sizeof(UByte);

  HBITMAP hBitmapNew;
  HDC hTmpDC;
  int i;
  struct
  {
    BITMAPINFOHEADER Header;
    RGBQUAD ColorTable[256];
  }BmpInfo;


  //パレットの初期化
  for (i=0; i<256; i++)
  {
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
  for (i=0; i<256; i++)
  {
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

  if (!hBitmapNew || !buf_)
  {
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
  memset(buf_, 0, sizeof(UByte)*offset_*h_);
}


/*==============================================================
 *  PaintMemDeviceIndexed::~PaintMemDevicePal()
 *  割り当てハンドルの解放
 *============================================================*/
urania::PaintMemDeviceIndexed::~PaintMemDeviceIndexed()
{
  HBITMAP hbmp;

  if (hdc_ && oldbmp_)
  {
    // hdcが確保されている場合だけ開放する
    hbmp = (HBITMAP)SelectObject(hdc_, oldbmp_);
    DeleteObject(hbmp);
    DeleteDC(hdc_);
  }

  // パレットの解放
  if (hpal_)
    DeleteObject(hpal_);
}


/*============================================================
 *  PaintMemDeviceIndexed::updatePalette()
 *  パレットハンドルの更新
 *==========================================================*/
void urania::PaintMemDeviceIndexed::updatePalette()
{
  using namespace std;

  HPALETTE h_old,h_neu;

  h_old = hpal_;
  h_neu = createPalHandle_(pal_);

  if (!h_neu)
    return;
  hpal_ = h_neu;

  if (h_old)
    DeleteObject(h_old);

  RGBQUAD rgbq[256];
  for (int i=0; i<256; i++)
  {
    rgbq[i].rgbRed = pal_[i].r;
    rgbq[i].rgbGreen = pal_[i].g;
    rgbq[i].rgbBlue = pal_[i].b;
    rgbq[i].rgbReserved = 0;
  }

  SetDIBColorTable(hdc_, 0, 256, rgbq);
  copy(pal_, pal_ + 256, oldpal_);
}


/*======================================================
 *  PaintMemDeviceIndexed::create()
 *  Object生成
 *  引數: unsigned w : デバイス幅
 *        unsigned h : デバイス高さ
 *  返値: 生成されたオブジェクトへのポインタ
 *====================================================*/
urania::PaintMemDeviceIndexed* urania::PaintMemDeviceIndexed::create(
  unsigned w, unsigned h) throw()
{
  try
  {
    PaintMemDeviceIndexed* pvd = new PaintMemDeviceIndexed(w, h);

    if (pvd->buf_)
      return pvd;
    else
      return nullptr;
  }
  catch (std::bad_alloc)
  {
    return nullptr;
  }
}


/*===============================================================
 *  PaintMemDeviceIndexed::create()
 *  Object生成 (既存 PictureIndexed インスタンスの "複製")
 *  引數:  PicturePal* pct : コピー元 PictureIndexed
 *  返値:  オブジェクトのポインタ
 *=============================================================*/
urania::PaintMemDeviceIndexed*
urania::PaintMemDeviceIndexed::create(const polymnia::PictureIndexed* pct)
{
  using namespace themis;
  using namespace polymnia;
  using namespace std;

  int ww = pct->width();
  int hh = pct->height();

  PaintMemDeviceIndexed* vd = create(ww,hh);
  if (!vd)
    return nullptr;

  const UByte* src = pct->buffer();
  UByte* res = vd->buffer();

  int i;
  int p = 0;
  int q = 0;
  int o = pct->offset();
  int oo = vd->offset_;
  for (i=0; i<hh; i++, p+=o, q+=oo)
    copy(src + p, src + p + ww, res + q);

  const RgbColor* sp = pct->paletteBuffer();
  C_* rp = vd->paletteBuffer();
  copy(sp, sp + 256, rp);
  vd->updatePalette();

  return vd;
}


/*=====================================================
 *  PaintMemDeviceIndexed::createPicture()
 *  同内容の PictureIndexed インスタンスの生成
 *  返値: オブジェクトのポインタ
 *===================================================*/
polymnia::PictureIndexed*
urania::PaintMemDeviceIndexed::createPicture() const
{
  using namespace themis;
  using namespace polymnia;
  using namespace std;

  PictureIndexed* pct = PictureIndexed::create(w_, h_);
  if (!pct)
    return nullptr;

  UByte* res = pct->buffer();
  int i;
  int p=0;
  int q=0;
  int oo = pct->offset();
  for (i=0; i<h_; i++, p+=offset_, q+=oo)
    copy(buf_ + p, buf_ + p + w_, res + q);

  RgbColor* rp = pct->paletteBuffer();
  copy(pal_, pal_ + 256, rp);

  return pct;
}


/*===========================================
 *  PaintMemDeviceIndexed::clone()
 *  同内容の Object の生成
 *  返値: オブジェクトのポインタ
 *=========================================*/
urania::PaintMemDeviceIndexed* urania::PaintMemDeviceIndexed::clone() const
{
  using namespace std;

  PaintMemDeviceIndexed* res = create(w_, h_);
  if (!res)
    return nullptr;

  copy(buf_, buf_ + h_ * offset_, res->buf_);
  copy(pal_, pal_ + 256, res->pal_);
  return res;
}


//eof
