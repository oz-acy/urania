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
 * @file cmndlg.h
 * @author oZ/acy (名賀月晃嗣)
 * @brief コモンダイアログラッパー
 *
 * @date 2016.2.29  OpenFileDialogとSaveFileDialogをFileDialogに一本化
 * @date 2016.8.10  FileDialog::crearFilePath()の型を修正
 * @date 2019.8.30  各クラスのcreate()の返却型をunique_ptrに修正
 * @date 2021.3.27  修正
 *
 * @date 2021.6.12
 *   依據ライブラリをthemis+polymniaからeunomiaに切り替へるための修正
 *
 */
#ifndef INCLUDE_GUARD_URANIA_COMMONDLG_H
#define INCLUDE_GUARD_URANIA_COMMONDLG_H

#include <memory>
#include <cwchar>
#include "wbase.h"

/**
 * @brief コモンダイアログ基底クラス
 *
 * 各種コモンダイアログを操作するクラスの抽象基底となるクラス。
 */
class urania::CommonDialogBase : eunomia::Noncopyable<urania::CommonDialogBase>
{
protected:
  CommonDialogBase() = default;

public:
  virtual ~CommonDialogBase() = default;
};


/**
 * @brief ファイルダイアログ
 *
 * ファイルダイアログを操作するためのクラス。
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
  bool doModalOpenFile(const urania::WndBase* win);
  bool doModalSaveFile(const urania::WndBase* win);

  std::wstring getFilePath() const { return name_; }
  std::wstring getFileName() const { return name_ + ofn_.nFileOffset; }

  std::wstring getFileDir() const
  {
    wchar_t tmp[MAX_PATH];
    std::wcsncpy(tmp, name_, ofn_.nFileOffset);
    tmp[ofn_.nFileOffset] = L'\0';
    return tmp;
  }

  std::wstring getFileExt() const
  {
    if (!ofn_.nFileExtension)
      return L"";
    else
      return name_ + ofn_.nFileExtension;
  }

  void clearFilePath() { name_[0] = L'\0'; }

  void setFilePath(const std::wstring& path)
  {
    std::wcsncpy(name_, path.c_str(), MAX_PATH);
    name_[MAX_PATH - 1] = L'\0';
  }

  void setInitDir(const std::wstring& path)
  {
    wcsncpy(initDir_, path.c_str(), MAX_PATH);
    initDir_[MAX_PATH - 1] = L'\0';
  }

  static
  std::unique_ptr<FileDialog>
  create(const std::wstring& flt, const std::wstring& ext =L"")
    { return std::unique_ptr<FileDialog>(new FileDialog(flt, ext)); }
};




#endif // INCLUDE_GUARD_URANIA_COMMONDLG_H
