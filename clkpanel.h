/**************************************************************************//**
 *
 *  @file clkpanel.h
 *  @author oZ/acy (名賀月晃嗣)
 *
 *  @date 2016.2.28  修正
 *  @date 2019.8.30  修正
 */

#ifndef INCLUDE_GUARD_URANIA_CLKPANEL_H
#define INCLUDE_GUARD_URANIA_CLKPANEL_H

#include "bwin.h"


namespace urania {


class ClickPanel : public urania::BasicWindow
{
public:
  using PH_ = void (*)(urania::BasicWindow*, urania::PaintDevice*, void*);

private:
  HWND pw_;
  int id_;
  PH_ ph_;
  void* app_;

  ClickPanel() : ph_(nullptr), app_(nullptr) {}

  LRESULT wproc_(UINT msg, WPARAM wp, LPARAM lp) override;

  void init_(HWND hw) override
  {
    bindHWND_(hw);
  }

  void uninit_() override
  {
    unbindHWND_();
  }


public:
  ~ClickPanel() {}

  /// クリックパネルを生成する。
  /// @param x パネルの位置(X座標)
  /// @param y パネルの位置(Y座標)
  /// @param w パネルの幅
  /// @param h パネルの高さ
  /// @param ph 描畫ハンドラ
  /// @param ap ハンドラ用データ
  /// @param par 親Window
  /// @param id パネルの子WindowID
  static
  std::unique_ptr<ClickPanel>
  create(
    int x, int y, int w, int h, PH_ ph, void* ap, WndBase* par, int id);

  ///  描畫ハンドラを呼び出す。
  void paint(BasicWindow* w, PaintDevice* pdev)
  {
    if (ph_)
      ph_(w, pdev, app_);
  }
};


}// end of namespace urania


#endif // INCLUDE_GUARD_URANIA_CLKPANEL_H
