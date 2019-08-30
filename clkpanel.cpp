/**************************************************************************
 *
 *  clkpanel.cpp
 *  by oZ/acy (名賀月晃嗣)
 *
 */
#include <functional>
#include "clkpanel.h"


std::unique_ptr<urania::ClickPanel> urania::ClickPanel::create(
  int x, int y, int w, int h, ClickPanel::PH_ ph, void* ap,
  urania::WndBase* par, int id)
{
   std::unique_ptr<ClickPanel> panel(new ClickPanel);
   if (!panel)
     return nullptr;

   panel->ph_ = ph;
   panel->app_ = ap;

   D0_ de;
   de.title = L"";
   de.popup = true;
   de.x = x;
   de.y = y;
   de.w = w;
   de.h = h;

   panel->pw_ = getHW_(par);
   de.pwnd = panel->pw_;
   panel->id_ = id;
   de.hm = reinterpret_cast<HMENU>(id);
   panel->createWindow0_(de);

   return panel;
}


LRESULT urania::ClickPanel::wproc_(UINT msg, WPARAM wp, LPARAM lp)
{
  switch(msg)
  {
  case WM_PAINT:
    onPaint(
      [this](BasicWindow* bw, PaintDevice* pd){ this->paint(bw, pd); },
      wp, lp);
    return 0;

  case WM_LBUTTONUP:
    PostMessage(pw_, WM_COMMAND, id_, (LPARAM)hw_);
    return 0;
  }

  return defHandler(msg, wp, lp);
}


//eof
