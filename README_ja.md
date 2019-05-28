# Medic

* MedicはOpenSourceのデータチェックツールです（for Autodesk Maya)。karteファイルを作成しcustomのチェックリストを定義することも、またc++もしくはpythonのpluginを追加しチェック項目を増やすこともできます。

<img src="https://github.com/sol-ansano-kim/medic/wiki/images/medic.png" width="480"> 
<br><br><br>

* デモ  
<br>

[![Alt text](https://github.com/sol-ansano-kim/medic/wiki/images/medic_youtube.png)](https://youtu.be/FjWmc0GHiL8)

<br>

* また[pyblish](http://pyblish.com)のpluginとして使う場合は、[medicToPyblish](https://github.com/sol-ansano-kim/medicToPyblish)を参照してください。

<br>

# Usage

1. [パッケージ](https://github.com/sol-ansano-kim/medic/releases)をダウンロード・解凍します。(パッケージがないなどでmedicをビルドする場合は[Medic Wiki](https://github.com/sol-ansano-kim/medic/wiki/Japaness#ビルド)を参照してください)
2. 解凍したフォルダの中身をmodule pathフォルダにコピーするか、環境変数MAYA_MODULE_PATHにフォルダのパスを追加します。

   Moduleフォルダ

    windows : C:/Users/<User名>/Documents/Maya/<Mayaバージョン>/modules  
    osx : ~/Library/Preferences/Autodesk/maya/<Mayaバージョン>/modules  
    linux : ~/maya/<Mayaバージョン>/modules  

3. Mayaの中で以下のコマンドを実行します。(もっと詳しい使い方は[使用例](https://github.com/sol-ansano-kim/medic/wiki/Japaness#使用例)を参照してください)

```python
import medicUI
medicUI.Show()
```

4. チェックリストを新しく作成する場合は[Karteの書き方](https://github.com/sol-ansano-kim/medic/wiki/Japaness#karte)を参照してください。
5. 新しいチェック項目を増やす場合は[Testerの書き方](https://github.com/sol-ansano-kim/medic/wiki/Japaness#custom-tester)を参照してください。