/**************************************************************************
 *
 *  ctrl.cpp
 *  by oZ/acy
 *  (c) 2002-2011 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  last update : 8 Sep MMXI
 *************************************************************************/
#include "wbase.h"


///////////////////////////////////
//  WndBase
//  Controls の生成メソッド
///////////////////////////////////
void urania::WndBase::createPushButton(
  int id, const std::wstring& str, const urania::CtrlDesc& de)
{
    CreateWindow(
      L"BUTTON", str.c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
      de.x, de.y, de.w, de.h, hw_, (HMENU)id, getHI__(), nullptr);
}

void urania::WndBase::createLabel(
  int id, const std::wstring& str, const urania::CtrlDesc& de)
{
   CreateWindow(
     L"STATIC", str.c_str(), WS_CHILD | WS_VISIBLE | SS_LEFT,
     de.x, de.y, de.w, de.h, hw_, (HMENU)id, getHI__(), nullptr);
}

void urania::WndBase::createCheckBox(
  int id, const std::wstring& str, const urania::CtrlDesc& de)
{
   CreateWindow(
     L"BUTTON", str.c_str(), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
     de.x, de.y, de.w, de.h, hw_, (HMENU)id, getHI__(), nullptr);
}

void urania::WndBase::createSEditBox(int id, const urania::CtrlDesc& de)
{
   CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
     de.x, de.y, de.w, de.h, hw_, (HMENU)id, getHI__(), nullptr);
}

void urania::WndBase::createMEditBox(int id, const urania::CtrlDesc& de)
{
   CreateWindow(L"EDIT", L"",
     WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_NOHIDESEL |
     ES_WANTRETURN | ES_AUTOVSCROLL | WS_VSCROLL,
     de.x, de.y, de.w, de.h, hw_, (HMENU)id, getHI__(), nullptr);
}

void urania::WndBase::createListBox(int id, const urania::CtrlDesc& de)
{
   CreateWindow(
     L"LISTBOX", L"",
     WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_DISABLENOSCROLL | WS_VSCROLL,
     de.x, de.y, de.w, de.h, hw_, (HMENU)id, getHI__(), nullptr);
}

void urania::WndBase::createComboBox(int id, const urania::CtrlDesc& de)
{
  CreateWindow(L"COMBOBOX", L"",
    WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DISABLENOSCROLL | CBS_DROPDOWNLIST,
    de.x, de.y, de.w, de.h, hw_, (HMENU)id, getHI__(), nullptr);
}


/////////////////////////////////////
//  WndBase
//  Controls
/////////////////////////////////////

// Enable&Disable
void urania::WndBase::enableCtrl(int id)
{
  HWND w = GetDlgItem(hw_, id);
  ::EnableWindow(w, TRUE);
}

void urania::WndBase::disableCtrl(int id)
{
  HWND w = GetDlgItem(hw_, id);
  ::EnableWindow(w, FALSE);
}


//// EditBox ////
/*===========================================
 *  WndBase::getEBText()
 *  エディットボックスの文字列を取得
 */
std::wstring urania::WndBase::getEBText(int id)
{
  HWND w = GetDlgItem(hw_, id);

  int len = ::SendMessage(w, WM_GETTEXTLENGTH, 0, 0);
  if (!len)
    return std::wstring();

  wchar_t* tmp = new wchar_t[len+1];
  ::SendMessage(w, WM_GETTEXT, len+1, (LPARAM)tmp);

  std::wstring res(tmp);
  delete[] tmp;

  return res;
}

/*===========================================
 *  WndBase::setEBText()
 *  エディットボックスに文字列を設定
 */
void urania::WndBase::setEBText(int id, const std::wstring& txt)
{
  HWND w = ::GetDlgItem(hw_, id);
  ::SendMessage(w, WM_SETTEXT, 0, (LPARAM)(txt.c_str()));
}

void urania::WndBase::clearEB(int id)
{
   HWND w = ::GetDlgItem(hw_, id);
   ::SendMessage(w, WM_CLEAR, 0, 0);
}

