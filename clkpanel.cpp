/**************************************************************************
 *
 *  clkpanel.cpp
 *  by oZ/acy
 *  (c) 2000-2011 oZ/acy.  All Rights Reserved.
 *
 *  last update: 2011.9.8
 *
 *************************************************************************/

#include <functional>
#include "clkpanel.h"


/*================================================
 *  ClickPanel::create()
 *  クリックパネルを生成
 */
urania::ClickPanel* urania::ClickPanel::create(
  int x, int y, int w, int h, ClickPanel::PH_ ph, void* ap,
  urania::WndBase* par, int id)
{
   PCP_ panel = new ClickPanel;
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

   panel->pw_ = getHW__(par);
   de.pwnd = panel->pw_;
   panel->id_ = id;
   de.hm = (HMENU)id;
   panel->createWindow0__(de);

   return panel;
}


/*================================================
 *  ClickPanel::wproc__()
 *  WNDPROC呼び出し時に呼ばれるメンバ
 */
LRESULT urania::ClickPanel::wproc__(UINT msg, WPARAM wp, LPARAM lp)
{
  using namespace std;
  using namespace std::placeholders;

  switch(msg)
  {
  case WM_PAINT:
    onPaint(bind(&ClickPanel::paint, this, _1, _2), wp, lp);
    return 0;

  case WM_LBUTTONDOWN:
    PostMessage(pw_, WM_COMMAND, id_, (LPARAM)hw_);
    return 0;
  }

  return defHandler(msg, wp, lp);
}


//eof
