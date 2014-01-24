/**************************************************************************
 *
 *  canvas.h
 *  by oZ/acy
 *  (c) 2000-2012 oZ/acy, All Rights Reserved.
 *
 *  Canvas Panel for part URANIA
 *
 *  last update: 1 Mar 2012
 *************************************************************************/

#ifndef INC_URANIA_CANVAS_H_
#define INC_URANIA_CANVAS_H_

#include <memory>
#include <functional>
#include "bwin.h"
#include "csr.h"


namespace urania
{

/*-------------------------------------------------
 *  class CanvasBase<B_, Dev_>
 *
 *  B_   : 畫像バッファ
 *  Dev_ : 描畫デバイス
 *-----------------------------------------------*/
template<class B_, class Dev_>
class CanvasBase : public BasicWindow
{
public:
  enum
  {
    MODE_DOT  // 點描畫モード
  };

  typedef CanvasBase<B_, Dev_>* PCB_;
  typedef typename B_::ColorType C1_;
  typedef typename Dev_::ColorType C2_;
  typedef std::shared_ptr<B_> QB_;

  //typedef std::shared_ptr<Dev_> QD_;


private:
  QB_ buf_;
  int mode_;
  C1_ col_;
  bool isclk_;
  int last_x_,last_y_;

  CanvasBase() : mode_(MODE_DOT),isclk_(false) {}

  LRESULT wproc__(UINT msg,WPARAM wp,LPARAM lp);
  void init__(HWND hw) { bindHWND__(hw); }
  void uninit__() { unbindHWND__(); }

  void doLDown(int x,int y);
  void doLUp(int x,int y);
  void doMMove(int x,int y,bool lon);


public:
  ~CanvasBase() {}

  //================================================
  //  static create()
  //  Canvas Object を生成
  //  引數 gw,gh   : イメージの幅と高さ
  //       x,y,w,h : パネル位置と幅と高さ
  //       par     : 親 Window
  //       id      : パネルの子 Window ID
  //================================================
  static PCB_ create(
    int gw, int gh, int x, int y, int w, int h, WndBase* par, int id);


  //========================================
  //  getImage()
  //  内部イメージを晒す
  //========================================
  QB_ getImage() { return buf_; }


  //===================================================
  //  setDrawColor(), getDrawColor()
  //  描畫色の設定と取得
  //===================================================
  void setDrawColor(const C1_& col) { col_ = col; }
  C1_ getDrawColor() const { return col_; }


  //======================================
  //  setMode(),getMode()
  //  描畫モードの設定と取得
  //======================================
  void setMode(int m) { mode_ = m; }
  int getMode() const { return mode_; }

  /*==============================
  *  paint()
  *  パネル描畫
  *=============================*/
  void paint(urania::BasicWindow* w, urania::PaintDevice* pdev);
};


typedef CanvasBase<polymnia::Picture, urania::PaintMemDevice> CanvasPanel;
typedef
  CanvasBase<polymnia::PictureIndexed, urania::PaintMemDevicePal>
    CanvasPanelPal;


}// end of namespace urania


template<class B_, class Dev_>
urania::CanvasBase<B_, Dev_>* urania::CanvasBase<B_, Dev_>::create(
  int gw, int gh, int x, int y, int w, int h, urania::WndBase* par, int id)
{
  using namespace urania;

  QB_ buf(B_::create(gw, gh));
  if (!buf)
    return nullptr;

  PCB_ panel = new CanvasBase<B_, Dev_>;
  if (!panel)
    return nullptr;

  panel->buf_ = buf;

  D0_ de;
  de.title = L"";
  de.cursor = CV_CSR;
  de.popup = true;
  de.x = x;
  de.y = y;
  de.w = w;
  de.h = h;

  de.pwnd = getHW__(par);
  de.hm = reinterpret_cast<HMENU>(id);
  panel->createWindow0__(de);

  return panel;
}


template<class B_,class Dev_>
void urania::CanvasBase<B_,Dev_>::paint(
  urania::BasicWindow* w, urania::PaintDevice* pdev)
{
  (void)w;

  std::auto_ptr<Dev_> mdev(Dev_::create(buf_->width(), buf_->height()));
  if (!mdev.get())
    return;

  mdev->blt(0, 0, buf_.get(), 0, 0, buf_->width(), buf_->height());
  pdev->blt(mdev.get());
}


template<class B_,class Dev_>
LRESULT urania::CanvasBase<B_,Dev_>::wproc__(UINT msg,WPARAM wp,LPARAM lp)
{
  using namespace urania;
  using namespace std;
  using namespace std::placeholders;

  switch(msg)
  {
  case WM_PAINT:
    {
      onPaint(bind(&CanvasBase<B_, Dev_>::paint, this, _1, _2), wp, lp);
    }
    return 0;

  case WM_LBUTTONDOWN:
    doLDown((short)LOWORD(lp), (short)HIWORD(lp));
    return 0;

  case WM_LBUTTONUP:
    doLUp((short)LOWORD(lp), (short)HIWORD(lp));
    return 0;

  case WM_MOUSEMOVE:
    doMMove((short)LOWORD(lp), (short)HIWORD(lp), wp & MK_LBUTTON);
    return 0;
  }

  return defHandler(msg,wp,lp);
}


template<class B_,class Dev_>
void urania::CanvasBase<B_,Dev_>::doLDown(int x, int y)
{
  switch(mode_)
  {
  case MODE_DOT:
    isclk_ = true;
    last_x_ = x * buf_->width() / getWidth();
    last_y_ = y * buf_->height() / getHeight();
    buf_->pixel(last_x_, last_y_) = col_;
    SetCapture(hw_);
    invalidate();
    update();
    break;
  }
}


template<class B_,class Dev_>
void urania::CanvasBase<B_,Dev_>::doLUp(int x,int y)
{
  switch(mode_)
  {
  case MODE_DOT:
    ReleaseCapture();

    if (!isclk_)
      break;

    isclk_ = false;
    buf_->line(
      last_x_, last_y_,
      x * buf_->width() / getWidth(), y * buf_->height() / getHeight(),
      col_);
    invalidate();
    update();
    break;
  }
}


template<class B_,class Dev_>
void urania::CanvasBase<B_,Dev_>::doMMove(int x, int y, bool lon)
{
  int xx = x * buf_->width() / getWidth();
  int yy = y * buf_->height() / getHeight();

  switch(mode_)
  {
  case MODE_DOT:
    if (isclk_)
    {
      if (x < 0 || x >= getWidth() || y < 0 || y >= getHeight())
      {
        if (!lon)
        {
          doLUp(x,y);
          return;
        }
      }

      buf_->line(last_x_, last_y_, xx, yy, col_);
      last_x_ = xx;
      last_y_ = yy;

      invalidate();
      update();
    }
    else if(lon)
    {
      doLDown(x,y);
      return;
    }
  }
}


#endif // INC_URANIA_CANVAS_H_
