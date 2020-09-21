# shared-editor-server
How to install mysql drivers in linux:
```bash
sudo apt-get install g++
sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev
sudo apt-get install libssl-dev
sudo apt-get install libmysqlclient-dev
cd /home/[yourUsername]/Qt/[QtVersion]/Src/qtbase/src/plugins/sqldrivers/mysql
/home/[yourUsername]/Qt/[QtVersion]/gcc/bin/qmake
make
make install
```
