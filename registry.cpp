/**************************************************************************
*
*  registry.cpp
*  by oZ/acy
*  (c) 2004 oZ/acy. ALL RIGHTS RESERVED.
*
*  レジストリ操作クラス
*
*  履歴
*    22 Feb 2004:  作成
**************************************************************************/

#include "registry.h"


/*=========================================================
*  RegistryBase::getRoot()
*  ルートキーのハンドルを返す
*========================================================*/
HKEY urania::RegistryBase::getRoot_(urania::RegRootKey root)
{
  switch (root)
  {
  case REGKEY_CLASSES_ROOT:
    return HKEY_CLASSES_ROOT;

  case REGKEY_CURRENT_USER:
    return HKEY_CURRENT_USER;

  case REGKEY_LOCAL_MACHINE:
    return HKEY_LOCAL_MACHINE;

  case REGKEY_USERS:
    return HKEY_USERS;

  case REGKEY_CURRENT_CONFIG:
    return HKEY_CURRENT_CONFIG;

  default:
    return NULL;
  }
};


//eof
