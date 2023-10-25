# lua_x68
Lua 5.4.6 を X680x0向けにポーティングしてみました。X-BASICの一部の関数を取り込んだりしています。

## インストール
* "lua.x" と "luac.x"を環境変数PATHに含まれるディレクトリにコピー
* "lualib.l" を環境変数LIBで指定するディレクトリにコピー
* 以下のヘッダファイルを環境変数INCLUDEで指定するディレクトリにコピー
    * lauxlib.h
    * lua.h
    * lua.hpp
    * luaconf.h
    * lualib.h

## 起動と起動オプション
* lua.xを実行すると対話環境が起動します。luac.xはコンパイル用のフロントエンドです。
* 対話環境を終了するには "os.exit()"と入力するか、ctrl-zを入力してください。
* 起動時に指摘できるオプションは無効なオプションを指定すると見ることができます("lua -z"等)
* X680x0向けに以下のオプションを追加で実装しています。
    * -M=nnnn : ヒープメモリをnnnnKbytes追加で確保して起動します
    * -MM : ヒープメモリを可能なだけ確保して起動します。

## X680x0向け独自拡張について
"x68k.～"という関数でX-BASICのオーディオ・グラフィック・スプライト・マウス・ジョイスティックに関する処理を実装しています。詳細はx68klib.txtを参照してください。

## r2-->r3の主な修正点
* -M/-MMオプションを実装しました
* X-BASIC系の関数を実装しました。
* 環境変数LUA_PATH/LUA_CPATHが定義されてない場合のデフォルト値をLinuxスタイルからWindowsスタイルに変更しました。合わせてディレクトリのセパレータキャラクターを'/'から'\\\\'に変更しています。

## ビルド環境
以下のツール／ライブラリでビルドしています。
* cc: gcc.x
* as: has.x
* lk: hlk.x
* make: gnu make
* lib: XC2.0 libraryおよび gccで必要になるライブラリ

## ビルド
* "src"ディレクトリの中で "gmake -f Makefile.x68" でビルドできます。
* "Makefile2.x68"はgcc2環境向けです。
* IOCSLIB.Hのstruct INQUIRY構造体の記述がgccではエラーになる場合があります。その場合はlibcのcdecl.hの _ZEROARRAY および sys/scsi.h内の struct _inquiry の記述を参考にヘッダファイルを修正する必要があります
* gcc2環境でビルドする場合、lvm.c内で LUA_USE_JUMPTABLE を有効にすることで処理が若干高速になります。

## 謝辞
Lua for X680x0 への機能追加にあたって、MicroPython for X68k のソースコードを非常に参考にさせていただいております。

## ライセンス
Luaは下記のようにMITライセンスで配布されています。Lua for X680x0の独自改変部分についても同様の扱いとします。  
[Copyright © 1994–2023 Lua.org, PUC-Rio.](https://www.lua.org/license.html)
