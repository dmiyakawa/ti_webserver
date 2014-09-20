= PythonとGAEを理解しよう

//lead{
本節ではその中でも今回採用するPythonとGoogle App Engineについて学びます。
//}

== この節で学ぶこと

 * Python言語入門
 * GAEの機能

本節は授業で用いる範囲で有用と思われるトピックを選択したものです。
PythonとGAEの詳細については、適宜Web上のドキュメントや書籍を利用してください。

== Python

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

PythonではJavaと異なり、
ユーザが明示的にプログラムをコンパイル(もしくはビルド)をする必要はありません。
それどころか、プログラムを一行ずつ実行することで、
結果を確認しながら言語を学習することができます。

以下は、行が「>>>」で始まっている部分が
Pythonインタープリタ内でプラグラムを実行しているところで、
それの後にPythonのインタープリタが表示する結果が続きます。

//cmd{
>>> 1 + 10
11
>>> "Hello World!"  # Pythonでは半角シャープ文字の後がコメントです！
'Hello World!'
//}

なお、"Hello World"の文字列の中身を変える場合、
文字列の冒頭にuをつけて「u"日本語！"」とします。
Python2では


=== Pythonは動的型付言語

Javaでは各変数の型を明示する必要がありましたが、
Pythonでは必要ありません。

//cmd{
>>> x = 333
>>> y = 400
>>> x * y
133200
//}

=== ソースコードファイルの読み込み方

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

=== データ型

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
Javaと似たような継承のような仕組みはPythonにも存在します。


=== Pythonではインデントに意味がある

ここで、Pythonを学ぶ初学者が陥りがちな罠を紹介しましょう。
Pythonでは「インデントに意味があります」。

if文の書き方を通じてJavaとPythonを比較してみましょう。
まずJavaのif文の例を以下に示します。

//list[using_if_with_java][Javaのif文]{
if (x == 0) {
   print("x is 0")
} else {
   print("x is not 0")
}
//}

次にPythonで同様にif文を書いた場合は以下のとおりです。

//list[using_if_with_python][Pythonのif文]{
if (x == 0):
   print("x is 0")
else:
   print("x is not 0")
//}

注目するべきは、中括弧がどこにもないことです。
代わりに、ブロックの開始は:(コロン)によって示されています。

Pythonでは、次のように書いては「いけません」

//list[using_if_with_python_wrong][Pythonのif文(間違った例)]{
# THIS IS WRONG
if (x == 0):
print("x is 0")
else:
print("x is not 0")
# THIS IS WRONG
//}

これはエラーになります。

インデントつまり行頭の空白が、各行がどのブロックに属しているかを示します。
次の例を見てみましょう。

//list[using_if_2][Pythonのif文(間違った例)]{
if (x > 0):
    print("x > 0")
    if (x < 30):
        print("0 < x < 30")
    else:
        print("x >= 30")
//}

これを次のように書くとprint()文と実際のxの中身が一致しなくなります。

//list[using_if_2_wrong][Pythonのif文(間違った例)]{
if (x > 0):
    print("x > 0")
    if (x < 30):
        print("0 < x < 30")
else:                        # この行以降のインデントが壊れたとする。
    print("x >= 30")         # x が例えば -30 であってもこれが表示される。
//}

例えば、コピーアンドペーストをした際に、
@<list>{using_if_2_wrong} のような意図しないインデントが発生しがちです。

=== if

既にifは登場していますが、説明していないことがあります。
Pythonではifの後の括弧は必要ありません。

//list[using_if_without_paren][ifの例]{
if x > 0:  # ()がない
   print("x is greater than 0")
//}

ifの後が一行であれば、次のようにも書けます。

//list[using_if_without_paren_2][ifの例]{
if x > 0: print("x is greater than 0")
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
if x: print("x is not empty")
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

=== import


=== その他の情報

 * http://docs.python.jp/2/tutorial/



== GAE


