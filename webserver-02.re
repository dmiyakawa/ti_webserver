= 自分のWebサーバを作ろう

//lead{
本章では視点を変え、
Androidアプリにデータを提供するWebサーバの側の技術を学びます。
実際にデータを配信するサーバを実装してみましょう。
//}

== 本章全体で学ぶこと

 * Webサーバとは？
 * シラバスアプリのサーバの実装と拡張
 * Webサーバと通信する上でAndroid開発者が知っているべきこと

この節で出てくるキーワード

 * Webサーバ
 * IaaS, PaaS, SaaS
 * Google App Engine (GAE)
 * Python
 * HTTP, HTTPS
 * HTTPのGETリクエストとPOSTリクエスト
 * クロスサイト・スクリプティング (XSS)

第10章「ユーティリティによる実践」で実装した
「シラバスアプリ」などで、Webサーバにアクセスするプログラムは
既に登場しました。
そのときは、一般に公開されている商用のWebサーバや、
予め講師が準備したWebサーバへアクセスするだけでした。

本章では「シラバスアプリ」のデータを提供するWebサーバの実装を行います。
Androidアプリとサーバをセットで考えることで、
サーバ側で何が起きているかを理解しつつ、
Androidアプリ開発でサーバと通信する際に
意識するべき点について、学習しましょう。

本章では、第13章「ネットワークプログラミング」
で学んだ概念を前提としています。
一度復習してから以降を読んだほうが、理解は深まるでしょう。

== Webサーバとは？

Webブラウザを用いてWebページを閲覧するのが当たり前の時代ですから、
Webという言葉自体についてはイメージが付くかもしれません。

「Webサーバ」の「サーバ(Server)」という単語は、
「ジュースサーバ」といった表現で日常会話でも登場します。
IT分野以外では、「給仕する人、接客係」といった意味で使われる単語です。

//image[juiceserver][ジュースサーバ。画像の版権確認なし。要確認]{
//}

堅苦しく言えば、サーバは「資源を要求する側に、その資源を提供」します。
「資源を要求する側」を、「クライアント」と呼びます。
ジュースサーバの場合、クライアントが要求する「資源」はジュースでした。

資源を補充されることもあります。
ジュースサーバでは、ジュースがなくなったら、誰かが継ぎ足します。

Webサーバの場合にも、資源を求めてWebブラウザやAndroidアプリがアクセスします。
あるいは資源を補充するためにもアプリがアクセスすることがあります。
GMailのようなWebメールと呼ばれるようなサービスでは、どちらも行われます。

ごく簡単に言えば、蜘蛛の巣(英単語でWeb)のようにコンピュータの
ネットワークが世界規模で張り巡らされていて、
WebブラウザやAndroidアプリといった「クライアント」と「サーバ」が
容易に接続出来るのがWebの特徴です。


=== 自分のサーバが必要な場面

第10章や第14章で、インターネット接続を用いるAndroidアプリは登場しています。
サーバを自分で用意することはありませんでした。

自分でサーバを準備する必要があるときとは、どういうときでしょうか。

Androidアプリがサーバと通信する場合、いくつかの選択肢がありますが、
常にサーバを実装しなければいけないわけではないどころか、
多くのケースでは自力で作成する必要は「ありません」。

おそらくアプリ開発を行う際に頻繁に発生するのは、
サーバは何らかのサービスプロバイダから提供されていて、
ユーザは単にそのWeb上のAPIを使うというケースです。

例えばGoogle Play Servicesの機能を使う場合がこれにあたります。
この場合、ユーザはサーバの実装については直接手をつけることはできません。
指定されたサーバから提供されるAPIをそのまま使います。
あるいはそのAPIのやりとりを外部ライブラリとして利用します。

こういう場合には、単にインターネットというものがあって、
そこに誰かがサーバを立てていて、そのサーバの使い方についても
決められていて、それを使うことになります。

本章ではこれと真逆の「サーバとアプリをセットで作る」ケースを考えます。
このとき期待されているのは「サービス全体の設計」です。
Androidアプリ単体のことだけでなく、
インターネット上のサーバについてもある程度自分で面倒を見ます。

こういったケースはAndroidアプリ開発でも目新しいことではない点については、
注意してください。
特に、新しいサービスを世界に展開するような企業でアプリ開発をする場合には、
サーバ側の知識や、アプリとサーバ間を通信する際の知識が、
一定レベルで必要になります。

先日食事をご一緒したある別のAndroid開発者の方は、
もちろんAndroidアプリを開発しつつも、
すでに存在しているWebサービス実装をAndroidアプリと緊密に連携させるため、
Webサーバ側のインターフェースの実装も、同時にされているとのことでした。

=== シラバスアプリのサーバを作るケースを考える

第10章「ユーティリティによる実践」で、シラバスアプリを実装しました。
シラバスアプリは、Webサーバに保存されたJSONデータを
Volleyというライブラリで非同期にダウンロードし、
その内容をListViewに表示しました。

//image[syllabus-screenshot][]{
//}

