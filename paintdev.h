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
 * @file paintdev.h
 * @author oZ/acy (名賀月晃嗣)
 * @brief Windows DC 描畫用クラス
 *
 * @date 2016.3.2   修正
 * @date 2019.8.29  修正
 * @date 2021.3.23
 *   PaintDevice::blt()の仕樣を一部變更。PaintDevice::stretchBlt()を追加。
 *
 */
#ifndef INCLUDE_GUARD_URANIA_PAINTDEVICE_H
#define INCLUDE_GUARD_URANIA_PAINTDEVICE_H

#include <windows.h>
#include <polymnia/picture.h>

namespace urania
{
  ///  Windows用BGRカラー構造體
  class Color
  {
  public:
    std::uint8_t b;
    std::uint8_t g;
    std::uint8_t r;

  public:
    constexpr Color() : b(0), g(0), r(0) {}
    constexpr Color(std::uint8_t rr, std::uint8_t gg, std::uint8_t bb)
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
        (std::uint8_t)r | ((std::uint8_t)g << 8) | ((std::uint8_t)b << 16);
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



/**
 *  PaintDevice互換メモリ上假想デバイス(24bit color)
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

  /// PaintMemDeviceを生成する。
  /// @param w 幅
  /// @param h 高さ
  /// @return 生成したオブジェクトを保持するunique_ptr
  static std::unique_ptr<PaintMemDevice> create(unsigned w, unsigned h);

  /// Pictureを複製したPaintMemDeviceを生成する。
  /// @param pct 複製元Picture
  /// @return 生成したオブジェクトを保持するunique_ptr
  static std::unique_ptr<PaintMemDevice>
  duplicate(const polymnia::Picture* pct);

  /// 内容を複製したPictureを生成する。
  std::unique_ptr<polymnia::Picture> duplicatePicture() const;

  /// 複製する。
  std::unique_ptr<PaintMemDevice> clone() const;
};



/**
 *  PaintDevice互換メモリ上假想デバイス(256 palette color)
 */
class urania::PaintMemDeviceIndexed
  : public polymnia::ImageBuffer<std::uint8_t>
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

  /// PaintMemDeviceIndexedを生成する。
  /// @param w 幅
  /// @param h 高さ
  /// @return 生成したオブジェクトを保持するunique_ptr
  static std::unique_ptr<PaintMemDeviceIndexed> create(unsigned w, unsigned h);

  /// PictureIndexedを複製したPaintMemDeviceIndexedを生成する。
  /// @param pct 複製元Picture
  /// @return 生成したオブジェクトを保持するunique_ptr
  static std::unique_ptr<PaintMemDeviceIndexed>
  duplicate(const polymnia::PictureIndexed* pct);

  /// 内容を複製したPictureIndexedを生成する。
  std::unique_ptr<polymnia::PictureIndexed> duplicatePictureIndexed() const;

  /// 複製する。
  std::unique_ptr<PaintMemDeviceIndexed> clone() const;

  urania::Color& palette(int id) { return pal_[id]; }
  const urania::Color& palette(int id) const { return pal_[id]; }
  urania::Color* paletteBuffer() { return pal_; }
  const urania::Color* paletteBuffer() const { return pal_; }

  /// パレットハンドルを更新する。
  void updatePalette();
};



/**
 *  Windowsデバイスコンテキストのラッパー
 */
class urania::PaintDevice : themis::Noncopyable<urania::PaintDevice>
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


  /// オブジェクトを生成する。
  /// @param dc デバイスコンテキストハンドル
  /// @param dp DC解放用コールバック。不要ならnullptr。
  /// @param a  dpに渡すパラメタ
  /// @param w  デバイスの幅
  /// @param h  デバイスの高さ
  /// @return オブジェクトを保持するunique_ptr
  static
  // urania::PaintDevice*
  std::unique_ptr<PaintDevice>
  create(HDC dc, DestProc dp, void* a, int w, int h);


  /// 描画領域全體を塗り潰す。
  /// @param col 塗り潰し色
  void clear(const urania::Color& col);

  /// 點を描畫する。
  /// @param x 描畫位置(X座標)
  /// @param y 描畫位置(Y座標)
  /// @param col  描畫色
  void dotset(int x, int y, const urania::Color& col);

  /// 線分を描畫する。
  /// @param x1 線分の一方の端のX座標
  /// @param y1 線分の一方の端のY座標
  /// @param x2 線分のもう一方の端のX座標
  /// @param y2 線分のもう一方の端のY座標
  /// @param col 描畫色
  void line(int x1, int y1, int x2, int y2, const urania::Color& col);

  /// 長方形を描畫する。
  /// @param x1 長方形の一の頂點のX座標
  /// @param y1 長方形の一の頂點のY座標
  /// @param x2 長方形の(x1, y1)の對角のX座標
  /// @param y2 長方形の(x1, y1)の對角のY座標
  /// @param col 描畫色
  /// @param f trueなら中を塗り潰し、falseなら枠線のみ描畫する。
  void box(
    int x1, int y1, int x2, int y2, const urania::Color& col, bool f =false);

  /// 楕圓を描畫する。
  /// @param x    中心のX座標
  /// @param y    中心のY座標
  /// @param a    X半徑
  /// @param b    Y半徑
  /// @param col  描畫色
  /// @param f    trueなら中を塗り潰し、falseなら圓弧のみ描畫する。
  void ellipse(
    int x, int y, int a, int b, const urania::Color& col, bool f =false);

