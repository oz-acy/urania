/**************************************************************************//**
 *
 *  @file menu.h
 *  @author oZ/acy
 *  @brief メニュー記述・操作系クラス
 *
 *  @date 2016.2.28  修正
 *  @date 2019.8.30  修正
 */

#ifndef INCLUDE_GUARD_URANIA_MENU_H
#define INCLUDE_GUARD_URANIA_MENU_H

#include <vector>
#include <string>
#include <memory>
#include <windows.h>
#include <themis/noncopyable.h>
#include "decl.h"


///  メニュー記述用クラス
class urania::MenuDesc
{
  friend class urania::Menu;

public:
  class Node_
  {
    friend class MenuDesc;

  public:
    enum
    {
      MN_BRANCH = 0,
      MN_SEPARATOR = -1
    };

  private:
    std::vector<Node_> ary_;
    std::wstring name_;
    int id_;

    Node_() : id_(MN_SEPARATOR) {}
    Node_(const std::wstring& s) : name_(s), id_(MN_BRANCH) {}
    Node_(const std::wstring& s, int i) : name_(s), id_(i) {}

  public:
    ~Node_() {}

    /// 區切り線を追加する。
    bool addSeparator();

    /// ポップアップ項目(メニューツリーの枝部分)を追加する。
    bool addPopup(const std::wstring& s);

    /// 選擇項目(メニューツリーの葉部分)を追加する。
    /// @param s 表示文字列
    /// @param i ID
    bool addPopup(const std::wstring& s, int i);

    Node_& operator[](int i) { return ary_[i]; }
    const Node_& operator[](int i) const { return ary_[i]; }
  };

private:
  std::vector<Node_> node_;

public:
  MenuDesc(){}
  ~MenuDesc(){}

  /// ポップアップ項目(メニューツリーの枝部分)を追加する。
  bool addPopup(const std::wstring& s);

  /// 選擇項目(メニューツリーの葉部分)を追加する。
  /// @param s 表示文字列
  /// @param i ID
  bool addPopup(const std::wstring& s, int i);

  Node_& operator[](int i) { return node_[i]; }
  const Node_& operator[](int i) const { return node_[i]; }

private:
  /// MenuDescの内容を反映したメニューを生成する。
  HMENU createHandle_() const;

  /// createHandle_()の下請けとしてポップアップメニューを生成する。
  static void addPopupMenu_(HMENU pm, const Node_& item);
};


///  メニューハンドル(HMENU)のラッパー
class urania::Menu : themis::Noncopyable<urania::Menu>
{
  friend class urania::Window;

private:
  typedef std::shared_ptr<urania::Menu> PP_;

private:
  HMENU hmenu_;
  bool dst_;

  Menu() : hmenu_(NULL), dst_(false) {}

  ///  ObjectとHMENUを強固に連結する。
  ///  Object破棄時にMENUも破棄するやうにする。
  void link_(HMENU h)
  {
    attach_(h);
    if (hmenu_)
      dst_ = true;
  }

  ///  HMENUとObjectを切り離す。
  ///  もしlink_()されてゐればHMENUを破棄する。
  void kill_()
  {
    if (dst_ && hmenu_)
      ::DestroyMenu(hmenu_);
     detach_();
  }


  ///  ObjectとHMENUを弱く連結する。
  ///  Object破棄時にHMENUを破棄しない。
  void attach_(HMENU h)
  {
    kill_();
    hmenu_ = h;
  }

  ///  HMENUとの連結を切る。HMENUの破棄はしない。
  void detach_()
  {
    hmenu_ = NULL;
    dst_ = false;
  }

  ///  Windowにハンドルを渡し、
  ///  link_()状態からattach_()状態に遷移
  HMENU giveHM_()
  {
    dst_ = false;
    return hmenu_;
  }


public:
  ~Menu() { kill_(); }

  /// MenuDescからMenuを生成する。
  static PP_ create(const urania::MenuDesc& desc);

  /// リソースIDからMenuを生成する。
  static PP_ create(int rc);

  /// サブメニューを取得する。
  PP_ getSub(unsigned id);


  /// 指定項目にチェックをつける。
  void checkItem(unsigned cmdid);

  /// 指定項目のチェックを外す。
  void uncheckItem(unsigned cmdid);

  /// 指定項目のチェックの有無を得る。
  bool getItemCheck(unsigned cmdid);


  /// 項目を選擇可能にする。
  void enableItem(unsigned cmdid);

  /// 項目を選擇不可にする。
  void disableItem(unsigned cmdid);

  /// 項目を淡色表示(選擇不可)にする。
  void grayItem(unsigned cmdid);
};


namespace urania
{
  using RCP_Menu = std::shared_ptr<Menu>;
  //typedef std::shared_ptr<Menu> RCP_Menu;
}


#endif // INCLUDE_GUARD_URANIA_MENU_H
