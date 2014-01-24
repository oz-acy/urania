/**************************************************************************
 *
 *  cmndlg.h
 *  by oZ/acy
 *  (c) 2002-2011 oZ/acy.  ALL RIGHTS RESERVED.
 *
 *  コモンダイアログラッパー
 *
 *  last update : 8 Sep 2011
 *************************************************************************/

#ifndef INC_URANIA_COMMONDLG_H___
#define INC_URANIA_COMMONDLG_H___

#include "wbase.h"
#include <boost/utility.hpp>

/*------------------------------------------------
 *  class CommonDialogBase
 *  Common Dialog ベースクラス
 *----------------------------------------------*/
class urania::CommonDialogBase : boost::noncopyable
{
 protected:
  static HWND getHW__(const urania::WndBase* wb) { return wb->hw_; }

 public:
  CommonDialogBase() {}
  virtual ~CommonDialogBase() =0;
};

inline urania::CommonDialogBase::~CommonDialogBase() {}



/*------------------------------------------------
 *  class FileDialogBase
 *  ファイルダイアログベースクラス
 *----------------------------------------------*/
class urania::FileDialogBase : public urania::CommonDialogBase
{
 protected:
  OPENFILENAME ofn_;
  wchar_t filter_[256];
  wchar_t name_[256];
  wchar_t def_ext_[5];

 public:
  FileDialogBase(const std::wstring& flt, const std::wstring& ext);
  virtual ~FileDialogBase() {}

  virtual bool doModal(const urania::WndBase* win) = 0;
  std::wstring getPath() const;
  std::wstring getTitle() const;
  std::wstring getExt() const;
  void setPath(const std::wstring& path);
};


/*------------------------------------------------
 *   class OpenFileDialog
 *   オープンファイルダイアログのラッパークラス
 *-----------------------------------------------*/
class urania::OpenFileDialog : public urania::FileDialogBase
{
private:
  typedef urania::OpenFileDialog* P_;

protected:
  OpenFileDialog(const std::wstring& flt, const std::wstring& ext)
    : FileDialogBase(flt, ext)
  {
    ofn_.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  }

public:
  ~OpenFileDialog() {}

  static P_ create(const std::wstring& flt, const std::wstring& ext =L"")
  {
    return new OpenFileDialog(flt, ext);
  }

  bool doModal(const urania::WndBase* win =nullptr);
};


/*----------------------------------------------
 *  class SaveFileDialog
 *   セーブファイルダイアログのラッパークラス
 *--------------------------------------------*/
class urania::SaveFileDialog : public urania::FileDialogBase
{
private:
  typedef urania::SaveFileDialog* P_;

protected:
  SaveFileDialog(const std::wstring& flt, const std::wstring& ext)
    : FileDialogBase(flt, ext)
  {
    ofn_.Flags = OFN_OVERWRITEPROMPT;
  }

public:
  ~SaveFileDialog() {}

  static P_ create(const std::wstring& flt, const std::wstring& ext =L"")
  {
    return new SaveFileDialog(flt, ext);
  }

  bool doModal(const urania::WndBase* win =nullptr);
};




#endif // INC_URANIA_COMMONDLG_H___
