/*
 * Copyright 2016-2021 oZ/acy (名賀月晃嗣)
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
 * @file getCLArgsW.cpp
 * @author by oZ/acy
 *
 * @date 27 Feb 2016  getCmdLineArgs()のwchar_t版を作成
 * @date 26 Mar 2021  修正
 * @date 12 Jun 2021  修正
 *
 */
#include <algorithm>
#include "system.h"

// もしかしてコンパイラ依存??
// 利用しない方針で書き換へる
//extern int __argc;
//extern wchar_t** __wargv;


std::vector<std::wstring> urania::System::getCmdLineArgsW()
{
  return getCmdLineArgsW(::GetCommandLineW());
}


std::vector<std::wstring>
urania::System::getCmdLineArgsW(const wchar_t* cmdline)
{
  int argc;
  auto argv = ::CommandLineToArgvW(cmdline, &argc);

  std::vector<std::wstring> args(argc);
  std::copy_n(argv, argc, args.begin());

  //std::vector<std::wstring> args(__argc);
  //std::copy_n(__wargv, __argc, args.begin());

  return args;
}


//eof
