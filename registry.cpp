/*
 * Copyright 2004-2021 oZ/acy (名賀月晃嗣)
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
/*
 * @file registry.cpp
 * @author oZ/acy
 * @brief レジストリ操作クラス
 *
 * @date 22 Feb 2004  作成
 *
 */
#include "registry.h"


/*=========================================================
 *  RegistryBase::getRoot()
 *  ルートキーのハンドルを返す
 */
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
