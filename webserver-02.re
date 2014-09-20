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

== GAE SDK

 * https://developers.google.com/appengine/downloads

//image[install-gae-1][@<href>{https://developers.google.com/appengine/downloads}のスクリーンショット]{
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
興味があれば覗いてみましょう。

//image[gae-result-2][管理画面]{
//}

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

@<href>{https://console.developers.google.com/}

自動生成されるプロジェクトIDを確認します。

//image[gae-project-id][プロジェクトID]{
//}

//cmd{
> ./appcfg.py update --oauth2 HelloWorld
//}

これにより、OSに設定されたデフォルトのブラウザが起動して
Googleアカウントでのログインを要求されます。


以下筆者注:

@<href>{https://developers.google.com/appengine/docs/python/gettingstartedpython27/uploading}
に基づく。
ただし説明が古い。

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

DEFAULT_COURSE_LIST_NAME = 'default_course_list'

class Course(ndb.Model):
    date = ndb.DateTimeProperty()
    title = ndb.StringProperty()
    teacher = ndb.StringProperty()
    detail = ndb.StringProperty()

class MainPage(webapp2.RequestHandler):
    def get(self):
        course_model_list = Course.query().order(-Course.date)
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
        course_model_list = Course.query(ancestor=course_list_key()).order(-Course.date)
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
        course_model_list = Course.query(ancestor=course_list_key()).order(-Course.date)
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



=== 関連URL

@<href>{https://developers.google.com/appengine/docs/python/gettingstartedpython27/usingdatastore}

Datastoreで指定できる型は以下の通り。

@<href>{https://developers.google.com/appengine/docs/python/ndb/properties}


@<href>{http://webapp-improved.appspot.com/}



== ログインを実装する。

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

//}




@<href>{https://developers.google.com/appengine/docs/python/gettingstartedpython27/templates}

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
