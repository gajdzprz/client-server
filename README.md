# Client-Server

Client-Server created by sajmon93 and gajdzprz

## How to clone project in linux

```
git clone git@github.com:gajdzprz/client-server.git client-server
```

## How to compile project

Before you start compile project you need to clone jsmn
```
git clone https://github.com/zserge/jsmn jsmn
```
next go to jsmn directory and run
```
make
```
you should get a libjsmn.a
next go to client-server directory and run
```
make
```
now you should get server and client programs

## How to run project

```
./server
./client 10.0.2.15
```

## How to connect to server via telnet

```
telnet 10.0.2.15 8081
```

## How to create new branch

click on Branch, write a name of branch and click enter

## How to create push changes on branch and create pull request

```
git add files
git commit -m "comment"
git push origin master:name_of_your_branch
```
click on github "Pull requests"
next "New pull request"
pick which one branch you want to compare
and enter
