/**************************************************************************
 *
 *  ctrl.cpp
 *  by oZ/acy
 *  (c) 2002-2018 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  ����
 *    2016.2.27   �C��
 *    2018.12.24  �C��
 */
#include <memory>
#include "wbase.h"


//// EditBox ////
/*===========================================
 *  WndBase::getTextEB()
 *  �G�f�B�b�g�{�b�N�X�̕�������擾
 */
std::wstring urania::WndBase::getTextEB(int id)
{
  HWND w = GetDlgItem(hw_, id);

  int len = ::SendMessage(w, WM_GETTEXTLENGTH, 0, 0);
  if (len == 0)
    return std::wstring();

  std::unique_ptr<wchar_t[]> tmp(new wchar_t[len + 1]);
  len = ::SendMessage(w, WM_GETTEXT, len + 1, (LPARAM)(tmp.get()));
  tmp[len] = L'\0';

  return std::wstring(tmp.get());
}

/*===========================================
 *  WndBase::getLineTextEB()
 *  �w�肵���s�̕�������擾
 *    id : �R���g���[��ID
 *    no : �s�ԍ�
 */
std::wstring urania::WndBase::getLineTextEB(int id, int no)
{
  HWND w = ::GetDlgItem(hw_, id);

  int idx = ::SendMessage(w, EM_LINEINDEX, (WPARAM)no, 0L);
  int len = ::SendMessage(w, EM_LINELENGTH, idx, 0);

  std::unique_ptr<wchar_t[]> tmp(new wchar_t[len + 4]);
  *(WORD*)(tmp.get()) = (WORD)(len + 4);
  len = ::SendMessage(w, EM_GETLINE, no, (LPARAM)(tmp.get()));
  tmp[len] = L'\0';

  return std::wstring(tmp.get());
}



//// ListBox ////
/*************************************************
 *  WndBase::getItemLB()
 *  ���X�g�{�b�N�X�̍��ڂ̕�������擾
 */
std::wstring urania::WndBase::getItemLB(int id, int no)
{
  HWND w = ::GetDlgItem(hw_, id);
  int len = ::SendMessage(w, LB_GETTEXTLEN, no, 0);
  std::unique_ptr<wchar_t[]> tmp(new wchar_t[len + 1]);
  ::SendMessage(w, LB_GETTEXT, no, (LPARAM)(tmp.get()));
  return std::wstring(tmp.get());
}


/***********************************************************
 *  WndBase::dirLB()
 *  ���X�g�{�b�N�X�̓��e���w��f�B���N�g���̓��e�Ƃ���
 */
void urania::WndBase::dirLB(int id, const std::wstring& path, int flag)
{
  HWND w = ::GetDlgItem(hw_, id);
  ::SendMessage(w, LB_DIR, flag, (LPARAM)(path.c_str()));
}


//// ComboBox ////
/*================================================
 *  WndBase::getItemCB()
 *  �R���{�{�b�N�X�̃��X�g���ڂ̕�������擾����
 */
std::wstring urania::WndBase::getItemCB(int id, int no)
{
  HWND w = ::GetDlgItem(hw_,id);
  int len = ::SendMessage(w, CB_GETLBTEXTLEN, no, 0);

  std::unique_ptr<wchar_t[]> tmp(new wchar_t[len + 1]);
  len = ::SendMessage(w,CB_GETLBTEXT,no,(LPARAM)(tmp.get()));
  tmp[len] = L'\0';
  return std::wstring(tmp.get());
}


/*===============================================================
 *  WndBase::dirCB()
 *  �R���{�{�b�N�X�̓��e���w��f�B���N�g���̃t�@�C�����ɂ���
 */
void urania::WndBase::dirCB(int id, const std::wstring& path, int flag)
{
   HWND w = ::GetDlgItem(hw_,id);
   ::SendMessage(w, CB_DIR, flag,(LPARAM)(path.c_str()));
}




//eof
