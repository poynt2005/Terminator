# Terminator

## Windows api 練習

利用列舉 process + 查詢 process name，找出目前運行的程序  
再使用程序的 pid + Terminate 結束進程

因為 TortoiseGit 每次開都會開一大堆，所以就想做這個程式  
這個專案主要用來自動清除 TortoiseGit 的 progress

## 編譯

要先安裝 mingw AMD64 版本  
安裝 node

1. cd node
2. npm install --ignore-scripts
3. npm run build-all

## 執行

cd node  
運行 npm run killgit
