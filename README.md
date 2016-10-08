Bashground
=====

Bash on Windows process runs in backgroud.

背景
----

Bash on Windows で tmux 起動しててもbashのウィンドウがすべて終了した時点でLinuxサブシステムがシャットダウンしてしまう。つまりtmuxセッションが維持できなくてとても悲しい。

そために、バックグラウンドでLinuxサブシステムを起動してそのままシャットダウンしないように維持するだけのプログラム。

special thanks
----

Alex Ionescu - lxss (https://github.com/ionescu007/lxss)

license
----

the software licensed under MIT license.

the software includes these opensource software.

- lxss (https://github.com/ionescu007/lxss)
