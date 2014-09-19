= 自分のWebサーバを作ろう

//lead{
前節ではWebサーバ
}

== Python実行環境のインストール

Python言語をPC上で実行するためにPython実行環境をPCへインストールします

以下のURLから取得します。

 * https://www.python.org/downloads/release/python-278/
 * (https://www.python.org/ftp/python/2.7.8/python-2.7.8.amd64.msi)

//image[install-python-1][@<fn>{https://www.python.org/downloads/release/python-278/}のスクリーンショット]{
//}

== GAE SDK

 * https://developers.google.com/appengine/downloads

//image[install-gae-1][@<fn>{https://developers.google.com/appengine/downloads}のスクリーンショット]{
//}

== PyCharm Community Edition のインストール

Pythonの場合でもEclipse同様の統合開発環境(IDE)があった方がやりやすいので、PyCharmと言うIDEをインストールします。

@<fn>{about_pydev}

//footnote[about_pydev][Eclipse上でPython開発環境を実現する方法としてPyDevというプラグインが存在しますが、現在環境にインストールされているJavaのバージョンが古いためそのままでは利用できません。Javaのバージョンを7にすれば動作するはずですが、Android開発環境を壊すリスクとなるため、演習では採用しません。]

===[column] PyCharm Professional Edition、IntelliJ IDEA, そして Android Studio

PyCharm には有料のProfessional Editionと
無料で利用できるCommunity Editionの2種類があります。

本文の説明は全てCommunity Editionに基づいて行います。
どちらを用いても本質的な結果は変わりませんが、
Professional Editionの方が後述する通り、
本演習の範囲でも効率的な開発を行うことができます。

Professional Editionは30日間評価のための利用が出来るので、
あくまで評価する目的で、そちらを選択しても構いません。
ただし30日後に同様の機能を継続して利用したい場合、購入する必要があります。
(購入するかの決断は、本コラムの最後まで待ってください！)

本演習でProfessional Editionを利用する最大のメリットはおそらく、
GAEと開発環境のより緊密な連携です。
PyCharm Professiona Editionでは、
ターミナルからコマンドを実行せずにGAEのローカルサーバを
立ちあげたり、Googleへアップロードしたりすることができます。
詳細な比較には、以下のページを参照してください。
@<href>{http://www.jetbrains.com/pycharm/features/editions_comparison_matrix.html}

さて、PyCharmを開発しているJetBRAINSはAndroid用の開発環境を、
Googleとの共同で無料で提供しています。
この開発環境はAndroid Studioと呼ばれ、今後のAndroid開発では標準的な
開発環境になっていくことが期待されています。

実はこのJetBRAINS社は、IntelliJ IDEAと呼ばれる有料の統合開発環境
を販売しており、この環境を購入すると、
PyCharm Professional EditionやAndroid Studio、
更にはJavaScriptやRubyといった他のプログラミング言語の開発環境も
プラグインとしてついてきます。
（その分だけ「お高い」開発環境です）

もしPythonとPyCharm Professional Editionにも興味をもち、
Androidアプリとの連携を考えているようであれば、
独立した開発環境であるPyCharmではなく、
Android Studioの機能も利用できるIntelliJ IDEAを選択するメリットが
生まれてきます。
貪欲に他のプログラミング言語も遊ぶということであればますますです。

商売とは熾烈なものです。

===[/column]

== Hello World サーバを作る

まずHello Worldと表示するだけのサーバを起動しましょう。

「File > New Project」 を選択します。

Project name は、"HelloWorld"とします。

出来上がった"HelloWorld"プロジェクトを右クリックし、"New > File"

ファイル名を helloworld.py とします

画面右側に以下を入力します。

//emlist[hello_world][helloworld.py]{
// -*- coding: utf-8 -*-
import webapp2

class MainPage(webapp2.RequestHandler):
    def get(self):
        self.response.headers["Content-Type"] = "text/plain"
        self.response.write("Hello, World!")

application = webapp2.WSGIApplication([
    ("/", MainPage),
], debug=True)
//}

一つ注意点があります。
各行冒頭の空白(インデント)を正しく入力してください。
Python言語ではインデントがJavaで言う中括弧({と})の役割を果たします。
Python言語の機能などについては次節にまとめますので、とりあえず
ここではそのまま入力してください。

またファイルを作ります。今度は"app.yaml"を作成します。

//emlist[app_yaml][app.yaml]{
application: your-app-id
version: 1
runtime: python27
api_version: 1
threadsafe: true

handlers:
- url: /.*
  script: helloworld.application
//}

2つのファイルを保存したら、以下を実行します。

//cmd{
> ./dev_appserver.py HelloWorld
}

すると、画面に Starting server "default" running at: http://localhost:8080 といった文字列が出てきます。
このURLをコピーして、ブラウザで表示させてください。

//image[gae-result-1][結果]{
//}

晴れて自分のWebサーバが出来ました。

なお、「Starting admin server at: http://localhost:8000」と言った文字列で、
ポート番号が異なるURLがもうひとつ表示されているはずです。
こちらは「管理用」のサーバです。
興味があれば覗いてみましょう。

//image[gae-result-2][管理画面]{
//}

=== メッセージを変える

Hello Worldでは味気がないので、別の文字列を出力してみましょう。
@<list>{hello_world}を以下のように変えます。

//emlist[hello_world_ja][例えばこのように変える]{
        self.response.write("こんにちは、あたち！")
//}

=== 隣の人に見えるようにする

http://localhost:8000 というのは、言ってみれば「特殊な」URLで、
他人からは見ることができません。

14-1-2「IPアドレスとは？」であったipconfig(Mac等ではifconfig)コマンド
を用いて自身のIPアドレスを確認します。
注意点として127.0.0.1といったIPアドレスではないもうひとつのIPアドレスを
探してください。

//cmd{
> ipconfig
...
10.0.90.181
...
//}


このようになっていたら、

//cmd{
> ./dev_appserver --host (ここに自分のIPアドレスをいれる) HelloWorld
//}

