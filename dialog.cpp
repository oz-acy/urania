/**************************************************************************
 *
 *  dialog.cpp
 *  by oZ/acy
 *  (c) 2002-2014 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  DialogBox クラス實裝
 *
 *  last update : 21 Jan 2014
 *
 *************************************************************************/

#include "dialog.h"


/*====================================
 *  Dialog::doModal()
 *  ModalなDialogBoxの運用
 *==================================*/
int urania::Dialog::doModal(int rid, Ini_ ini, Ini_ ui, H_ hnd, void* app)
{
  Dialog* dlg = new Dialog(ini, ui, hnd, true, app);

  return
    ::DialogBoxParam(
      getHI__(), MAKEINTRESOURCE(rid), NULL, (DLGPROC)dlgproc__,
      reinterpret_cast<LPARAM>(dlg));
}


/*-----------------------------
 *  Dialog::doModeless()
 *  Modeless Dialogの生成
 *---------------------------*/
urania::Dialog* urania::Dialog::doModeless(
  int rid, Ini_ ini, Ini_ ui, H_ hnd, void* app)
{
  Dialog* dlg = new Dialog(ini, ui, hnd, false, app);

  ::CreateDialogParam(
    getHI__(), MAKEINTRESOURCE(rid), NULL, (DLGPROC)dlgproc__,
    reinterpret_cast<LPARAM>(dlg));

  return dlg;
}


/*----------------------------------
 *  doOwnedModal()
 *  オーナー付きModal Dialog実行
 *--------------------------------*/
int urania::Dialog::doOwnedModal(
  int rid, urania::WndBase* par, Ini_ ini, Ini_ ui, H_ hnd, void* app)
{
  Dialog* dlg = new Dialog(ini, ui, hnd, true, app);

  return
    ::DialogBoxParam(
      getHI__(), MAKEINTRESOURCE(rid), getHW__(par), (DLGPROC)dlgproc__,
      reinterpret_cast<LPARAM>(dlg));
}


/*-------------------------------------
 *  doOwnedModeless()
 *  オーナー付きModelessDialog生成
 *-----------------------------------*/
urania::Dialog* urania::Dialog::doOwnedModeless(
  int rid, urania::WndBase* par, Ini_ ini, Ini_ ui, H_ hnd, void* app)
{
  Dialog* dlg = new Dialog(ini, ui, hnd, false, app);

  ::CreateDialogParam(
    getHI__(), MAKEINTRESOURCE(rid), getHW__(par), (DLGPROC)dlgproc__,
    reinterpret_cast<LPARAM>(dlg));

  return dlg;
}



/*-------------------------------------------------
 *  Dialog::dlgproc__()
 *  Dialogと結合しているDialogBox用のDlgProc
 *-----------------------------------------------*/
BOOL CALLBACK urania::Dialog::dlgproc__(HWND hw, UINT msg, WPARAM wp, LPARAM lp)
{
  Dialog* dlg = (Dialog*)(GetWindowLongPtr(hw, DWLP_USER));

  if (!dlg)
  {
    if (msg==WM_INITDIALOG)
    {
      dlg = (Dialog*)lp;
      dlg->init__(hw);
      return TRUE;
    }
    else
      return FALSE;
  }

  if (msg==WM_DESTROY)
  {
    dlg->destroyed__();
    return TRUE;
  }
  else
    return dlg->dproc__(msg, wp, lp);
}


/*------------------------------------------
 *  Dialog::dproc__()
 *  各Dialogのメッセージ処理プロシージャ
 *----------------------------------------*/
BOOL urania::Dialog::dproc__(UINT msg, WPARAM wp, LPARAM lp)
{
  if (handler_)
    return (*handler_)(this, msg, wp, lp);
  else
    return FALSE;
}


/*-----------------------------
 *  Dialog::endModal()
 *  Modal DialogBoxの終了
 *---------------------------*/
void urania::Dialog::endModal(int i)
{
  if (modal_)
    EndDialog(hw_, i);
}


/*------------------------------
 *  Dialog::destroyWindow__()
 *  DialogBoxの破棄
 *----------------------------*/
void urania::Dialog::destroyWindow__()
{
   if (modal_)
     endModal(0);
   else
     kill__();
}



/*-------------------------------
 *  Dialog::init__()
 *  メッセージ処理系の初期化
 *-----------------------------*/
void urania::Dialog::init__(HWND hw)
{
  // HWNDとWindow objectの結合
   if (modal_)
     attach__(hw);
   else
     link__(hw);

   SetWindowLong(hw, DWLP_USER, reinterpret_cast<ULONG_PTR>(this));

  // initializerの呼び出し
   if (init_)
     init_(this);
}


/*-------------------------------
 *  Dialog::uninit__()
 *  メッセージ処理系の後始末
 *  destroyedとdeletingの下請
 *-----------------------------*/
void urania::Dialog::uninit__()
{
  // uninitializerの呼び出し
  if (uninit_)
    uninit_(this);

  // HWND側からのリンクを切断
  if (hw_)
    ::SetWindowLong(hw_, DWLP_USER, reinterpret_cast<ULONG_PTR>(nullptr));
}



//eof
