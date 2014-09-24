= 参考トピック

//lead{
本節では、授業で用いる範囲で有用と思われるトピックを学びます。
授業の補助資料として利用してください。
//}

== この節で学ぶこと

 * Pythonについて
 * シラバスアプリのインポート

== Pythonを学ぶ

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

=== Pythonのインタープリタ(対話型環境)を起動する。

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

=== Pythonは動的型付言語

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

=== ソースコードの記述されたファイルを実行する

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

=== 文字列について

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

=== Python文字列での日本語の扱い

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


=== Pythonではインデントに意味がある

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

=== if

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
# インデントをなくして関数の終わりを明示する。
print(sum(1, 9))  # 10を表示する
//}

=== 標準ライブラリとimport

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
@<fn>{about_json}

//footnote[about_json][JSONデータはもともとJavaScriptという別の言語に由来しており、Pythonのデータ構造とピタリ一致しているわけではありません。]

Python 2.7の持つライブラリの内容は
@<href>{http://docs.python.jp/2.7/library/index.html}から参照できます。
特に、jsonモジュールの説明は@<href>{http://docs.python.jp/2.7/library/json.html}にあります。


== GAEその他のトピック
=== Googleアカウントを用いた「ログイン」を実装する。



== シラバスアプリのインポート

本項では、シラバスアプリをGitHubからダウンロードして、
既存のEclipseにインポートする方法を示します。

「ユーティリティによる実践」の演習でシラバスアプリが完成していない場合、
特にURLを指定してJSONデータをWebからダウンロードするところまで
到達していない場合に、本項を参考にしてください。

=== シラバスアプリのソースコードの取得

@<href>{https://github.com/ktaka/TechInstituteSyllabus}へアクセスします。

右下の「Download ZIP」ボタンをクリックします。

#@# Download ZIPの画像を用意

zipを展開し、展開したフォルダの場所を覚えておきます。

=== Eclipseでプロジェクトをインポートする


=== シラバスアプリの動作を確認する

