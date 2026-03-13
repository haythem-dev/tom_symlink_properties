# how to build and start the CSC_BATCHES:

## case debug
1) get it from git (git clone...)
- Example: git clone http://tfs.phoenix.loc:8080/tfs/defaultcollection/Pharmos/_git/pharmos.outbound.csc_batches
2) change to develop branch (git checkout...)
3) get all dependent libs and frameworks (ant retrieve in the dev\src folder)
4) make sure, that you have selected debug.x64
5) compile (rebuild all)
6) copy from dev\ext\ssuk\sharedlib\Debug\ all files except the QT libs
7) start it
8) have a username in UADM  (unix/db account) throw setnet32 app
9) have the permission on the CSC permission for UW for this user (set per branch)
10) login (application user)

## same for release build from step: 4 -> 6