void urania::WndBase::copyEB(int id)
{
   HWND w = ::GetDlgItem(hw_, id);
   ::SendMessage(w, WM_COPY, 0, 0);
}

void urania::WndBase::cutEB(int id)
{
   HWND w = ::GetDlgItem(hw_, id);
   ::SendMessage(w, WM_CUT, 0, 0);
}

void urania::WndBase::pasteEB(int id)
{
  HWND w = ::GetDlgItem(hw_, id);
  ::SendMessage(w, WM_PASTE, 0, 0);
}

bool urania::WndBase::canUndoEB(int id)
{
  HWND w = ::GetDlgItem(hw_, id);
  if (::SendMessage(w, EM_CANUNDO, 0, 0))
    return true;
  else
    return false;
}

void urania::WndBase::undoEB(int id)
{
   HWND w = ::GetDlgItem(hw_, id);
   ::SendMessage(w, EM_UNDO, 0, 0);
}

/*===========================================
 *  WndBase::getEBLineText()
 *  指定した行の文字列を取得
 *    id : コントロールID
 *    no : 行番号
 */
std::wstring urania::WndBase::getEBLineText(int id, int no)
{
   HWND w = ::GetDlgItem(hw_, id);

   int idx = ::SendMessage(w, EM_LINEINDEX, (WPARAM)no, 0L);
   int len = ::SendMessage(w, EM_LINELENGTH, idx, 0);
   wchar_t* tmp = new wchar_t[len+5];
   *(WORD*)tmp = (WORD)(len+4);
   len = ::SendMessage(w, EM_GETLINE, no, (LPARAM)tmp);
   tmp[len] = '\0';
   std::wstring res(tmp);
   delete[] tmp;

   return res;
}

int urania::WndBase::countEBLine(int id)
{
   HWND w = ::GetDlgItem(hw_, id);
   return ::SendMessage(w, EM_GETLINECOUNT, 0, 0);
}


//// ListBox ////
std::wstring urania::WndBase::getLBItem(int id, int no)
{
   HWND w = ::GetDlgItem(hw_, id);
   int len = ::SendMessage(w, LB_GETTEXTLEN, no, 0);
   wchar_t* tmp = new wchar_t[len + 5];
   ::SendMessage(w, LB_GETTEXT, no, (LPARAM)tmp);
   std::wstring res(tmp);
   delete[] tmp;

   return res;
}

void urania::WndBase::addLBItem(int id, const std::wstring& str)
{
   HWND w = ::GetDlgItem(hw_, id);
   ::SendMessage(w, LB_ADDSTRING, 0, (LPARAM)(str.c_str()));
}

void urania::WndBase::insertLBItem(int id, int no, const std::wstring& str)
{
   HWND w = ::GetDlgItem(hw_, id);
   ::SendMessage(w, LB_INSERTSTRING, no, (LPARAM)(str.c_str()));
}

void urania::WndBase::deleteLBItem(int id,int no)
{
   HWND w = ::GetDlgItem(hw_, id);
   ::SendMessage(w, LB_DELETESTRING, no, 0);
}

int urania::WndBase::countLB(int id)
{
   HWND w = ::GetDlgItem(hw_, id);
   return ::SendMessage(w, LB_GETCOUNT, 0, 0);
}

int urania::WndBase::getCurrentLB(int id)
{
   HWND w = ::GetDlgItem(hw_, id);
   int r = ::SendMessage(w, LB_GETCURSEL, 0, 0);
   if (r == LB_ERR)
     return -1;
   else
     return r;
}

void urania::WndBase::setCurrentLB(int id, int no)
{
   HWND w = ::GetDlgItem(hw_, id);
   ::SendMessage(w, LB_SETCURSEL, no, 0);
}

void urania::WndBase::resetLB(int id)
{
   HWND w = ::GetDlgItem(hw_, id);
   ::SendMessage(w, LB_RESETCONTENT, 0, 0);
}

