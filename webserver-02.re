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

本章では、第13章「ネットワークプログラミング」で学んだことを前提にしています。
一度復習してから本章を読んだほうが良いでしょう。

== Webサーバとは？

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
Server、Client、Resourceという英単語はWebサーバにまつわる
各種英語のドキュメントによく登場するので、せっかくですから覚えておきましょう。
本章では以降「サーバ」「クライアント」「リソース」と記載します。

ジュースサーバの場合、クライアントが要求するリソースはジュースでした。
Webサーバの場合にも、リソースを求めてWebブラウザやAndroidアプリが
アクセスします。
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


=== 自分のWebサーバが必要な場面

Webサーバとの関係で考えたとき、
Androidアプリはおおまかに以下の3つに分けられます。

 * そもそもネットワーク接続が必要ない
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

=== シラバスアプリのサーバを作るケースを考える

第10章「ユーティリティによる実践」で、シラバスアプリを実装しました。
本章ではこの例を拡張して、シラバス提供用のWebサーバを作ることにします。

シラバスアプリの演習では、
Webサーバに保存されたJSONデータをVolleyというライブラリを用いてダウンロードし、
その内容をListViewに表示しました。

//image[syllabus-screenshot][]{
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


=== Webサーバを支える技術

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
シラバスアプリが利用できるJSONデータを出力出来るようにし、
HTML経由でユーザが講義情報を追加出来るようにします。
それ以降の拡張は演習とします。

開発を続けながら、Googleが提供するGAEの「本番サーバ」へ
デプロイ(配置)する方法も説明します。
「本番サーバ」にアップロードされた実装は、世界中に公開されます。

主な目的はWebサーバのエキスパートになることではなく、
仕組みを理解することです。
PythonやGAEの習得が目的ではないので、
演習はPythonや深く理解する必要がないように構成しています。

ただしプログラミング言語の動作をある程度理解していないと、
ちょっとしたことでも試せなくなってしまいます。
そこで、Python言語自体の説明を別の節(？)に分けて教科書に収録しました。

Python言語について特に興味がある方は、
本章の説明の途中で行われるPython開発環境PyCharmをインストールした後で、
Python入門の記事を試しながら学ぶと、理解がスムーズになるかもしれません。


=== 演習を行う場合の注意

本演習では、実装したWebサーバを全世界に公開します。
Googleアカウントによるログインを除いて、
個人情報やプライバシーに関わる情報を実装に含めるのは避けてください。
全世界に公開することが難しい場合、
「Wi-Fi内でHello World!」の説明を参考にして、
Wi-Fi内の限られた環境で実装されたサーバを確認するに留めても良いでしょう。
本編ではこの方法は扱いません

また、Androidアプリ開発とは関係がないソフトをインストールします。
特に演習用PC以外で本演習を行う場合、
システムのバックアップを作成した方が良いでしょう。
Windowsでは演習開始前に「復元ポイント」を作成することで、
万が一の状況に備えることができます。
「Windows 8.1 でのバックアップの仕方」を参照してください。

== WebサーバでHello World!
=== Python実行環境のインストール

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

//image[install-python-beginning][]{
//}

インストール先を確認されます。
「C:\Python27\」となっていることを確認して「Next」を押します。

#@warn(TODO: 出来れば撮り直す。一時ファイルが画面に入っていて実際と異なる)

//image[install-python-destination][]{
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

//image[install-python-reboot][]{
//}

以上の作業が終了したら、Python実行環境の動作確認をします。
Windows画面左下の開始ボタンを押して「コマンド プロンプト」を選択後、
「Python」と入力して、Pythonの対話型環境が起動するかを確認してください。

//image[python-prompt][Pythonプロンプトの実行例]{
//}

=== Google App Engine SDKのインストール

