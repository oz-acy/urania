/*
 * Copyright 2009-2021 oZ/acy (名賀月晃嗣)
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
 * @file sys-ws2mbs.cpp
 * @author oZ/acy (名賀月晃嗣)
 *
 * @date 4 Apr MMIX    作成
 * @date 7 Nov MMIX    修正
 * @date 8 Sep MMXI    修正
 * @date 27 Feb MMXVI  メソッド名變更
 * @date 30 Aug MMXIX  修正
 *
 */
#include <memory>
#include "system.h"

std::string urania::System::strcpyWideToMultiByte(const std::wstring& ws)
{
  using namespace std;

  int len
    = WideCharToMultiByte(
        CP_THREAD_ACP, 0, ws.c_str(), -1, nullptr, 0, nullptr, nullptr);

  auto buf = std::make_unique<char>(len);

  WideCharToMultiByte(
    CP_THREAD_ACP, 0, ws.c_str(), -1, buf.get(), len, nullptr, nullptr);

  return string(buf.get());
}


//eof