void urania::WndBase::dirLB(int id, const std::wstring& path, int flag)
{
   HWND w = ::GetDlgItem(hw_, id);

   WPARAM wf = 0;
   if(flag & CTD_READONRY)
      wf |= DDL_READONLY;
   if(flag & CTD_HIDDEN)
      wf |= DDL_HIDDEN;
   if(flag & CTD_SYSTEM)
      wf |= DDL_SYSTEM;
   if(flag & CTD_DIRECTORY)
      wf |= DDL_DIRECTORY;
   if(flag & CTD_ARCHIVE)
      wf |= DDL_ARCHIVE;
   if(flag & CTD_DRIVE)
      wf |= DDL_DRIVES;
   if(flag & CTD_EXCLUSIVE)
      wf |= DDL_EXCLUSIVE;
   if(flag & CTD_READWRITE)
      wf |= DDL_READWRITE;

   ::SendMessage(w, LB_DIR, wf, (LPARAM)(path.c_str()));
}


//// ComboBox ////
/*================================================
 *  WndBase::getCBItem()
 *  コンボボックスのリスト項目の文字列を取得
 */
std::wstring urania::WndBase::getCBItem(int id, int no)
{
   HWND w = ::GetDlgItem(hw_,id);
   int len = ::SendMessage(w, CB_GETLBTEXTLEN, no, 0);
   wchar_t* tmp = new wchar_t[len+5];
   ::SendMessage(w,CB_GETLBTEXT,no,(LPARAM)tmp);
   std::wstring res(tmp);
   delete[] tmp;

   return res;
}

void urania::WndBase::addCBItem(int id, const std::wstring& str)
{
   HWND w = ::GetDlgItem(hw_,id);
   ::SendMessage(w,CB_ADDSTRING, 0, (LPARAM)(str.c_str()));
}

void urania::WndBase::insertCBItem(int id,int no,const std::wstring& str)
{
   HWND w = ::GetDlgItem(hw_,id);
   ::SendMessage(w,CB_INSERTSTRING,no,(LPARAM)(str.c_str()));
}

void urania::WndBase::deleteCBItem(int id,int no)
{
   HWND w = ::GetDlgItem(hw_,id);
   ::SendMessage(w,CB_DELETESTRING,no,0);
}

int urania::WndBase::countCB(int id)
{
   HWND w = ::GetDlgItem(hw_,id);
   return ::SendMessage(w,CB_GETCOUNT,0,0);
}

int urania::WndBase::getCurrentCB(int id)
{
   HWND w = ::GetDlgItem(hw_, id);
   int r = ::SendMessage(w, CB_GETCURSEL, 0, 0);
   if (r == CB_ERR)
     return -1;
   else
     return r;
}

void urania::WndBase::setCurrentCB(int id, int no)
{
   HWND w = ::GetDlgItem(hw_, id);
   ::SendMessage(w, CB_SETCURSEL, no, 0);
}

void urania::WndBase::resetCB(int id)
{
   HWND w = ::GetDlgItem(hw_,id);
   ::SendMessage(w, CB_RESETCONTENT, 0, 0);
}

void urania::WndBase::dirCB(int id,const std::wstring& path,int flag)
{
   HWND w = ::GetDlgItem(hw_,id);

   WPARAM wf = 0;
   if(flag & CTD_READONRY)  wf |= DDL_READONLY;
   if(flag & CTD_HIDDEN)    wf |= DDL_HIDDEN;
   if(flag & CTD_SYSTEM)    wf |= DDL_SYSTEM;
   if(flag & CTD_DIRECTORY) wf |= DDL_DIRECTORY;
   if(flag & CTD_ARCHIVE)   wf |= DDL_ARCHIVE;
   if(flag & CTD_DRIVE)     wf |= DDL_DRIVES;
   if(flag & CTD_EXCLUSIVE) wf |= DDL_EXCLUSIVE;
   if(flag & CTD_READWRITE) wf |= DDL_READWRITE;

   ::SendMessage(w,CB_DIR,wf,(LPARAM)(path.c_str()));
}


// Button Check
bool urania::WndBase::isChecked(int id)
{
   if(IsDlgButtonChecked(hw_, id))
      return true;
   else
      return false;
}


//eof
