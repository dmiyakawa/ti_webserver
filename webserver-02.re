= 自分のWebサーバを作ろう

//lead{
本節では実際にAndroidアプリと連携するWebサーバを開発します。
//}

== 本節で学ぶこと

 * Google App Engine (GAE)を用いた開発環境の準備
 * ローカルサーバの起動
 * 本番サーバへのデプロイ
 * 機能の追加

この節で出てくるキーワード

 * Python
 * Google App Engine (GAE)
 * HTTP GET, HTTP POST
 * XSS (クロスサイト・スクリプティング)

== シラバスアプリの復習

「ユーティリティによる実践」のシラバスアプリでは、
サーバに保存されたJSONデータをVolleyというライブラリで
非同期にダウンロードし、その内容をListViewに表示しました。

本節ではこのシラバスアプリで利用できるJSONデータを
アプリに返すWebサーバを作成します。

ブラウザを用いて、シラバスデータを動的に変更するためのフォーム入力画面も、
実装します。

== Google App Engineとは？

Google App Engine(GAE)はGoogleが提供する
Webアプリケーションフレームワークの一つです。

本節では、Webサーバを実装する環境として、
Google App Engine(略してApp Engine、もしくはGAE)の
Python言語による実装を用います。
GAEにはJavaによる実装もありますが、
準備に時間を費やしてしまうため、本講義では採用を見送ります。

PythonはJavaやC言語のようなプログラミング言語の一つです。
サーバ側の実装やプログラミング教育で豊富な実績があります。
@<fn>{python_for_education_in_gigazine}

//footnote[python_for_education_in_gigazine][「Pythonが大学の入門用プログラミング言語として人気を集めていると判明」 by Gigazine 2014年7月15日 @<href>{http://gigazine.net/news/20140715-python-most-popular-language/}]

ここからはWebアプリの開発環境を準備しつつ、
PC上でサーバ(ローカルサーバ)を起動する方法と、
世界中にWebアプリを公開するための「本番サーバ」へのデプロイの方法を
説明していきます。

== Python実行環境のインストール

まず、Python言語をPC上で利用するために、Python実行環境をインストールします。
インストールするバージョンはPython 2.7.8です。

