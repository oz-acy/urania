/*
 * Copyright 2009-2021 oZ/acy (名賀月晃嗣)
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
 * @file drop-h.cpp
 * @author oZ/acy (名賀月晃嗣)
 * @brief ドロップファイルの處理
 *
 * @date 2016.2.28  修正
 * @date 2019.8.31  修正
 *
 */
#include "window.h"


/*===============================================================
 *  WMHandler::onDropFiles_()
 *  ドロップファイルの定型處理を行ひ、ユーザ實裝部を呼び出す
 */
void urania::WMHandler::onDropFiles_(urania::Window* win, WPARAM wp)
{
  HDROP hd = (HDROP)wp;
  POINT p;
  DragQueryPoint(hd, &p);

  std::vector<std::wstring> astr;
  wchar_t temp[MAX_PATH];
  int n = DragQueryFile(hd, 0xffffffff, nullptr, 0);

  for (int i = 0; i < n; i++) {
    DragQueryFile(hd, i, temp, MAX_PATH);
    astr.push_back(temp);
  }

  DragFinish(hd);

  // ユーザがサブクラスでオーバーライドするハンドラを呼び出し
  onDropFiles(win, astr, p.x, p.y);
}




//eof
