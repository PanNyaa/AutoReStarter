AutoReStarter
=============

windows用の、「スリープ・スタンバイ復帰後を検知して指定されたアプリケーションとかを再実行する、常駐型アプリケーション」です。
(以下、スリープ・スタンバイをまとめてスリープと呼称します)

アプリケーションの指定方法は、AutoReStarter.exeを実行すると生成されるAutoReStarter.iniに、スリープ復帰後に再実行したいアプリケーションのファイルパスを設定します。(たぶん相対パス可、たぶんっ)

※指定できるファイルは1つだけです。ごめんなさい:;(∩´﹏`∩);:

使用想定としては、「AutoReStarter.exe」と「スリープ復帰後も再実行したいアプリケーション」を両方スタートアップに置いて、
「スリープ復帰後も再実行したいアプリケーション」を、常駐しているAutoReStarter.exeがスリープ復帰後を検知して起動する。といった感じです。

このソフトの利点は、「スリープ復帰後も再実行したいアプリケーション」が常駐アプリケーションでは無かった場合でも「スリープ復帰後に自動的に再実行される常駐アプリケーション」のように扱えるようになるところです(粉蜜柑)。

※10秒以上スリープされていた場合のみ、スリープ復帰を検知します。



Q.スリープ復帰するときに黒画面でるんだけど…

A.仕様です。

Q.消せないの？

A.system() を使うのをやめれば消せます。(しろめ)

Q.WinAPI使えばいいじゃん。

A.そうだね:;(∩´﹏`∩);:




※ソースコードありますが、汚いのでお察しください

