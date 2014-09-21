== (オプショナル) JSONを静的ファイルとしてGAEサーバで配布する。

静的ファイルを送ります

@<href>{https://dl.dropboxusercontent.com/u/1088314/tech_institute/2014/syllabus.json}


参考: @<href>{https://developers.google.com/appengine/docs/python/gettingstartedpython27/staticfiles}

@<href>{http://model-shelter-709.appspot.com/static/syllabus.json}

「ユーティリティによる実践」で使用したシラバスアプリのURL部分を
自分のサーバのURLに変更してみましょう。

JSONの中身を変更し、アプリを再起動した際に項目が変わっていれば、
ひとまず成功です。

ただしこの方法では、アップロードしたデータを変更することが出来ません。

ここから、
Datastoreと呼ばれるGAE特有の仕組みを用いてデータを保存できるように
改造していきます。