以下のURLからWindows向けPythonインストーラを取得します。
注意点として、3で始まるPythonバージョンは、
インストールしないでください。

 * @<href>{https://www.python.org/downloads/release/python-278/}
 * (@<href>{https://www.python.org/ftp/python/2.7.8/python-2.7.8.amd64.msi})

//image[install-python-1][@<href>{https://www.python.org/downloads/release/python-278/}のスクリーンショット]{
//}

//image[install-python-add-path-1][「Add python.exe to Path」をクリックし、表示された2つの選択肢のうち"Will be installed on local hard drive"を選ぶ]{
//}

//image[install-python-add-path-2][×印がなくなったら「Next」]{
//}

確認のため、コマンドプロンプトで「Python」と入力して
Pythonの対話型環境が起動するかを確認してください。

対話型環境は次節のPython言語についての学習時に使うことが出来ます。



=== Google App Engine SDK

Google App Engineでは多くの言語とフレームワークをサポートしています。
本節ではその中でも、GAE のPython版を使用します。

 * https://developers.google.com/appengine/downloads

「Google App Engine SDK for Python」をクリックします。

//image[install-gae-1][]{
//}

適切なOSのインストーラを選びます。

//image[install-gae-2][]{
//}

ダウンロード後、インストーラを起動し、
Python 2.7をSDKインストーラが認識していることを確認した上で、
Nextボタンをクリックします。

//image[install-gae-python-checked][]{
//}

//image[install-gae-tos][]{
//}

3つの全てにチェックが入っていることを確認します。

//image[install-gae-all-checked][]{
//}

最後にInstallボタンをクリックします。

//image[install-gae-do-install][]{
//}

Install中、ダウンロードしたソフトウェアを信用するかどうかを
確認するシステムダイアログが表示されることがあります。
このとき、「確認済みの発行元」が「Google Inc」となっていることを
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
この現象は、特にWindows 8で日本語名のユーザを作成した際に発生します。

//image[japanese_user_name][ユーザ名が「大輔」で日本語になっている]{
//}

このケースに当てはまる場合、さらに以下の作業を行なってください。

 * 「C:\GoogleAppEngine」のような日本語を含まないフォルダを作成する。
 * TMP環境変数に上記のアドレスを指定する。
 ** 環境変数はJavaのPATHを指定した時と同じ方法で設定できます。
 * プロジェクトを上記フォルダに作る。

この作業を行わずにGAEの各種作業を行おうとすると、
以下のような問題が発生します。

 * テスト用のsqlite DBが作成できないという旨のエラーが発生する。
 * GAEが動作しなくなる。
 * GAEが起動しなくなる。

最後の挙動に至った場合はユーザフォルダ配下の「Google」フォルダから
GAE Launcherが作成する一時ファイルを全て削除すると、復帰します。

ただし、本コラムの作業を行わない限り、GAE SDK自体は正しく動作しません。

===[/column]

== GAEでローカルサーバを起動する

ここで、"Hello world!"と表示するだけのサーバを作成しましょう。

Google App Engine Launcherで「File >> Create New Application」を選択します。

 * Application Name を「helloworld」等に設定します。
 ** ここでは一部の文字、例えば半角大文字は使用できません。
 * Parent Directory を「デスクトップ」等に設定します。
 ** 日本語ユーザである場合は、コラムの注意を参考にして設定します。

//image[create-helloworld][]{
//}

作成できたらそのプロジェクトを選択し「Run」をクリックします。

//image[run-helloworld][]{
//}

成功すると、プロジェクトの左にある小さなアイコンが
緑色の「再生ボタン」風になります。

//image[running-helloworld][]{
//}

この状態でブラウザから「http://localhost:8080」を開きます。

"Hello world!"と表示されたら、成功です。

//image[seeing-helloworld][]{
//}

8000番で、このプロジェクトの管理用Webページを開くこともできます。

このサーバは現在「実行中」の状態です。
これを停止するには、GAE Launcherから「Stop」ボタンを押します。
「再生」ボタンが黒い丸印になれば、サーバプロセスが停止しています。

//image[stopped-helloworld][]{
//}


入力したURLについて説明します。

 * http はどのようにサーバに接続するかを決めたもの(スキームと呼ばれる)。
 * localhostは「自分自身」で、つまりPCのこと。
 ** ここでは「ローカルサーバ」と呼ぶことにします。
 * TCPポートは「ネットワーク」で説明済みです。
 ** TCPポートの8080番でサーバを作成している、という意味になります。

この"Hello world!"を表示するまでに、
プログラムを1行も書く必要がありませんでしたが、ちょっと物足りなすぎです。

Pythonでプログラムを書いてサーバを拡張するために、
Pythonの開発環境をインストールしましょう。
@<fn>{ok_with_hidemaru}

//footnote[ok_with_hidemaru][他に『秀丸』等の高機能なエディタがあればそれを利用しても構いません。ただしWindows付属のメモ帳では文字コードの関係で期待したとおりに動作しないかもしれません。]

== PyCharm Community Edition のインストール

PythonでもEclipseのような統合開発環境(IDE)を利用できると便利です。
ここでは、PyCharmと言う商用IDEの無料版「PyCharm Community Edition」をインストールします。
@<fn>{about_pydev}

//footnote[about_pydev][EclipseをPython開発環境として利用する方法として、PyDevというEclipseプラグインが存在します。しかし、本講義において環境にインストールされているJavaのバージョンは若干古いため、そのままでは利用できません。Javaのバージョンを7にすればPyDevの動作要件を満たしますが、Android開発環境を壊してしまう可能性もあるため、演習では採用しません。]

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
なお、この設定は後から変更可能です。

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


よく似た見た目をしていますが、Eclipseと全く一致した使い勝手ではありませんので注意してください。
以下に、すぐに使い勝手を改善出来るヒントを示します。

 * 起動時にkeymapを変更していない場合「File >> Settings >> Keymap」でKeymapsを「Eclipse」に変更すると、Eclipse風になります。@<img>{eclipse_keymap}ただし全てがEclipseと同じになるわけではありません。
 * 「File >> Settings >> (左画面IDE Settingsの) Editor >> Editor Tabs >> Mark modified tabs with asterisk」のチェックを入れておくと、Eclipseの時と同様保存されていないファイルのタブに*(スター、もしくはアスタリスク)が付きます。@<img>{asterisk}

//image[eclipse_keymap][]{
//}

//image[asterisk][]{
//}


Pythonでプログラミングする準備が整いました。

プログラミングに自信があればこのままWebサーバの実装を進めても構いませんし、
ここで一旦、次節に進み、Pythonの基礎を学んでも構いません。

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

 * Pythonではインデントに意味があります。
 ** ブロックの開始と終了を示す中括弧は存在しません。
 * def function_name(args) という表現で関数(メソッド)を宣言します
 * webapp2.WSGIApplication()に渡されるリストは「WebサーバのどのパスをどのHandlerが処理するかを示したものです」
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

== Googleの本番サーバにアップロードし、世界にアプリケーションを公開する

Google App Engineの便利なところは、
この時点ですでに、世界へWebアプリケーションを公開する準備が
半分以上できていることです。

Googleアカウントで開発者登録をした上で、
Webアプリのための「アプリケーションID」を取得すると、
この"Hello world!"サーバは世界に公開できます。

以下のURLをタイプし、講義に用いるGoogleアカウントでログインします。

@<href>{https://appengine.google.com/}

Google App Engineの利用規約が表示された場合は内容を確認し、
"I accept these terms"をチェックした上でSubmitを押します。

//image[gae-tos][]{
//}

次画面で、"Create Application"をクリックします。

//image[gae-web-console][]{
//}

Application Identifierとして「世界中で唯一」の名前を一つ設定します。
「ti-(受講生番号)」といったものでも構いませんし、別のものでも構いません。
ただし他の人が利用していた場合は選択できませんので、
アプリケーションIDを考えたら、
右側の"Check Availability"ボタンで利用できるかを確認します。

IDを利用できるようであれば、最下段の"Create Application"ボタンを押します。

//image[gae-ti-dmiyakawa][]{
//}

成功すると次のような画面がでます。

//image[gae-project-creation-successful][]{
//}

"Application Registered Successfully"と出たら、
PyCharm内でapp.yamlを開き、
最初の行の"application"の右側の文字列を自分が選択したものに変えます。

//emlist[]{
application: (ここを先ほど選択したIDに変更する)
//}

アプリケーション固有パスワード

@<href>{https://security.google.com/settings/security/apppasswords}

最下段の「端末を選択」で「その他(名前を入力)」を選び
「Google App Engine」と入力し、「生成を押します」

4文字が空白で4つ区切りになった、合計で16文字のパスワードが
表示されますので、これをDeployで入力します。

ログで「Deployment successful」と出たら成功です。


「@<href>{https://(自分が生成したID).appspot.com/}」へアクセスしてみましょう。
なお、URLの冒頭は「http」ではなく「https」とすることをおすすめします。
理由は座学パートで説明しています。

隣の人にアプリケーションIDを教えてもらい、
他の人のWebサーバを見せてもらってもよいでしょう。

=== 参考

(原稿に入れない可能性あり)

なお、本項の説明は2014年09月21日時点のものです。
GoogleのWebコンソールは終始変化しており、
執筆時点でも複数のプロジェクト作成方法が存在します。

万が一本項による作成方法を採れない場合、
以下のURLから「プロジェクトID」を取得して先に進んでください。
これがapp.yamlのapplicationに入ります。

この場合、アプリケーションIDに相当する文字列は
自動生成されるため、
自分で自由に選ぶことは出来ません。

@<href>{https://console.developers.google.com/}

//image[gae-project-id][プロジェクトID]{
//}

=== (オプショナル) Wi-Fi内でだけ公開する

ここまでで、二通りのWebサーバの起動方法を学びました。

一つは「ローカルサーバ」を立てる方法です。
これは開発途中では便利です。
しかし、@<href>{http://localhost:8080}というURLは
他人からは見ることができません。

一方、「本番サーバ」にアップロードして
Googleにホスティングしてもらう方法ですと、
@<href>{http://(自分が生成したID).appspot.com/}という
URLが世界に公開されてしまいます。

本講義中には、「世界中に公開」とは行かないまでも、
同じWi-Fiネットワーク内の他の人にサーバを見せたい、
というケースもあるかもしれません。

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
「http://localhost:8080」ではWebページは
表示されなくなり、代わりにIPアドレスを指定したURLだけで表示できます。
こののURLを隣の人に教えれば、
隣の人のPCからそのWebページを見られるようになります。
この方法は、世界中にサーバを公開せずに近くの人と
挙動を確認しあうのに使えます。
後述するAndroidアプリとも、Wi-Fiネットワークの範囲内であれば、通信を行えます。

本節のこれ以降の説明では、ローカルサーバは引き続きlocalhostを使って、
つまり自分だけが見る目的で起動することにします。

厳密には、この方法は「Wi-Fi環境のみに公開」という意味ではありませんが、
本講義ではそのように利用できます。


== 本番サーバの状況を確認する

GAE Launcherには「Dashboard」というボタンもあります。
このボタンはアプリケーションIDを適切に指定した時にのみ、
本番サーバにおける、そのWebアプリ向けの管理者画面を
ブラウザに表示してくれます。

//image[gae-dashboard][]{
//}


== シラバスアプリのデータ構造を確認しよう

「ユーティリティによる実践」で、
JSON形式のデータをAndroidアプリで利用する方法を学びました。
その時利用したJSON取得用URLは
@<href>{https://dl.dropboxusercontent.com/u/1088314/tech_institute/2014/syllabus.json}
でした。
これは、Dropbox@<fn>{about_dropbox}と呼ばれるファイル共有サービス
上で講師が公開している静的ファイルです。

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

== 自分のWebサーバから、自動生成したJSONデータを返す

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
        logging.debug(pprint.pformat(data))
        self.response.write(data)
# 以下省略
//}

ここでは、サーバが返すデータの種類と文字コードを「Content-Type」つまり
「返答するデータのタイプ」として明示した上で、
Python言語で生成したデータ構造を(json.dumps()関数で)JSONの文字列
として返しています。
これらはHTTPと呼ばれるプロトコルによって、
クライアント、すなわちブラウザやAndroidアプリに返されます。

ログにはJSONフォーマットする前のデータが表示されます。

=== デバッグ方法

Pythonではloggingというモジュールを用いることで、
AndroidにおけるLogCatと似たようなログ機構を頼ることができます。
@<fn>{logging_has_no_tag}

//footnote[logging_has_no_tag][ただし、LogCatの「TAG」の仕組みはPythonのlogging関数には存在しません。ログのグループ化は別の方法で実現するのですが、本稿では割愛します。]

ローカルサーバの標準のログレベルはINFOです。
今回のサーバ実装ではlogging.info()だけ使うのがシンプルで良いでしょう。

先ほどのコードで、JSONデータをクライアントに送信する直前に
logging.info()を使って結果をログに表示しているので、
特にローカルサーバのログで確認してみてください。

== 生成されたJSONデータをブラウザで確認する。

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

== シラバスアプリにサーバのJSONデータを読み込ませる

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

== 講義表を保存・変更出来るようにする

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
ちょっと異なるのはURLに"/create"というパスが表示されたことでしょうか。

== データを追加するための画面を作る

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

== セキュリティについて一言

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
(攻撃を受けるところまでは日常と考えてください。
攻撃が「成功」するのはそうではありません。)

今回は「Hello」としか出て来ませんが、大手のWebアプリケーションで
このようなバグ(「脆弱性」と呼ばれます)は、
しばしばパスワード漏洩やありとあらゆる問題の温床になります。

例えば、今回の講義が秘密の講義表を持っていたとしましょう。
後述するログインによって秘密を守っていたかと思いきや、
今回のような脆弱性がWebアプリケーションの「どこかに」あったとします。
すると、そこに今回のように任意の「プログラム」をHTMLに
第三者が埋め込めてしまえると、秘密の講義表はそのプログラムによって
覗き見られてしまいます。
Webサーバからすれば、認証情報を持っているブラウザが講義表を覗いていることに
なるので講義表を見せない理由がありません。
悪意のあるプログラムは、講義表をまた別のサーバに送信すれば良いわけです。

これ以外にも、Webサーバを実装する際にはちょっとした油断で
全ユーザの全情報を奪われる「脆弱性」を簡単に作りこめてしまいます。

これは規模は異なるものの、Android側でも事情が似通っています。
油断すると、例えば「知られたくない個人情報が駄々漏れ」となったりするのです。
そういった諸々のお話は「セキュリティ」の章で詳しく学びます。


=== 関連URL

@<href>{https://developers.google.com/appengine/docs/python/gettingstartedpython27/usingdatastore}

Datastoreで指定できる型は以下の通り。

@<href>{https://developers.google.com/appengine/docs/python/ndb/properties}

@<href>{http://webapp-improved.appspot.com/}

== 演習 (オプショナル) HTMLの描画にテンプレートとCSSを使う

本演習の範囲では「見栄え」を意識することはありませんでした。
HTMLとして表示されればよかったわけです。

これをもう少しわかり易く管理し、さらにスタイルシートを使って装飾をしてみます。

app.yamlに以下を追加します。

//emlist[app.yamlに以下を追加]{
libraries:
- name: webapp2
  version: latest
- name: jinja2
  version: latest
//}

index.htmlを新たに作成し、HelloWorld/ディレクトリに置きます。

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
また、前述した「クロスサイト・スクリプティング」の脆弱性も
自動的になくなります。
なぜならテンプレートエンジンは与えられた文字列をチェックし、
自動的に「<」や「>」といった危なそうな文字を、
安全な文字に置き換えているからです。

さらにこのテンプレートではCSSテンプレートを使っているため、
ボタン等の見栄えもちょっと変わります。

演習: 講義作成画面もテンプレートを使って見栄えを変えてみましょう。


参考:
@<href>{https://developers.google.com/appengine/docs/python/gettingstartedpython27/templates}


== (オプショナル) ログインを実装する。

Googleアカウントによる簡単なログインを実装してみましょう。

//list[helloworld_with_login][ログインを実装する]{
# -*- coding: utf-8 -*-
from google.appengine.api import users
import webapp2

class MainPage(webapp2.RequestHandler):

    def get(self):
        # Checks for active Google account session
        user = users.get_current_user()

        if user:
            self.response.headers['Content-Type'] = 'text/plain'
            self.response.write('こんにちは, ' + user.nickname())
        else:
            self.redirect(users.create_login_url(self.request.uri))


application = webapp2.WSGIApplication([
    ('/', MainPage),
], debug=True)
//}

ローカルサーバでとりあえず挙動を試してみましょう。

//image[gae_local_with_login][ローカルサーバのログイン画面]{
//}

ローカルサーバではどのようなアドレスであっても構いません。
appcfg.pyでアップロードした先では、
本当のGoogleアカウントでログインする必要があります。

APIの詳細は以下にあります。

@<href>{https://developers.google.com/appengine/docs/python/users/}
@<href>{https://developers.google.com/appengine/docs/python/users/userobjects}


== GAEを使い続ける場合の注意

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

== 課題
=== Puttyを使って応答をデバッグする。

サーバとAndroidアプリのどちらもおかしくないように見えて、
挙動がおかしいときがあります。
こういったとき、通信プロトコルを解析するのが有用なことがあります。
「ネットワーク」において一度Puttyを利用していますので、
ここでも一度使ってみることにしましょう。


=== Androidアプリとの緊密な連携

本演習ではデータの更新はすべてWebフォーム上から行いました。
第14章「ネットワーク」の知識と組み合わせることで、
この部分もAndroid端末から行うことも可能です。

 * 編集用のActivityを作り、ボタンやメニューから移動できるようにしましょう
 * 編集後、「キャンセル」と「送信」が出来るようにしましょう
 * 「送信」がされたら、Serviceのような仕組みでサーバにデータを送りましょう。このとき「バックグラウンド」と「ネットワーク」の章を復習すると方法が分かります。
 * (難) ログインシナリオをここでも有効に機能するようにしましょう。
 * (難) 管理者だけから変更を許すようにしましょう。方法はお任せしますが、管理者アカウントのパスワードは使わないようにしたほうが良いでしょう。サーバ側に動的に変更可能なOAuth風の「トークン」を生成する機能を作ると、そこそこ信頼できる機構になります。
 * (難) Android端末にひもづけられているアカウント情報を使い無駄なログインを省きましょう。


=== Tech Institute アプリの作成

本章の知識を用いて、WebサーバとAndroidアプリのセットで
「Tech Institute アプリ」を作れないか考えてみましょう。

どのような連携を行えばよいか、
あるいはどのような情報が特定のユーザに「行き渡ってはいけないか」
を考え、アプリを実装する上でどのようにそれを実現するかを考えてみます。

今回のシラバスの実装に加えて、例えば以下のような機能が想像出来るかもしれません。

 * 講師、受講生、管理者の3つの役割がある。
 * 投稿は特定の講義に対して行われる。
 * 受講生のみ、授業についての感想を投稿できる。
 * 受講生は、自分の投稿は見ることができるが、投稿後の変更は出来ない(管理者に連絡することで変更を依頼することは本アプリの範囲である必要はないだろう)。他の受講生の投稿を読むことは出来ない。
 * 講師は自分が担当した講義について、受講生が投稿した感想を読むことが出来るが、誰が投稿したかの情報を見ることは出来ない。
 * 管理者と講師は受講生に連絡を送ることが出来ても便利かもしれない。これはアプリからも見られ、また受講生が明示的に許諾した場合には同受講生のメールアドレス宛にメールを送っても良い (GAEにはメールを送信する機能がある)
 * 上記の一部・全ての機能がHTMLとAndroidアプリのいずれかで利用できる。
 * 管理者は講師と受講生を管理でき、以上の全ての情報について、閲覧・修正・削除を行え、かつ受講生と講師の管理を行うことが出来る。

この他に自分が期待する機能があれば、適宜追加して考察を繰り返すと良いかもしれない。

実際に作成する必要はないが、本講座の内容を修了した受講生であれば理屈上はこのレベルの実用アプリケーションは作成出来ると考えられる。

「グループウェアを作りました」と面接で言えれば、IT企業での面接には有利だろう。
