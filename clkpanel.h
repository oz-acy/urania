/**************************************************************************
 *
 *  clkpanel.h
 *  by oZ/acy
 *  (c) 2000-2016 oZ/acy.  All Rights Reserved.
 *
 *  履歴
 *    2016.2.28  修正
 *
 *************************************************************************/

#ifndef INC_URANIA_CLKPANEL_H___
#define INC_URANIA_CLKPANEL_H___

#include "bwin.h"

namespace urania {


class ClickPanel : public urania::BasicWindow
{
public:
  typedef ClickPanel* PCP_;
  typedef void (*PH_)(urania::BasicWindow*, urania::PaintDevice*, void*);

private:
  HWND pw_;
  int id_;
  PH_ ph_;
  void* app_;

  ClickPanel() : ph_(nullptr), app_(nullptr) {}

  LRESULT wproc__(UINT msg, WPARAM wp, LPARAM lp);

  void init__(HWND hw)
  {
    bindHWND__(hw);
  }

  void uninit__()
  {
    unbindHWND__();
  }


public:
  ~ClickPanel() {}


  //================================================================
  //  static create()
  //  Canvas Object を生成
  //  引數  x,y,w,h : パネル位置と幅と高さ
  //        ph      : 描畫ハンドラ
  //        ap      : ハンドラ用データ
  //        par     : 親 Window
  //        id      : パネルの子WindowID
  //================================================================
  static ClickPanel* create(
    int x, int y, int w, int h, PH_ ph, void* ap, WndBase* par, int id);

  //=====================================
  //  paint()
  //  描畫ハンドラを呼び出す
  //=====================================
  void paint(BasicWindow* w, PaintDevice* pdev)
  {
    if (ph_)
      ph_(w, pdev, app_);
  }
};


}// end of namespace urania


#endif // INC_URANIA_CLKPANEL_H___