次に、Google App EngineのPython SDKをインストールします。
以下のURLを開いてください。

 * @<href>{https://developers.google.com/appengine/downloads}


「Google App Engine SDK for Python」をクリックします。

#@warn(GAEのバージョンが1.9.11から1.9.12に変わった。注意)

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
@<fn>{if_using_japanese_name}

//image[gae-installed][]{
//}

//footnote[if_using_japanese_name][Windows 8等で日本語ユーザ名になっている場合、ここからさらに作業が必要です。〜「ユーザフォルダに日本語が含まれる場合のGAEインストールの追加作業」を参照してください。]


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

なお、PCによっては@<img>{pycharm-firewall}のような
警告が表示されることがあります。
ここではそのまま「アクセスを許可する」ボタンを押します。

//image[pycharm-firewall][]{
//}

さらに@<img>{pycharm-tip-of-the-day}のような
ダイアログが表示されることがあります。

//image[pycharm-tip-of-the-day][]{
//}

今回は左下の「Show Tips on Startup」(起動時にTips画面を表示する)のチェックを
外して、「Close」ボタンを押します。

既にhelloworldプロジェクトを作っていますので、ここではそれを
PyCharm上で編集してみましょう。
「Open Directory」を選択し、helloworldプロジェクトが存在するフォルダを開きます。

//image[pycharm-select-helloworld][]{
//}

@<img>{pycharm-helloworld}のような画面が出れば、
プロジェクトを開けたことになります。

//image[pycharm-helloworld][]{
//}

PyCharmは同じく統合開発環境であるEclipseと似た外見をしています。
しかし、全く一致した使い勝手というわけではありません。
以下に、すぐに使い勝手を改善出来るヒントを示しますので、
興味があればさらにカスタマイズしてみましょう。


 * 起動時にkeymapを変更していない場合「File >> Settings >> Keymap」でKeymapsを「Eclipse」に変更すると、Eclipse風になります。@<img>{eclipse_keymap}ただし全てがEclipseと同じになるわけではありません。
 * 「File >> Settings >> (左画面IDE Settingsの) Editor >> Editor Tabs >> Mark modified tabs with asterisk」のチェックを入れておくと、Eclipseの時と同様保存されていないファイルのタブに*(スター、もしくはアスタリスク)が付きます。@<img>{asterisk}

//image[eclipse_keymap][]{
//}

//image[asterisk][]{
//}

ここまででようやくPythonでプログラミングする準備が整いました。
プログラミングに自信があれば、このままGAEによるWebサーバの実装を進めましょう。


=== Hello Worldサーバへブラウザからアクセスする

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

=== Hello World!プログラムの意味

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

//image[my_own_404][]{
//}

「全てのパスをこのMainHandlerクラスがが受け持つ」ように実装を変更してみましょう。

//emlist[]{
application = webapp2.WSGIApplication([
    ("/.*", MainHandler),
], debug=True)
//}

このようにした場合、404 Not Foundは表示されなくなり
「Hello world!」が再び表示されるようになるはずです。


=== HTTPのステータスコードについて

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

//image[github_not_found][]{
//}

GAEでも、ステータスコードをPython実装上で指定したり、
GitHubのように見た目の異なる404画面を表示させることもできますが、
今回は割愛します。
GAE Pythonでのレスポンスを操作する際の詳細は、例えば@<href>{https://cloud.google.com/appengine/docs/python/tools/webapp/redirects}を参照してください。

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

"Create Application"をクリックします。

//image[gae-web-console][]{
//}

@<img>{gae-app-id}で、Application Identifierとして
「世界中で唯一」の名前を一つ設定します。
他の人が利用している名前は選択できません。
これはそのまま公開するサーバ名になるので、
おかしな名前も避けたほうが良いでしょう。

//image[gae-app-id][]{
//}

アプリケーションIDを考えたら、
右側の"Check Availability"ボタンで利用できるかを確認し、
実際にそのアプリケーションIDを使用出来ることを確認します。

Google App Engineの利用規約が表示されている場合は内容を確認し、
「I accept these terms」をチェックした上で「Submit」を押します。


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
アプリケーション固有パスワードを準備してそれを使用します。

ログで「Deployment successful」と出たら成功です。

「@<href>{http://(自分が生成したID).appspot.com/}」へアクセスしてみましょう。

せっかくですから、隣の人に自分のWebサーバを見てもらってはいかがでしょうか。


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

今回のサーバ実装ではlogging.info()だけ使えば十分でしょう。
そのモジュールを用いるため、Javaと同じように「import logging」
という命令を予め入れておく必要があります。
@<fn>{about_python_logging}

//footnote[about_python_logging][Pythonのloggingについて詳細に学びたいは、まず@<href>{http://docs.python.jp/2/howto/logging.html#logging-basic-tutorial}を読むことから始めると良いでしょう。]

=== 本番サーバのダッシュボードとリソース割り当てについて

GAE Launcherには「Dashboard」というボタンもあります。
このボタンを押すと、本番サーバの管理者画面をブラウザに表示してくれます。
ここから本番サーバでのログ出力も見ることができます。

//image[gae-dashboard][]{
//}

Google App Engineは小規模なサーバを起動する分には無料で利用できます。
より正確には、それぞれのアプリに無料で利用できる一日あたりの割り当て
が決まっており、その範囲内のアクセスであれば料金はかかりません。
割り当てとその消費量は「Quota Details」から見ることができます。

//image[quota][]{
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


== シラバスアプリのサーバを作りこもう
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

=== 自分のWebサーバから、自動生成したJSONデータを返す

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

=== 生成されたJSONデータをブラウザで確認する。

ローカルサーバ(@<href>{http://localhost:8080})から、
JSON形式のデータを閲覧出来ることをブラウザを用いて確認してください。

//image[json-on-browser][]{
//}

問題無さそうであれば、
作成したサーバアプリケーションを再び本番サーバにアップロードします。

本番サーバのURLへブラウザでアクセスし、
やはり同様のJSONデータを取得できることを確認します。

ここまでで、世界に公開された状態で講義表データを返すWebサーバが完成しました！

=== シラバスアプリにサーバのJSONデータを読み込ませる

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

=== Chromeの開発者コンソールを開いて通信の詳細を覗いてみる

現在はブラウザ自体に豊富な機能があるため、Webサーバとの通信の様子を
調べるのはまずこの機能を使うことが多いです。
Chromeでは「デベロッパーツール」と呼ばれています。

デベロッパーツールを起動するには、
JSONデータを表示したChromeで右クリックをし「要素を検証」をクリックしてください。
すると、Chromeの下方にデベロッパーツールが表示されます。

//image[developer-tools][]{
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


=== 講義表を保存・変更出来るようにする

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

# MainHandler
//}

保存する形式にはプログラミング言語と似たデータ型があります。
GAEのDatastoreで利用できるデータ型は@<href>{https://developers.google.com/appengine/docs/python/ndb/properties}に掲載されています。
今回は「日付」に対応するデータと文字列データさえあれば事足ります。

course_list_key()関数は、GAE内で本来バラバラで保存されている
データをひとまとめにするおまじないとでも考えてください。
すぐ後で使います。

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

//emlist[結果はこうなる]{
{"course": []}
//}

なお、本番サーバへindex.yamlを最初にアップロードした後、
暫くの間@<img>{need_index_error}のように
「NeedIndexError: The index for this query is not ready to serve. ...」(検索に使うインデックスが準備できていません)というエラーが表示されます。
この場合は、少し様子を見てください。
GAE側で関連するデータベースのインデックスを作成するのに時間がかかるためです。

//image[need_index_error][]{
//}

なお、似たようなエラーとして「NeedIndexError: no matching index found.」(一致するインデックスがありません)というエラーメッセージが出ることがあります。
この場合はPythonプログラムで指定しているソートの方法とindex.yamlで生成しているインデックスが一致していません。
つまりプログラミング上に何か間違いがあります。

JSONデータが空なのは残念ですが、
JSONデータを追加する方法について考える前に、
先にHTMLで講義データを表示する実装も作ってしまいましょう。
さらに次で使えるように、HTMLの画面では、
次に作る「講義の登録」画面のためのHTMLタグも入れてしまいます。

これまでは、言ってみればWebサーバのトップページがJSON形式のデータを
出力していましたが、ここでHTMLとJSONの両方を出力するように変更します。
URLには、パスの後に「?」をつけると、
以降に「クエリ文字列」という文字列を追加することができます。
この仕組みを利用して、以下のような動作に変更します。

 * 何も指定がなければHTMLを返す (例 @<href>{http://localhost:8080/})
 * 「output=json」 というクエリ文字列をつけたらJSONを返す (例 @<href>{http://localhost:8080/?output=json})

GAEでは「self.request.get('output')」といった形で簡単にクエリ文字列の
「キー」と「値」の組み合わせを取得できます。
特に今回は「指定がなかったらHTMLを返す」という動作にするため、
もしキーと値の組がなければhtmlが送られてきたことにするため
「self.request.get('output', 'html')」と書くことにします。
get()関数の第二引数は「存在しない場合のデフォルト値」です。

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

この変更で、JSONを受け取るURLが変わった点に特に注意してください。
これ以降、アプリ側で結果を見たい場合は、
Androidのシラバスアプリで指定したURLの末尾に
"?output=json" をつけておく必要があります。
もっとも、今の時点ですと空のListViewが出てくるだけですが……。

//emlist[しらばす]{
private static final String syllabusUrl = "http://(ID).appspot.com/?output=json";
//}

HTML上では「講義数合計: 0」の下にCreateというリンクが出てきますが、
ここをクリックしても404 Not Foundが表示されるだけです。


=== データを追加するための画面を作る

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
POSTリクエストを用いてブラウザがWebサーバにデータを送信するように
指定しています。なぜPOSTを使うかはすぐに説明します。

これで、送信するフォームがブラウザに表示されるようになります。
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

=== なぜ更新はPOSTなのか

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
                lst.append('<li>{}, {}, {}</li>'
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


