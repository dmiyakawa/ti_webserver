= PythonとGAEを理解しよう

//lead{
本節ではその中でも今回採用するPythonとGoogle App Engineについて学びます。
//}

== この節で学ぶこと

本節は主にプログラミング言語Pythonの機能について、
授業で用いる範囲で有用と思われるトピックを取捨選択したものです。
Pythonの詳細については、適宜Web上のドキュメントや書籍を利用してください。
本節の最後で参考書籍を紹介します。

== Pythonとは？

Pythonは世界的にもよく利用されているプログラミング言語です。
PYPL(PopularitY of Programming Language Index)や
TIOBE Programming Community Indexといった
プログラミング言語の人気度を測る指標でJavaやC言語と並んでトップ10以内
を維持しています。

また、特に教育関係での採用が増えていると言われており、
マサチューセッツ工科大学(MIT)や
カリフォルニア大学バークレー校といった
コンピュータサイエンス教育において世界的に有名な大学で、
プログラミング入門の授業で採用されています。

本節では受講生がJava言語の基礎を習得済と仮定し、
主にJava言語との違いを中心としてPython言語を紹介します。

興味がある方は、Python入門として最適と言われる「Python Tutorial」の
日本語版 @<href>{http://docs.python.jp/2/tutorial/} なども
参照してみてください。

なお、Pythonには、古いバージョンであるPython2とPython3の、
大きく分けて2種類の系統があります。
本章で採用するのは現在Google App Engineで利用できる
Python2 (バージョン2.7)の方です。

自力で検索をかける際には、
このPython2とPython3で挙動が大きく異なることがある点に注意してください。

=== Pythonのインタープリタ(対話型環境)を起動する。

Pythonプログラムを一行ずつ実行し、
結果を確認しながら言語を学習することができます。
「Python Console」もしくは、
より一般にプログラミング言語の「対話型環境」と呼ばれています。

前節でインストールしたPyCharmを利用している場合、
「Tools >> Run Python Console」で対話型環境が画面下方に表示されます。
Windowsのターミナルから"python"と入力することでも起動できます。

以降、行が「>>>」で始まっている部分が
このPython Console内でプラグラムを実行しているところを表します。

試しに電卓のように使ってみるとしましょう。

//cmd{
>>> 1 + 10
11
//}

 * 演習1: "1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10" の結果を計算してみよう
 * 演習2: "1/2" と "1/2.0" を計算してみよう

=== Pythonは動的型付言語

Javaでは各変数の型を明示する必要がありましたが、Pythonでは必要ありません。
ただし、型が存在しないわけではありません。
一定のルールに基づいて型が変化します。

//cmd{
>>> x = 333
>>> y = 400
>>> x * y
133200
//}

定数や変数の型はtype()で調べることができます。

 * 演習1: type(1) type(2.0) の結果を見てみよう。
 * 演習2: "1/2" "1/2.0" の結果が異なる理由について、型の違いから理由を想像してみよう。

=== ソースコードを実行する

メモ帳や、あるいは前項のIDEであるPyCharmで作ったプログラムのファイルを
Pythonの実行環境に読み込ませて実行させることができます。

例えば次のようなプログラムを作ったとします。

//list[simpleprogram.py][簡単なプログラムの例]{
x = 10
y = 20
z = 30
x = y * z - x
y = z * x - y
z = x * y - z
print(x + y + z)
//}

これをJavaと同様、1つのファイルに保存して実行することができます。
PyCharmでは特定のPythonファイルを右クリックし「Run 'xxx'」

次のように毎回一行一行入力することもできます。

//cmd{
>>> x = 10
>>> y = 20
>>> z = 30
>>> x = y * z - x
>>> y = z * x - y
>>> z = x * y - z
>>> print(x + y + z)
(出てくる数字はいくつだろうか？)
//}

プログラムをファイルに保存して実行する場合、
print()で表示したい内容を囲う必要があります。
これはJavaで言うSystem.out.println()と同じです。
@<fn>{about_println}

//footnote[about_println][5-1-2「Javaプログラムを実際に動かしてみよう」に登場しています。]

=== 文字列について

Javaと同様、Pythonにも文字列があります。
"(ダブルクオート)もしくは'(シングルクオート)で囲った部分が
文字列になります。

//cmd{
>>> "Hello World!"
'Hello World'
>>> 'Hello World!'
'Hello World'
//}


例えばダブルクオートの中でダブルクオートを使いたい場合、
バックスラッシュでエスケープをしてあげる必要があります。

//cmd{
>>> print("Say \"Hello World!\"")
Say "Hello World!"
//}

しかし、シングルクオート内でのダブルクオートを
エスケープする必要はありません。
逆もしかりです。

//cmd{
>>> print('Say "Hello World!"')
Say "Hello World!"
//}

Pythonのインタープリタの中では上記のような瑣末のものを除いて、
文字列の囲い方について大きな違いはありません。
本章でも適宜両方使っています。

JavaやC言語などと異なり、Pythonでは""と''で囲った場合で
文字列の挙動はほとんど変わりません。
異なるのは文字列の中でのそれらの文字の扱いです。
PythonにはC言語のcharに相当する型は存在しません。

//cmd{
>>> type('Hello World!')
<type 'str'>
>>> type('a')
<type 'str'>
>>> type('Hello World!') == type('a')
True
//}

ダブルクオートもしくはシングルクオートを3つ並べた場合、
複数行の文字列を簡単に作成することができます。

//cmd{
>>> print('''Hello
... World
... !!!!!''')
Hello
World
!!!!!
//}

最後に、文字列の中に数字や他の文字列を埋め込む方法を説明します。

最も単純なのは「{}」を文字列に埋め込んだ上でformat()
メソッドを呼ぶ方法です。
本講義の範囲では以下の方法を覚えるだけで事足りるでしょう。

//cmd{
>>> print('Hello {} and {}!!'.format(20, 10))
Hello 20 and 10!!
//}

なお、Pythonでは文字列型と数値型は「+」演算子で結合出来ません。
以下の方法は利用することが出来ませんので注意してください。

//cmd{
>>> 'Hello ' + 20 + ' and ' + 10 + '!!'
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: cannot concatenate 'str' and 'int' objects
//}

「{}」という書式は非常に多彩な機能をもっています。
もし興味がある場合には、以下のドキュメント等を見て、
必要に応じて使ってみましょう。

@<href>{http://docs.python.jp/2/library/string.html#formatstrings}

なお、この方法を、Pythonにおける「書式指定文字列」と呼びます。
(検索するときに重要です)。

これとよく似た機能として「文字列フォーマット操作」というものもあります。
こちらは、C言語のprintf()やJavaのSystem.out.printf()に
影響を受けたと思われる書式ですが、
現在は書式指定文字列を用いる方が望ましいとされています。


=== Python文字列での日本語の扱い

Pythonの文字列についてしばしば混乱を誘うのが、
日本語のような非アスキー文字の扱いです。

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

最後の例(「'日本語' == u'日本語'」)では不穏な警告まで表示されます。
一体どういうことでしょうか。

本講義全体で「文字コード」についての説明をしていないため、
ここではこの件について、
正確さを犠牲にした簡略な説明だけを行うことにします。

文字列をコンピュータの2進数で表現する方法は、いくらでも考えられます。
0をA(大文字)、1をBと並べるのも有りですし、
0をa(小文字)、1をbとしても、辻褄を合わせることはできます。
ただ、各コンピュータ間で「統一」していなければ、役に立ちません。

世界中のコンピュータがバラバラな表現方法を使うのは大変都合が悪いですから、
ITの世界では半角アルファベットや半角数字、一部の記号については
ほぼ世界的に一つの規格(US-ASCII)が使われるようになりました。

@<href>{http://ja.wikipedia.org/wiki/ASCII}

日本語について言えば、
US-ASCIIのように足並みを揃えることには成功しませんでした。
これは日本語だけでなく、他の言語でも起きていることです。

こういった世界的な問題に対応するため、
現在国際標準で「ユニコード」と呼ばれる規格が策定されています。
これは世界で一つのルールを決めるという試みで、
実際には有名なプログラミング言語の多くが対応しています。

@<href>{http://ja.wikipedia.org/wiki/Unicode}

Pythonは、US-ASCIIだけ表示できれば十分なコミュニティで当初発展した関係で、
ユニコードのような非アスキーの扱いについて若干雑な対応をしてきた歴史があります。
ただ、言語の仕様をみだりに変えるとそれはそれでユーザが困るため、
「綺麗なもの」にばっさり変えるわけにもいかないのが悩ましいところです。

上記のような「混乱がある」と言う状況が、不幸にも
プログラミング言語のユーザの手元でそのまま再現してしまう、
というのがPythonでの文字列の混乱の本質です。
『小飼弾のコードなエッセイ』で
ブロガーでハッカーである小飼弾氏も「Pythonの美点をぶち壊すもの」として
この周囲の状況を批判しています。

Python2からPython3に移行する際、Pythonの開発者はこの点について
慎重に文字列の扱いを変えようとしたのですが、
混乱が収束したわけではありません。
本章の演習でももしかするとみなさんも深みにハマるかも。


=== 文字列以外のデータ型について

動的型付とは「柔軟に型の異なる値を入れ替えられる」という意味であって、
型が「存在しない」わけではありません。

 * 整数
 * 文字列
 * Unicode文字列
 * 真偽値 (True, False。Javaのbooleanと異なり冒頭は「大文字」なので注意)
 * リスト
 * 辞書
 * オブジェクト型

本項ではPythonのオブジェクト指向言語としての側面はあまり取り上げませんが、
Javaのようなオブジェクト指向の仕組みはPythonにも存在します。


=== Pythonではインデントに意味がある

ここで、Pythonを学ぶ初学者が陥りがちな罠を紹介しましょう。
Pythonでは「インデントに意味があります」。

if文の書き方を通じてJavaとPythonを比較してみましょう。
まずJavaのif文の例を以下に示します。

//list[using_if_with_java][Javaのif文]{
if (x == 0) {
   print(u"x is 0")
} else {
   print(u"x is not 0")
}
//}

次にPythonで同様にif文を書いた場合は以下のとおりです。

//list[using_if_with_python][Pythonのif文]{
if (x == 0):
   print(u"x is 0")
else:
   print(u"x is not 0")
//}

注目するべきは、中括弧がどこにもないことです。
代わりに、ブロックの開始は:(コロン)によって示されています。

Pythonでは、次のように書いては「いけません」

//list[using_if_with_python_wrong][Pythonのif文(間違った例)]{
# THIS IS WRONG
if (x == 0):
print(u"x is 0")
else:
print(u"x is not 0")
# THIS IS WRONG
//}

これはエラーになります。

インデントつまり行頭の空白が、各行がどのブロックに属しているかを示します。
次の例を見てみましょう。

//list[using_if_2][Pythonのif文(間違った例)]{
if (x > 0):
    print(u"x > 0")
    if (x < 30):
        print(u"0 < x < 30")
    else:
        print(u"x >= 30")
//}

これを次のように書くとprint()文と実際のxの中身が一致しなくなります。

//list[using_if_2_wrong][Pythonのif文(間違った例)]{
if (x > 0):
    print(u"x > 0")
    if (x < 30):
        print(u"0 < x < 30")
else:                        # この行以降のインデントが壊れたとする。
    print(u"x >= 30")         # x が例えば -30 であってもこれが表示される。
//}

例えば、コピーアンドペーストをした際に、
@<list>{using_if_2_wrong} のような意図しないインデントが発生しがちです。

=== if

既にifは登場していますが、説明していないことがあります。
Pythonではifの後の括弧は必要ありません。

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
この場合、おおまかに「空でないか」をチェックします。
空の意味は変数に実際に保存されているデータの型で変わります。

 * 整数なら 0
 * 文字列なら ''
 * リストなら []
 * 辞書なら {}


//list[using_if_with_not_no_int][ifの例]{
if x: print(u"x is not empty")
//}

なお、このような書き方を許す言語はPythonに限らず多数ありますが、
逆にそれが混乱を招く可能性があります。

例えば、Pythonと良く比較対象になるRuby言語では、
整数の0を真に振るか偽に振るかがPythonと異なります。
Rubyでは0は真で、Pythonでは上述の通り0は偽です。
よって、Rubyとそっくりだと思って "if x:" と書いた場合、
その結果として分岐が完全に入れ替わる可能性があります。

//list[ruby_if][Rubyのコードだとyay!が出る。0は「真」]{
(rubyの対話環境)> x = 0
(rubyの対話環境)> if x; "yay!"; end
=> "yay!"
//}

=== for

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

==== 課題 (初級)

Pythonのrange()関数の機能を調べてみよう。
20から始まり40未満までの数字を4つ飛ばしで表示してみよう。

//cmd{
>>> for (rangeを使って自分で作る): print(i)
... (何も入力せずにエンター)
20
24
28
32
36
//}

==== 課題 (やや高度)

今回扱うPython2にはxrange()という関数という関数もある。
表示する内容はrange()と同じになる。

//cmd{
>>> for i in xrange(5): print(i)
... (何も入力せずにエンター)
0
1
2
3
4
//}

たった1字の違いだが、大きな差がある。
ここではその違いを理解する。

Pythonの関数とyieldという命令を用いて
大きな配列の要素を返すgeneratorを作ってみよう。
作られたgeneratorはfor文のinの後ろで利用することが出来る。

//cmd{
>>> def generator():
...     for i in xrange(5): yield i
...
>>> iter = generator()
>>> for item in iter:
...     print(item)
...
0
1
2
3
4
//}

range()とxrange()で、最初の要素を取り出すときの速度を計測してみよう。

//cmd{
>>> size = 200000000
>>> def generator_1():
...     for i in xrange(size): yield i
... (エンター)
>>> iter = generator_1()
>>> iter.next()
0
>>> def generator_2():
...     for i in range(size): yield i
... (エンター)
>>> iter = generator_1()
>>> iter.next()
0
//}

なお、新しいバージョンであるPython3でrange()が「なくなり」、
xrange()が標準のrange()になった。
言語の実装者の気持ちになって、理由を考えてみよう。



=== 関数

=== モジュールとimport



=== その他の情報

 * http://docs.python.jp/2/tutorial/



== GAE


=== logging

本節で広く利用したprint()は、
PyCharmやインタラクティブ環境でデバッグをする際には便利ですが、
今回のようなサーバ側の挙動を記録する目的には不向きです。
AndroidにおけるLogCatのような「ログ」を保存する方法が欲しくなるでしょう。

