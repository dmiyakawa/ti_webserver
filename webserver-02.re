= 自分のWebサーバを作ろう

//lead{
前節ではWebサーバ云々
//}

== Python実行環境のインストール

Python言語をPC上で実行するためにPython実行環境をPCへインストールします

以下のURLから取得します。

 * @<href>{https://www.python.org/downloads/release/python-278/}
 * (@<href>{https://www.python.org/ftp/python/2.7.8/python-2.7.8.amd64.msi})

//image[install-python-1][@<href>{https://www.python.org/downloads/release/python-278/}のスクリーンショット]{
//}

//image[install-python-add-path-1][「Add python.exe to Path」をクリックし、表示された2つの選択肢のうち"Will be installed on local hard drive"を選ぶ]{
//}

//image[install-python-add-path-2][×印がなくなったら「Next」]{
//}

=== GAE SDK

Google App Engineでは多くの言語とフレームワークをサポートしています。
本節では全体を通じて Google App Engine のPython版、
その中でも特にwebapp2というフレームワークを用います。

 * https://developers.google.com/appengine/downloads

//image[install-gae-1][「Google App Engine SDK for Python」をクリックして]{
//}

//image[install-gae-2][適切なOSのインストーラを選ぶ]{
//}

ダウンロード後、インストーラを起動し、Python 2.7が認識されていることを
確認した上で、Nextボタンをクリックします。

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

===[column] ユーザフォルダに日本語が含まれる場合

このコラムの作業が必要なのは「C:\ユーザー\(ユーザ名)」
の「ユーザ名」部分が日本語になっている場合だけです。

そのような例を@<img>{japanese_user_name}に示します。
この現象は、特にWindows 8で日本語名のユーザを作成した際に発生します。

//image[japanese_user_name][ユーザ名が「大輔」で日本語になっている]{
//}

このケースに当てはまる場合、さらに以下の作業を行なってください。

 * 「C:\GoogleAppEngine」のような日本語を含まないフォルダを作成する。
 * TMP環境変数に上記のアドレスを指定する。
 ** 環境変数はJavaのPATHを指定した時と同じ方法で設定できます。
 * プロジェクトを上記フォルダに作る。

この作業が必要なのは、日本語を含むフォルダの扱いを
Google App Engineが正しく処理できないためです。

この作業を行わずにGAEの各種作業を行おうとすると、
以下のような問題が発生します。

 * テスト用のsqlite DBが作成できないという旨のエラーが発生する。
 * GAEが動作しなくなる。
 * GAEが起動しなくなる。

最後の挙動に至った場合はユーザフォルダ配下の「Google」フォルダから
GAE Launcherが作成する一時ファイルを全て削除すると、復帰します。
ただし本コラムの作業を行わない限り、問題は改善しません。


===[/column]

== GAEでローカルサーバを起動する
== Hello World サーバを作る

Hello Worldと表示するだけのサーバを作成しましょう。

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

URLについて説明しましょう。

 * http はどのようにサーバに接続するかを決めたもの(スキームと呼ばれる)。
 * localhostは「自分自身」で、つまりPCのこと。
 * TCPポートの8080番でサーバを作成している、という意味になる。

8000番で、このプロジェクトの管理用Webページを開くこともできます。

このサーバは現在「実行中」になっています。
止めるにはGoogle App Engine Launcherから「Stop」ボタンを押します。
「再生」ボタンが黒い丸印になれば、止まったことになります。

//image[stopped-helloworld][]{
//}

== PyCharm Community Edition のインストール

Pythonの場合でもEclipse同様の統合開発環境(IDE)があった方がやりやすいので、PyCharmと言うIDEをインストールします。
@<fn>{about_pydev}

//footnote[about_pydev][Eclipse上でPython開発環境を実現する方法としてPyDevというプラグインが存在しますが、現在環境にインストールされているJavaのバージョンが古いためそのままでは利用できません。Javaのバージョンを7にすれば動作するはずですが、Android開発環境を壊すリスクとなるため、演習では採用しません。]

商用のソフトウェアということもあってEclipseより好んで使うユーザが
いるほどなのですが、Eclipseと全く一致した使い勝手ではありませんので
注意してください。
以下に何点か、すぐに使い勝手を改善出来るヒントを示します。

 * 「File >> Settings >> Keymap」でKeymapsを「Eclipse」に変更すると、Eclipse風になります。ただし全てではありません。@<img>{eclipse_keymap}
 * 「File >> Settings >> (左画面IDE Settingsの) Editor >> Editor Tabs >> Mark modified tabs with asterisk」のチェックを入れておくと、Eclipseの時と同様保存されていないファイルのタブに*(スター、もしくはアスタリスク)が付きます。@<img>{asterisk}

//image[eclipse_keymap][]{
//}

//image[asterisk][]{
//}


=== (deprecated): PyCharmでHello Worldサーバを作る

「File > New Project」 を選択します。

Project name は、"HelloWorld"とします。

出来上がった"HelloWorld"プロジェクトを右クリックし、"New > File"

ファイル名を helloworld.py とします

画面右側に以下を入力します。

//list[hello_world][helloworld.py]{
 // -*- coding: utf-8 -*-
 import webapp2

 class MainPage(webapp2.RequestHandler):
     def get(self):
         self.response.headers["Content-Type"] = "text/plain"
         self.response.write("Hello, World!")

 application = webapp2.WSGIApplication([
     ("/.*", MainPage),
 ], debug=True)
//}

一つ注意点があります。
各行冒頭の空白(インデント)を正しく入力してください。
Python言語ではインデントがJavaで言う中括弧({と})の役割を果たします。
Python言語の機能などについては次節にまとめますので、とりあえず
ここではそのまま入力してください。

またファイルを作ります。今度は"app.yaml"を作成します。

//list[app_yaml][app.yaml]{
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
//}

すると、画面に Starting server "default" running at: http://localhost:8080 といった文字列が出てきます。
このURLをコピーして、ブラウザで表示させてください。

//image[gae-result-1][結果]{
//}

晴れて自分のWebサーバが出来ました。

なお、「Starting admin server at: http://localhost:8000」と言った文字列で、
ポート番号が異なるURLがもうひとつ表示されているはずです。
こちらは「管理用」のサーバです。

//image[gae-result-2][管理画面]{
//}


=== HelloWorldプロジェクトをPyCharmから編集する



===[column] PyCharm Professional Edition、IntelliJ IDEA, そして Android Studio

PyCharm には有料のProfessional Editionと
無料で利用できるCommunity Editionの2種類があります。

本文の説明は全てCommunity Editionに基づいて行います。
どちらを用いても結果は変わりませんが、
Professional Editionの方が、本演習の範囲でも効率的な開発を行うことができます。

Professional Editionは30日間評価のための利用が出来るので、
あくまで評価する目的で、Professional Editionの方を選択しても構いません。
足りなくなる機能はありませんので、本文の指示通りに演習を進められなくなる
ことはないでしょう。

ただし、30日後に同様の機能を継続して利用したい場合、
Professional Editionはご自身で購入する必要があります。

本演習でProfessional Editionを利用する最大のメリットはおそらく、
GAEと開発環境のより緊密な連携です。
PyCharm Professiona Editionでは、
ターミナルからコマンドを実行せずにGAEのローカルサーバを
立ちあげたり、Googleへアップロードしたりすることができます。
詳細な比較には、以下のページを参照してください。
@<href>{http://www.jetbrains.com/pycharm/features/editions_comparison_matrix.html}

さて、PyCharmを開発しているJetBRAINS社は、
Android用の開発環境もGoogleとの共同で提供しています。
実はこれはEclipseと同様無料で利用できます。
PyCharmのような梯子はずしはありません。

この開発環境はAndroid Studioと呼ばれ、
今後のAndroid開発では標準的な開発環境に
なっていくことが予想されています。

JetBRAINS社は、IntelliJ IDEAと呼ばれる有料の統合開発環境を販売しており、
この環境を購入すると、
PyCharm Professional EditionやAndroid Studio、
更にはJavaScriptやRubyといった他のプログラミング言語の開発環境も
プラグインとしてついてきます。
ただし、その分だけ「お高い」開発環境です。
しっかりと、商売です。

もしPythonとPyCharm Professional Editionにも興味をもち、
さらにAndroidアプリとの連携を考えている今回のようなケースであれば、
独立した開発環境であるPyCharmではなく、
Android Studioの機能も利用できるIntelliJ IDEAを選択するメリットが
あるかもしれません。

===[/column]



=== メッセージを変える

Hello Worldでは味気がないので、別の文字列を出力してみましょう。
@<list>{hello_world}を以下のように変えます。

//list[hello_world_ja][例えばこのように変える]{
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

これで、隣の人にサーバを見せることができます。

本節のこれ以降の開発では、ローカルサーバは引き続きlocalhostを使って、
つまり自分だけが見る目的で起動することにします。


== 公開する

=== Google App Engine の開発者サイト

Google Developer Consoleでアプリを作成

=== アップロード

以下のURLをタイプし、講義に用いるGoogleアカウントでログインしてください。

@<href>{https://appengine.google.com/}

Google App Engineの利用規約を確認し、
"A accept these terms"をチェックした上でSubmitを押します。

//image[gae-tos][]{
//}

"Create Application"をクリックします。

//image[gae-web-console][]{
//}

Application Identifierとして「世界中で唯一」の名前を一つ設定します。
「ti-(受講生番号)」といったものでも構いませんし、別のものでも構いません。
ただし他の人が利用していた場合は選択できませんので、
右側の"Check Availability"ボタンで利用できるかを確認します。

その上で最下段の"Create Application"ボタンを押します。

//image[gae-ti-dmiyakawa][]{
//}

成功すると次のような画面がでます。

//image[gae-project-creation-successful][]{
//}

"Application Registered Successfully"と出たら、
app.yamlの"application"の右側の文字列を自分が選択したものに変えます。

//emlist[]{
application: (ここを先ほど選択したIDに変更する)
//}

//cmd{
> ./appcfg.py update --oauth2 HelloWorld
//}

これにより、OSに設定されたデフォルトのブラウザが起動して
Googleアカウントでのログインを要求されます。


なお、本項の説明は2014年09月21日時点のものです。
GoogleのWebコンソールは終始変化しており、
執筆時点でも複数のプロジェクト作成方法が存在します。

万が一本項による作成方法を採れない場合、
以下のURLから「プロジェクトID」を取得して先に進んでください。
これがapp.yamlのapplicationに入ります。

@<href>{https://console.developers.google.com/}

//image[gae-project-id][プロジェクトID]{
//}

===[column] 筆者注

@<href>{https://developers.google.com/appengine/docs/python/gettingstartedpython27/uploading}
に基づく。ただし説明が古い。

GAEのコンソールは今後近いうちに消滅する可能性が高いが、
特にDatastoreの内容を削除する機能が現時点で新コンソールに存在せず、
GAEでの開発の視点からすると明らかに破綻しているため、
敢えて旧コンソールを強行するという手に出ている。

特に次期以降にこの操作系の一部は書き換えになる可能性が高く、
最悪の場合GAEのサービス終了もありえるだろう。
そのくらい、現状のGAEはGCEの影響で混乱している。

===[/column]

=== デバッグ方法

Pythonではloggingというモジュールを用いることで、
AndroidにおけるLogCatと似たようなログ機構を頼ることができます。

@<href>{https://developers.google.com/appengine/articles/logging}

ローカルサーバの標準のログレベルはINFOです。
DEBUGレベルまで表示する場合は以下のようにします。
ただし、GAE側のデバッグログも表示されるため、
かなり見づらくなります。
今回はlogging.info()等を使うのが良いでしょう。

//cmd{
> ./dev_appserver.py --dev_appserver_log_level=debug HelloWorld
//}

本番サーバでは「計算処理 >> App Engine >> ログ」からloggingの結果を見ることができます。

//image[logging_on_gdc][本番サーバのログ]{
//}



== JSONを静的ファイルとしてGAEサーバで配布する。

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


== JSONを動的に生成して返してみる。

Webサーバに「動的に」データを入れます。
その前に、どのようなプロトコルで通信されているかを理解しましょう。

//emlist[JSONフォーマットを整形したもの、一部]{
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

シラバスアプリでは、この中から一部の情報を取り出して画面に表示します。
今回は以下の4つの情報をサーバから提供してみることにします。

 * date 講義が行われる日付
 * title その講義のタイトル
 * teacher 講師の名前
 * detail 講義の詳細 (サンプルのJSONファイルでは空でした)

これらを講義数分、上記のJSON形式で返せば良いわけです。
データをプログラムに埋め込んで返答させてみましょう。

//emlist[こーど]{
# 以上省略
from datetime import datetime, timedelta
from json import dumps

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
        self.response.write(data)
# 以下省略
//}

ローカル環境でデバッグ等を終えたら、アップロードします。

シラバスアプリのMainActivity.javaのsyllabusUrlを自分のサーバのURLに変更します。

//emlist[しらばす]{
private static final String syllabusUrl = "http://model-shelter-709.appspot.com";
//}

もしサーバとAndroidアプリの通信がうまくいけば、
サーバが自動生成した講義が配信されます。

//image[custom_syllabus][こうなる]{
//}


いつまでも「読込中」のSpinnerが表示され続ける場合には、
サーバのログ画面ととAndroidアプリのLogCatをよく見て問題を解決してください。

例えば、返答するJSONで"course"となっているべきところが"courses"(sが余計)
となっていた場合には、サーバではエラーは出ず、
LogCatで問題が報告されているはずです。

サーバとAndroidアプリをセットで開発する際には「どこが」問題になっているかを
調べる必要があります。


== 講義表を保存・変更出来るようにする

ここまでの方法ではまだデータを保存できていません。しましょう。

JSONを返すだけのサーバであれば良いのですが、
保存するためには他の画面も必要です。

 * JSONを表示するページ
 * Webサーバのトップページ (HTML)
 * 講義の新規登録
 * 講義の編集、削除

画面としてはこれらを実装します。

この背後では、先ほどの4つのデータをGAEに保存しておく必要があります。

 * date 講義が行われる日付
 * title その講義のタイトル
 * teacher 講師の名前
 * detail 講義の詳細 (サンプルのJSONファイルでは空でした)

先にこのデータ構造をサーバに実装することにしましょう。


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



== 演習 (オプショナル): Puttyを使って応答をデバッグする。

サーバとAndroidアプリのどちらもおかしくないように見えて、
挙動がおかしいときがあります。
こういったとき、通信プロトコルを解析するのが有用なことがあります。
「ネットワーク」において一度Puttyを利用していますので、
ここでも一度使ってみることにしましょう。


== 課題 (オプショナル)

本演習ではデータの更新はすべてWebフォーム上から行いました。
第14章「ネットワーク」の知識と組み合わせることで、
この部分もAndroid端末から行うことも可能です。

 * 編集用のActivityを作り、ボタンやメニューから移動できるようにしましょう
 * 編集後、「キャンセル」と「送信」が出来るようにしましょう
 * 「送信」がされたら、Serviceのような仕組みでサーバにデータを送りましょう。このとき「バックグラウンド」と「ネットワーク」の章を復習すると方法が分かります。
 * (難) ログインシナリオをここでも有効に機能するようにしましょう。
 * (難) 管理者だけから変更を許すようにしましょう。方法はお任せしますが、管理者アカウントのパスワードは使わないようにしたほうが良いでしょう。サーバ側に動的に変更可能なOAuth風の「トークン」を生成する機能を作ると、そこそこ信頼できる機構になります。
 * (難) Android端末にひもづけられているアカウント情報を使い無駄なログインを省きましょう。


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


== 最終課題

Tech Instituteの講義情報アプリケーションを
本章の知識を用いてWebサーバとAndroidアプリのセットで作れないか考えてみよう。

どのような連携を行えばよいか、
あるいはどのような情報が特定のユーザに「行き渡ってはいけないか」
を考え、アプリを実装する上でどのようにそれを実現するかを考えてみよう。

シラバスアプリに加えて、例えば以下のような機能が期待される。

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
