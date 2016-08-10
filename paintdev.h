/**************************************************************************
 *
 *  paintdev.h
 *  by oZ/acy
 *  (C) 2002-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  Windows DC 描畫用クラス
 *
 *  履歴
 *    2016.03.02  修正
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
    constexpr Color() : b(0), g(0), r(0) {}
    constexpr Color(themis::UByte rr, themis::UByte gg, themis::UByte bb)
      : b(bb), g(gg), r(rr) {}
    constexpr Color(const polymnia::RgbColor& org)
      : b(org.b), g(org.g), r(org.r) {}

    constexpr explicit Color(COLORREF cr)
      : b(GetBValue(cr)), g(GetGValue(cr)), r(GetRValue(cr)) {}

    Color& operator=(const polymnia::RgbColor& org)
    {
      r = org.r;
      g = org.g;
      b = org.b;
      return *this;
    }

    constexpr COLORREF getColorref() const
    {
      return
        (themis::UDWord)r
        | ((themis::UDWord)g << 8)
        | ((themis::UDWord)b << 16);
    }

    constexpr operator polymnia::RgbColor() const
    {
      return polymnia::RgbColor(r, g, b);
    }
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

protected:
  HDC hdc_;
  HBITMAP oldbmp_;


protected:
  PaintMemDevice(unsigned ww, unsigned hh);

public:
  ~PaintMemDevice();

  static urania::PaintMemDevice* create(unsigned w, unsigned h);
  static urania::PaintMemDevice* create(const polymnia::Picture* pct);

  polymnia::Picture* createPicture() const;
  urania::PaintMemDevice* clone() const;
};



/*===============================================================
 *  PaintMemDeviceIndexed
 *  PaintDevice互換メモリ上仮想デバイス(256 palette color)
 */
class urania::PaintMemDeviceIndexed
  : public polymnia::ImageBuffer<themis::UByte>
{
  friend class urania::PaintDevice;

protected:
  HDC hdc_;
  urania::Color pal_[256];
  HPALETTE hpal_;
  HBITMAP oldbmp_;
  polymnia::RgbColor oldpal_[256];

  PaintMemDeviceIndexed(unsigned ww, unsigned hh);

 public:
  ~PaintMemDeviceIndexed();

  static PaintMemDeviceIndexed* create(unsigned w, unsigned h);
  static PaintMemDeviceIndexed* create(const polymnia::PictureIndexed* pct);

  polymnia::PictureIndexed* createPicture() const;
  PaintMemDeviceIndexed* clone() const;

  urania::Color& palette(int id) { return pal_[id]; }
  const urania::Color& palette(int id) const { return pal_[id]; }
  urania::Color* paletteBuffer() { return pal_; }
  const urania::Color* paletteBuffer() const { return pal_; }

  void updatePalette();
};



/*================================================
 *  PaintDevice
 *  Windows デバイスコンテキストのラッパー
 */
class urania::PaintDevice : boost::noncopyable
{
public:
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
  static urania::PaintDevice*
  create(HDC dc, DestProc dp, void* a, int w, int h);


  //========================================================
  //  clear()
  //  描畫領域の全塗り潰し(クリア)
  //   引數 : const Color_PD& col : 塗り潰し色
  //========================================================
  void clear(const urania::Color& col);


  //====================================
  //  dotset()
  //  點を描畫
  //   引數  x,y : 座標
  //         col : 描畫色
  //====================================
  void dotset(int x, int y, const urania::Color& col);


  //==================================================================
  //  line()
  //  直線を描畫
  //   引數  x1, y1, x2, y2 : 座標 (x1, y1)-(x2, y2)間に直線を引く
  //         col            : 描畫色
  //==================================================================
  void line(int x1, int y1, int x2, int y2, const urania::Color& col);


  //=============================================================
  //  box()
  //  長方形を描畫
  //   引數  x1, y1, x2, y2 : 對角頂点の座標 (x1, y1), (x2, y2)
  //         col            : 描畫色
  //         f              : trueで塗り潰し、falseで枠線のみ
  //=============================================================
  void box
  (int x1, int y1, int x2, int y2, const urania::Color& col, bool f =false);


  //===================================================
  //  ellipse()
  //  楕円を描畫
  //   引數  x,y : 中心座標
  //         a,b : x半径及びy半径
  //         col : 描畫色
  //         f   : trueで塗り潰し、falseで枠線のみ
  //===================================================
  void ellipse
  (int x, int y, int a, int b, const urania::Color& col, bool f =false);


  //===================================================
  //  circle()
  //  円を描畫
  //   引數  x,y : 中心座標
  //         r   : 半径
  //         col : 描畫色
  //         f   : trueで塗り潰し、falseで枠線のみ
  //===================================================
  void circle(int x, int y, int r, const urania::Color& col, bool f =false)
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
  polymnia::Point text
  (int x, int y, const std::wstring& str, const urania::Color& col);


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
  text(int x, int y, int w, const std::wstring& str, const urania::Color& col);


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
  //   引數 : int id : システムカラーのID (decl.hで定義)
  //   返値 : 指定したシステムカラーの色
  //=============================================================
  static urania::Color getSysColor(int id)
  {
    return Color(::GetSysColor(id));
  }


  //=======================================================================
  //  static setSysColor()
  //  システムカラー設定
  //   引數 : int id                   : システムカラーID (decl.hで定義)
  //          const urania::Color& col : 設定する色
  //=======================================================================
  static void setSysColor(int id, const urania::Color& col);


private:
  void changeBrush__(const urania::Color& col);
  void changePen__(const urania::Color& col);
};



#endif
//eof
