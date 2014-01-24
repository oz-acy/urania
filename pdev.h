/**************************************************************************
 *
 *  pdev.h
 *  by oZ/acy
 *  (C) 2002-2011 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Paint DEVice
 *  Windows DC 描畫用クラス
 *
 *  last update : 2011.9.8
 *
 *************************************************************************/
#ifndef INC_URANIA_PAINTDEVICE_H___
#define INC_URANIA_PAINTDEVICE_H___

#include <windows.h>
#include <themis/inttype.h>
#include <polymnia/picture.h>

namespace urania
{
  //-------------------------------
  //  Color
  //  Windows用BGRカラー構造体
  //-------------------------------
  class Color
  {
  public:
    themis::UByte b;
    themis::UByte g;
    themis::UByte r;

  public:
    Color() : r(0),g(0),b(0) {}
    Color(themis::UByte rr, themis::UByte gg, themis::UByte bb)
      : r(rr),g(gg),b(bb) {}
    Color(const polymnia::RgbColor& org) : r(org.r),g(org.g),b(org.b) {}

    explicit Color(COLORREF cr)
    {
      r = GetRValue(cr);
      g = GetGValue(cr);
      b = GetBValue(cr);
    }

    Color& operator=(const polymnia::RgbColor& org)
    {
      r = org.r;
      g = org.g;
      b = org.b;
      return *this;
    }

    COLORREF getColorref() const
    {
      return (themis::UDWord)r|((themis::UDWord)g<<8)|((themis::UDWord)b<<16);
    }

    operator polymnia::RgbColor() const { return polymnia::RgbColor(r,g,b); }
  };


  // class宣言
  class PaintDevice;
  class PaintMemDevice;
  class PaintMemDeviceIndexed;

}//end of namespace urania



/*=====================================================
 *  PaintMemDevice
 *  PaintDevice互換メモリ上仮想デバイス(24bit color)
 */
class urania::PaintMemDevice : public polymnia::ImageBuffer<urania::Color>
{
  friend class urania::PaintDevice;

private:
  typedef urania::PaintMemDevice* PP_;
  typedef urania::Color C_;

protected:
  HDC hdc_;
  HBITMAP oldbmp_;


protected:
  PaintMemDevice(unsigned ww, unsigned hh);

public:
  ~PaintMemDevice();

  static PP_ create(unsigned w, unsigned h) throw();
  static PP_ create(const polymnia::Picture* pct);

  polymnia::Picture* createPicture() const;
  PP_ clone() const;
};



/*===============================================================
 *  PaintMemDeviceIndexed
 *  PaintDevice互換メモリ上仮想デバイス(256 palette color)
 */
class urania::PaintMemDeviceIndexed
  : public polymnia::ImageBuffer<themis::UByte>
{
  friend class urania::PaintDevice;

private:
  typedef PaintMemDeviceIndexed* PP_;
  typedef urania::Color C_;

protected:
  HDC hdc_;
  C_ pal_[256];
  HPALETTE hpal_;
  HBITMAP oldbmp_;
  polymnia::RgbColor oldpal_[256];

  PaintMemDeviceIndexed(unsigned ww, unsigned hh);

 public:
  ~PaintMemDeviceIndexed();

  static PP_ create(unsigned w, unsigned h) throw();
  static PP_ create(const polymnia::PictureIndexed* pct);

  polymnia::PictureIndexed* createPicture() const;
  PP_ clone() const;

  C_& palette(int id) { return pal_[id]; }
  const C_& palette(int id) const { return pal_[id]; }
  C_* paletteBuffer() { return pal_; }
  const C_* paletteBuffer() const { return pal_; }

  void updatePalette();
};



/*================================================
 *  PaintDevice
 *  Windows デバイスコンテキストのラッパー
 */
class urania::PaintDevice : boost::noncopyable
{
public:
  typedef urania::PaintDevice* PP_;
  typedef urania::Color C_;
  typedef void (*DestProc)(HDC, void*);

protected:
  HDC hdc_;
  int width_, height_;

  HBRUSH obr_;
  HPEN   opn_;
  HFONT  oft_;
  int    obm_;

  void* app_;
  DestProc dst_;


protected:
  PaintDevice(HDC dc, DestProc dp, void* a, int w, int h);

public:
  ~PaintDevice();


  //=============================================================
  //  static create()
  //  Object生成
  //   引數:  HDC dc      : デバイスコンテキストハンドル
  //          DestProc dp : DC解放用コールバック(不要ならnullptr)
  //          void* a     : コールバックに渡すパラメータ
  //          unsigned w  : デバイス幅
  //          unsigned h  : デバイス高さ
  //   返値:  オブジェクトのスマートポインタ(RCPtr)
  //=============================================================
  static PP_ create(HDC dc, DestProc dp, void* a, int w, int h) throw();


  //========================================================
  //  clear()
  //  描畫領域の全塗り潰し(クリア)
  //   引數 : const Color_PD& col : 塗り潰し色
  //========================================================
  void clear(const C_& col);


