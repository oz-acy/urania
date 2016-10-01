/**************************************************************************
 *
 *  dialog.cpp
 *  by oZ/acy
 *  (c) 2002-2016 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  DialogBox クラス實裝
 *
 *  履歴
 *    2016.2.28  修正
 *    2016.10.1  修正
 */

#include <memory>
#include "dialog.h"

/**
 * @brief Modalなダイアログを作成
 * @param rid リソースID
 * @param ini イニシャライザ。nullptrも可。
 *            プロトタイプは void (\*ini)(urania::Dialog*);
 * @param ui アンイニシャライザ。nullptrも可。
 *             プロトタイプは void (\*ui)(urania::Dialog*);
 * @param hnd メッセージハンドラ。
 *         プロトタイプは BOOL (\*hnd)(urania::Dialog*, UINT, WPARAM, LPARAM);
 * @param app 任意のポインタ(void*)。
 *     設定しておくと getAppData() で參照できる。
 * @return ダイアログの返した整數値
 */
int urania::Dialog::doModal(int rid, Ini_ ini, Ini_ ui, H_ hnd, void* app)
{
  std::unique_ptr<Dialog> dlg(new Dialog(ini, ui, hnd, true, app));
  return
    ::DialogBoxParam(
      getHI__(), MAKEINTRESOURCE(rid), NULL, (DLGPROC)dlgproc__,
      (LPARAM)dlg.get());
}


/**
 * @brief Modelessなダイアログを作成
 * @param rid リソースID
 * @param ini イニシャライザ。nullptrも可。
 *            プロトタイプは void (\*ini)(urania::Dialog*);
 * @param ui アンイニシャライザ。nullptrも可。
 *             プロトタイプは void (\*ui)(urania::Dialog*);
 * @param hnd メッセージハンドラ。
 *         プロトタイプは BOOL (\*hnd)(urania::Dialog*, UINT, WPARAM, LPARAM);
 * @param app 任意のポインタ(void*)。
 *     設定しておくと getAppData() で參照できる。
 * @return 作成されたダイアログへのポインタ
 */
urania::Dialog* urania::Dialog::doModeless(
  int rid, Ini_ ini, Ini_ ui, H_ hnd, void* app)
{
  Dialog* dlg = new Dialog(ini, ui, hnd, false, app);

  ::CreateDialogParam(
    getHI__(), MAKEINTRESOURCE(rid), NULL, (DLGPROC)dlgproc__, (LPARAM)dlg);

  return dlg;
}


/**
 * @brief 所有者付きのModalなダイアログを作成
 * @param rid リソースID
 * @param par 所有者
 * @param ini イニシャライザ。nullptrも可。
 *            プロトタイプは void (\*ini)(urania::Dialog*);
 * @param ui アンイニシャライザ。nullptrも可。
 *             プロトタイプは void (\*ui)(urania::Dialog*);
 * @param hnd メッセージハンドラ。
 *         プロトタイプは BOOL (\*hnd)(urania::Dialog*, UINT, WPARAM, LPARAM);
 * @param app 任意のポインタ(void*)。
 *     設定しておくと getAppData() で參照できる。
 * @return ダイアログの返した整數値
 */
int urania::Dialog::doOwnedModal(
  int rid, urania::WndBase* par, Ini_ ini, Ini_ ui, H_ hnd, void* app)
{
  std::unique_ptr<Dialog> dlg(new Dialog(ini, ui, hnd, true, app));
  return
    ::DialogBoxParam(
      getHI__(), MAKEINTRESOURCE(rid), getHW__(par), (DLGPROC)dlgproc__,
      (LPARAM)dlg.get());
}


/**
 * @brief 所有者付きのModelessなダイアログを作成
 * @param rid リソースID
 * @param par 所有者
 * @param ini イニシャライザ。nullptrも可。
 *            プロトタイプは void (\*ini)(urania::Dialog*);
 * @param ui アンイニシャライザ。nullptrも可。
 *             プロトタイプは void (\*ui)(urania::Dialog*);
 * @param hnd メッセージハンドラ。
 *         プロトタイプは BOOL (\*hnd)(urania::Dialog*, UINT, WPARAM, LPARAM);
 * @param app 任意のポインタ(void*)。
 *     設定しておくと getAppData() で參照できる。
 * @return 作成されたダイアログへのポインタ
 */
urania::Dialog* urania::Dialog::doOwnedModeless(
  int rid, urania::WndBase* par, Ini_ ini, Ini_ ui, H_ hnd, void* app)
{
  Dialog* dlg = new Dialog(ini, ui, hnd, false, app);

  ::CreateDialogParam(
    getHI__(), MAKEINTRESOURCE(rid), getHW__(par), (DLGPROC)dlgproc__,
    (LPARAM)dlg);

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


/**
 * @brief Modalなダイアログを終了する
 * @param i doModal() あるいは doOwnedModal() に返す整數値
 */
void urania::Dialog::endModal(int i)
{
  if (modal_)
    EndDialog(hw_, i);
}


/*
 *  ダイアログを破棄する。Modalなダイアログの場合は作成元に0を返す。
 */
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

   SetWindowLongPtr(hw, DWLP_USER, (LONG_PTR)this);

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
    ::SetWindowLongPtr(hw_, DWLP_USER, (LONG_PTR)nullptr);
}




//eof
