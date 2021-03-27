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
 * @file decl.h
 * @author oZ/acy (名賀月晃嗣)
 * @brief 各種宣言
 *
 * @date 2016.2.27  ファイル名變更
 * @date 2016.2.29  ファイルダイアログ關係の變更
 *
 */
#ifndef INCLUDE_GUARD_URANIA_DECL_H
#define INCLUDE_GUARD_URANIA_DECL_H

namespace urania
{
  class System;

  class WndBase;
    class BasicWindow;
      class Window;
//        class MDIFrame;
//        class MDIChild;
      class Dialog;

  class CommonDialogBase;
    class FileDialog;

  class Menu;
  class MenuDesc;

  class PaintDevice;
  class PaintMemDevice;
  class PaintMemDevicePal;


  /// @brief コントロールの位置、幅、高さを指定する構造體
  struct CtrlDesc
  {
    int x; ///< コントロールのx座標
    int y; ///< コントロールのy座標
    int w; ///< コントロールの幅
    int h; ///< コントロールの高さ
  };

  enum // window style
  {
    WST_NORMAL  = 0,
    WST_POPUP   = 1,        // 枠なしウィンドウ
    WST_BORDER  = 1 << 1,     // 枠のみのウィンドウ
    WST_THICK   = 1 << 2,     // サイズ変更可能ウィンドウ
    WST_VSCROLL = 1 << 3,     // 縦スクロールバー付き
    WST_HSCROLL = 1 << 4,     // 横スクロールバー付き
    WST_MAXBOX  = 1 << 5,     // 最大化ボタン付き
    WST_MINBOX  = 1 << 6,     // 最小化ボタン付き
    WST_DRAGANDDROP = 1 << 7  // Drag&Drop受け付け
  };

  enum //system color
  {
    SYSC_SCROLLBAR =0,        // スクロールバー灰色領域
    SYSC_BACKGROUND,          // デスクトップ
    SYSC_ACTIVECAPTION,       // アクティブウィンドウのキャプション
    SYSC_INACTIVECAPTION,     // 非アクティブウィンドウキャプション
    SYSC_MENU,                // メニュー背景
    SYSC_WINDOW,              // ウィンドウ背景
    SYSC_WINDOWFRAME,         // ウィンドウ枠
    SYSC_MENUTEXT,            // メニューのテキスト
    SYSC_WINDOWTEXT,          // ウィンドウのテキスト
    SYSC_CAPTIONTEXT,         // キャプションのテキスト
    SYSC_ACTIVEBORDER,        // アクティブウィンドウ境界
    SYSC_INACTIVEBORDER,      // 非アクティブウィンドウ境界
    SYSC_APPWORKSPACE,        // MDI背景
    SYSC_HIGHLIGHT,           // 選択項目背景
    SYSC_HIGHLIGHTTEXT,       // 選択項目文字
    SYSC_BTNFACE,             // ボタン表面の影
    SYSC_BTNSHADOW,           // ボタン縁の影
    SYSC_GRAYTEXT,            // 淡色テキスト
    SYSC_BTNTEXT,             // ボタンテキスト
    SYSC_INACTIVECAPTIONTEXT, // 非アクティブキャプションのテキスト
    SYSC_BTNHIGHLIGHT,        // 選択されたボタン
  };

/*廢用 Win32 API の DDL_hogehogeを直接用ゐるべし
  enum //ComboBox & ListBox   DIR OPTION
  {
    CTD_READWRITE = 0x0100, //読み書き両用
    CTD_READONRY = 0x0001,  //読み取り専用
    CTD_HIDDEN = 0x0002,    //隠しファイル
    CTD_SYSTEM = 0x0004,    //システムファイル
    CTD_DIRECTORY = 0x0010, //ディレクトリ
    CTD_ARCHIVE = 0x0020,   //アーカイブファイル
    CTD_DRIVE = 0x4000,     //ドライブ
    CTD_EXCLUSIVE = 0x8000  //指定タイプ以外を排除
  };
*/

  enum // WC_ & WCIN_ 用定数定義  for Icon&Cursorリソース
  {
     DEFAULT_RC =0
  };

  enum // WC_ & WCIN_ 用定数定義  for BackGroundColor
  {
     BG_WHITE =0,
     BG_BLACK,
     BG_GRAY,
     BG_LTGRAY,
     BG_DKGRAY,
     BG_APPWORKSPACE,
     BG_BKWINDOW
  };

  enum // Scroll Bar の 假想ID (下二つ以外は子ウィンドウIDで識別)
  {
    ID_SBV =0,
    ID_SBH =1
  };


}//end of namespace urania


#endif // INCLUDE_GUARD_URANIA_DECL_H