  //====================================
  //  dotset()
  //  點を描畫
  //   引數  x,y : 座標
  //         col : 描畫色
  //====================================
  void dotset(int x, int y, const C_& col);


  //==================================================================
  //  line()
  //  直線を描畫
  //   引數  x1, y1, x2, y2 : 座標 (x1, y1)-(x2, y2)間に直線を引く
  //         col            : 描畫色
  //==================================================================
  void line(int x1, int y1, int x2, int y2, const C_& col);


  //=============================================================
  //  box()
  //  長方形を描畫
  //   引數  x1, y1, x2, y2 : 對角頂点の座標 (x1, y1), (x2, y2)
  //         col            : 描畫色
  //         f              : trueで塗り潰し、falseで枠線のみ
  //=============================================================
  void box(int x1, int y1, int x2, int y2, const C_& col, bool f =false);


  //===================================================
  //  ellipse()
  //  楕円を描畫
  //   引數  x,y : 中心座標
  //         a,b : x半径及びy半径
  //         col : 描畫色
  //         f   : trueで塗り潰し、falseで枠線のみ
  //===================================================
  void ellipse(int x, int y, int a, int b, const C_& col, bool f =false);


  //===================================================
  //  circle()
  //  円を描畫
  //   引數  x,y : 中心座標
  //         r   : 半径
  //         col : 描畫色
  //         f   : trueで塗り潰し、falseで枠線のみ
  //===================================================
  void circle(int x, int y, int r, const C_& col, bool f =false)
  {
    ellipse(x,y,r,r,col,f);
  }



  //===================================================
  //  text()
  //  テキストを描畫
  //   引數  x,y : 左上角座標
  //         str : 描畫する文字列
  //         col : 字の色
  //   返値  描畫長方形の右下角座標
  //===================================================
  polymnia::Point text(int x, int y, const std::wstring& str, const C_& col);


  //==============================================
  //  text()
  //  テキストを描畫 (幅指定バージョン)
  //   引數  x,y : 左上角座標
  //         w   : 幅
  //         str : 描畫する文字列
  //         col : 字の色
  //   返値  描畫長方形の右下角座標
  //==============================================
  polymnia::Point
  text(int x, int y, int w, const std::wstring& str, const C_& col);


  //==================================================================
  //  changeFont()
  //  フォントの変更
  //   引數  size : フォントサイズ
  //         fn   : フォント名(L""も可能、そのときはデフォルト)
  //         ro   : trueならロマーンタイプ(主にデフォルト時に影響)
  //         fx   : trueなら等幅フォント、falseならプロポーショナル
  //         bo   : trueならボールド体
  //         itl  : trueなら斜体
  //         ul   : trueなら下線付き
  //         sk   : trueなら打ち消し線付き
  //==================================================================
  void
  changeFont(
    int size, const std::wstring& fn, bool ro, bool fx,
    bool bo, bool itl, bool ul, bool sk);


  //===============================
  //  width(), height()
  //  描畫領域の幅と高さの取得
  //===============================
  int width() const { return width_; }
  int height() const { return height_; }


  //==============================================
  //  blt()  × 4
  //  描畫内容の轉送
  //
  //   引數  dx, dy  : 轉送對象左上角座標
  //         src     : 轉送元Object
  //         sx, sy  : 轉送元左上角座標
  //         w, h    : 轉送幅・高さ
  //         mask    : 轉送對象の轉送可能領域
  //==============================================
  void blt(
    int dx, int dy, const urania::PaintMemDevice* src, int sx, int sy,
    int w, int h);

  void blt(
    int dx, int dy, const urania::PaintMemDevice* src, int sx, int sy,
    int w, int h, const polymnia::Rect& mask);

  void blt(
    int dx, int dy, urania::PaintMemDeviceIndexed* src,
    int sx, int sy, int w, int h);

  void blt(
    int dx, int dy, urania::PaintMemDeviceIndexed* src,
    int sx, int sy, int w, int h, const polymnia::Rect& mask);


  //===================================================
  //  blt()  × 2
  //  描畫内容の轉送、但し擴大縮小附きで全領域同士
  //
  //   引数  src    : 転送元Object
  //===================================================
  void blt(const urania::PaintMemDevice* src);
  void blt(urania::PaintMemDeviceIndexed* src);


  //=============================================================
  //  static getSysColor()
  //  システムカラー取得
  //   引數 : int id : システムカラーのID (dec.hで定義)
  //   返値 : 指定したシステムカラーの色
  //=============================================================
  static C_ getSysColor(int id);


  //=======================================================================
  //  static setSysColor()
  //  システムカラー設定
  //   引數 : int id                   : システムカラーID (dec.hで定義)
  //          const urania::Color& col : 設定する色
  //=======================================================================
  static void setSysColor(int id, const C_& col);


private:
  void changeBrush__(const C_& col);
  void changePen__(const C_& col);
};



#endif
//eof
