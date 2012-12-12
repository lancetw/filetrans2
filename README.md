說明
--------
資管所企業資料通訊作業 - 簡易 CLI 傳檔工具程式 TCP Client/Server V2

本版本提供 client 端可以選擇上傳或下載模式


鄭新霖 aka Hsin-lin Cheng <<lancetw@gmail.com>>

- - - - 

Command:

    $ make

DEBUG mode:

    $ make debug
    
Use (server side):

    $ ./filetrans2
    
Use (client side):

    (U: Upload mode / D: Download mode)
    $ ./filetrans2 127.0.0.1 U uploads/sample.txt
    $ ./filetrans2 127.0.0.1 D uploads/sample_small.jpg
    
Screenshot:

![Screenshot](http://i.imgur.com/5gHLh.png "Screenshot")

