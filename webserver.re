= Webサーバ
== 自分のWebサーバを作ろう

//lead{
本章では視点を変え、
Androidアプリにデータを提供するWebサーバの側の技術を学びます。
実際にデータを配信するサーバを実装してみましょう。
//}

=== 本章全体で学ぶこと

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

本章では、第13章「ネットワークプログラミング」で学んだことを前提にしています。
一度復習してから本章を読んだほうが良いでしょう。

=== Webサーバとは？

Webブラウザを用いてWebページを閲覧するのが当たり前の時代ですから、
Webという言葉自体についてはイメージが付くかもしれません。

「Webサーバ」の「サーバ(Server)」という単語は、
「ジュースサーバ」といった表現で日常会話でも登場します。
IT業界以外でも「給仕する人、接客係」といった意味で使われる言葉です。

//image[juiceserver][ジュースサーバ。画像の版権確認なし。要確認]{
//}

堅苦しく言えば、サーバは「資源を要求する側に、その資源を提供」します。
資源をリソース(Resource)と呼び、「資源を要求する側」を
「クライアント(Client)」と呼びます。
クライアントという言葉は、日本ではコンサルタントを受けるお客さんに対してもよく使われます。

本章では以降「サーバ」「クライアント」「リソース」と記載します。
Server、Client、Resourceという英単語はAndroidを含めた英語の
ドキュメントにはよく登場するので、覚えておきましょう。

ジュースサーバの場合、クライアントが要求するリソースはジュースでした。
Webサーバの場合にも、リソースを求めてWebブラウザやAndroidアプリがアクセスします。
あるいは、リソースを補充するためにもアプリがアクセスすることがあります。
GMailのようなWebメールと呼ばれるようなサービスでは、どちらも行われます。
ジュースサーバであれば、店員がジュースを補充するはずです。

ごく簡単に言えば、蜘蛛の巣(英単語でWeb)のように、
コンピュータを相互接続したネットワークが世界規模で張り巡らされていて、
WebブラウザやAndroidアプリといった「クライアント」と「サーバ」が
容易に接続出来るのがWebの特徴です。

本章では「インターネット」と「Web」の区別や、
「Webサーバ」と「インターネット上の他のサーバ」の区別や、
「Webサーバ」と「Webアプリケーション」の区別といった点については深く扱いません。
みなさんがWebブラウザで見る「向こう側」を全て
「Webサーバ」「Webアプリケーション」という一括りで考えることで、
物事を単純化して説明を行います。


==== 自分のWebサーバが必要な場面

Webサーバとの関係で考えたとき、
Androidアプリはおおまかに次の3種類に分けられます。

 * そもそもネットワーク接続が必要ない。
 * 誰かが用意したWebサーバを利用する
 * (本章のように)自分でWebサーバを準備する

1つ目のタイプは今回はおいておきましょう。
本章では3つ目のタイプを実践することを通じて、
Androidアプリ開発者が最も多く出会うであろう、
2つ目のタイプについても役立つ知見を得ることが目標です。

