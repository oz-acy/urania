# LIBURANIA

## これはなに?
C++によるWin32APIのラッパー。


## Uraniaとは?
[女神ウーラニアー](https://ja.wikipedia.org/wiki/%E3%82%A6%E3%83%BC%E3%83%A9%E3%83%8B%E3%82%A2%E3%83%BC)の名を拜借した。


## 内容
ライブラリの提供する機能を、ヘッダファイル毎に示す。

|ヘッダファイル|内容|
|:---|:---|
|urania/decl.h|各種宣言|
|urania/system.h|システム周り、或は雜多な周邊機能|
|urania/wbase.h|ウィンドウハンドルの管理|
|urania/bwin.h|ウィンドウやダイアログの基底クラス|
|urania/window.h|ウィンドウ|
|urania/dialog.h|ダイアログ|
|urania/clkpanel.h|「クリックパネル」コントロール|
|urania/cmndlg.h|コモンダイアログ|
|urania/menu.h|メニュー|
|urania/paintdev.h|描畫デバイス|
|urania/registry.h|レジストリ操作|


## 依存してゐるライブラリ
<!--
* [libthemis](https://github.com/oz-acy/themis)
* [libpolymnia](https://github.com/oz-acy/polymnia)
-->
* [libeunomia](https://github.com/oz-acy/eunomia)



## 利用方法

### 導入
CMakeを用ゐてインストールする。([libeunomia](https://github.com/oz-acy/eunomia)の導入は大前提。)

```bash
$ cmake -G "Unix Makefiles" .
$ make
$ make install
```

ジェネレータは環境に應じて適宜變更すること。他にもcmakeにあれこれオプション指定する必要があるかもしれない。

ヘッダファイルは${prefix}/include/uraniaの下にインストールされる。

ライブラリを利用するときには、CMakeを用ゐるのであれば find_package(urania) すれば色々捗る、はず。


### リファレンス
Doxygenを用ゐて生成したリファレンスを
<https://www.hiemalis.org/~acy/swlib/urania/>
に置いてゐる。


## ライセンス
[二條項BSDライセンス](https://opensource.org/licenses/BSD-2-Clause)とする。


## 作者
oZ/acy (名賀月晃嗣)
* <acy@hiemalis.org>
* <https://www.hiemalis.org/~acy/>
