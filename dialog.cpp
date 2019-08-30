/**************************************************************************
 *
 *  dialog.cpp
 *  by oZ/acy (名賀月晃嗣)
 *
 *  DialogBox クラス實裝
 *
 *  履歴
 *    2016.2.28  修正
 *    2016.10.1  修正
 *    2019.8.30  修正
 */
#include <memory>
#include "dialog.h"


int urania::Dialog::doModal(int rid, Ini_ ini, Ini_ ui, H_ hnd, void* app)
{
  std::unique_ptr<Dialog> dlg(new Dialog(ini, ui, hnd, true, app));
  return
    ::DialogBoxParam(
      getHI_(), MAKEINTRESOURCE(rid), NULL, (DLGPROC)dlgproc_,
      (LPARAM)dlg.get());
}


urania::Dialog* urania::Dialog::doModeless(
  int rid, Ini_ ini, Ini_ ui, H_ hnd, void* app)
{
  Dialog* dlg = new Dialog(ini, ui, hnd, false, app);

  ::CreateDialogParam(
    getHI_(), MAKEINTRESOURCE(rid), NULL, (DLGPROC)dlgproc_, (LPARAM)dlg);

  return dlg;
}


int urania::Dialog::doOwnedModal(
  int rid, urania::WndBase* par, Ini_ ini, Ini_ ui, H_ hnd, void* app)
{
  std::unique_ptr<Dialog> dlg(new Dialog(ini, ui, hnd, true, app));
  return
    ::DialogBoxParam(
      getHI_(), MAKEINTRESOURCE(rid), getHW_(par), (DLGPROC)dlgproc_,
      (LPARAM)dlg.get());
}


urania::Dialog* urania::Dialog::doOwnedModeless(
  int rid, urania::WndBase* par, Ini_ ini, Ini_ ui, H_ hnd, void* app)
{
  Dialog* dlg = new Dialog(ini, ui, hnd, false, app);

  ::CreateDialogParam(
    getHI_(), MAKEINTRESOURCE(rid), getHW_(par), (DLGPROC)dlgproc_,
    (LPARAM)dlg);

  return dlg;
}




BOOL CALLBACK urania::Dialog::dlgproc_(HWND hw, UINT msg, WPARAM wp, LPARAM lp)
{
  Dialog* dlg = (Dialog*)(GetWindowLongPtr(hw, DWLP_USER));

  if (!dlg)
  {
    if (msg == WM_INITDIALOG)
    {
      dlg = (Dialog*)lp;
      dlg->init_(hw);
      return TRUE;
    }
    else
      return FALSE;
  }

  if (msg == WM_DESTROY)
  {
    dlg->destroyed_();
    return TRUE;
  }
  else
    return dlg->dproc_(msg, wp, lp);
}




BOOL urania::Dialog::dproc_(UINT msg, WPARAM wp, LPARAM lp)
{
  if (handler_)
    return (*handler_)(this, msg, wp, lp);
  else
    return FALSE;
}


void urania::Dialog::endModal(int i)
{
  if (modal_)
    EndDialog(hw_, i);
}


void urania::Dialog::destroyWindow_()
{
  // ダイアログを破棄する。Modalなダイアログの場合は作成元に0を返す。
  if (modal_)
    endModal(0);
  else
    kill_();
}


void urania::Dialog::init_(HWND hw)
{
  // メッセージ處理系の初期化

  // HWNDとWindow objectの結合
  if (modal_)
    attach_(hw);
  else
    link_(hw);

  SetWindowLongPtr(hw, DWLP_USER, (LONG_PTR)this);

  // initializerの呼び出し
  if (ini_)
    ini_(this);
}


void urania::Dialog::uninit_()
{
  // メッセージ處理系の後始末

  // uninitializerの呼び出し
  if (uini_)
    uini_(this);

  // HWND側からのリンクを切断
  if (hw_)
    ::SetWindowLongPtr(hw_, DWLP_USER, (LONG_PTR)nullptr);
}




//eof
