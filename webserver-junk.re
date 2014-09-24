= 墓場
== JSONを静的ファイルとしてGAEサーバで配布する。

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

== GAEを扱うための別のコンソールについて

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


== GAE Pythonを採用した理由

今回GAE、それもJava版を採用せずPython版にした理由は何か、
気になる方もいると思うので判断基準について説明しておきます。
この部分は講義中に理解できる必要はありません。

 * GAEはPaaSとして必要なコード量が極めて少ないため、本質的な部分だけに集中できる。
 ** 本講義はAndroidアプリ講座であってLinux等のOS講座ではないので、IaaS環境は不適。
 ** Webサーバ側の事情であるため、VolleyによるRESTアクセスや外部ライブラリのインポートで済むようなSaaS環境の仕様はやはり不適。
 * 講義で利用しているGoogleアカウントをそのまま利用して開発を行える。
 * 受講生環境はJDK 6がインストールされているが、多くのWebサーバ関連の実装ではJava 7が進んでいる。
 ** GAE JavaもJDK 7が必須。
 * 受講生の環境はAndroid開発のためのものであり、Webサーバ学習のためだけにみだりに変更したくない。
 * 第三者フレームワーク含めてGAE Javaは担当者の知見が十分でない。

ただしGAE全体について、現段階でもWebサーバ開発のプラットフォームとしては、
気になる点があるのは事実です。

 * GAEは実践的なサービスとしての魅力を失いつつある。
 ** GCEの登場も重なって、GAEにまつわるWeb上のチュートリアルドキュメント等は明らかに混乱している。
 * PaaSとして特殊なデータ構造を持つため、他の環境との組み合わせに難がある。
 * 複数の言語があるためGAEと言っても結局ノウハウが分断されており、特にPython実装の情報集積は日本では相対的に少ない。

#@# * GAE API ExpertがGoogle USに行ってしまいGCEも出て日本のコミュニティがオワコン

上記の点から総合して、本項ではGAE Pythonを選ぶという選択となりました。