今回は、例えばGoogle Play Game Services(@<href>{https://developer.android.com/intl/ja/google/play-services/games.html})の機能を使って、
ゲームの「アチーブメント」情報をアップロードする、
といった場合も2つ目に含めています。
こういう場合にはしばしばVolleyのような外部ライブラリを
用いてネットワーク通信を意識しないことがありますが、
ライブラリの背後ではしばしば今回のような事情が発生しています。

3つ目のケースはAndroidアプリというよりはサービス全体の設計も含めて
考える職業の場合に発生します。
仮にベンチャー企業で全く新しいWebサービスを運営しようと思って
Androidアプリとセットで開発しようと思った場合に発生します。

すでに存在しているWebサービスをAndroidアプリと緊密に連携させるため、
Webサーバ側も修正しつつAndroidアプリを実装するという状況も少なくありません。

==== シラバスアプリのサーバを作るケースを考える

第10章「ユーティリティによる実践」で、シラバスアプリを実装しました。
本章ではこの例を拡張して、シラバス提供用のWebサーバを作ることにします。

シラバスアプリの演習では、
Webサーバに保存されたJSONデータをVolleyというライブラリを用いてダウンロードし、
その内容をListViewに表示しました。(@<img>{syllabus-screenshot})

//image[syllabus-screenshot][シラバスアプリ]{
//}

この時に使用したデータ取得URLは、
@<href>{https://dl.dropboxusercontent.com/u/1088314/tech_institute/2014/syllabus.json}
です。
このURLは講師が公開している単なる静的な(中身が変わらない)ファイルを表しています。
変更するには、公開している人自身が直接そのファイルを修正する必要があります。

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


==== Webサーバを支える技術

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
この辺りの事情は詳述しません。

本章ではその中で、Googleが提供するWebアプリケーションフレームワークの
一つである、Google App Engineを選択します。

==== Google App EngineとそのPython実装について

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

主な目的はWebサーバのエキスパートになることではなく、
仕組みを理解することです。
PythonやGAEの習得が目的ではないので、
演習はPythonや深く理解する必要がないように構成しています。
@<fn>{about_additional_material}

//footnote[about_additional_material][念の為、Python言語自体の説明を別の節(？)に分けて教科書に収録しました。]

==== 本演習を行う上での注意事項

本演習では、実装したWebサーバを全世界に公開します。
Googleアカウントによるログインを除いて、
個人情報やプライバシーに関わる情報を実装に含めるのは避けてください。

また、Androidアプリ開発とは関係がないソフトを複数インストールします。
特に以外で本演習を行う場合、システムのバックアップを作成しておいた方が良いでしょう。
Windows 8.1の場合について次節の
「Windows 8.1 でのバックアップの仕方」
に記載があります。


=== WebサーバでHello World!
==== Python実行環境のインストール

GAEを用いてWebサーバを開発するために、
まずPython実行環境をPCへインストールします。
インストールするバージョンはPython 2.7.8です。

以下のURLからWindows向けPythonインストーラを取得します。
「Windows X86 MSI Installer (2.7.8)」を選択してください。

Pythonには3で始まる新しいバージョンがありますが、
今回はインストールしないでください。

 * @<href>{https://www.python.org/downloads/release/python-278/}
 ** (@<href>{https://www.python.org/ftp/python/2.7.8/python-2.7.8.amd64.msi})

//image[install-python-1][@<href>{https://www.python.org/downloads/release/python-278/}のスクリーンショット]{
//}

ダウンロードした「python-2.7.8.msi」というファイルを実行すると、
@<img>{install-python-beginning}のような画像が表示されるはずです。
ここでは「Install for all users」が選択された状態で「Next」ボタンを押します。

//image[install-python-beginning][Install for all users]{
//}

インストール先を確認されます。
「C:¥Python27¥」となっていることを確認して「Next」を押します。

#@warn(TODO: 出来れば撮り直す。一時ファイルが画面に入っていて実際と異なる)

//image[install-python-destination][C:¥Python27¥]{
//}

@<img>{install-python-add-path-1}のような画面が出たら
「Add python.exe to Path」をクリックし、
表示された2つの選択肢のうち"Will be installed on local hard drive"を選びます。

//image[install-python-add-path-1][画面表示時の×印を変更する様子]{
//}

すると×印がなくなりますので「Next」を押します。

//image[install-python-add-path-2][×がなくなった状態になったら、「Next」]{
//}

@<img>{install-python-reboot}のように再起動を要求されることがあります。
このときは、PCを再起動してください。

//image[install-python-reboot][再起動を要求されたらPCを再起動する]{
//}

以上の作業が終了したら、Python実行環境の動作確認をします。
Windows画面左下の開始ボタンを押して「コマンド プロンプト」を選択後、
「Python」と入力して、Pythonの対話型環境が起動するかを確認してください。

//image[python-prompt][Pythonプロンプトの実行例]{
//}

==== Google App Engine SDKのインストール

次に、Google App EngineのPython SDKをインストールします。
以下のURLを開いてください。

 * @<href>{https://developers.google.com/appengine/downloads}


「Google App Engine SDK for Python」をクリックします。@<img>{install-gae-1}

#@warn(GAEのバージョンが1.9.11から1.9.12に変わった。注意)

//image[install-gae-1][Google App Engine SDK for Python]{
//}

適切なOSのインストーラを選びます。

//image[install-gae-2][Windows PCであればWindows版のmsiファイルを選ぶ]{
//}

ダウンロード後、インストーラを起動します。
Python 2.7をSDKインストーラが認識していることを確認した上で、
Nextボタンをクリックします。@<img>{install-gae-python-checked}

//image[install-gae-python-checked][Next]{
//}

「End-User License Agreement」では「I accept the terms in the License Agreement」をチェックを入れて、「Next」を押します。

//image[install-gae-tos][「End-User License Agreement」]{
//}

インストールフォルダは変更しません。
表示されるチェックボックス3つの全てにチェックが入っていることを確認して、
「Next」を押します。

//image[install-gae-all-checked][3つのチェックが入っていること]{
//}

最後にInstallボタンをクリックします。

//image[install-gae-do-install][Install]{
//}

Install中、ダウンロードしたソフトウェアを信用するかどうかを確認するシステムダイアログが表示されます
このときは、「確認済みの発行元」が「Google Inc」となっていることを
確認した上で、「はい」を押します。

インストールが成功したら、「Run Launcher」ボタンを押した後、
「Finish」ボタンを押します。

//image[install-gae-done][]{
//}

「Google App Engine Launcher」というWindowが起動すれば、
GAE SDKのインストールに成功しています。
@<fn>{if_using_japanese_name}

//image[gae-installed][]{
//}

//footnote[if_using_japanese_name][Windows 8等で日本語ユーザ名になっている場合、ここからさらに作業が必要です。〜「ユーザフォルダに日本語が含まれる場合のGAEインストールの追加作業」を参照してください。]


==== ローカルサーバでHello World!

GAEのプロジェクトを作成し、"Hello world!"と表示するだけの
Webサーバを作成しましょう。

Google App Engine Launcherで「File >> Create New Application」を選択します。

 * Application Name を「helloworld」等に設定します。
 ** ここでは一部の文字、例えば半角大文字は使用できません。
 ** 半角英数字だけで構成するのがベストです。
 * Parent Directory を「デスクトップ」に設定します。
 ** 日本語ユーザである場合は、別途用意したフォルダを設定します。

//image[create-helloworld][Application Nameは今回はhelloworldとした]{
//}

作成できたらそのプロジェクトを選択し「Run」をクリックします。

//image[run-helloworld][helloworldプロジェクトが出来た]{
//}

成功すると、プロジェクトの左にある小さなアイコンが
緑色の「再生ボタン」のようになります。

//image[running-helloworld][helloworldの横の緑の「再生ボタン」に注目]{
//}

この状態でブラウザから「@<href>{http://localhost:8080}」を開きます。

"Hello world!"と表示されたら、成功です。

//image[seeing-helloworld][Webブラウザで@<href>{http://localhost:8080}を開く]{
//}

8000番で、このプロジェクトの管理用Webページを開くこともできます。
@<fn>{about_tcp_port}

//footnote[about_tcp_port][サーバが利用するTCPポート番号は、複数のプロジェクトを生成すると変化する可能性があります。その場合、以降の説明の数字を変更してください。ポート番号そのものについての説明は14-1-5「ポート番号とは？」を参照してください。]

このサーバは現在「実行中」の状態です。
これを停止するには、GAE Launcherから「Stop」ボタンを押します。
「再生」ボタンが黒い丸印になれば、サーバプロセスが停止しています。

//image[stopped-helloworld][サーバ停止時には左端のマークは黒い丸印]{
//}

入力したURLについて説明します。

 * http はどのようにサーバに接続するかを決めたもの(スキームと呼ばれる)。
 * localhostは「自分自身」で、つまりPCのことで、「ローカルサーバ」と呼んでいます。
 * TCPポートの8080番でサーバを作成している、という意味になります。

この"Hello world!"を表示するまでに、プログラムを1行も書く必要がありませんでした。
ちょっと物足りなすぎです。

==== PyCharm Community Edition のインストール

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

//image[pycharm-select-page][PyCharmのダウンロードページ。無料版は右側のCommunity Edition]{
//}

途中で表示される「Create Desktop shortcut」
(デスクトップにショートカットを作成する)
のチェックボックスをつけておくと良いでしょう。

//image[pycharm-installer-create-desktop-shortcut][Create Desktop shortcutのチェックをつけておくと、演習を効率的に行える。]{
//}

最後に「Run PyCharm Community Edition」(PyCharm Community Editionを起動する)
にチェックボックスを入れて「Finish」を押します。

//image[pycharm-installer-run-pycharm][Run PyCharm Community Editionをチェックして、Finish]{
//}

初回起動時に@<img>{pycharm-complete-installation}のような表示が出ます。
今回はそのまま「OK」を押します。

//image[pycharm-complete-installation][初回起動時に表示される。そのままOK]{
//}

さらに@<img>{pycharm-first-configuration}のような画面が出ます。
ここで、Eclipse風のキーバインドにしたい場合は
「Keymap scheme」を適切に変更します。
なお、この設定は後述する通り、変更可能です。

//image[pycharm-first-configuration][初回起動時に開発環境の初期設定をどうするか、聞かれる]{
//}

最後に@<img>{pycharm-welcome}のような画面が出れば
PyCharmの起動が成功しています。

//image[pycharm-welcome][起動成功]{
//}

なお、PCによっては@<img>{pycharm-firewall}のような
警告が表示されることがあります。
ここではそのまま「アクセスを許可する」ボタンを押します。

//image[pycharm-firewall][Windowsのファイアウォールの設定によっては警告を発することがある]{
//}

既にhelloworldプロジェクトを作っていますので、ここではそれを
PyCharm上で編集してみましょう。
「Open Directory」を選択し、helloworldプロジェクトが存在するフォルダを開きます。

//image[pycharm-select-helloworld][helloworldプロジェクトの場所を指定する]{
//}

@<img>{pycharm-helloworld}のような画面が出れば、
プロジェクトを開けたことになります。

//image[pycharm-helloworld][helloworldプロジェクトをインポートした様子。画面左側に注目]{
//}

ここで@<img>{pycharm-tip-of-the-day}のような
ダイアログが表示されることがあります。

//image[pycharm-tip-of-the-day][PyCharmのTipsを表示してくれるが、今回は不要]{
//}

今回は左下の「Show Tips on Startup」(起動時にTips画面を表示する)のチェックを
外して、「Close」ボタンを押します。

PyCharmは同じく統合開発環境であるEclipseと似た外見をしています。
しかし、全く一致した使い勝手というわけではありません。
以下に、すぐに使い勝手を改善出来るヒントを示しますので、
興味があればさらにカスタマイズしてみましょう。


 * 起動時にkeymapを変更していない場合「File >> Settings >> Keymap」でKeymapsを「Eclipse」に変更すると、Eclipse風になります。@<img>{eclipse_keymap}ただし全てがEclipseと同じになるわけではありません。
 * 「File >> Settings >> (左画面IDE Settingsの) Editor >> Editor Tabs >> Mark modified tabs with asterisk」のチェックを入れておくと、Eclipseの時と同様保存されていないファイルのタブに*(スター、もしくはアスタリスク)が付きます。@<img>{asterisk}

//image[eclipse_keymap][Eclipseキー配列「風」にする]{
//}

//image[asterisk][ファイルの変更を保存していない時に気づきやすくなる]{
//}

ここまででようやくPythonでプログラミングする準備が整いました。
プログラミングに自信があれば、このままGAEによるWebサーバの実装を進めましょう。


==== Hello Worldサーバへブラウザからアクセスする

「Hello world!」以外の文字列を出力してみましょう。
PyCharmの画面左、「helloworld」となっている部分をクリックし、
展開したファイル一覧を確認してください。

 * app.yaml
 * favicon.ico
 * index.yaml
 * main.py

それぞれのファイルの意味は以下のとおりです。

 * main.pyはWebアプリケーション本体のソースコードです。
 * app.yamlとindex.yamlは、Androidで言えばAndroidManifest.xmlに相当するような、アプリの設定ファイルです。
 * favicon.icoはWebページを開いたときにタブに表示されるアイコンです。

主に他の3つのファイルを編集することで機能を追加していきます。

main.pyのPythonコードは、コメントを除いて@<list>{all_source}に示したとおりです

//list[all_source][GAEを用いたWebサーバの全文]{
import webbapp2

class MainHandler(webapp2.RequestHandler):
    def get(self):
        self.response.write('Hello world!')

app = webapp2.WSGIApplication([
    ('/', MainHandler)
], debug=True)
//}

「Hello world!」という文字列を変更したいだけなら、
@<list>{hello_world}の行を変更すればよさそうです。

//list[hello_world][このような行が見つかる]{
        self.response.write("Hello world!")
//}

helloworld.pyの内容を変更してファイルを保存後、
GAE Launcherで再びローカルサーバを起動して変更を確認してみましょう。

==== Hello World!プログラムの意味

Pythonプログラミングに関する部分を除くと、
今回のプログラムの意味はおおよそ次のとおりになります。

まず、webapp2というPython特有のライブラリでWebサーバを実装しています。
Webサーバを実装するためのフレームワークは、
使用するプログラミング言語に限らず似ている部分がありますので、
瑣末な点にはこだわらず、今回は共通部分に注目していきます。

Webサーバを実装する上での「共通部分」とは何かと言えば、
クライアントとサーバの間でやりとりする通信のプロトコル(protocol)です。
@<fn>{about_protocol}
Webサーバとクライアントのやりとりには
HTTP(Hyper Text Transfer Protocol)を用います。
この言葉は14章「ネットワークプログラミング」でも説明されています。
そこで、HTTPにまつわる用語がとても良く登場します。

//footnote[about_protocol][「プロトコル」と言う言葉は他の分野では「外交儀礼」という意味で使われたりします。たとえば他国の王族をもてなす際には一定の手順、すなわちプロトコルを踏まえるのがならわしです。この場合、プロトコルを違反すると、場合によっては外交問題になります。]

MainHandlerというクラスのget()関数が実際にWebサーバで何を返すか示しています。
ここでは第14章「ネットワーク」で既に登場している、
HTTPのGETリクエストを処理するためのget()関数を定義しています。

HTTPには、目的に応じて何種類かリクエストの種類がありますが、
クライアント(Androidアプリやブラウザ)がデータを受け取る場合は
GETリクエストが一般的です。
データを更新する場合はPOSTリクエストという別のリクエストを用います。
POSTについては本項で後で登場します。

ここでは一旦GETリクエストのみ実装します。
GAEでは実装していないリクエストは適切にエラー処理されます。
それ以外のHTTPのリクエストの種類は今回は省略します。

Hello Worldサーバでは、
「app = webapp2.WSGIApplication(...)」
という関数に渡されるリストによって、
Webサーバにクライアントからのリクエストが到着した際に、
URLのどのパスをどのクラスが担当するかを決めています。
@<fn>{class_and_instance}

//footnote[class_and_instance][実際には、GAEによって、自動的にこのクラスのオブジェクトが生成されており、それがリクエストに応答します。]

@<href>{http://localhost:8080/}」のポート番号「8080」の後の部分が
「パス」(path, 「経路」)です。
今回は「/」、つまりパスなしのケース「だけ」を
MainHandlerクラスが受け持つことになっています。

試しにブラウザに入力するURLを@<href>{http://localhost:8080/example}
に変更して、サーバにアクセスしてみてください。

「404 Not Found」と出てくるはずです。
@<fn>{gae_responsible_for_404}

//footnote[gae_responsible_for_404][実装がない際の表示はGAE Pythonのローカルサーバが出しています。]

//image[my_own_404][404]{
//}

「全てのパスをこのMainHandlerクラスがが受け持つ」ように実装を変更してみましょう。

//emlist[]{
application = webapp2.WSGIApplication([
    ("/.*", MainHandler),
], debug=True)
//}

このようにした場合、404 Not Foundは表示されなくなり
「Hello world!」が再び表示されるようになるはずです。


==== HTTPのステータスコードについて

「404 Not Found」は普通のWebブラウジングでも見たことがあるかもしれません。
HTTPというプロトコルつまりルールで、サーバはそのリクエストに対応する
リソースがない場合、404番という番号をクライアントに返すことを期待されています。

クライアントがサーバにリクエストを送信した際、
成功しても失敗しても、サーバからは何らかの数字が返されます。
これをステータスコードと呼びます。
Webブラウジングしているときには意識しませんが、
Webページが正しく存在している時、
サーバからは「200番」という数字が返されます。

ステータスコードは左端の桁が主要な理由を示しており、
残りの2桁でより具体的な状況を説明します。
いくつか挙げてみます。

 * 200 OK: リクエストで要求されたものが存在したので送ります。
 * 301 Moved Permanently: リクエストしたリソースは永久に別の場所へ移動しました。
 * 404 Not Found: そんなリソースは知りません。
 * 508 Internal Server Error: サーバ内部でエラーが発生しちゃった！

なお、404 Not Foundの場合でもコンテンツを返せないわけではありません。
GitHubのWebサイトで存在しないページへアクセスした時の例を@<img>{github_not_found}示します。

//image[github_not_found][GitHubの404 Not Foundは絵付き]{
//}

GAEでも、ステータスコードをPython実装上で指定したり、
GitHubのように見た目の異なる404画面を表示させることもできますが、
今回は割愛します。
GAE Pythonでのレスポンスを操作する際の詳細は、例えば@<href>{https://cloud.google.com/appengine/docs/python/tools/webapp/redirects}を参照してください。

==== 世界にHello World!

Google App Engineの便利なところは、
この時点ですでに、世界へWebアプリケーションを公開する準備がほぼできていることです。
Googleの本番サーバにアップロードし、世界にアプリケーションを公開してみましょう。
@<fn>{be_careful_about_privacy}

//footnote[be_careful_about_privacy][公開したサーバは講義室内に限らず、インターネット上のユーザから見ることができます。今回の演習では、開発段階から個人情報やプライバシーに関わる情報は含めないほうが良いでしょう。]

公開するには、Googleアカウントで開発者登録をした上で、
Webアプリのための「アプリケーションID」を取得します。
以下のURLをタイプし、講義に用いるGoogleアカウントでログインします。

@<href>{https://appengine.google.com/}

"Create Application"をクリックします。

//image[gae-web-console][ブラウザから「Create Application」]{
//}

@<img>{gae-app-id}で、Application Identifierとして
「世界中で唯一」の名前を一つ設定します。
他の人が利用している名前は選択できません。
これはそのまま公開するサーバ名になるので、
おかしな名前も避けたほうが良いでしょう。

//image[gae-app-id][他のユーザが使っていないIDを決める。URLに含まれるので注意。]{
//}

アプリケーションIDを考えたら、
右側の"Check Availability"ボタンで利用できるかを確認し、
実際にそのアプリケーションIDを使用出来ることを確認します。

Google App Engineの利用規約が表示されている場合は内容を確認し、
「I accept these terms」をチェックした上で「Submit」を押します。


最後に、最下段の"Create Application"ボタンを押します。

//image[gae-ti-dmiyakawa][Create Application]{
//}

成功すると次のような画面がでます。

//image[gae-project-creation-successful][出来た。]{
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
アプリケーション固有パスワードを準備してそれを使用します。

ログで「Deployment successful」と出たら成功です。

「@<href>{http://(自分が生成したID).appspot.com/}」へアクセスしてみましょう。

せっかくですから、隣の人に自分のWebサーバを見てもらってはいかがでしょうか。


==== ログ出力方法

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

今回のサーバ実装ではlogging.info()だけ使えば十分でしょう。
そのモジュールを用いるため、Javaと同じように「import logging」
という命令を予め入れておく必要があります。
@<fn>{about_python_logging}

//footnote[about_python_logging][Pythonのloggingについて詳細に学びたいは、まず@<href>{http://docs.python.jp/2/howto/logging.html#logging-basic-tutorial}を読むことから始めると良いでしょう。]

==== 本番サーバのダッシュボードとリソース割り当てについて

GAE Launcherには「Dashboard」というボタンもあります。
このボタンを押すと、本番サーバの管理者画面をブラウザに表示してくれます。
ここから本番サーバでのログ出力も見ることができます。

//image[gae-dashboard][GAEのダッシュボード]{
//}

Google App Engineは小規模なサーバを起動する分には無料で利用できます。
より正確には、それぞれのアプリに無料で利用できる一日あたりの割り当て
が決まっており、その範囲内のアクセスであれば料金はかかりません。
割り当てとその消費量は「Quota Details」から見ることができます。

//image[quota][Quota(割り当て)]{
//}

第三者からの大量のアクセスがあったりした際に、
この割り当てを使い尽くしてしまう可能性があります。
その場合、すぐに課金が発生するわけではなく、ユーザにはエラーが表示されます。
明示的にクレジットカードを登録しない限りは発生しません。

今回の演習の範囲でこの問題を考える必要はないはずですが、
仮にGAEで規模の大きなサービスを実装する場合、
少なくとも以下の2点について対処する必要があります。

 * クレジットカードを登録し、アクセスに応じた課金を行えるようにする。
 * アクセス負荷が小さくなるよう、GAEに適した形でアプリケーションの設計を考えなおす。

電気代やインターネット接続料金を誰がどう支払うか、
といった点を含めて、様々な料金体系のサービスがあります。


=== シラバスアプリのサーバを作りこもう
==== シラバスアプリでやりとりするデータ構造を考える

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

元のデータは人が読めるようにフォーマットされていないため大変読みづらいですが、本質的には@<list>{json_data}で示されるような構造になっています。（構造が分かりやすい回を抜粋して掲載しています）

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
以下の4つの情報だけを使っていることが分かります。

 * date 講義が行われる日付
 * title その講義のタイトル
 * teacher 講師の名前
 * detail 講義の詳細 (サンプルのJSONファイルでは空でした)

それ以外のデータは全く使っていないようなので、
今回のWebサーバでは、これら4種類のデータだけを扱うことにします。

言い換えると、Webサーバは講義数分だけこの4種類のデータを繰り返し持つ
JSON形式のデータをクライアントに返答すれば良いのです。

==== 自分のWebサーバから、自動生成したJSONデータを返す

Hello Worldサーバの実装を変更し、
講義情報が機械的に生成されるようにして、
その結果ををクライアント側に返送するWebサーバにしてみます。

なおここで日本語を表示するのですが、
Pythonのルールとして冒頭に「-*- coding: utf-8 -*-」
というコメント行をファイル冒頭に含めておく必要があります。

//emlist[helloworld.pyをこのように変更する]{
# -*- coding: utf-8 -*-

import webapp2

from datetime import datetime, timedelta
from json import dumps
import logging, pprint

class MainHandler(webapp2.RequestHandler):
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

application = webapp2.WSGIApplication([
    ("/", MainHandler),
], debug=True)
//}

ここでは、Python言語で生成したデータ構造を(json.dumps()関数で)JSONの文字列
として返答しています。

サーバが返すデータがどのような形式のデータかを、
ブラウザやAndroidアプリは事前に理解する必要があるため、
「Content-Type」という情報を指定しておく必要があります。
あわせて日本語を表示するために文字コードの指定もします。
本講義の範囲では次のことが分かれば十分です。
つまり、サーバから送られるデータがHTMLなのか、
あるいはPDFや動画データといった別のデータかを区別するために、
サーバはクライアントとユーザに見えないところでやり取りをしています。
サーバを実装するにはこれらを意識して送信しないと、
ブラウザの挙動が不適切になったりするのです。

==== 生成されたJSONデータをブラウザで確認する。

ローカルサーバ(@<href>{http://localhost:8080})から、
JSON形式のデータを閲覧出来ることをブラウザを用いて確認してください。

//image[json-on-browser][JSONをブラウザから見る]{
//}

問題無さそうであれば、
作成したサーバアプリケーションを再び本番サーバにアップロードします。

本番サーバのURLへブラウザでアクセスし、
やはり同様のJSONデータを取得できることを確認します。

ここまでで、世界に公開された状態で講義表データを返すWebサーバが完成しました！

==== シラバスアプリにサーバのJSONデータを読み込ませる

早速、シラバスアプリでこのデータを表示させてみましょう。
ここで一旦、Eclipseで作業を行います。混乱しませんように。

シラバスアプリがEclipseに存在することを前提に説明をします。
もし準備ができていない場合には、先にそちらを完成させるか、
GitHubから最新のプロジェクトをダウンロードし、
Eclipseにインポートして準備してください。

次節で、シラバスアプリをインポートする方法を説明しています。

シラバスアプリのMainActivity.javaのsyllabusUrlを自分の本番サーバのURLに変更します。

//emlist[しらばす]{
private static final String syllabusUrl = "http://(自分が指定したID).appspot.com";
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

サーバとブラウザの間のやりとりができていることは分かりました。
しかし、Androidアプリの画面を見るだけでは、
具体的にどのようなやり取りをしているのかは分かりません。

AndroidアプリとWebサーバをセットで開発している際、
実際にクライアントとサーバが何を通信しているのかを詳しく調べたいことがあります。
以下では2つの方法を簡単に紹介します。

==== Chromeの開発者コンソールを開いて通信の詳細を覗いてみる

現在はブラウザ自体に豊富な機能があるため、Webサーバとの通信の様子を
調べるのはまずこの機能を使うことが多いです。
Chromeでは「デベロッパーツール」と呼ばれています。

デベロッパーツールを起動するには、
JSONデータを表示したChromeで右クリックをし「要素を検証」をクリックしてください。
すると、Chromeの下方にデベロッパーツールが表示されます。

//image[developer-tools][Chromeのデベロッパーツール]{
//}

試しに「Network」タブを選択し、その状態でWebページを再読み込みしてみてください。
すると、@<img>{developer-tools-2}のように表示されるはずです。
今回はただJSONデータを取得するだけなので1行だけ増えるでしょう。

//image[developer-tools-2][デベロッパーツールで通信の内容を見る例]{
//}

この例では、ブラウザがGETリクエストをサーバに送信し、
サーバから約18ミリ秒でステータスコード200とデータが帰ってきたことが分かります。

他のWebページで試してみるとより面白いでしょう。
Webページ1ページを表示するためにどれだけのGETリクエストが
行われているかが分かるはずです。

Webサーバとクライアント間をまたぐサービスを実装する場合、
サーバ側のログとクライアントの画面表示のみならず、
ネットワーク間の通信がどのように行われたかも把握する必要がしばしばあります。
そういった場合、ブラウザ付属のツールを覚えておくととても便利です。

「Chrome デベロッパーツール」などと検索をすると多くの情報が得られるはずですので、デベロッパーツールについて詳細に学びたい方は調べてみてください。
@<fn>{putty_is_useful}

//footnote[putty_is_useful][ブラウザの開発者向けツールよりも更に込み入った細かい情報を調べたい時があります。こういったときには、第14章「ネットワークプログラミング」で利用したPuttyのような「生」のデータを扱えるツールが有用なことがあります。]


==== 講義表を保存・変更出来るようにする

ここまでで、Webサーバ上で生成したJSONデータを
Androidアプリに与えることに成功しました。
しかし、このデータはPythonが勝手に生成したもので、
本当に価値のあるデータとは言えません。

このWebサーバをさらに発展させて、人の手で講義データを追加出来るように
してみましょう。

本節では、以下の4つのシナリオに対応することにします。

 * 講義一覧をHTMLで表示するトップページ
 * JSON形式で講義を表示するページ
 * 講義の新規登録ページ

講義の編集や削除は、演習問題とします。

講義データを人の手で追加出来るWebサーバを実現するには、
GAE上でデータを保存できるようにする必要があります。
既に議論したJSONデータの中にある4種類の情報をひとまとめにして、
そのまとまりを複数保存できる必要があるはずです。

 * date 講義が行われる日付
 * title その講義のタイトル
 * teacher 講師の名前
 * detail 講義の詳細 (サンプルのJSONファイルでは空でした)

この4つのデータを、
言ってみれば「1つのオブジェクト」のようにして保存して欲しいわけです。

まず、このデータ構造をサーバに実装することにしましょう。
今回はGAEのDatastoreという仕組みを利用します。
helloworld.pyに次の実装を追加してください。

//emlist[]{
# (その他のimport文)

from google.appengine.ext import ndb

def course_list_key():
    return ndb.Key('CourseList', 'default_course_list')

class Course(ndb.Model):
    date = ndb.DateTimeProperty()
    title = ndb.StringProperty()
    teacher = ndb.StringProperty()
    detail = ndb.StringProperty()

# (MainHandlerの実装が続く)
//}

保存する形式にはプログラミング言語と似たデータ型があります。
GAEのDatastoreで利用できるデータ型は@<href>{https://developers.google.com/appengine/docs/python/ndb/properties}に掲載されています。
今回は「日付」に対応するデータと文字列データさえあれば事足ります。

course_list_key()関数は、GAE特有のおまじないとでも考えてください。
この関数自体はすぐ後で使うため、省略しないように。

GAEではここでさらに、データを順序立てて検索するために
「インデックス」と呼ばれるデータを用意する必要があります。
詳細は省略しますが、今回は「日付」でソートする前提で、
index.yamlに次のように書いておきます。

//emlist[index.yaml (app.yamlではないので注意)]{
indexes:
- kind: Course
  ancestor: yes
  properties:
  - name: date
//}

この時点ではまだ保存するデータ構造を定義しているだけで、
使っていません。
とりあえずJSONデータを表示するのに使ってみましょう。

//emlist[]{
# (import文やCourseクラス)

class MainHandler(webapp2.RequestHandler):
    def get(self):
        course_model_list = Course.query(ancestor=course_list_key()).order(Course.date)
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
    ("/", MainHandler),
], debug=True)
//}

まずローカルサーバを起動してブラウザで結果を確認します。
ここではまだ講義データを一切登録していないので、
JSONは以下のような「空」のコース一覧を表示します。

//emlist[結果は空の結果を持つJSON]{
{"course": []}
//}

なお、本番サーバへindex.yamlを最初にアップロードした後しばらくの間は、
@<img>{need_index_error}のように
「NeedIndexError: The index for this query is not ready to serve. ...」(検索に使うインデックスが準備できていません)というエラーが表示されます。
これが発生するのは、GAEでデータを取得する際にインデックス(索引)が必要で、
それをGAE自身が用意するのに少し時間がかかるためです。
大体数分程度待てばエラーはなくなります。

//image[need_index_error][NeedIndexError]{
//}

なお、似たようなエラーとして「NeedIndexError: no matching index found.」(一致するインデックスがありません)というエラーメッセージが出ることがあります。
この場合はPythonプログラムで指定しているソートの方法とindex.yamlで生成しているインデックスが一致していません。
つまりプログラミング上に何か間違いがあります。

講義作成画面が欲しいところですが、
その前にHTMLで講義データを表示する実装も作っておきます。
講義作成画面を実装した際、ユーザからすぐにそれを使えるように、
HTMLに次に作る画面のためのHTMLタグも入れてしまいます。

これまでは、WebサーバのトップページがJSON形式のデータを出力していました。
ここで、HTMLとJSONの両方を出力するように変更します。
URLのパスの後に「?」をつけると、その後に「クエリ文字列」という
文字列を追加することができ、サーバに簡単な追加のデータを送ることができます。
一つのURLでHTMLとJSONを同時に送ってもクライアントが混乱するだけですから、
helloworldサーバの挙動をクライアントから見た場合の挙動を、
以下のように変更します。

 * 何も指定がなければHTMLを返す (例 @<href>{http://localhost:8080/})
 * 「output=json」 というクエリ文字列をつけたらJSONを返す (例 @<href>{http://localhost:8080/?output=json})

GAEでは「self.request.get('output')」といった形式でで、
クエリ文字列の「キー」と「値」の組み合わせを簡単に取得できます。
特に今回は「指定がなかったらHTMLを返す」という動作を実現するため、
「self.request.get('output', 'html')」と書くことにします。
get()関数の第二引数は「キーが存在しない場合のデフォルト値」です。

この値を元にif文で分岐をすれば良さそうです。

//emlist[講義数0]{
# -*- coding: utf-8 -*-
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

class MainHandler(webapp2.RequestHandler):
    def get(self):
        course_model_list = Course.query(ancestor=course_list_key()).order(Course.date)
        output = self.request.get('output', 'html')
        if output == 'html':
            lst = ['<html><body>']
            lst.append('<h1>講義数合計: {}</h1>'
                       .format(course_model_list.count()))
            lst.append('<ul>')
            for course_model in course_model_list:
                lst.append('<li>{}, {}, {}</li>'
                           .format(course_model.title,
                                   course_model.teacher,
                                   course_model.detail))
            lst.append('</ul>')
            lst.append('<a href="/create">Create</a>')
            lst.append('</body></html>')
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

app = webapp2.WSGIApplication([
    ("/", MainHandler),
], debug=True)
//}

Pythonではインデントがブロックの範囲を表すので、
既にあるJSON側の実装のインデントを1段階下げるのも忘れないようにしてください。

この「仕様変更」によって、講義表のJSON形式データを受け取るURLが
変わった点にも注意してください。
これ以降、Androidアプリ側で結果を取得するURLの末尾には
"?output=json" をつけておく必要があります。
もっとも、今の時点ですと空のListViewが出てくるだけですが……。

//emlist[HTMLに関わる変更で、シラバスアプリのURLの末尾に「?output=json」が必要となる。]{
private static final String syllabusUrl = "http://(ID).appspot.com/?output=json";
//}

HTML上では「講義数合計: 0」の下にCreateというリンクが出てきますが、
ここをクリックしても404 Not Foundが表示されるだけです。


==== データを追加するための画面を作る

まだデータが空のままです。
追加できる画面を作ることにしましょう。

ここで行うべきことは大きく分けて2つです

 * フォーム画面を作ります
 * フォーム画面から送信されたデータを受け取り、登録する部分を作ります


//emlist[この実装だけではデータを送信した後にエラーが発生します]{
# (MainHandlerまで)

form_html = '''\
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


# CreateCourseの行が増えている点に注意！
application = webapp2.WSGIApplication([
    ("/create", CreateCourse),
    ("/.*", MainHandler),
], debug=True)
//}

今回の実装ではMainHandler以外にもうひとつ、
データを追加するフォーム画面を表示するためのCreateCourseという
クラスを追加しました。このクラスは@<href>{http://localhost:8080/create}
に対応するURLへユーザがアクセスした時に、所定のHTMLを表示します。

HTMLフォームでは「method="POST"」で、HTTPのGETリクエストではなく
POSTリクエストを用いてブラウザがWebサーバにデータを送信するように指定しています。
なぜGETリクエストではなくPOSTリクエストを使うかは、すぐに説明します。

ここまでの変更で、送信するフォームがブラウザに表示されるようになります。
しかし、このフォームからHTTPのPOSTリクエストでデータが送られるのに、
この実装ではそれを受け取るための関数をを作っていません。
このままでは「405 Method Not Allowed」
というエラーメッセージが出ます。
なお、405番もまた、HTTPで定められたステータスコードの一つです。

//image[method_not_allowed][POSTの受け口を作っていない]{
//}

というわけで、CreateCourseクラスにさらに受ける側を作ります。
get()関数ではなく、今度はpost()関数を作成します。

//emlist[CreateCourseの全実装]{
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
        logging.info('Saving "{}", "{}", "{}", "{}"'
                     .format(date, title, teacher, detail))
        new_course = Course(parent=course_list_key(),
                            date=date,
                            title=title,
                            teacher=teacher,
                            detail=detail)
        new_course.put()
        self.redirect('/')
//}

 * @<href>{http://localhost:8080/?output=json} でJSON形式の出力が出ることを確認してください。
 ** AndroidアプリでURLの末尾に"/?output=json"がついていることを再確認してください。
 * 本番サーバへアップロードします。
 ** ローカルサーバと本番サーバはデータを共有しません。
 * 本番サーバで講義データを登録します。
 * Androidアプリで講義データが追加されたことを確認します。

以上で講義情報を追加できるだけの「シラバスサービス」が出来たことになります。
実際に使うのであれば編集と削除機能が必要なところですが、
演習の本質から外れてしまうので演習課題としましょう。

====[column] なぜ更新はPOSTリクエストで行うのか

HTTPにはGETとPOST以外にも他にもリクエストメソッドがありますが、
本節ではGETとPOSTについてだけ説明しています。

HTTPのGETとPOSTの最も大きな違いは詳細にはいくつもあります。
一つの大きな違いは、クライアント(ブラウザ・Androidアプリ)から送信するデータを
「どのように送るか」です。

GETリクエストを行った際には、クエリ文字列と呼ばれる文字列がURLの末尾にくっつき、
それを用いてWebサーバにデータを伝えます。
POSTリクエストの場合、クエリ文字列も指定できますが、
同時にHTTPのより深い部分で追加でWebサーバに送信されます。
具体的にはHTTPリクエストを送る際にリクエストの末尾に続けて
データを送るのですが、ここでは詳細は省きます。

URLの長さの最大値は仕様で決まっており、
GETで本格的なデータを送ることはそもそも不可能です。

また、HTTPの仕様ではGETとPOSTでWebサーバが行なって良いことが実は全く異なります。
今回は意識する機会が全くありませんが、
例えば「プログラムが計算して出力した結果をGETはキャッシュして良いが、POSTはしてはいけない」といった明快な違いがあります。

これが何を示すかというと「GETでデータを保存させようとすると、
しばしばWebサーバのどこかで作られるキャッシュのせいで
データが保存されなかったりする」
可能性があることを意味します。

こういった部分についての議論を行うためには、
Webサーバとは何かを本節よりも更に詳細に理解しなければならないため、
本節で説明することはは控えます。
ただ「出来るのだからGETで保存する実装を作っても大丈夫」
と考えるにはまずい理由がある点は、覚えておくと良いでしょう。

HTTPを含めて、Webサーバを取り巻く仕様は、
シンプルなようでいて実は非常に混乱を招きやすく複雑であることがしばしばです。
さらに言えば、クライアントやサーバが仕様とは異なる挙動を示すことすらあります。

====[/column]


=== 追加トピック

「シラバスアプリと通信できるWebサーバを作る」という目標は達成されました。
本章では参考のためのトピックを2つ取り扱います。

 * セキュリティ
 * HTMLテンプレートとスタイルシートを用いたWebサーバの改善例

==== セキュリティ

シラバスサーバが一旦出来上がりましたが、
実際のサービスとしてこのWebサーバを公開したら、
他人が利用した時点で即座にトラブルに見舞われるはずです。
実装が足りないというだけでなく、セキュリティの面で課題が多いからです。

以降では、すぐに判明するセキュリティ課題の一部を検討してみましょう。

===== ログイン機能ととアクセス制御を実装するには

現在は誰が来てもこの講義表を見ることができます。
これはまだ問題ありませんが、さらに誰でも講義を追加することができます
こちらは大問題です。

GAEではGoogleアカウントによるログイン処理を実装するのは簡単です。
試しに、"Hello world!"の代わりにユーザの名前を表示する方法を
@<list>{helloworld_with_login}に示します。

一旦シラバスアプリを離れて、ただのHello Worldサーバに
ログイン機能を追加した例を挙げます。

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

ローカルサーバでもユーザ名(eメールアドレス)とパスワードを聞かれますが、
こちらはどのようなメールアドレスであっても動作します。
本番サーバでは、実際のGoogleアカウントでログインする必要があります。

講義表を公開するのまでは良いとして、講義表を編集するページだけは、
特定の人が利用できるようにするのが適切でしょう。
例えば user.user_id() を用いて、自分がログインした際の「User ID」を把握し、
それ以外のユーザから来た際にはアクセスを拒否するといった実装が考えられます。
実際に実装する部分は追加課題とします。

ログイン関連APIの詳細は以下のWebページに説明があります。

@<href>{https://developers.google.com/appengine/docs/python/users/}
@<href>{https://developers.google.com/appengine/docs/python/users/userclass}


===== クロスサイト・スクリプティング脆弱性とセキュアプログラミング

シラバスサービスが受け入れる講師名といった文字列に、
HTMLタグを入力したらどうなるでしょうか。

試しに「<script>alert("Hello");</script>」を講義タイトルとして入力して、
保存してみましょう。

//image[injection-1][HTMLタグを名前として入力する]{
//}

すると、その文字列が表示される代わりに、JavaScriptプログラムが
実行され、アラートダイアログが表示されてしまいます。
明らかにシラバスサービスが意図していない挙動です。

//image[injection-2][クロスサイト・スクリプティング攻撃が成功している]{
//}

これはいわゆる「クロスサイト・スクリプティング(XSS)」と
呼ばれる立派なWebサイト攻撃方法の一つです。
サービスが攻撃を受けるところまでは仕方ありませんが、
その攻撃が成功してしまうのはいけません。

今回は一見すれば無害ですが、
大手のWebアプリケーションでこのようなバグがあると、
しばしばパスワード漏洩やありとあらゆる問題の温床になります。
一般にこのようなセキュリティに関するバグを「脆弱性」と言います。

言い方を変えると、これまで実装したシラバスサービスには
「クロスサイト・スクリプティング脆弱性」が存在しています。

今回のように単純に迷惑なプログラムを埋め込まれる、
というだけでも十分問題ですが、
場合によっては情報漏洩のきっかけになることもあります。
埋め込んだプログラムが他人によって実行された際、
その他人のログイン情報もプログラムは使い放題です。
それが仮に管理者アカウントだとすれば、
アカウントの乗っ取りすら可能かもしれません。

ユーザから提供されるデータをHTMLに埋め込む場合、
最低でも5種類の文字を適切に書き換える必要があると言われています。
HTMLで特別な解釈をされるからです。

 * 「&」 ... 「&amp;」に変更 (英語のampersandから)
 * 「"」 ... 「&quot;」に変更 (英語のquoteから)
 * 「'」 ... 「&apos;」に変更 (英語のapostropheから)
 * 「>」 ... 「&gt;」に変更 (英語の greater than から)
 * 「<」 ... 「&lt;」 に変更 (英語の less than から)

この書き換えを自分で実装しても良いですが、
より安全なのは、他の人によって実装・検証された方法を使うことです。
今回はPython自体に付属しているxml.sax.saxutils.escape()関数を使えば良いでしょう。

以下に同関数を利用した例を挙げます。(@<list>{using_escape})
@<fn>{about_template}

//footnote[about_template][追加演習で登場する「テンプレート」の仕組みを用いる場合はこの処理は必要ありません。テンプレートエンジンが自動的にエスケープを行うからです。]

//list[using_escape][escape()を使う]{
# 以下の行を追加するのを忘れないように
from xml.sax.saxutils import escape

# (略)
   
            for course_model in course_model_list:
                lst.append('<li>{}, {}, {}</li>'
                           .format(escape(course_model.title),
                                   escape(course_model.teacher),
                                   escape(course_model.detail)))
# (略)
//}

Webアプリを開発する場合には、
今回出会った「クロスサイト・スクリプティング脆弱性」以外にも
いくつか「定番」の脆弱性を作りこみがちです。

IPA(情報処理推進機構)が「安全なウェブサイトの作り方」というドキュメントを
無料公開しています。
本格的にWebアプリ作りこむ場合には、事前に目を通しておくことをおすすめします。

@<href>{https://www.ipa.go.jp/security/vuln/websecurity.html}

===== https

URLに「http」で始まるケースと「https」で始まるケースがあります。

httpの場合、TCP接続して相手と通信する際、
ネットワーク上の途中経路に存在する全てのコンピュータから、
通信しているデータを盗み見ることが出来ます。
ログイン機能を用いて適切なアクセス制御を実現していても、
インターネット上に秘密のデータがそのまま平文(ひらぶん)で
送受信されている状態なので、言ってみれば盗聴し放題です。

一方、httpsではサーバとクライアント間で通信を開始する際に、
その二者しかわからない形で暗号化するので、データを盗み見られる可能性が大きく下がります。
特に送受信するデータを他人から見られたくない場合は、
「https」のURLを用いてやりとりを検討してください。
@<fn>{https_is_wrongly_used}

//footnote[https_is_wrongly_used][不適切な方法で暗号化通信を行っている場合はhttpsの意味がないこともあります。]

今回用いるGAEの本番サーバは、初めからhttpとhttpsの両方をサポートしています。
これまでの本番サーバのURLの「http」を「https」に変更すれば、
ブラウザなどによる表示はそのままで、暗号化の恩恵を得られるでしょう。

ただし、多くの場合はhttpとhttpsを即座に切り替えられるわけではありません。
事実、GAEで開発中に使う@<href>{http://localhost:8080}というURLでは
httpsを利用することは出来ません。
その代わり、このURLはPC内からしか見ることが出来ないことによってセキュリティを確保しています。

盗み見られても全く害がない場合にはhttpのURLを使っても実害がないこともありますが、
近年ではそのような分野は少なくなっています。
皆に公開されているニュース等であれば良いのですが、
例えば企業秘密をやりとりするといった場合には注意が必要です。

なお、httpsと言っても「どのように」暗号化通信を行うかについては、
実は細かい違いがたくさんあります。
Webサーバを真面目に設計・開発・運用する際には、
そういった部分も併せて考える必要があります。

==== HTMLテンプレートとスタイルシートを用いたWebサーバの改善例

これまでは、Webブラウザに表示させるHTMLを
Pythonに直接書き込んでいます。
Androidアプリで言えば、
XML形式のレイアウトファイルを書かずに、
Javaを用いてActivityに直接レイアウトについての指示を書き込んでいる状態です。
また、デザインについての情報に至っては一切含めていませんので、
普段見るWebサイトと比べると、だいぶ簡素です。

しかし、大きなWebアプリを作るのであればこの方法は色々と不都合です。
Androidアプリの開発に措いて「UI基礎編」で既に述べられている通り、
デザインとコードは分離出来てしかるべきです。

Webアプリケーションの「デザイン」は主に、HTMLとスタイルシート(良く「CSS」と呼ばれます)、
そして本項では紹介していない「Webプログラミング」言語であるJavaScriptによって実現されます。
これらは、今回実装したPythonのWebサーバ実装とは分離できます。

GAEでは、jinja2と呼ばれるテンプレートエンジンを用いて、
デザインと実装の分離を達成することができます。
ここではトップページの講義表にこの仕組みを使ってみましょう。
また、デザインについてはBootstrap(@<href>{http://getbootstrap.com/})
と呼ばれるソフトをHTMLに埋め込みます。

なお、今回はダウンロードしてプロジェクトに含める手続きを省略するため、
Bootstrapの各種ファイルをオンラインで取得するようにHTML内で指示します。
インターネット接続がない状態でローカルサーバだけで開発している場合には
見栄えがおかしくなるかもしれません。

まず、jinja2を使えるようにするため、app.yamlに以下を追加します。(@<list>{adding_jinja2}

//list[adding_jinja2][app.yamlに以下の「jinja2」に関する2行を追加する]{
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

また、単に見栄えが良くなるだけではなく、
実はセキュリティという面からも若干改善があります。

テンプレートを用いたこのバージョンノ場合、
前述した「クロスサイト・スクリプティング攻撃」を避けるための
エスケープ処理をPython実装側で行う必要がありません。

テンプレートエンジンは与えられた文字列をチェックし、
自動的にエスケープ処理を実行してくれるからです。
もしエスケープを解除したいケースがあるようであれば、
そのときだけ明示的にエスケープをしないように指定します。

プログラミングミスを全て回避することは熟練者でも難しいと言えます。
jinja2テンプレートエンジンが採用している
「していなければエスケープして安全よりに倒す」
という方式の方が、安全なプログラミングとしては、より適切と言えます。

時間があれば、講義作成画面も、テンプレートを使って見栄えを変えてみましょう。

=== おわりに

本章では、Androidアプリ開発者がWebサーバを理解するための入り口と、
より本格的に開発する上で必要な要素についても少し紹介しました。

Androidアプリ開発者としては、
Webサーバの全ての要素を理解する必要はありませんが、
知っている方がサーバとのやりとりを実装する上での落とし穴にははまりにくくなるでしょう。

参考まで、Tech Boosterによる「Android Open Textbook Project」
(@<href>{https://github.com/TechBooster/AndroidOpenTextbook})
に、本章学習後に学ぶべきやや詳細なトピックを扱った内容を含めています。
なお、最新版のPDF版やHTML版は、
@<href>{https://tcb.mowa-net.jp/griflet/github/TechBooster/AndroidOpenTextbook/}から得ることができます。(2014年10月現在)

== 参考トピック

//lead{
本節では、授業で用いる範囲で有用と思われるトピックを学びます。
授業の補助資料として利用してください。
//}

=== この節で学ぶこと

 * Pythonについて
 * シラバスアプリのインポート

=== Pythonを学ぶ

Pythonにはオンラインで読めるドキュメントが多数存在します。
例えば、Python公式ドキュメントの日本語訳はとして、以下のリンクがあります。
ここでは本編で役に立つトピックを中心にPythonを紹介しますが、
オンラインの資料も読んだほうがPythonをより深く理解できるでしょう。

 * @<href>{http://docs.python.jp/2/tutorial/}
 * @<href>{http://docs.python.jp/2.7/library/}

なお、Pythonには大きく分けてPython2とPython3の2つの系統があります。
同じPythonという名前がついていますが文法を含めて多くの面で互換性がないため、
分けて考える必要があります。

本章で採用するのは現在Google App Engineで利用できるPython2 (バージョン2.7)の方です。

==== Pythonのインタープリタ(対話型環境)を起動する。

Pythonプログラムを一行ずつ実行し、
結果を確認しながら言語を学習することができます。
「Python Console」もしくは、
より一般にプログラミング言語の「対話型環境」と呼ばれています。

統合開発環境PyCharmを利用している場合、
「Tools >> Run Python Console」を選択することで、
アプリ内で対話型環境を起動できます。
@<fn>{using_terminal}

//footnote[using_terminal][PyCharmを利用していない場合でも、Python実行環境をインストール済であれば、Windowsのターミナルから"python"と入力することでも起動できます。]

試しに電卓のように使ってみましょう。

//cmd{
>>> 1 + 10
11
>>> 3 ** 10
59049
//}

「**」は累乗です。つまり、3^10 = 3*3*3*3*3*3*3*3*3*3 = 59049が即座に計算できます。

==== Pythonは動的型付言語

Javaでは各変数を宣言する際、「型」を明示する必要がありました。
Pythonではその必要はありません。

//cmd{
>>> x = 333
>>> y = 400
>>> x * y
133200
//}

ただし、各データに型が存在しないわけではありません。
次の例は、整数と浮動小数点での割り算の挙動の違いを示しています。

//cmd{
>>> 1/2
0
>>> 1/2.0
0.5
//}

定数や変数の型はtype()で調べることができます。

//cmd{
>>> type(1)
<type 'int'>
>>> type(2.0)
<type 'float'>
//}

型の名前と意味がJavaと似ていて異なることがあります。
Pythonでは整数はint、浮動小数点はfloatのみが存在しますが、
Pythonのintに最大数についての制限はなく、
PythonのfloatはJavaでいうdouble(倍精度浮動小数点)です。
電卓のような用途では、JavaよりもPythonの方が数字を扱いやすいと言えるでしょう。

==== ソースコードの記述されたファイルを実行する

PyCharm等で作成したプログラムファイルを、
Pythonの実行環境に実行させることができます。

例えば@<list>{simpleprogram_py}のようなプログラムを作成し、
simpleprogram.pyとして保存したとします。

//list[simpleprogram_py][簡単なプログラムの例]{
x = 10
y = 20
z = 30
x = y * z - x
y = z * x - y
z = x * y - z
print(x + y + z)
//}

PyCharmで、このPythonファイルを右クリックし「Run 'xxx'」
を選択すると、結果がPyCharm内に表示されます。
ファイルに保存して実行するときは、
表示させたい結果はprint()で囲ってください。
@<fn>{println}

//footnote[println][5-1-2「Javaプログラムを実際に動かしてみよう」に登場した、JavaのSystem.out.println()と同等です。]

対話型環境で次のように毎回一行一行入力することもできますが、
この場合はプログラムがどこにも保存されていないため、
対話型環境を再起動したりした場合には、最初から入力し直す必要があります。

//cmd{
>>> x = 10
>>> y = 20
>>> z = 30
>>> x = y * z - x
>>> y = z * x - y
>>> z = x * y - z
>>> print(x + y + z)
(出てくる数字はいくつ？)
//}

==== 文字列について

Javaと同様、Pythonにも文字列があります。
"(ダブルクオート)もしくは'(シングルクオート)で囲った部分が文字列になります。
Javaと異なり、Pythonでは""と''で囲った場合で、
文字列の挙動は基本的に変わりません。
PythonにはC言語のcharに相当する型は存在しません。

//cmd{
>>> "Hello World!"
'Hello World'
>>> 'Hello World!'
'Hello World'
//}

ダブルクオートで囲った文字列中でダブルクオートを使いたい場合、
バックスラッシュでエスケープする必要があります。

//cmd{
>>> print("Say \"Hello World!\"")
Say "Hello World!"
//}

シングルクオートで囲った文字列内では、
ダブルクオートをエスケープする必要はありません。
同様に、ダブルクオートで囲った文字列内では、
シングルクオートをエスケープする必要はありません。

//cmd{
>>> print('Say "Hello World!"')
Say "Hello World!"
//}

ダブルクオートもしくはシングルクオートを3つ並べた場合、
複数行の文字列を作成することができます。
GAEの項では実際に、
複数行に渡るHTMLを、まとめてソースコードに記述する際に使っています。

//cmd{
>>> print('''Hello
... World
... !!!!!''')
Hello
World
!!!!!
//}

Pythonでは文字列型と数値型は「+」演算子で結合出来ません。

//cmd{
>>> 'Hello ' + 20 + ' and ' + 10 + '!!'
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: cannot concatenate 'str' and 'int' objects
//}

数字や他の文字列を埋め込む最も単純な方法は、
数字などを埋め込みたい場所に「{}」を挿入した上で、
文字列のformat()メソッドを呼ぶ方法です。

//cmd{
>>> print('Hello {} and {}!!'.format(20, 10))
Hello 20 and 10!!
//}

「{}」という書式はPythonにおける「書式指定文字列」と呼ばれ、
JavaのSystem.out.printf()に負けない非常に多彩な機能をもっています。
@<href>{http://docs.python.jp/2/library/string.html#formatstrings}
等で詳細な機能を確認することが出来ます

==== Python文字列での日本語の扱い

Pythonの文字列についてしばしば混乱を誘うのが、
日本語のような、いわゆる「非アスキー文字」の扱いです。

Pythonについて学ぶ際、文字列の前にuをつけて「u'日本語'」
のように表記する例を見ることがあるかもしれません。
これは「ユニコード文字列」と呼ばれる文字列で、本来の文字列と異なります。

//cmd{
>>> type('str')
<type 'str'>
>>> type(u'str')
<type 'unicode'>
>>> type('str') == type(u'str')
False
>>> 'str' == u'str'
True
>>> '日本語' == u'日本語'
__main__:1: UnicodeWarning: Unicode equal comparison failed to convert both arguments to Unicode - interpreting them as being unequal
False
//}

最後の例(「'日本語' == u'日本語'」)では、不穏な警告まで表示されています。
その上で文字列とユニコード文字列の比較では、結果はFalseです。

Python言語で日本語を扱う際にはしばしば「ユニコード文字列」
と呼ばれるフォーマットを使う必要があることを覚えておいてください。
@<fn>{about_python3_string}

//footnote[about_python3_string][Python 3では文字列の扱いが大幅に変わっています。]

==== 文字列以外のデータ型について

動的型付とは「柔軟に型の異なる値を入れ替えられる」という意味であって、
型が「存在しない」わけではありません。

 * 整数
 * 文字列
 * Unicode文字列
 * 真偽値 (True, False。Javaのbooleanと異なり冒頭は「大文字」なので注意)
 * リスト (Javaの配列やList(ArrayList)に似た仕組み)
 * マップ型 (JavaのMap(HashMap)に似た仕組み)
 * オブジェクト型

Javaのようなオブジェクト指向の仕組みはPythonにも存在しますが、本説では省略します。
「class」で始まるブロックがあったら似たようなことが行われていると想像できれば十分です。

リストとマップ型は本編でも使われています。
以下のコマンド実行例で、まず感覚を掴んでみてください。

//cmd{
>>> lst = [1, 2, 3]
>>> print(lst)
[1, 2, 3]
>>> lst.append(5)
>>> lst
[1, 2, 3, 5]
>>> d = {'a': 100, 'b': 200}
>>> d
{'a': 100, 'b': 200}
>>> d['b']
200
>>> d['c'] = 1000
>>> d
{'a': 100, 'b': 200, 'd': 1000}
//}

それぞれ@<href>{http://docs.python.jp/2.7/library/index.html}の
「組み込み型 >> シーケンス型」と「組み込み型 >> マップ型」
に利用できる機能の全てが掲載されています。


==== Pythonではインデントに意味がある

ここで、Pythonを学ぶ初学者が陥りがちな罠を紹介しましょう。
Pythonでは「インデントに意味があります」。

if文の書き方を通じてJavaとPythonを比較してみましょう。
まずJavaのif文の例を以下に示します。

//list[using_if_with_java][Javaのif文]{
if (x == 0) {
   System.out.println("x is 0");
} else {
   System.out.println("x is not 0");
}
System.out.println("After if.");
//}

次にPythonで同様にif文を書いた場合は以下のとおりです。

//list[using_if_with_python][Pythonのif文]{
if (x == 0):
   print("x is 0")
else:
   print("x is not 0")
print("After if.")
//}

ブロックの開始は:(コロン)で指定していますが、
終了を明示するものはありません。
その代わり、行頭の空白が存在することで
「ifブロックの中であること」と「その外に存在すること」を区別します。

Pythonでは、次のように書いては「いけません」

//list[using_if_with_python_wrong][Pythonのif文(間違った例)]{
# THIS IS WRONG
if (x == 0):
print("x is 0")
else:
print("x is not 0")
print("After if.")
# THIS IS WRONG
//}

これはエラーになります。
また、「x is not 0」と「After if.」の行についてはブロックの内外の区別が出来ません。

インデントつまり行頭の空白が、各行がどのブロックに属しているかを示します。
次の例を見てみましょう。

//list[using_if_2][Pythonのif文(間違った例)]{
if (x > 0):
    print("x > 0")
    if (x < 30):
        print(u"0 < x < 30")
    else:
        print(u"x >= 30")
print("After if.")
//}

これを次のように書くとprint()文と実際のxの中身が一致しなくなります。

//list[using_if_2_wrong][Pythonのif文(間違った例)]{
if (x > 0):
    print(u"x > 0")
    if (x < 30):
        print(u"0 < x < 30")
else:                        # この行以降のインデントが壊れたとする。
    print(u"x >= 30")         # x が例えば -30 であってもこれが表示される。
print("After if.")
//}

例えば、コピーアンドペーストをした際に、
@<list>{using_if_2_wrong} のような意図しないインデントが発生しがちです。

==== if

既にifは登場していますが、説明していないことがあります。
Pythonではifの評価文前後の丸括弧は必要ありません。

//list[using_if_without_paren][ifの例]{
if x > 0:  # ()がない
   print(u"x is greater than 0")
//}

ifの後が一行であれば、次のようにも書けます。

//list[using_if_without_paren_2][ifの例]{
if x > 0: print(u"x is greater than 0")
//}

Javaのif文はboolean型しか評価の対象としませんでしたが、
Pythonではデータ型は概ね何でも受け取ります。

真偽を表すTrueとFalse(先頭が「大文字」です)以外の場合、
おおまかにそのデータが「空でないか」をチェックします。
空の意味は変数に実際に保存されているデータの型で変わります。

 * 整数なら 0
 * 文字列なら ''
 * リストなら []
 * マップなら {}

//list[using_if_with_not_no_int][ifの例]{
if x: print(u"x is not empty")
//}

なお、このような書き方を許す言語はPythonに限らず多数ありますが、
逆にそれが混乱を招く可能性があります。

例えば、Pythonと良く比較対象になるRuby言語では、
整数の0を真と判定するか、偽に安定するかについて、Pythonと異なります。
Rubyでは0は真で、Pythonでは上述の通り0は偽です。
よって、Rubyとそっくりだと思って "if x:" と書いた場合、
その結果として分岐が完全に入れ替わる可能性があります。

//list[ruby_if][Rubyのコードだとyay!が出る。0は「真」]{
(rubyの対話環境)> x = 0
(rubyの対話環境)> if x; "yay!"; end
=> "yay!"
//}

==== for

forはJavaと文法が異なります。
例えば0から4まで表示するプログラムは以下のとおりです。

//cmd{
>>> for i in range(5): print(i)
... (何も入力せずにエンター)
0
1
2
3
4
//}

==== 関数

Pythonの関数は次のように書きます。
インデントの有無で関数の開始と終了を明示します。
ifやforと同様、中括弧は必要ありません。

//emlist[関数の例]{
def sum(a, b):
    return a + b
# インデントをなくして関数の終わりを明示する。
print(sum(1, 9))  # 10を表示する
//}

==== 標準ライブラリとimport

Pythonではインストールした段階で豊富なライブラリが付属しています。

JSONデータを扱うjsonモジュールを例に、標準ライブラリに含まれるモジュールの使い方を説明します。
Pythonでは外部ライブラリは明示的に利用することを宣言する必要があります。
jsonモジュールを使うには「import json」とします。
これ以降、対話型環境およびそのソースコードファイルではjsonモジュールを使うことが出来ます。

試しにPythonマップ型データをJSONデータに変換してみます。

//cmd{
>>> import json
>>> json.dumps({'a': 1, 'b': 2})
'{"a": 1, "b": 2}'
//}

一見、入力したデータとjson.dumps()関数の結果がそっくり同じに見えますが、
よく見ると関数の出力結果は「文字列」です。
Pythonのデータ構造とJSONフォーマットの文字列データは一見してよく似ています。

Python 2.7の持つライブラリの内容は
@<href>{http://docs.python.jp/2.7/library/index.html}から参照できます。
特に、jsonモジュールの説明は@<href>{http://docs.python.jp/2.7/library/json.html}にあります。

モジュール内の一部だけインポートしたいことがあります。
今回の範囲ではxml.sax.saxutilsモジュールのespace()関数を使う例がありますが、
毎回「xml.sax.saxutils.escape()」と書くのは面倒なので、
以下のように書いています

//emlist[モジュール部分の指定を省略できるようにしている]{
from xml.sax.saxutils import escape
//}

Pythonモジュールのインポート方法の詳細は、@<href>{http://docs.python.jp/2/tutorial/modules.html}などを参照してください。

=== シラバスアプリのインポート

第10章「ユーティリティによる実践」
でシラバスアプリが完成していない場合にだけ本作業を行ってください。
この作業を行う場合にも、同章に記述されたVolleyライブラリの準備は必要です。
10-2-1「ネットワーク経由でデータを取得する」
でVolleyライブラリの準備方法は扱われていますので、ここでは割愛します。

@<href>{https://github.com/ktaka/TechInstituteSyllabus}へアクセスします。

@<img>{download_from_github}のようなWebページが表示されたら、
右下の「Download ZIP」ボタンをクリックします。

//image[download_from_github][]{
//}

zipファイルを解凍し、
解凍したフォルダ(「TechInstituteSyllabus-master」)の場所を覚えておきます。
今回は「デスクトップ」へ配置したとします。

Eclipseで「File >> New >> Project...」を選択します。

//image[import_syllabus_eclipse_1][]{
//}

「Android」を選択し、つぎに「Android project from Existing Code」
(既存のAndroidプロジェクト)を選択します。

//image[import_syllabus_eclipse_2][]{
//}

「Import Projects」というウィンドウが表示されたら、
右側の「Browse...」ボタンをクリックします。

//image[import_syllabus_eclipse_3][]{
//}


解凍した「TechInstituteSyllabus-master」を選択して「OK」を押します。

//image[import_syllabus_eclipse_4][]{
//}

このプロジェクトは「MainActivity」という名前でEclipseに認識されます。

ここで、インポートしたプロジェクトがビルドエラーが発生している場合があります。
この場合、そのプロジェクトを右クリックし、Propertiesを選択します。

//image[import_syllabus_eclipse_5][]{
//}

左ペインで「Android」を選択します。
もしビルドに失敗している場合、

 * 右側のProject Build Targetのチェックがどこにもついていない
 * 右下のLibrary部分で「volley」という行の左に赤く×がついている

のどちらかが発生しているはずです。

1番目の問題が発生している場合、
授業で案内されているバージョン(多くのケースでAndroid 4.4.2)を選択します。

@<img>{import_syllabus_eclipse_6}の示すように、赤い×印がついている場合、
右側のRemoveボタンをまず押し、次にAddボタンを押します。

//image[import_syllabus_eclipse_6][]{
//}

「Project Selection」というウィンドウが開いたら、Volleyを選択し、OKを押します。

//image[import_syllabus_eclipse_7][]{
//}

@<img>{import_syllabus_eclipse_8}のように、赤い×印が緑のチェックマーク
になっていることを確認したらOKを押して
「Properties for MainActivity」を閉じます。
その後Eclipseのメイン画面の「Project >> Clean...」を選択し
「Clean all projects」を実行してください。

//image[import_syllabus_eclipse_8][]{
//}

これで、第10章「ユーティリティによる実践」のシラバスアプリの完成版を利用できるようになります。


=== Windows 8.1 でのバックアップの仕方

ここではWindowsにおける「復元ポイント」の作成の仕方を説明します。
本演習でインストールしたソフトウェアや中間生成物を一切残したくない時に、
演習終了後にその復元ポイントに戻れば、
それまでのPC内の変更を取り消すことができます。

なお、その間に行われた他の作業についてもリセットされてしまいます。
「復元ポイント」自体にディスク容量も必要なため、
実際にこの項目を行うかは自己の判断でおねがいします。

左下のスタートボタンを右クリックし「システム」を選択します。

//image[recovery_select_system][]{
//}

「システム」ウィンドウ左側の「システムの保護」をクリックします。

//image[recovery_click_protection][]{
//}

「システムのプロパティ」ウィンドウ下方の「作成」をクリックします。

//image[recovery_click_create][]{
//}

「復元ポイントの作成」において分かりやすい名前を入力します。
@<img>{recovery_enter_manual_recovery}では「manual-recovery」と入力していますが、
覚えやすいものであれば、他の文字列でも構いません。

名前を入力したら「作成」ボタンを押します。

//image[recovery_enter_manual_recovery][]{
//}

復元ポイントの作成には、PC内のデータ量に応じた時間がかかります。
最後に@<img>{recovery_done}と表示されれば、成功です。

//image[recovery_done][]{
//}

実際に復元したくなった場合、@<img>{recovery_click_protection}の
「システム復元」ボタンを押して、指示に従います。


=== ユーザフォルダに日本語が含まれる場合のGAEインストールの追加作業

GAEインストール時にこの作業が必要なのは、
「C:\ユーザー\(ユーザ名)」の「ユーザ名」部分が日本語になっている場合だけです。
Google App Engine Launcherが日本語を含むフォルダの扱いを正しく処理できないため、
この作業が必要となります。

そのような例を@<img>{japanese_user_name}に示します。
この現象は、Windows 8の初期設定で、
日本語名のユーザを作成した際に発生します。

//image[japanese_user_name][ユーザ名が「大輔」で日本語になっている]{
//}

このケースに当てはまる場合、さらに以下の作業を行なってください。

 * 「C:\GoogleAppEngine」のような日本語を含まないフォルダを作成する。
 * TMP環境変数に上記のアドレスを指定する。
 ** 環境変数は第4章「開発環境セットアップ」p61でPATHを指定した時と同じ方法で設定します。今回、変数名は「TMP」とし「変数値」を上記のフォルダとします。既に存在している場合には一旦値を削除してください。
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


=== Wi-Fi内でHello World!

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
