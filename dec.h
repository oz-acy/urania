/*******************************************************************************
 *  dec.h
 *
 *  by oZ/acy
 *  last update : 13 May 2012
 *
 *  DEClaration classes  for part "URANIA"
 */
#ifndef INC_URANIA_DEC_H___
#define INC_URANIA_DEC_H___

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
    class FileDialogBase;
      class OpenFileDialog;
      class SaveFileDialog;

  class Menu;
  class MenuDesc;

  class PaintDevice;
  class PaintMemDevice;
  class PaintMemDevicePal;


  struct CtrlDesc
  {
    int x, y, w, h;
  };

  enum // window style
  {
    WST_NORMAL  = 0,
    WST_POPUP   = 1,        // 枠なしウィンドウ
    WST_BORDER  = 1<<1,     // 枠のみのウィンドウ
    WST_THICK   = 1<<2,     // サイズ変更可能ウィンドウ
    WST_VSCROLL = 1<<3,     // 縦スクロールバー付き
    WST_HSCROLL = 1<<4,     // 横スクロールバー付き
    WST_MAXBOX  = 1<<5,     // 最大化ボタン付き
    WST_MINBOX  = 1<<6,     // 最小化ボタン付き
    WST_DRAGANDDROP = 1<<7  // Drag&Drop受け付け
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

  enum //WC_ & WCIN_ 用定数定義  for Icon&Cursorリソース
  {
     DEFAULT_RC =0
  };

  enum //WC_ & WCIN_ 用定数定義  for BackGroundColor
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


#endif
//eof
