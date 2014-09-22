= Pythonリファレンス

//lead{
本説では、授業で用いる範囲で有用と思われるPythonトピックを学びます。
授業の補助資料として利用してください。
//}

== この節で学ぶこと

本節は補助資料です。
プログラミング言語Pythonについて、
演習でで有用と思われる内容を紹介します。

Pythonの詳細については、Web上に詳細なドキュメントが公開されています。
そちらを参照してください。

 * @<href>{http://docs.python.jp/2/tutorial/}

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

Pythonには、古いバージョンであるPython2とPython3の、
大きく分けて2種類の系統があります。
本章で採用するのは現在Google App Engineで利用できる
Python2 (バージョン2.7)の方です。

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

=== ソースコードの記述されたファイルを実行する

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

なお、対話型環境で次のように毎回一行一行入力することもできます。
ただしこの場合、どこにも保存されていないため、
再度実行したいときには最初から入力し直す必要があります。

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
これは5-1-2「Javaプログラムを実際に動かしてみよう」に登場した、
JavaのSystem.out.println()と同じです。

=== 文字列について

Javaと同様、Pythonにも文字列があります。
"(ダブルクオート)もしくは'(シングルクオート)で囲った部分が
文字列になります。
Javaと異なり、Pythonでは""と''で囲った場合で
文字列の挙動はほとんど変わりません。
PythonにはC言語のcharに相当する型は存在しません。

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

最後の例(「'日本語' == u'日本語'」)では、不穏な警告まで表示されています。

本講義全体で文字コードについての説明はされていないため、詳細は省きます。
Python言語で日本語を扱う際にはしばしば「ユニコード文字列」
と呼ばれるフォーマットを使う必要があることを覚えておいてください。


=== 文字列以外のデータ型について

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
以下の例で感覚を掴んでみてください。

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
に利用できるAPIの全てが掲載されています。

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

=== 関数

Pythonの関数は次のように書きます。
インデントの有無で関数の開始と終了を明示します。
ifやforと同様、中括弧は必要ありません。

//emlist[関数の例]{
def sum(a, b):
    return a + b
//}



=== 標準ライブラリとimport

Pythonではインストールした段階で豊富なライブラリが付属しています。

以下で今回使うPython 2.7の持つライブラリの内容を知ることが出来ます。
@<href>{http://docs.python.jp/2.7/library/index.html}

JSONデータを扱うjsonモジュールを例に、標準ライブラリに含まれるモジュールの使い方を説明します。
Pythonでは外部ライブラリは明示的に利用することを宣言する必要があります。
jsonモジュールを使うには「import json」とします。
これ以降、対話型環境およびそのソースコードファイルではjsonモジュールを使うことが出来ます。

試しにPythonマップ型データをJSONデータに変換してみます。

//cmd[実行時の時刻を取得する]{
>>> import json
>>> json.dumps({'a': 1, 'b': 2})
'{"a": 1, "b": 2}'
//}

一見、入力したデータとjson.dumps()関数の結果がそっくり同じに見えますが、
よく見ると関数の出力結果は「文字列」です。
Pythonのデータ構造とJSONフォーマットの文字列データは一見してよく似ています。
@<fn>{about_json}

//footnote[about_json][JSONデータはもともとJavaScriptという別の言語に由来しており、Pythonのデータ構造とピタリ一致しているわけではありません。]

jsonモジュールの説明は@<href>{http://docs.python.jp/2.7/library/json.html}にあります。

== さらなる学習

本講義はAndroidアプリの開発を目的とするものですのでPythonの詳細な説明は行いません。

興味深いことに、Python入門として @<href>{http://docs.python.jp/2/tutorial/} を学び、
@<href>{http://docs.python.jp/2.7/library/} のURLを覚えておくだけで、
Pythonの基本的な機能はほぼ理解できます。
Javaをすでにご存知であれば、Pythonを理解するのはさらに簡単でしょう。
