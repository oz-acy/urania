/**************************************************************************
 *
 *  pdev.cpp
 *  by oZ/acy
 *  (C) 2002-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  class urania::PaintDevice の実装定義その壱
 *   コンストラクタ、デストラクタ、システムカラー系、クリア及びBLT処理
 *
 *  履歴
 *    2016.03.02  修正
 *************************************************************************/

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
  try
  {
    return std::unique_ptr<PaintDevice>(new PaintDevice(dc, dp, a, w, h));
  }
  catch (std::bad_alloc&)
  {
    return nullptr;
  }
}


/*====================================================================
 *  PaintDevice::setSysColor()
 *  システムカラー設定
 *   引数 : int id  システムカラーのID(gpGuiDef.hでenum定義)
 *          const urania::Color& col  設定する色(BGR)
 */
void urania::PaintDevice::setSysColor(int id, const urania::Color& col)
{
  int i[1] = { id };
  COLORREF c[1] = { col.getColorref() };
  SetSysColors(1, i, c);
}


/*=====================================================
 *  PaintDevice::clear()
 *  描画領域のクリア
 *   引数 : const C_& col : クリアするときの色
 */
void urania::PaintDevice::clear(const urania::Color& col)
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
  polymnia::imp_::Clip_ clip(sx, sy, w, h, dx, dy, mask);
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
