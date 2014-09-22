= Webサーバについて学ぶ

//lead{
この章ではAndroidアプリと通信するWebサーバについて学びます。
これまでは事前に容易されたデータをAndroidアプリから取得するだけでしたが、
本章ではそのデータを提供する側を実装します。
//}

== この節で学ぶこと

 * Webサーバとは？
 * サーバはなぜ重要？

この節で出てくるキーワード

 * Webサーバ
 * DNS
 * HTTP

すでにWebサーバにアクセスするプログラムは何度も登場してきました。
しかしその時でも、一般に公開されている商用のWebサーバや、
予め講師が準備したWebサーバへアクセスし、欲しいデータを取得するだけでした。

本章では反対側の「Webサーバ」を自分で用意・実装し、それを通じて
AndroidアプリがWebサーバと通信する際にサーバ側で何が起きているかを学習します。

本章では、特に第13章「ネットワークプログラミング」で学んだ概念を前提とします。
理解に不安がある際には復習してから以降を読み進めてください。

== Webサーバとは？

Webブラウザを用いてWebページを閲覧するのが当たり前の時代ですから、
Webという言葉についてはイメージが付くでしょう。

サーバ(Server)という単語は、
「ジュースサーバ」といった表現で日常会話でも登場します。
IT分野以外では、「給仕する人、接客係」といった意味で使われる単語です。

//image[juiceserver][ジュースサーバ。画像の版権確認なし。要確認]{
//}

堅苦しく言えば、サーバは「資源を要求する側に、その資源を提供」します。
「資源を要求する側」はながったらしーので、「クライアント」と呼びます。
ジュースサーバの場合、クライアントが要求する「資源」はジュースでした。

資源を補充されることもあります。
ジュースサーバでは、ジュースがなくなったら、誰かが継ぎ足します。

Webサーバの場合にも、資源を求めてAndroidアプリがアクセスします。
あるいは資源を補充するためにもアプリがアクセスします。

Webサーバは、実際にはAndroidアプリ以外とも通信します。
冒頭のWebページのことを思い出してください。
Webページを受け取るPCでは、AndroidアプリではなくWebブラウザが
起動しているかもしれません。

Webサーバは、準備があれば、等しく「資源」をやり取りします。
本章では後半でこれを意識したサーバ実装を行います。


== Content-Type

== GET/POST

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

== https

URLに「http」で始まるケースと「https」で始まるケースがあります。
httpの場合、TCP接続して相手と通信する際、
途中の経路に存在する全てのホストからデータを盗み見ることが出来ます。
httpsでは通信経路で暗号化が行われるので、
不適切な方法で通信しなければデータを盗み見られることはありません。
@<fn>{https_is_wrongly_used}

//footnote[https_is_wrongly_used][ただし、Androidはその性質上、しばしば不適切な方法でhttpsを用いた通信がなされており、httpsの意味が全くないことがあります。「セキュリティ」の章でより詳細に学ぶことになるでしょう。]

送受信するデータを他人から見られたくない場合は必ず「https」の
URLを用いてやりとりをしてください。
今回用いるGAEの本番サーバは初めからhttpとhttpsの両方をサポートしています。

ただし、いつもこうとは限りません。
事実、GAEで開発中に使う@<href>{http://localhost:8080}というURLでは
httpsを利用することは出来ません。
その代わり、このURLはPC内からしか見ることが出来ないため、それによってセキュリティを確保しています。

Androidアプリ開発者の視点からは、
インターネット上でのhttpの使用は「暗号化されていない」と、
とりあえず覚えておいてください。

盗み見られても全く害がない場合には使っても問題ありませんが、
近年ではそのような分野はどんどん少なっています。
Googleも以前はhttpで検索結果を表示していたのですが、
現在ではほぼ全てhttpsによって暗号化しています。
おそらく読者が目にする主要なWebサービスはほとんどhttpsを使っているはずです。

アプリ開発者としてWebサーバからデータを取得する場合には、
こういった点にも注意を払ってください。

== 更なる学習のために

本節ではWebサーバを作る上で最小限把握して欲しい内容をまとめました。
Androidアプリから状況を把握する上ではこれで十分でしょう。

本節の内容は入門と言うにも相当単純化しています。

@<href>{https://github.com/TechBooster/AndroidOpenTextbook}
に、本節では説明していない要素も含めた全内容が含まれています。
オープンソースとして自由に参照できますので、

2014年10月現在、
@<href>{https://tcb.mowa-net.jp/griflet/github/TechBooster/AndroidOpenTextbook/}
から上記のPDF版やHTML版を得ることができます。

Webサーバの技術を理解するにはインターネットもしくは「ネットワーク」についての
知識が必要になります。
さらなる詳細については、例えば以下のような書籍を参照してください。

 * マスタリングTCP/IP 入門編 第5版 http://www.amazon.co.jp/dp/4274068765
 * マスタリングTCP/IP 情報セキュリティ編 http://www.amazon.co.jp/dp/4274069214/
 * コンピュータネットワーク 第5版 http://www.amazon.co.jp/dp/482228476X
