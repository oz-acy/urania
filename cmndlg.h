/**********************************************************************//**
 *
 *  @file cmndlg.h
 *  @author oZ/acy (名賀月晃嗣)
 *  @brief コモンダイアログラッパー
 *
 *  @date 2016.2.29  OpenFileDialogとSaveFileDialogをFileDialogに一本化
 *  @date 2016.8.10  FileDialog::crearFilePath()の型を修正
 *  @date 2019.8.30  各クラスのcreate()の返却型をunique_ptrに修正
 */
#ifndef INCLUDE_GUARD_URANIA_COMMONDLG_H
#define INCLUDE_GUARD_URANIA_COMMONDLG_H

#include <memory>
#include "wbase.h"

/*------------------------------------------------
 *  class CommonDialogBase
 *  Common Dialog ベースクラス
 *----------------------------------------------*/
class urania::CommonDialogBase : themis::Noncopyable<urania::CommonDialogBase>
{
 protected:
  static HWND getHW__(const urania::WndBase* wb)
  {
    return wb->hw_;
  }

 public:
  CommonDialogBase() {}
  virtual ~CommonDialogBase() =0;
};

inline urania::CommonDialogBase::~CommonDialogBase() {}


/*------------------------------------------------
 *  class FileDialog
 *  ファイルダイアログ
 */
class urania::FileDialog : public urania::CommonDialogBase
{
private:
  OPENFILENAME ofn_;
  wchar_t name_[MAX_PATH];
  wchar_t initDir_[MAX_PATH];
  std::wstring filter_;
  std::wstring defExt_;

protected:
  FileDialog(const std::wstring& flt, const std::wstring& ext);

public:
  virtual ~FileDialog() {}

  bool doModalOpenFile(const urania::WndBase* win);
  bool doModalSaveFile(const urania::WndBase* win);

  std::wstring getFilePath() const;
  std::wstring getFileName() const;
  std::wstring getFileDir() const;
  std::wstring getFileExt() const;

  void clearFilePath()
  { 
    name_[0] = L'\0';
  }

  void setFilePath(const std::wstring& path);
  void setInitDir(const std::wstring& path);

  static
  std::unique_ptr<FileDialog>
  create(const std::wstring& flt, const std::wstring& ext =L"")
  {
    return std::unique_ptr<FileDialog>(new FileDialog(flt, ext));
    //return new FileDialog(flt, ext);
  }
};




/*------------------------------------------------
 *  class FileDialogBase
 *  ファイルダイアログベースクラス
 *----------------------------------------------*/
/*
class urania::FileDialogBase : public urania::CommonDialogBase
{
protected:
  OPENFILENAME ofn_;
  wchar_t name_[MAX_PATH];
  wchar_t initDir_[MAX_PATH];
  std::wstring filter_;
  std::wstring defExt_;

public:
  FileDialogBase(const std::wstring& flt, const std::wstring& ext);
  virtual ~FileDialogBase() {}

  virtual bool doModal(const urania::WndBase* win) = 0;
  std::wstring getFilePath() const;
  std::wstring getFileName() const;
  std::wstring getFileDir() const;
  std::wstring getFileExt() const;

  std::wstring clearFilePath()
  { 
    name_[0] = L'\0';
  }

  void setFilePath(const std::wstring& path);
  void setInitDir(const std::wstring& path);
};
*/

/*------------------------------------------------
 *   class OpenFileDialog
 *   オープンファイルダイアログのラッパークラス
 *-----------------------------------------------*/
/*
class urania::OpenFileDialog : public urania::FileDialogBase
{
protected:
  OpenFileDialog(const std::wstring& flt, const std::wstring& ext)
    : FileDialogBase(flt, ext)
  {
    ofn_.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  }

public:
  ~OpenFileDialog() {}

  static urania::OpenFileDialog* create(
    const std::wstring& flt, const std::wstring& ext =L"")
  {
    return new OpenFileDialog(flt, ext);
  }

  bool doModal(const urania::WndBase* win =nullptr);
};
*/

/*----------------------------------------------
 *  class SaveFileDialog
 *   セーブファイルダイアログのラッパークラス
 *--------------------------------------------*/
/*
class urania::SaveFileDialog : public urania::FileDialogBase
{
protected:
  SaveFileDialog(const std::wstring& flt, const std::wstring& ext)
    : FileDialogBase(flt, ext)
  {
    ofn_.Flags = OFN_OVERWRITEPROMPT;
  }

public:
  ~SaveFileDialog() {}

  static urania::SaveFileDialog* create(
    const std::wstring& flt, const std::wstring& ext =L"")
  {
    return new SaveFileDialog(flt, ext);
  }

  bool doModal(const urania::WndBase* win =nullptr);
};
*/




#endif // INCLUDE_GUARD_URANIA_COMMONDLG_H