  /// 圓を描畫する。
  /// @param x    中心のX座標
  /// @param y    中心のY座標
  /// @param r    半徑
  /// @param col  描畫色
  /// @param f    trueなら中を塗り潰し、falseなら圓弧のみ描畫する。
  void circle(int x, int y, int r, const urania::Color& col, bool f =false)
  {
    ellipse(x, y, r, r, col, f);
  }

  /// テキストを描畫する。
  /// @param x 描畫長方形の左上角のX座標
  /// @param y 描畫長方形の左上角のY座標
  /// @param str 描畫する文字列
  /// @param col 描畫色
  /// @return 描畫長方形の右下角の座標
  polymnia::Point
  text(int x, int y, const std::wstring& str, const urania::Color& col);

  /// 幅を指定してテキストを描畫する。
  /// @param x 描畫長方形の左上角のX座標
  /// @param y 描畫長方形の左上角のY座標
  /// @param w 幅
  /// @param str 描畫する文字列
  /// @param col 描畫色
  /// @return 描畫長方形の右下角の座標
  polymnia::Point
  text(int x, int y, int w, const std::wstring& str, const urania::Color& col);

  /// フォントを變更する。
  /// @param size フォントサイズ
  /// @param fn フォント名。L""を指定すると、デフォルトを用ゐる。
  /// @param ro trueのときはローマンタイプ。主にデフォルトの時に影響する。
  /// @param fx trueのときは等幅フォント、falseならプロポーショナルフォント。
  /// @param bo trueのときはボールド體
  /// @param itl trueのときは斜體
  /// @param ul trueのときは下線附
  /// @param sk trueのときは打ち消し線附
  void
  changeFont(
    int size, const std::wstring& fn, bool ro, bool fx,
    bool bo, bool itl, bool ul, bool sk);


  /// 描畫領域の幅を取得する。
  int width() const { return width_; }

  /// 描畫領域の高さを取得する。
  int height() const { return height_; }


  /// @brief 轉送
  /// @param dx 轉送對象左上角のX座標
  /// @param dy 轉送對象左上角のX座標
  /// @param src 轉送元
  /// @param sx 轉送元左上角のX座標
  /// @param sy 轉送元左上角のY座標
  /// @param w 轉送幅
  /// @param h 轉送高さ
  void blt(
    int dx, int dy, const urania::PaintMemDevice* src, int sx, int sy,
    int w, int h);

  /// @brief 轉送
  /// @param dx 轉送對象左上角のX座標
  /// @param dy 轉送對象左上角のX座標
  /// @param src 轉送元
  /// @param sx 轉送元左上角のX座標
  /// @param sy 轉送元左上角のY座標
  /// @param w 轉送幅
  /// @param h 轉送高さ
  /// @param mask 被轉送可能領域
  void blt(
    int dx, int dy, const urania::PaintMemDevice* src, int sx, int sy,
    int w, int h, const polymnia::Rect& mask);

  /// @brief 轉送
  /// @param dx 轉送對象左上角のX座標
  /// @param dy 轉送對象左上角のX座標
  /// @param src 轉送元
  /// @param sx 轉送元左上角のX座標
  /// @param sy 轉送元左上角のY座標
  /// @param w 轉送幅
  /// @param h 轉送高さ
  void blt(
    int dx, int dy, urania::PaintMemDeviceIndexed* src,
    int sx, int sy, int w, int h);

  /// @brief 轉送
  /// @param dx 轉送對象左上角のX座標
  /// @param dy 轉送對象左上角のX座標
  /// @param src 轉送元
  /// @param sx 轉送元左上角のX座標
  /// @param sy 轉送元左上角のY座標
  /// @param w 轉送幅
  /// @param h 轉送高さ
  /// @param mask 被轉送可能領域
  void blt(
    int dx, int dy, urania::PaintMemDeviceIndexed* src,
    int sx, int sy, int w, int h, const polymnia::Rect& mask);

  /// @brief 轉送
  ///
  /// 轉送元の全領域から自己の全領域に轉送する。
  /// 領域の大きさが異なる場合は擴大縮小される。
  /// @param src 轉送元
  void stretchBlt(const urania::PaintMemDevice* src);

  /// @brief 轉送
  ///
  /// 轉送元の全領域から自己の全領域に轉送する。
  /// 領域の大きさが異なる場合は擴大縮小される。
  /// @param src 轉送元
  void stretchBlt(urania::PaintMemDeviceIndexed* src);

  /// @brief 轉送
  ///
  /// 轉送元の全領域から自己の全領域に轉送する。
  /// 領域の大きさが異なる場合はアスペクト比を保存しつつ擴大縮小される。
  /// 轉送元と自己のアスペクト比が異なる場合、餘白が生じる。
  /// @param src 轉送元
  void blt(const urania::PaintMemDevice* src);

  /// @brief 轉送
  ///
  /// 轉送元の全領域から自己の全領域に轉送する。
  /// 領域の大きさが異なる場合はアスペクト比を保存しつつ擴大縮小される。
  /// 轉送元と自己のアスペクト比が異なる場合、餘白が生じる。
  /// @param src 轉送元
  void blt(urania::PaintMemDeviceIndexed* src);


  /// システムカラーを取得する。
  /// @param id システムカラーID。<urania/decl.h>に定義する。
  /// @return 設定されてゐる色
  static urania::Color getSysColor(int id)
  {
    return Color(::GetSysColor(id));
  }

  /// システムカラーを設定する。
  /// @param id システムカラーID。<urania/decl.h>に定義する。
  /// @param col 設定する色
  static void setSysColor(int id, const urania::Color& col);


private:
  void changeBrush_(const urania::Color& col);
  void changePen_(const urania::Color& col);
};



#endif // INCLUDE_GUARD_URANIA_PAINTDEVICE_H