この時に使用したデータ取得URLは、
@<href>{https://dl.dropboxusercontent.com/u/1088314/tech_institute/2014/syllabus.json}
です。
このURLは、Dropbox@<fn>{about_dropbox}と呼ばれるファイル共有サービス
上で講師が公開している静的ファイルでした。
つまり、変更するには直接そのファイルを修正する必要があります。
ブラウザからシラバスの内容を追加・編集出来る方が、
管理上の都合は良いでしょう。

本章では、この「シラバス」データを管理するWebサービス、
「シラバスサービス」のWebサーバを実装することを考えます。
主に行うのは以下の実装です。

 * JSON形式とHTML形式による講義データの配布
 * Webサーバへのデータ追加
 * セキュリティの確保

想像できる全ての機能のうち、コアとなるいくつかの機能を集中的に作ることで、
Webサーバの実装方法を学びます。
これにより、AndroidアプリとWebサーバの両方をセットで作るための勘所を
理解できるようになるはずです。


=== Webサーバの実装を支える技術

Webサーバを0から作るのは大変であるため、
色々な技術や商用サービスを組み合わせて構成するのが一般的です。
世の中でWeb開発の話題で、しばしば名前に挙がる技術やサービスをいくつかあげてみます。
聞いたことがある名前が含まれているかもしれません。

 * Google App Engine (今回利用するもの)
 * Amazon Web Services (略して「AWS」)
 * Virtual Private Server (略して「VPS」)
 * Ruby on Rails, Python Django, Heroku, etc.
 * Apache Tomcat (Jetty, JBOSS, etc.)
 * Microsoft Azure
 * Parse
 * その他その他

これらは提供するサービスの性質に応じて「IaaS」「PaaS」「SaaS」などと
分類分けされて紹介されることがあります。
「どのくらいOSそのものの機能に近いか」で分けされることが一般的です。
本節はサーバを含めた「ITインフラ」の講習ではないので、
この辺りの事情の説明はこれ以上行いません。

本章では数ある中で、Googleが提供するWebアプリケーションフレームワークの
一つである、Google App Engineを選択しました。

=== Google App Engineとは？

Google App Engine(略してApp EngineもしくはGAE)は、
Googleが提供するWebアプリケーションフレームワークの一つです。

これは上述した中では「PaaS」(Platform as a Service)のカテゴリに入ります。
Webサーバの機能を自分で作り込める一方、LinuxやWindows ServerといったOSに
ついての深い知識を必要としません。

GAEでは現在Java, Python, PHP, Go言語の4種類を使うことが出来ます。
本節ではこの中でPython言語による実装を用います。
@<fn>{why_not_java}

//footnote[why_not_java][言語がAndroidと共通であることからJavaによる実装を採用するという選択肢も検討しましたが、準備のためだけに逆に多くの時間を費やしてしまうため、本講義では採用を見送ります。]

PythonはJavaと同様世界的にもよく利用されているプログラミング言語です。
プログラミング言語の人気を図る指標ではしばしば上位を占め、
マサチューセッツ工科大学(MIT)やカリフォルニア大学バークレー校といった
コンピュータ科学教育で有名な大学でプログラミングの入門コースで採用されるなど、
プログラミング教育でも豊富な実績があります。

=== ここからの演習の流れ

まず、Webアプリの開発環境を準備しつつ、PC上でローカルサーバを起動します。
このサーバは自分しか見ることが出来ないため、安全に色々試すことができます。

最初に作るのはHello Worldサーバです。
「Hello World!」とだけブラウザで表示されることだけを確認します。
自動生成されるアプリケーションプロジェクトを用いるため、
ここまではプログラミングは一切発生しません。

次に、プログラミング言語Pythonを用いて少しずつサーバを修正します。
Pythonの機能はJavaから予想できるものも多いため、
本編では細かい説明は省きます。

Pythonそれ自体の説明を別の節(？)に分けて教科書に収録してあります。
Python言語が不安な方は、この時点でインストールする
Python開発環境PyCharmをインストールの上、
その段階で一度、次節のPython入門を試すと、理解がスムーズに成るかもしれません。
なお本演習はPythonやGAEの習得が目的ではないので、
Pythonや深く理解する必要がないよう構成されています。

開発を続けながら、Googleが提供するGAEの「本番サーバ」へのデプロイの方法を説明していきます。
「本番サーバ」にアップロードされた実装は、世界中に公開されます。
@<fn>{about_payment}

//footnote[about_payment][GAEはユーザの同意なしに自動的に課金を行ったりしないのでその点を今心配する必要はありません。後述します。]

本演習では、実際に世界中にサーバを公開します。
公開したからといって即座に問題になるわけではありませんが、
Googleアカウントによるログインを除いて、
個人情報やプライバシーに関わる情報をサーバに含めるのは避けたほうが無難です。


== WebサーバでHello World!
=== Python実行環境のインストール

GAEを用いてWebサーバを開発するために、
まずPython実行環境をPCへインストールします。
インストールするバージョンはPython 2.7.8です。

以下のURLからWindows向けPythonインストーラを取得します。
3で始まるPythonバージョンは今回はインストールしないでください。

 * @<href>{https://www.python.org/downloads/release/python-278/}
 ** (@<href>{https://www.python.org/ftp/python/2.7.8/python-2.7.8.amd64.msi})

//image[install-python-1][@<href>{https://www.python.org/downloads/release/python-278/}のスクリーンショット]{
//}

//image[install-python-add-path-1][「Add python.exe to Path」をクリックし、表示された2つの選択肢のうち"Will be installed on local hard drive"を選ぶ]{
//}

//image[install-python-add-path-2][×印がなくなったら「Next」]{
//}

インストールが終了したら、確認のため、
コマンドプロンプトで「Python」と入力して、
Pythonの対話型環境が起動するかを確認してください。

=== Google App Engine SDKのインストール

本節ではGoogle App Engineに複数ある言語実装の中でも、Python版を採用します。

 * https://developers.google.com/appengine/downloads

「Google App Engine SDK for Python」をクリックします。

//image[install-gae-1][]{
//}

適切なOSのインストーラを選びます。

//image[install-gae-2][]{
//}

ダウンロード後、インストーラを起動します。
Python 2.7をSDKインストーラが認識していることを確認した上で、
Nextボタンをクリックします。

//image[install-gae-python-checked][]{
//}

//image[install-gae-tos][]{
//}

表示されるチェックボックス3つの全てにチェックが入っていることを確認します。

//image[install-gae-all-checked][]{
//}

最後にInstallボタンをクリックします。

//image[install-gae-do-install][]{
//}

Install中、ダウンロードしたソフトウェアを信用するかどうかを
確認するシステムダイアログが表示されることがあります。
このときは、「確認済みの発行元」が「Google Inc」となっていることを
確認した上で、「はい」を押します。

インストールが成功したら、「Run Launcher」ボタンを押した後、
「Finish」ボタンを押します。

//image[install-gae-done][]{
//}

「Google App Engine Launcher」というWindowが起動すれば、
GAE SDKのインストールに成功しています。

//image[gae-installed][]{
//}

===[column] ユーザフォルダに日本語が含まれる場合の追加作業。

このコラムの作業が必要なのは「C:\ユーザー\(ユーザ名)」
の「ユーザ名」部分が日本語になっている場合だけです。
日本語を含むフォルダの扱いをGoogle App Engineが正しく処理できないため、
この作業が必要です。

そのような例を@<img>{japanese_user_name}に示します。
この現象は、Windows 8の初期設定で、
日本語名のユーザを作成した際に発生します。

//image[japanese_user_name][ユーザ名が「大輔」で日本語になっている]{
//}

このケースに当てはまる場合、さらに以下の作業を行なってください。

 * 「C:\GoogleAppEngine」のような日本語を含まないフォルダを作成する。
 * TMP環境変数に上記のアドレスを指定する。
 ** 環境変数は第4章「開発環境セットアップ」p61でPATHを指定した時と同じ方法で設定します。今回、変数名は「TMP」とし「変数値」を上記のフォルダとします。
 * プロジェクトを上記フォルダに作る。

この作業を行わずにGAEの各種作業を行おうとすると、
以下のような問題が発生します。

 * テスト用のsqlite DBが作成できないという旨のエラーが発生する。
 * GAEが動作しなくなる。
 * GAEが起動しなくなる。

特に、最後の挙動に至った場合、PCの再起動等では復帰しません。
そのPCの「ユーザ」フォルダに「Google」というフォルダができているはずですので、
そのフォルダの中にあるGAE Launcherが作成する一時ファイルを全て削除すると、
復帰します。

===[/column]

=== ローカルサーバでHello World!

ここで、"Hello world!"と表示するだけのサーバを作成しましょう。

Google App Engine Launcherで「File >> Create New Application」を選択します。

 * Application Name を「helloworld」等に設定します。
 ** ここでは一部の文字、例えば半角大文字は使用できません。
 ** 半角英数字だけで構成するのがベストです。
 * Parent Directory を「デスクトップ」に設定します。
 ** 日本語ユーザである場合は、コラムの注意を参考にして、新しく用意したフォルダを設定します。

//image[create-helloworld][]{
//}

作成できたらそのプロジェクトを選択し「Run」をクリックします。

//image[run-helloworld][]{
//}

成功すると、プロジェクトの左にある小さなアイコンが
緑色の「再生ボタン」のようになります。

//image[running-helloworld][]{
//}

この状態でブラウザから「http://localhost:8080」を開きます。

"Hello world!"と表示されたら、成功です。

//image[seeing-helloworld][]{
//}

8000番で、このプロジェクトの管理用Webページを開くこともできます。
@<fn>{about_tcp_port}

//footnote[about_tcp_port][サーバが利用するTCPポート番号は、複数のプロジェクトを生成すると変化する可能性があります。その場合、以降の説明の数字を変更してください。ポート番号そのものについての説明は14-1-5「ポート番号とは？」を参照してください。]

このサーバは現在「実行中」の状態です。
これを停止するには、GAE Launcherから「Stop」ボタンを押します。
「再生」ボタンが黒い丸印になれば、サーバプロセスが停止しています。

//image[stopped-helloworld][]{
//}

入力したURLについて説明します。

 * http はどのようにサーバに接続するかを決めたもの(スキームと呼ばれる)。
 * localhostは「自分自身」で、つまりPCのことで、「ローカルサーバ」と呼んでいます。
 * TCPポートの8080番でサーバを作成している、という意味になります。

この"Hello world!"を表示するまでに、プログラムを1行も書く必要がありませんでした。
ちょっと物足りなすぎです。

=== PyCharm Community Edition のインストール

ここから、Pythonでプログラムを書いてサーバを拡張するのですが、
Windows付属のメモ帳は文字コードの特に改行コードの処理などに難点があるため、
Pythonの開発には若干不適切です。
また、PythonでもEclipseのような統合開発環境(IDE)を利用できると、
文法チェックなど、多くの面で便利でしょう。
@<fn>{ok_with_hidemaru}

ここでは、PyCharmと言う商用IDEの無料版「PyCharm Community Edition」をインストールします。
@<fn>{about_pydev}

//footnote[ok_with_hidemaru][他に『秀丸』等の高機能なエディタがあればそれを利用しても構いません。その場合はPyCharmについての以降の説明は無視してください。]

//footnote[about_pydev][EclipseをPython開発環境として利用するためのPyDevというEclipseプラグインが存在します。しかし、本講義において環境にインストールされているJavaのバージョンは若干古いため、そのままでは利用できません。Javaのバージョンを7にすればPyDevの動作要件を満たしますが、Android開発環境を壊してしまう可能性もあるため、本演習では採用しません。]

@<href>{http://www.jetbrains.com/pycharm/download/}をブラウザで開き、
右側の「Community Edition」を選択してダウンロードし、
インストーラを実行し、指示に従います。

//image[pycharm-select-page][]{
//}

途中で表示される「Create Desktop shortcut」
(デスクトップにショートカットを作成する)
のチェックボックスをつけておくと良いでしょう。

//image[pycharm-installer-create-desktop-shortcut][]{
//}

最後に「Run PyCharm Community Edition」(PyCharm Community Editionを起動する)
にチェックボックスを入れて「Finish」を押します。

//image[pycharm-installer-run-pycharm][]{
//}

初回起動時に@<img>{pycharm-complete-installation}のような表示が出ます。
今回はそのまま「OK」を押します。

//image[pycharm-complete-installation][]{
//}

さらに@<img>{pycharm-first-configuration}のような画面が出ます。
ここで、Eclipse風のキーバインドにしたい場合は
「Keymap scheme」を適切に変更します。
なお、この設定は後述する通り、変更可能です。

//image[pycharm-first-configuration][]{
//}

最後に@<img>{pycharm-welcome}のような画面が出れば
PyCharmの起動が成功しています。

//image[pycharm-welcome][]{
//}

既にhelloworldプロジェクトを作っていますので、ここではそれを
PyCharm上で編集してみましょう。
「Open Directory」を選択し、helloworldプロジェクトが存在するフォルダを開きます。

//image[pycharm-helloworld][]{
//}

@<img>{pycharm-helloworld}のような画面が出れば、
プロジェクトを開けたことになります。

なお、同時に@<img>{pycharm-tip-of-the-day}のような
ダイアログが表示されることがあります。

//image[pycharm-tip-of-the-day][]{
//}


今回は左下の「Show Tips on Startup」(起動時にTips画面を表示する)のチェックを
外して、「Close」ボタンを押します。

Eclipseと似た外見をしていますが、全く一致した使い勝手というわけではありません。
以下に、すぐに使い勝手を改善出来るヒントを示します。

 * 起動時にkeymapを変更していない場合「File >> Settings >> Keymap」でKeymapsを「Eclipse」に変更すると、Eclipse風になります。@<img>{eclipse_keymap}ただし全てがEclipseと同じになるわけではありません。
 * 「File >> Settings >> (左画面IDE Settingsの) Editor >> Editor Tabs >> Mark modified tabs with asterisk」のチェックを入れておくと、Eclipseの時と同様保存されていないファイルのタブに*(スター、もしくはアスタリスク)が付きます。@<img>{asterisk}

//image[eclipse_keymap][]{
//}

//image[asterisk][]{
//}

Pythonでプログラミングする準備が整いました。

プログラミングに自信があればこのままWebサーバの実装を進めても構いませんし、
一旦次節に進み、Pythonの基礎を学んでも構いません。

=== Webサーバのメッセージを変える

Hello Worldでは味気がないので、別の文字列を出力してみましょう。
作成したプロジェクトの"Hello world!"部分を、
PyCharmのPythonエディタで変更します。

PyCharmの画面左、「helloworld」となっている部分をクリックし、
展開したファイル一覧を確認してください。

 * app.yaml
 * favicon.ico
 * index.yaml
 * main.py

GAEの初期プロジェクトはAndroidと比べるとシンプルです。
それぞれのファイルの意味を説明しましょう。

 * main.pyはWebアプリケーション本体のソースコードです。
 * app.yamlとindex.yamlは、Androidで言えばAndroidManifest.xmlに相当するような、アプリの設定ファイルです。
 * favicon.icoはブラウザ上で各タブに表示されるアイコンのためのファイルです。

favicon.icoは、今回扱いません。
主に他の3つのファイルを編集することで機能を追加していきます。

main.pyのPythonコードは実質だけです。
これだけで"Hello world!"を表示するWebサーバになります。

//list[all_source][GAEを用いたWebサーバの全文]{
import webbapp2

class MainHandler(webapp2.RequestHandler):
    def get(self):
        self.response.write('Hello world!')

app = webapp2.WSGIApplication([
    ('/', MainHandler)
], debug=True)
//}

現段階で把握しておきたいのは以下のことです。

 * Pythonではインデントがブロックの範囲を示します。勝手にインデントを使うことが出来ません。
 ** 代わりにブロックの開始と終了を示す中括弧が存在しません。
 * 「def function_name(args):」という表現で関数(メソッド)を宣言します。
 ** 閉じ括弧の後のコロン「:」は必須です。
 * 「webapp2.WSGIApplication()」に渡されるリストは「WebサーバのどのパスをどのHandlerが処理するか」を示したものです。
 ** []で囲われており、Javaの配列に見えますが、どちらかと言えばArrayList等に近い、より柔軟なデータ構造です。
 * get() は HTTP GETを、post()はHTTP POSTを処理する関数になります。
 ** 今回、GETはデータを取得するのに使い、POSTはデータを追加・更新するのに使います。

単に「Hello world!」という文字列を変更したいだけなら、
Pythonの深い部分を知る必要はありません。
@<list>{hello_world}の行を変更すればよさそうです。

//list[hello_world][このような行が見つかる]{
        self.response.write("Hello world!")
//}

helloworld.pyの内容を変更してファイルを保存後、
GAE Launcherで再びローカルサーバを起動して変更を確認してみましょう。

=== 世界にHello World!

Google App Engineの便利なところは、
この時点ですでに、世界へWebアプリケーションを公開する準備がほぼできていることです。
Googleの本番サーバにアップロードし、世界にアプリケーションを公開してみましょう。
@<fn>{be_careful_about_privacy}

//footnote[be_careful_about_privacy][公開したサーバは講義室内に限らず、インターネット上のユーザから見ることができます。今回の演習では、開発段階から個人情報やプライバシーに関わる情報は含めないほうが良いでしょう。]

公開するには、Googleアカウントで開発者登録をした上で、
Webアプリのための「アプリケーションID」を取得します。
以下のURLをタイプし、講義に用いるGoogleアカウントでログインします。

@<href>{https://appengine.google.com/}

Google App Engineの利用規約が表示された場合は内容を確認し、
「I accept these terms」をチェックした上で「Submit」を押します。

//image[gae-tos][]{
//}

次画面で、"Create Application"をクリックします。

//image[gae-web-console][]{
//}

Application Identifierとして「世界中で唯一」の名前を一つ設定します。
「ti-(受講生番号)」といったものでも構いませんし、別のものでも構いません。
他の人が利用している名前は選択できません。
これはそのまま公開するサーバ名になるので、
おかしな名前も避けたほうが良いでしょう。

アプリケーションIDを考えたら、
右側の"Check Availability"ボタンで利用できるかを確認し、
実際にそのアプリケーションIDを使用出来ることを確認します。

最後に、最下段の"Create Application"ボタンを押します。

//image[gae-ti-dmiyakawa][]{
//}

成功すると次のような画面がでます。

//image[gae-project-creation-successful][]{
//}

"Application Registered Successfully"と出たら、PyCharmでapp.yamlを開き、
最初の行の"application"の右側の文字列を自分が選択したものに変えます。

//emlist[]{
application: (ここを先ほど選択したIDに変更する)
//}

GAE Consoleで「Deploy」ボタンを押すとユーザ名とパスワードを聞かれます。
ここで、ユーザ名はGoogleアカウント名、
パスワードはGoogleアカウントのパスワードか、
仮に2段階認証プロセスを有効にしている場合は、
アプリケーション固有パスワードを使用します。

ログで「Deployment successful」と出たら成功です。

「@<href>{https://(自分が生成したID).appspot.com/}」へアクセスしてみましょう。
なお、URLの冒頭は「http」ではなく「https」とすることをおすすめします。
理由は座学パートで説明しています。

隣の人にアプリケーションIDを教えてもらい、
他の人のWebサーバを見せてもらってもよいでしょう。

===[column] 2段階認証とアプリケーション固有パスワード

開発者ならば、出来れば2段階認証について理解し、積極的に活用してほしいと思います。
それによって、結果的にアプリを利用するユーザをも守ることになるからです。

2段階認証については、例えば@<href>{https://www.google.com/intl/ja/landing/2step/}などを見るのが、良いでしょう。

2段階認証を用いる場合、GAE Consoleの「Deploy」ボタン押下時に
要求されるパスワードに自分のGoogleアカウントのパスワードを入れてはいけません。
Googleアカウントを用いてアプリケーション固有パスワードを準備する
必要があります。

まず以下のURLへアクセスします。

@<href>{https://security.google.com/settings/security/apppasswords}

最下段の「端末を選択」で「その他(名前を入力)」を選び
例えば「Google App Engine」と入力し、「生成」を押します。

4文字が空白で4つ区切りになった、合計で16文字のパスワードが表示されますので、
これを使います。
本演習が終わった後、
不要であればこのアプリケーション固有パスワードを削除することで、
GAE Consoleが万が一乗っ取られた状況に対処することができます。

なお、OAuth2という技術を用いることで、
アプリケーション固有パスワードなしに
GAEの本番サーバへデプロイする方法もあるのですが、
GAE Consoleが対応していないので省略します。

===[/column]

=== 講義室内でHello World!

同じWi-Fiネットワーク内の他の人にだけサーバを見せたい、
というケースがあるかもしれません。

まず、14-1-2「IPアドレスとは？」であったipconfig(Mac等ではifconfig)コマンド
を用いて自身のIPアドレスを確認します。
注意点として127.0.0.1といったIPアドレスではないもうひとつのIPアドレスを
探してください。

//cmd{
> ipconfig
...
10.0.90.181
...
//}

このようになっていたら、GAE Launcherの「Edit >> Application Settings..」
を開き、「Extra Command Line Flags」に「--host=10.0.90.181」と
追加します。IPアドレス部分は状況に応じて変更してください。

//image[specify-hostname][]{
//}

このあとに起動すると、@<href>{http://(自分のIPアドレス):8080/}
でローカルサーバを起動します。

このようにホスト側IPアドレスを指定してサーバを起動すると、
「http://localhost:8080」ではWebページは
表示されなくなります。
代わりに、指定したIPアドレスを含むURLでだけ、表示できます。

このIPアドレスとそのURLへは、Wi-Fiを共有している人たちの間でだけ
共有することができます。
@<href>{http://(自分のIPアドレス):8080/}を隣の人に教えれば、
隣の人のPCやAndroid端末から、そのWebページを見られるようになります。
この方法は、世界中にサーバを公開せず、近くの人と挙動を確認しあうのに使えます。
後述するAndroidアプリとも、Wi-Fiネットワークの範囲内であれば、通信を行えます。

教室や自宅で現在利用されている多くのネットワークでは、
NAT(Network Address Translation)と呼ばれる技術によって、
インターネットと内部のネットワークが分断されています。
そういった状況でのIPアドレスはインターネットからは見えないため、
「世界に公開」していることにもなりません。


=== ログ出力方法

PythonでもAndroidにおけるLogCatと似たようなログ機構を実装できます。
以下にloggingモジュールを用いた"Hello world!"の例を示します。

//emlist[loggingを用いた例]{
import webbapp2
import logging  # この行を追加

class MainHandler(webapp2.RequestHandler):
    def get(self):
        logging.info('Hello world from logging!')  # この行を追加
        self.response.write('Hello world!')
...(以下省略
//}

今回のサーバ実装ではlogging.info()だけ使えば十分です。
そのモジュールを用いるため、Javaと同じように「import logging」
という命令を予め入れておく必要があります。

もしPythonのloggingについて詳細に学びたいは、まず
@<href>{http://docs.python.jp/2/howto/logging.html#logging-basic-tutorial}
から読むのが良いでしょう。

=== 本番サーバの状況を確認する

GAE Launcherには「Dashboard」というボタンもあります。
このボタンを押すと、本番サーバの管理者画面をブラウザに表示してくれます。
ここから本番サーバでのログ出力も見ることができます。

//image[gae-dashboard][]{
//}

=== GAEのリソース割り当てを確認する

Google App Engineは小規模なサーバを起動する分には無料で利用できます。
より正確には、それぞれのアプリに無料で利用できる一日あたりの割り当て
が決まっており、その範囲内のアクセスであれば料金はかかりません。
割り当てとその消費量はGoogle Developers Consoleの
「計算処理 >> App Engine >> 割り当ての詳細」を見ることで確認ができます。

//image[gae_usage][割り当て]{
//}

第三者からの大量のアクセスがあったりした際に、
この割り当てを使い尽くしてしまう可能性があります。
その場合、ユーザからはサービスが停止するように見えます。

今回の実験的利用を越えた、相対的に大きな負荷が予想される場合には
クレジットカードを登録し、アクセスに応じた課金を受け入れる必要があります。

本章ではGAEの内部でどのようにそれぞれの処理が実装されるかについての説明を
省略しています。
今回のようなシラバスアプリの範囲であれば、全く問題なく動作するはずですが、
大規模利用する際には問題が起こる可能性があります。

GAEにも興味を持たれた方は一度公式のドキュメントに目を通し、
今回無視した背後の仕組みを今一度理解した上で利用することをお勧めします。


== シラバスアプリのサーバをつくろう
=== シラバスアプリでやりとりするデータ構造を考える

「ユーティリティによる実践」で、
JSON形式のデータをAndroidアプリで利用する方法を学びました。
その時利用したJSON取得用URLは
@<href>{https://dl.dropboxusercontent.com/u/1088314/tech_institute/2014/syllabus.json}
でした。

//footnote[about_dropbox][https://www.dropbox.com/]

本節以降では、このURLに対応するダウンロード用のWebサーバと、
そのサーバが返すデータを変更するためのWebページを実装します。

まず確認したいのは、
「どのような方法でAndroidアプリとWebサーバが通信するか」です。
このルールがアプリとサーバ側で違っていると、通信は出来ません。

まず、上記のJSONデータをブラウザで見てみましょう。
大変読みづらいですが、本質的には@<list>{json_data}で示されるような構造になっています。（分かりやすい部分を抜粋しています）

//list[json_data][JSONフォーマットを整形したもの、一部]{
{
  "course": [
    ...
    {
      "No": 3,
      "date": "2014-07-03T07:00:00.000Z",
      "day": "木",
      "time": "19：00-21：00",
      "chapter": "3章",
      "title": "プランニング（1）",
      "teacher": "嶋　是一",
      "supporter-1": "金澤創平",
      "supporter-2": "あくやん",
      "supporter-3": "中川茂樹",
      "supporter-4": "宮川大輔",
      "supporter-5": "",
      "detail": ""
    }, 
    ...
    {
      "No": 65,
      "date": "2014-12-10T08:00:00.000Z",
      "day": "水",
      "time": "19：00-21：00",
      "chapter": "26章",
      "title": "レビューとブラッシュアップ（2）",
      "teacher": "嶋　是一",
      "supporter-1": "下川敬弘",
      "supporter-2": "武藤繁夫",
      "supporter-3": "中川茂樹",
      "supporter-4": "",
      "supporter-5": "",
      "detail": ""
    }
  ]
}
//}

これに加えて、今度はEclipse上でシラバスアプリの実装を見てみましょう。

シラバスアプリでは、JSONで指定されたこれらのデータの中から、
以下の4つの情報だけを使っています。

 * date 講義が行われる日付
 * title その講義のタイトル
 * teacher 講師の名前
 * detail 講義の詳細 (サンプルのJSONファイルでは空でした)

それ以外のデータは全く使っていないようなので、
今回のWebサーバではこれらのデータだけを扱うことにしましょう。

言い換えると、Webサーバは講義数分だけこの4種類のデータを
持つJSON形式のデータを返答すれば良いのです。

=== 自分のWebサーバから、自動生成したJSONデータを返す

まず、固定のデータをプログラムが
自動的に生成するようにし、それをクライアント側に
返答するWebサーバに変更してみます。

//emlist[こーど]{
# 以上省略
from datetime import datetime, timedelta
from json import dumps
import logging, pprint

class MainPage(webapp2.RequestHandler):
    def get(self):
        self.response.headers['Content-Type'] = 'application/json; charset=UTF-8'
        courses = []  # 空のリスト
        for i in xrange(10):  # 0から9までループ
            single_course = {'date': str(datetime.today() + timedelta(days=i)),
                             'title': 'たいとるその{}'.format(i),
                             'teacher': '第{}代目 高橋名人'.format(i),
                             'detail': ''}
            courses.append(single_course)  # リストの末尾に追加している
        # 注意: 辞書のキー(左側)は単数形 course です。sは要りません
        data = dumps({'course': courses}, ensure_ascii=False)
        logging.info(pprint.pformat(data))
        self.response.write(data)
# 以下省略
//}

ここでは、Python言語で生成したデータ構造を(json.dumps()関数で)JSONの文字列
として返しています。
これらはHTTPと呼ばれるプロトコルによって、
クライアント、すなわちブラウザやAndroidアプリに返されます。
サーバが返すデータがどのような形式のデータかを、
ブラウザやAndroidアプリは事前に理解する必要があるため、
「Content-Type」という情報を指定しておく必要があります。
この点は後述します。

logging.info()を指定しているので、
ログにはJSONフォーマットする前のデータが表示されます。


=== 生成されたJSONデータをブラウザで確認する。

ローカルサーバ(@<href>{http://localhost:8080})から、
JSON形式のデータがダウンロードできることを、
ブラウザを用いて確認してください。

#@# TODO: サーバの結果

表示されるデータが問題無さそうであれば、
作成したサーバアプリケーションをGoogleの本番サーバにアップロードします。
アップロードする方法は既に説明しましたので、省略します。

本番サーバのURLへブラウザでアクセスし、
やはり同様のJSONデータを取得できることを確認します。

ここまでで、世界に公開された状態で
講義表データを返すWebサーバが完成しました！

=== Puttyを使って応答の中身を見てみる

サーバとブラウザの間のやりとりができていることは分かりましたが、
具体的にどのようなやり取りをしているのかはこのままでは分かりません。

こういったとき、実際に双方がやりとりしている「通信プロトコル」を解析するのが有用なことがあります。
「ネットワーク」において一度Puttyを利用していますので、
ここでもう一度、使ってみることにしましょう。

#@# TODO: putty の結果

HTTPというプロトコルによってクライアントであるPuttyで「GET」リクエストを
サーバに送ると、今回は自分で作成したサーバがContent-Typeを指定した上で、
JSONデータを返している様子がわかります。

HTTPレスポンスのほとんどの部分はGAEが用意してくれているため、
今回の範囲ではGAEのAPIを通してPythonのデータ構造(self.response.headersやself.response.write()など)に渡すだけで処理が終わります。

HTTPというプロトコルは単純そうに見えて意外に複雑な部分が多いため、
GAEのような「Webアプリケーションフレームワーク」という補助的なAPI
を使ってWebサーバを作ることが一般的です。

==== Content-Type / GET


=== シラバスアプリにサーバのJSONデータを読み込ませる

早速、シラバスアプリでこのデータを表示させてみましょう。
ここで一旦、Eclipseで作業を行います。混乱しませんように。

シラバスアプリがEclipseに存在することを前提に説明をします。
もし準備ができていない場合には、先にそちらを完成させるか、
GitHubから最新のプロジェクトをダウンロードし、
Eclipseにインポートして準備してください。

#@# TODO: 念の為、次節で、シラバスアプリをインポートする方法を説明します。

シラバスアプリのMainActivity.javaのsyllabusUrlを自分の本番サーバのURLに変更します。

//emlist[しらばす]{
private static final String syllabusUrl = "http://model-shelter-709.appspot.com";
//}

この変更を行った上で、これまでと同様にAndroidアプリをビルド・実行します。

もしサーバとAndroidアプリの通信がうまくいけば、
サーバが自動生成した講義が配信されるようになるはずです。

//image[custom_syllabus][こうなる]{
//}

シラバスアプリ上で、いつまでも「読込中」を示すSpinnerが表示され続ける場合には、
サーバのログ画面ととAndroidアプリのLogCatをよく見て問題を解決してください。

例えば、返答するJSONで"course"となっているべきところが"courses"(sが余計)
となっていた場合には、サーバではエラーは出ず、
LogCatで問題が報告されているはずです。

WebサーバとAndroidアプリを交互に開発する際には、
原因がWebサーバにあるのか、
Androidアプリにあるのかを見極める必要があります。

=== 講義表を保存・変更出来るようにする

ここまでで、Webサーバ上で生成したJSONデータを
Androidアプリに与えることに成功しています。
しかし、このデータはPythonが勝手に生成したもので、
本当に価値のあるデータとは言えないでしょう。

ここからは更に一歩突き進み、人の手で講義データを追加し、
それをブラウザとアプリの両方で表示出来るようにすることにします。

今回は以下の4つのシナリオに対応することにします。

 * 講義一覧をHTMLで表示するトップページ
 * 講義の新規登録ページ
 * JSON形式で講義を表示するページ

講義の編集や削除は演習課題とします。

これを達成するには、GAE上でデータを保存できるようにする必要があります。
具体的には、先ほどのJSONデータと全く同じ4種類のデータをひとまとめにして、
それらを複数保存できる必要があります。

 * date 講義が行われる日付
 * title その講義のタイトル
 * teacher 講師の名前
 * detail 講義の詳細 (サンプルのJSONファイルでは空でした)

この4つのデータを、
言ってみれば「1つのオブジェクト」のようにして保存して欲しいわけです。

先にこのデータ構造をサーバに実装することにしましょう。

#@# TODO: ここにからのデータのあれを入れる。

//emlist[index.yaml (app.yamlではないので注意)]{
indexes:
- kind: Course
  ancestor: yes
  properties:
  - name: date
    direction: desc
  - name: date
    direction: asc
//}

まだコースを登録していないので、JSONは以下のような「空」のコース一覧を
表示します。

//emlist[結果はこうなる]{
{"course": []}
//}

JSONではなくHTMLも表示出来るようにしてみましょう。
具体的にはHTTP GETのquery stringで出力方法(output)を指定出来るようにします。

HTMLの画面では、次に作る「講義の登録」画面のためのHTMLタグも入れてしまいます。

//emlist[講義数0]{
# -*- coding: utf-8 -*-
from google.appengine.api import users
from google.appengine.ext import ndb
import webapp2

from datetime import datetime, timedelta
from json import dumps

def course_list_key():
    return ndb.Key('CourseList', 'default_course_list')

class Course(ndb.Model):
    date = ndb.DateTimeProperty()
    title = ndb.StringProperty()
    teacher = ndb.StringProperty()
    detail = ndb.StringProperty()

class MainPage(webapp2.RequestHandler):
    def get(self):
        course_model_list = Course.query(ancestor=course_list_key()).order(Course.date)
        output = self.request.get('output', 'html')
        if output == 'html':
            lst = [u'<html><body>']
            lst.append(u'<h1>講義数合計: {}</h1>'
                       .format(course_model_list.count()))
            lst.append(u'<ul>')
            for course_model in course_model_list:
                lst.append(u'<li>{}, {}, {}</li>'
                           .format(course_model.title,
                                   course_model.teacher,
                                   course_model.detail))
            lst.append(u'</ul>')
            lst.append(u'<a href="/create">Create</a>')
            lst.append(u'</body></html>')
            self.response.headers['Content-Type'] = 'text/html; charset=UTF-8'
            for line in lst: self.response.write(line)
        else:
            courses = []  # 空のリスト
            for course_model in course_model_list:
                single_course = {'date': str(course_model.date),
                                 'title': course_model.title,
                                 'teacher': course_model.teacher,
                                 'detail': course_model.detail}
                courses.append(single_course)
            self.response.headers['Content-Type'] = 'application/json; charset=UTF-8'
            self.response.write(dumps({'course': courses}, ensure_ascii=False))

application = webapp2.WSGIApplication([
    ("/.*", MainPage),
], debug=True)
//}

この変更でJSONを受け取るURLが変わった点に注意してください。
これ以降、アプリ側で結果を見たい場合は、
Androidのシラバスアプリで指定したURLの末尾に
"?output=json" をつけておく必要があります。

//emlist[しらばす]{
private static final String syllabusUrl = "http://model-shelter-709.appspot.com/?output=json";
//}

HTML上では「講義数合計: 0」の下にCreateというリンクが出てきますが、
ここをクリックしても同じ表示が出てくるだけです。
ちょっと異なるのは、URLに"/create"というパスが表示されたことでしょうか。

データを保存するために、GAEのDatastoreと呼ばれる仕組みを使っています。
指定できるデータ型は以下の通りです。

@<href>{https://developers.google.com/appengine/docs/python/ndb/properties}


=== データを追加するための画面を作る

まだデータが空のままです。
追加できる画面を作ることにしましょう。

行うべきことは大きく分けて2つです

 * フォーム画面を作ります
 * フォーム画面から送信されたデータを受け取り、登録する部分を作ります

一般的に、クライアントからWebサーバにデータを送る際には
GETではなくPOSTリクエストを送ります。

//emlist[えらーがでます]{
# -*- coding: utf-8 -*-
from google.appengine.api import users
from google.appengine.ext import ndb
import webapp2

from datetime import datetime, timedelta
from json import dumps

def course_list_key():
    return ndb.Key('CourseList', 'default_course_list')

class Course(ndb.Model):
    date = ndb.DateTimeProperty()
    title = ndb.StringProperty()
    teacher = ndb.StringProperty()
    detail = ndb.StringProperty()

class MainPage(webapp2.RequestHandler):
    def get(self):
        course_model_list = Course.query(ancestor=course_list_key()).order(Course.date)
        output = self.request.get('output', 'html')
        if output == 'html':
            lst = [u'<html><body>']
            lst.append(u'<h1>講義数合計: {}</h1>'
                       .format(course_model_list.count()))
            lst.append(u'<ul>')
            for course_model in course_model_list:
                lst.append(u'<li>{}, {}, {}</li>'
                           .format(course_model.title,
                                   course_model.teacher,
                                   course_model.detail))
            lst.append(u'</ul>')
            lst.append(u'<a href="/create">Create</a>')
            lst.append(u'</body></html>')
            self.response.headers['Content-Type'] = 'text/html; charset=UTF-8'
            for line in lst: self.response.write(line)
        else:
            courses = []  # 空のリスト
            for course_model in course_model_list:
                single_course = {'date': str(course_model.date),
                                 'title': course_model.title,
                                 'teacher': course_model.teacher,
                                 'detail': course_model.detail}
                courses.append(single_course)
            self.response.headers['Content-Type'] = 'application/json; charset=UTF-8'
            self.response.write(dumps({'course': courses}, ensure_ascii=False))

form_html = u'''\
<html><body>
<h1>コース作成画面</h1>
<form action="/create" method="post">
講義日: <input type="date" name="date"><br>
講義名: <input type="text" name="title" size="40"><br>
講師: <input type="text" name="teacher" size="20"><br>
詳細: <textarea name="detail" rows="4" cols="50"></textarea><br>
<input type="submit" value="送信">
</form>
</body></html>
'''

class CreateCourse(webapp2.RequestHandler):
    def get(self):
        self.response.headers['Content-Type'] = 'text/html; charset=UTF-8'
        self.response.write(form_html)

application = webapp2.WSGIApplication([
    ("/create", CreateCourse),
    ("/.*", MainPage),
], debug=True)
//}

送信するフォームはこれでOKですが、
受け取る側を作っていません。このままでは「405 Method Not Allowed」
というエラーメッセージが出ます。

//image[method_not_allowed][POSTの受け口を作っていない]{
//}

というわけで、受ける側を作ります。

//emlist[今回作る全部]{
# -*- coding: utf-8 -*-
from google.appengine.api import users
from google.appengine.ext import ndb
import webapp2

from datetime import datetime, timedelta
from json import dumps

import logging

def course_list_key():
    return ndb.Key('CourseList', 'default_course_list')

class Course(ndb.Model):
    date = ndb.DateTimeProperty()
    title = ndb.StringProperty()
    teacher = ndb.StringProperty()
    detail = ndb.StringProperty()

class MainPage(webapp2.RequestHandler):
    def get(self):
        course_model_list = Course.query(ancestor=course_list_key()).order(Course.date)
        output = self.request.get('output', 'html')
        if output == 'html':
            lst = [u'<html><body>']
            lst.append(u'<h1>講義数合計: {}</h1>'
                       .format(course_model_list.count()))
            lst.append(u'<ul>')
            for course_model in course_model_list:
                lst.append(u'<li>{}, {}, {}</li>'
                           .format(course_model.title,
                                   course_model.teacher,
                                   course_model.detail))
            lst.append(u'</ul>')
            lst.append(u'<a href="/create">Create</a>')
            lst.append(u'</body></html>')
            self.response.headers['Content-Type'] = 'text/html; charset=UTF-8'
            for line in lst: self.response.write(line)
        else:
            courses = []  # 空のリスト
            for course_model in course_model_list:
                single_course = {'date': str(course_model.date,
                                 'title': course_model.title,
                                 'teacher': course_model.teacher,
                                 'detail': course_model.detail}
                courses.append(single_course)
            self.response.headers['Content-Type'] = 'application/json; charset=UTF-8'
            self.response.write(dumps({'course': courses}, ensure_ascii=False))

form_html = u'''\
<html><body>
<h1>コース作成画面</h1>
<form action="/create" method="post">
講義日: <input type="date" name="date"><br>
講義名: <input type="text" name="title" size="40"><br>
講師: <input type="text" name="teacher" size="20"><br>
詳細: <textarea name="detail" rows="4" cols="50"></textarea><br>
<input type="submit" value="送信">
</form>
</body></html>
'''

class CreateCourse(webapp2.RequestHandler):
    def get(self):
        self.response.headers['Content-Type'] = 'text/html; charset=UTF-8'
        self.response.write(form_html)

    def post(self):
        date_str = self.request.get('date')
        if date_str:
            date = datetime.strptime(date_str, '%Y-%m-%d')
        else:
            date = datetime.today()
        title = self.request.get('title')
        teacher = self.request.get('teacher')
        detail = self.request.get('detail')
        logging.info(u'Saving "{}", "{}", "{}", "{}"'
                     .format(date, title, teacher, detail))
        new_course = Course(parent=course_list_key(),
                            date=date,
                            title=title,
                            teacher=teacher,
                            detail=detail)
        new_course.put()
        self.redirect('/')

application = webapp2.WSGIApplication([
    ("/create", CreateCourse),
    ("/.*", MainPage),
], debug=True)
//}

 * @<href>{http://localhost:8080/?output=json} でJSON形式の出力が出ることを確認してください。
 ** AndroidアプリでURLの末尾に"/?output=json"がついていることを再確認してください。
 * 本番サーバへアップロードします。
 ** ローカルサーバと本番サーバはデータを共有しません。
 * 本番サーバで講義データを登録します。
 * Androidアプリで講義データが追加されたことを確認します。

なお、index.yamlをアップロードした直後、暫くの間、
@<img>{need_index_error}や@<img>{need_index_error_2}のようなエラーが
表示されます。
この場合、少し様子を見てください。
GAE側で関連するデータベースのインデックスを作成するのに時間がかかるためです。

//image[need_index_error][]{
//}

//image[need_index_error_2][]{
//}

=== GET/POST

HTTPには多くのメソッドがありますが、ここではGETとPOSTについてだけ説明します。
GETとPOSTの最も大きな違いは、
クライアント(ブラウザ・Androidアプリ)から送信するデータをどう送るかです。

GETリクエストを行った際には、query stringと呼ばれる文字列がURLの末尾にくっつき、
それを用いてWebサーバにデータを伝えます。
POSTリクエストの場合、データはURLにはつかず、
HTTPのより深い部分で追加でWebサーバに送信されます。

HTTPの仕様でURLの最大の長さが決まっており、GETで本格的なデータを送ることは
そもそも不可能です。
また、短いデータについてもURLの一部として処理されるので、
Webサーバでキャッシュされたり、あるいはブラウザの履歴に残ったりします。
セキュリティを意識したいケースではなおさらGETによるデータ送信は行うべきではありません。

データをクライアントからWebサーバへ送信する際には
基本的にPOSTを使うのが適切です。
例外は例えば「どのように表示して欲しい」といった一時データです。

=== セキュリティへの配慮
==== ログイン機能ととアクセス制御を実装する

現在は誰が来てもこの講義表を見ることができます。
これは問題ありませんが、さらに誰でもこの講義情報を増やせます。
特に後者は大問題です。

GAEではGoogleアカウントによるログイン処理をを実装するのは簡単です。
"Hello world!"の代わりにユーザの名前を表示する方法を
@<list>{helloworld_with_login}に示します。

//list[helloworld_with_login][ログインを実装する]{
    def get(self):
        user = users.get_current_user()
        if user:
            self.response.headers['Content-Type'] = 'text/plain'
            self.response.write('Hello ' + user.nickname())
        else:
            self.redirect(users.create_login_url(self.request.uri))
//}

//image[gae_local_with_login][ローカルサーバのログイン画面]{
//}

ローカルサーバではどのようなアドレスであっても構いません。
appcfg.pyでアップロードした先では、
本当のGoogleアカウントでログインする必要があります。

講義表を公開するのまでは良いとして、講義表を編集するページだけは、
特定の人が利用できるようにするのが適切でしょう。
例えば user.user_id() を用いて、自分がログインした際の「User ID」を把握し、
それ以外のユーザから来た際にはアクセスを拒否するといった実装が考えられます。

ログイン関連APIの詳細は以下のWebページに説明があります。

@<href>{https://developers.google.com/appengine/docs/python/users/}
@<href>{https://developers.google.com/appengine/docs/python/users/userclass}

====[column] 認証・認可

以下の2つの用語を区別できると、
Androidアプリを開発する際にも都合が良いことがあります。

 * 認証(Authentication) ... 本人確認, 相手が「誰なのか」を確認する
 * 認可(Authorization) ... アクセス許可, 相手がその操作をして良いかを決定する

実際の生活でも、免許証を見せた上で証明書を発行してもらったりすることが
あると思います。
大まかに言えば、免許証を見せることが「認証」で、
実際に証明書を発行を許可されることが「認可」といった分類です。

Androidアプリ開発でもこの2つを区別したい時があるかもしれません。
英単語も似ているので混乱しがちですが、「authn」と「authz」と分けると、
混乱せずに済みます。

プログラミングの世界でも、長らくこのふたつは区別が曖昧でした。
そのため、今でも混乱するようなAPI等を見ることが多いですが、
これからの開発者はなるべく意識して使い分けてください。

====[/column]
==== https

URLに「http」で始まるケースと「https」で始まるケースがあります。
httpの場合、TCP接続して相手と通信する際、
途中の経路に存在する全てのホストからデータを盗み見ることが出来ます。
ログイン機能を用いて「認証・認可」を実現していても、
インターネット上に秘密のデータがそのまま平文(ひらぶん)で
送受信されている状態なので、言ってみれば盗聴し放題です。

一方、httpsでは通信経路で暗号化が行われるので、
不適切な方法で通信しなければデータを盗み見られることはありません。
@<fn>{https_is_wrongly_used}

//footnote[https_is_wrongly_used][ただし、Androidはその性質上、しばしば不適切な方法でhttpsを用いた通信がなされており、httpsの意味が全くないことがあります。「セキュリティ」の章でより詳細に学ぶことになるでしょう。]

送受信するデータを他人から見られたくない場合は必ず「https」の
URLを用いてやりとりをしてください。
今回用いるGAEの本番サーバは初めからhttpとhttpsの両方をサポートしています。

ただし、GAEのようにhttpとhttpsを即座に切り替えられるとは限りませんし、
多くの場合はそうではありません。
事実、GAEで開発中に使う@<href>{http://localhost:8080}というURLでは
httpsを利用することは出来ません。
その代わり、このURLはPC内からしか見ることが出来ないため、それによってセキュリティを確保しています。

Androidアプリ開発者の視点からは、
インターネット上でのhttpの使用は「暗号化されていない」と、
とりあえず覚えておいてください。

盗み見られても全く害がない場合には使っても問題ありませんが、
近年ではそのような分野は少なくなっています。
皆に公開されているニュース等であれば良いのですが、
ビジネス上の秘密をWebページでやりとりするといった場合には、
相応の考慮が必要です。

Googleも以前はhttpで検索結果を表示していたのですが、
現在ではほぼ全てhttpsによって暗号化しています。
おそらく読者が目にする主要なWebサービスは、
今ではほとんどhttpsを使っているはずです。

また、httpsと言っても「どのような方法で」
暗号化通信を行うかには細かい違いがいくつもあります。
ためしに、Chromeをブラウザで使っているとして、
httpsを利用しているときに表示されるURLの左側の錠前をクリックしてみましょう。
表示されるポップアップの「接続」タブを押してください。

//image[gae-certificate-connection][]{
//}

 * TLS 1.1である (最新はTLS 1.2だが、それ以前よりはマシ)
 * AES_128_CBCで暗号化されている
 * メッセージ認証にはSHA1を用いている
 * 鍵交換メカニズムにはECDHE_RSAを用いている

Webサーバを真面目に設計する際には、
こういった部分も併せて考える必要があったりします。
個人的には、SHA1がここで出てくるのは少し意外でした。

アプリ開発者としてWebサーバからデータを取得する場合には、
こういった点にも注意を払えるとセキュアなアプリを作る第一歩になります。

==== クロスサイト・スクリプティング脆弱性とセキュアプログラミング

ここでHTMLタグを講師名等に入れるとどうなるでしょうか。
試しに<script>alert("Hello");</script>などと入れてみましょう。

//image[injection-1][]{
//}

こんなのがでます。

//image[injection-2][]{
//}

これはいわゆる「クロスサイト・スクリプティング(XSS)」と
呼ばれる立派なWebサイト攻撃方法の一つをWebサーバが受けたことを意味します。
そして同時に攻撃が「成功」していることも意味します。
なお、攻撃を受けるところまでは日常と考えてください。
一方、攻撃が「成功」してしまうのは、日常であってはいけません。

今回は「Hello」としか出て来ませんが、
大手のWebアプリケーションでこのようなバグ(「脆弱性」と呼ばれます)は、
しばしばパスワード漏洩やありとあらゆる問題の温床になります。

例えば、今回の講義が秘密の講義表を持っていたとしましょう。
ログイン機能によって秘密を守っていたかと思いきや、
今回のような脆弱性がWebアプリケーションの「どこかに」あったとします。
すると、そこに今回のように任意の「プログラム」をHTMLに
第三者が埋め込めてしまえると、秘密の講義表はそのプログラムによって
覗き見られてしまいます。
Webサーバからすれば、認証情報を持っているブラウザが講義表を覗いていることに
なるので講義表を見せない理由がありません。
悪意のあるプログラムは、講義表をまた別のサーバに送信すれば良いわけです。

ユーザから得られるデータをHTMLに埋め込む場合、
最低でも5種類の文字を適切に書き換える必要があると言われています。

 * 「&」 ... 「&amp;」に変更 (英語のampersandから)
 * 「"」 ... 「&quot;」に変更 (英語のquoteから)
 * 「'」 ... 「&apos;」に変更 (英語のapostropheから)
 * 「>」 ... 「&gt;」に変更 (英語の greater than から)
 * 「<」 ... 「&lt;」 に変更 (英語の less than から)

これに相当する対策としてxml.sax.saxutils.escape()関数を使うことが挙げられます。
以下に例を挙げます。
@<fn>{about_template}

//footnote[about_template][追加演習で登場する「テンプレート」の仕組みを用いる場合はこの処理は必要ありません。テンプレートエンジンが自動的にエスケープを行うからです。]

//emlist[escape()を使う]{
from xml.sax.saxutils import escape

.. (中略)
   
            for course_model in course_model_list:
                lst.append(u'<li>{}, {}, {}</li>'
                           .format(escape(course_model.title),
                                   escape(course_model.teacher),
                                   escape(course_model.detail)))
//}

Webアプリを開発する場合には、
今回出会った「クロスサイト・スクリプティング脆弱性」以外にも
いくつか「定番」の脆弱性を作りこみがちです。

幸い、IPA(情報処理推進機構)がWebアプリケーションを作成する際に
発生しがちな脆弱性についての情報をまとめていますので、
Webプログラミングに興味がある方は一度目を通すとよいかもしれません。

@<href>{https://www.ipa.go.jp/security/vuln/websecurity.html}

Androidアプリ側でもセキュリティについて考慮するべき点はたくさんあるため、
受講生はどちらかといえばWebアプリケーションの事情よりも、
そちらを優先するべきと考えられます。
そういった諸々のお話は「セキュリティ」の章で扱います。


=== HTMLの描画にテンプレートとCSSを使う

本演習の範囲では「見栄え」を意識することはありませんでした。
とにかく表示されれば良かったのです。

しかし大きなWebアプリを作るのであればこの方法は色々と不都合です。
Androidアプリの開発に措いて「UI基礎編」で既に述べられている通り、
デザインとコードは分離出来てしかるべきです。

Webアプリケーションの「デザイン」は主に、HTMLとスタイルシート、
そして本項では紹介していない「Webプログラミング」言語であるJavaScript
によって実現されます。
これらは今回実装したPythonのWebサーバ実装とは分離できるべきです。

GAEでは、jinja2と呼ばれるテンプレートエンジンを用いて、
デザインと実装の分離を達成することができます。
ここではトップページの講義表にこの仕組みを使ってみましょう。

まず、jinja2を使えるようにするため、app.yamlに以下を追加します。


//emlist[app.yamlに以下を追加]{
libraries:
- name: webapp2
  version: latest
- name: jinja2
  version: latest
//}

index.htmlを新たに作成し、HelloWorld/ フォルダに置きます。


//emlist[index.html]{
<!DOCTYPE html>
<html lang="ja">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>講義表 with bootstrap</title>
    <link href="//maxcdn.bootstrapcdn.com/bootstrap/3.2.0/css/bootstrap.min.css" rel="stylesheet">
  </head>
  <body>
    <div class="container">
      <div style="text-align: center; padding-top: 40px;">
        <h1>講義表ページ</h1>
        <h2>総講義数: {{ course_count }}</h2>
        {% if course_model_list %}
        <table class="table" align="center"
               style="width: 300px;">
          <thead>
            <tr><td>日付</td><td>講師</td><td>詳細</td></tr>
          </thead>
          <tbody>
            {% for course_model in course_model_list %}
            <tr>
              <td>{{ course_model.date.strftime('%Y-%m-%d') }}</td>
              <td>{{ course_model.title }}</td>
              <td>{{ course_model.teacher }}</td>
            </tr>
            {% endfor %}
          </tbody>
        </table>
        {% endif %}
        <div>
          <a href="/create">Create</a>
        </div>
      </div>
    </div>
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>
    <script src="//maxcdn.bootstrapcdn.com/bootstrap/3.2.0/js/bootstrap.min.js"></script>
  </body>
</html>
//}

ほとんどHTMLファイルのようですが、
実は一部が「テンプレート化」されています。
つまり、プログラムで後から特定の文字列を埋め込めるようになっています。

 * course_countを指定することで、後から講義の総数を埋め込めます
 * course_modelにいくつかデータを入れることができます。

Pythonコードは以下のようになります。
HTMLを直接出力する必要がなくなるので、こちらの実装は簡素になります。

//emlist["html"]{
        if output == 'html':
            course_count = course_model_list.count()
            template_values = {'course_count': course_count,
                               'course_model_list': course_model_list}
            template = JINJA_ENVIRONMENT.get_template('index.html')
            self.response.write(template.render(template_values))
        else:
//}

これで見栄えがだいぶ変わります。

また、前述した「クロスサイト・スクリプティング攻撃」を避けるための
エスケープ処理を行う必要がありません。
なぜなら、テンプレートエンジンは与えられた文字列をチェックし、
自動的にエスケープ処理を実行してくれるからです。

もしエスケープを解除したいケースがあるようであれば、
そのときだけ明示的にエスケープをしないように指定します。

プログラミングミスを全て回避することは難しいでしょう。
jinja2テンプレートエンジンが採用している
「していなければエスケープして安全よりに倒す」
という方式の方が、安全なプログラミングとしては、より適切と言えます。

さらにこのテンプレートではCSSテンプレートを使っているため、
ボタン等の見栄えもちょっと変わります。

時間があれば、講義作成画面も、
テンプレートを使って見栄えを変えてみましょう。

参考:
@<href>{https://developers.google.com/appengine/docs/python/gettingstartedpython27/templates}

== 参考: シラバスアプリを拡張するには

講義の範囲からは離れますが、
今回のシラバスアプリとサーバを拡張するためにどうすればよいかを考えてみると、
次に勉強するべき内容が分かるかもしれません。

まず、現在明らかに不足している機能として、
操作を許可されているユーザからWebページを経由して
削除や編集を行うことが出来ません。
これですと、GAEの管理コンソールから直接データを削除する以外には
データを変更する方法がありません。
Python実装中で操作を行うには、Datastore APIについてより
深く理解する必要がありますが、それでも、さほど難しくはないでしょう。

@<href>{https://cloud.google.com/appengine/docs/python/ndb/}

講義表をアプリから操作出来るようにしたいとした場合、
これよりも複雑で時間のかかる作業が必要になります。

まず何より、講義表を変更するActivityが実装されていません。
EditText等を使って自分で準備する必要があります。
そのためのIntentについても考えなければなりません。

より本質的な課題として「どのように認証・認可を行うか」があります。
ブラウザのセッションは
Androidアプリ(より的確には「Androidのネイティブアプリ」)
でそのまま再利用するのは通常出来ません。

Androidアプリで編集を許可するもっとも単純な方法は
「アカウントに紐付いた専用の秘密の文字列を準備する」ことです。
アプリから特定のURLに向けてGETやPOSTリクエストを投げる際、
クエリ文字列などでその情報を含めます。
すでに議論した通り、通信を暗号化するhttpsを用いるべきです。
実際、ある種のWebアプリケーションはこのような方法を用います。

より高度な方法として、Android端末に紐付いたGoogleアカウント
を用いて認証を行い、それを元に認可のためのトークンをやりとりする方法があります。
本講義全体でもほぼ扱われていない比較的高度なトピックですが、
時間があればチャレンジしてみても良いでしょう。

 * まず OAuth2 という技術を知っておくべきです。
 * AndroidのAccountManagerについて知っておくほうが良いでしょう。
 * 以上を元に、@<href>{https://developer.android.com/intl/ja/google/auth/http-auth.html}や@<href>{http://blog.notdot.net/2010/05/Authenticating-against-App-Engine-from-an-Android-app}の内容を参考にして、Android端末に紐付いているGoogleアカウントの情報を取得し、認証してみましょう。
 * どのように「認可」するかも併せて考える必要があります。

Webサーバの立場からすると、
後者の方が「優れている」といつも言えるわけではない点は指摘しておきます。
仮に以下のようなアイディアが湧いたら、紹介した高度な方法の実装は再利用できないかもしれません (かろうじて、OAuth2という仕組みはオープンスタンダードのため、この知識はつぶしが利きます)

 * 他のスマートフォンでも講義を管理したい
 * Twitterや他のソーシャルサービスのアカウントを利用したい
 ** AccountManagerにアカウント管理機能を提供しているソーシャルサービスであれば、この方法は再利用出来る可能性が若干あります。

いずれにしても、すでに基礎的な内容からは相当乖離してますので、
今回はここまでとしましょう。


== 関連書籍・URL

本節ではWebサーバを作る上で最小限把握して欲しい内容をまとめました。
Androidアプリから状況を把握する上ではこれで十分でしょう。

@<href>{https://github.com/TechBooster/AndroidOpenTextbook}
に、本節では説明していない要素も含めたやや詳細な内容を掲載していますので、
参照してみてください。
そちらでは別のアプリケーションを題材に、GAE Pythonとは異なる
Webフレームワーク「Django」を紹介しています。

2014年10月現在、
@<href>{https://tcb.mowa-net.jp/griflet/github/TechBooster/AndroidOpenTextbook/}
から上記のPDF版やHTML版を得ることができます。

Webサーバの技術を理解するにはインターネットもしくは「ネットワーク」についての
知識が必要になります。
さらなる詳細については、例えば以下のような書籍を参照してください。

 * @<href>{https://www.ipa.go.jp/security/vuln/websecurity.html}
 * マスタリングTCP/IP 入門編 第5版 http://www.amazon.co.jp/dp/4274068765
 * マスタリングTCP/IP 情報セキュリティ編 http://www.amazon.co.jp/dp/4274069214/
 * コンピュータネットワーク 第5版 http://www.amazon.co.jp/dp/482228476X
 * Webを支える技術 http://www.amazon.co.jp/dp/4774142042/
 * 安全なWebアプリケーションの作り方 http://www.amazon.co.jp//dp/4797361190/
 ** PHPというWeb開発で使われるまた異なる言語が使われていますが、参考になります


