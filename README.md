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
# How to start mariadb with docker:

This command will run a mariadb container with password "mypass". You can change the volume and password.

```bash
docker run --name project_pds -p 3306:3306 -v /home/[yourUsername]/container_volume/projectPDS:/var/lib/mysql -e MYSQL_ROOT_PASSWORD=mypass -d mariadb 
```

After that when container starts connect to it with this command

```bash
docker exec -it project_pds bash
```

and run this command inside the container to create a database for the server.
Following commands will ask you the password that you have configured in docker run command.
```bash
mysql -p -e "CREATE DATABASE serverDB"
```
You can see available databases with this command:

```bash
mysql -p -e "SHOW databases;"
```
Now you can start correctly the server.



To see contents of users_table in mariadb container
execute following commands:

```bash
mysql -uroot -p
use serverDB;
Select* from users_table;
```