Bashground
=====

Bash on Windows process runs in backgroud.

背景
----

Bash on Windows で tmux 起動しててもbashのウィンドウがすべて終了した時点でLinuxサブシステムがシャットダウンしてしまう。つまりtmuxセッションが維持できなくてとても悲しい。

そのために、見えないところでBashを起動してLinuxサブシステムがシャットダウンしないように維持するだけのプログラム。