/**************************************************************************
 *
 *  pdev.cpp
 *  by oZ/acy
 *  (C) 2002-2011 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  class urania::PaintDevice の実装定義その壱
 *   コンストラクタ、デストラクタ、システムカラー系、クリア及びBLT処理
 *
 *  last update : 8 Sep 2011
 *************************************************************************/

#include "pdev.h"
#include <cstring>


/*==========================================================
 *  PaintDevice::PaintDevice()
 *  DCの各オブジェクトの原状保存と新規割り当て
 */
urania::PaintDevice::PaintDevice(HDC dc, DestProc dp, void* ap, int w, int h)
  : hdc_(dc), dst_(dp), app_(ap), width_(w), height_(h)
{
  HBRUSH tb = CreateSolidBrush(RGB(255, 255, 255));
  HPEN tp = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
  HFONT tf
   = CreateFont(
       16, 0, 0, 0, 0, 0, 0, 0, SHIFTJIS_CHARSET, 0, 0, 0, FIXED_PITCH,
       nullptr);

  obr_ = (HBRUSH)SelectObject(hdc_, tb);
  opn_ = (HPEN)SelectObject(hdc_, tp);
  oft_ = (HFONT)SelectObject(hdc_, tf);
  obm_ = SetBkMode(hdc_, TRANSPARENT);
}


/*===============================================================
 *  PaintDevice::~PaintDevice()
 *  DCの各オブジェクトの原状復帰・DC解放コールバック呼び出し
 */
urania::PaintDevice::~PaintDevice()
{
  HBRUSH tb = (HBRUSH)SelectObject(hdc_, obr_);
  HPEN tp = (HPEN)SelectObject(hdc_, opn_);
  HFONT tf = (HFONT)SelectObject(hdc_, oft_);
  SetBkMode(hdc_, obm_);

  DeleteObject(tb);
  DeleteObject(tp);
  DeleteObject(tf);

  if (dst_)
    dst_(hdc_, app_);
}


/*===============================================================
 *  PaintDevice::create()
 *  Object生成
 *   引数:  HDC dc      : デバイスコンテキストハンドル
 *          DestProc dp : DC解放用コールバック
 *          void* a     : コールバックに渡すパラメータ
 *          unsigned w  : デバイス幅
 *          unsigned h  : デバイス高さ
 *   返値:  オブジェクトのスマートポインタ(Qointer)
 */
urania::PaintDevice* urania::PaintDevice::create(
  HDC dc, DestProc dp, void* a, int w, int h) throw()
{
  try
  {
    return new PaintDevice(dc, dp, a, w, h);
  }
  catch (std::bad_alloc)
  {
    return nullptr;
  }
}


/*====================================================================
 *  PaintDevice::getSysColor()
 *  システムカラー取得
 *   引数 : int id : システムカラーのID(gpGuiDef.hでenum定義)
 *   返値 : 指定したシステムカラーの色(BGR)
 */
urania::Color urania::PaintDevice::getSysColor(int id)
{
  COLORREF col = GetSysColor(id);
  return C_(GetRValue(col),GetGValue(col),GetBValue(col));
}


/*====================================================================
 *  PaintDevice::setSysColor()
 *  システムカラー設定
 *   引数 : int id        : システムカラーのID(gpGuiDef.hでenum定義)
 *          const C_& col : 設定する色(BGR)
 */
void urania::PaintDevice::setSysColor(int id, const C_& col)
{
  int i[1];
  COLORREF c[1];

  i[0] = id;
  c[0] = col.getColorref();

  SetSysColors(1, i, c);
}


/*=====================================================
 *  PaintDevice::clear()
 *  描画領域のクリア
 *   引数 : const C_& col : クリアするときの色
 */
void urania::PaintDevice::clear(const C_& col)
{
  HBRUSH br = CreateSolidBrush(col.getColorref());
  RECT rc = {0, 0, width_, height_};
  FillRect(hdc_, &rc, br);
  DeleteObject(br);
}


/*=====================================================
 *  PaintDevice::blt()  × 4
 *  描画内容の転送
 *
 *   引数 : dx,dy  : 転送先左上角座標
 *          src    : 転送元Object
 *          sx,sy  : 転送元左上角座標
 *          w,h    : 転送幅・高さ
 *          mask   : マスクする領域
 */
void
urania::PaintDevice::blt(
  int dx, int dy, const urania::PaintMemDevice* src, int sx, int sy,
  int w, int h, const polymnia::Rect& mask)
{
  polymnia::imp_::Clip_ clip(sx,sy,w,h,dx,dy,mask);
  if (clip)
    BitBlt(
      hdc_, clip.dx, clip.dy, clip.w, clip.h, src->hdc_, clip.sx, clip.sy,
      SRCCOPY);
}

void
urania::PaintDevice::blt(
  int dx, int dy, const urania::PaintMemDevice* src, int sx, int sy,
  int w, int h)
{
  polymnia::Rect rect(0, 0, width_, height_);
  blt(dx, dy, src, sx, sy, w, h, rect);
}

void
urania::PaintDevice::blt(
  int dx, int dy, urania::PaintMemDeviceIndexed* src, int sx, int sy,
  int w, int h, const polymnia::Rect& mask)
{
  polymnia::imp_::Clip_ clip(sx, sy, w, h, dx, dy, mask);
  if (clip)
  {
    src->updatePalette();
    BitBlt(
      hdc_, clip.dx, clip.dy, clip.w, clip.h, src->hdc_, clip.sx, clip.sy,
      SRCCOPY);
  }
}

void
urania::PaintDevice::blt(
  int dx, int dy, urania::PaintMemDeviceIndexed* src, int sx, int sy,
  int w, int h)
{
  polymnia::Rect rect(0, 0, width_, height_);
  blt(dx, dy, src, sx, sy, w, h, rect);
}


/*=====================================================
 *  PaintDevice::blt()  × 2
 *  描画内容の転送、但し拡大縮小付きで全領域同士
 *
 *   引数 : src    : 転送元Object
 */
void urania::PaintDevice::blt(const urania::PaintMemDevice* src)
{
  StretchBlt(
    hdc_, 0, 0, width_, height_, src->hdc_, 0, 0, src->width(), src->height(),
    SRCCOPY);
}

void urania::PaintDevice::blt(urania::PaintMemDeviceIndexed* src)
{
  src->updatePalette();
  StretchBlt(
    hdc_, 0, 0, width_, height_, src->hdc_, 0, 0, src->width(), src->height(),
    SRCCOPY);
}



//eof
