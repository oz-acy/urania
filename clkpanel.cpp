/*
 * Copyright 2000-2024 oZ/acy (名賀月晃嗣)
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
/*
 * @file clkpanel.cpp
 * @author oZ/acy (名賀月晃嗣)
 *
 */
#include <functional>
#include "clkpanel.h"


std::unique_ptr<urania::ClickPanel> urania::ClickPanel::create(
  int x, int y, int w, int h, ClickPanel::PH_ ph,
  const std::any& ap, urania::WndBase* par, int id)
{
  std::unique_ptr<ClickPanel> panel(new ClickPanel(ph, ap));
  if (!panel)
    return nullptr;

  D0_ de;
  de.title = L"";
  de.popup = true;
  de.x = x;
  de.y = y;
  de.w = w;
  de.h = h;

  panel->pw_ = getHWND(par);
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
