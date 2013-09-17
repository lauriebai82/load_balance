load_balance
============

a simple realize of load balance by C
server provide datetime of now to client
client get datetime from proxy, then proxy choose one server whose load is minimal to get datetime,
then return to client

we can get ./server,./client,./test,./proxy by sh compile.sh
run test.sh
we can get some result in conn_num.txt and result

result include results all clients get
conn_num record the several servers' load in one moment
from them, we can see all server is equal in one moment, and client almost get datetime from serverlist in turn.
