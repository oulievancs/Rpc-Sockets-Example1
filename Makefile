#/*
# *	Author: Oulis Evangelos
# *	Class:	Friday 2-4
# *	Subject:	Distributed Systems
# *	Description:	Sockets-C/RPC Makefile code.
# *	File Name:	Makefile
# */

# Instractions:
# 1. To compile all sources run "make".
# 2. To start the rpc server run "make rpc_server"
# 3. To start the socket server/rpc client run "make rpc_cl_socket_srv"
# 4. To start the socket client run "make socket_client"
#
# **You can run "make clean" to clean all the executables packages.
# END.

# You can change this values.
PORT = 4002
HOST = 127.0.0.1
RPC_HOST = 127.0.0.1

# ===========================================================================
# Parameters

# Sources
SOCK_CLIENT = socket/socket_client
CLIENT = rpc/socket_server
SERVER = rpc/remoteproc_server

# Executables
CCLIENT = rpc/socket_server.c
CSERVER = rpc/remoteproc_server.c
SCLIENT = socket/socket_client.c

# Compiler Flags
SOCK_CFLAG_OUT = -o
CFLAG_THREAD = -pthread

SOURCES_CLNT.c = 
SOURCES_CLNT.h = 
SOURCES_SVC.c = 
SOURCES_SVC.h = 
SOURCES.x = rpc/remoteproc.x

TARGETS_SVC.c = rpc/remoteproc_svc.c $(CSERVER) rpc/remoteproc_xdr.c
# From the line below replaced rpc/remoteproc_client.c with rpc/socket_server.c. 
# We don't want the template rpc client.
# The role of rpc client is played by the socket_server.
TARGETS_CLNT.c = rpc/remoteproc_clnt.c $(CCLIENT) rpc/remoteproc_xdr.c
TARGETS = rpc/remoteproc.h rpc/remoteproc_xdr.c rpc/remoteproc_clnt.c rpc/remoteproc_svc.c $(CCLIENT) $(CSERVER)

OBJECTS_CLNT = $(SOURCES_CLNT.c:%.c=%.o) $(TARGETS_CLNT.c:%.c=%.o)
OBJECTS_SVC = $(SOURCES_SVC.c:%.c=%.o) $(TARGETS_SVC.c:%.c=%.o)
# Compiler flags 

CFLAGS += -g 
LDLIBS += -lnsl $(CFLAG_THREAD)
RPCGENFLAGS = 

# Targets 

all : $(CLIENT) $(SERVER)
	gcc $(SOCK_CFLAG_OUT) $(SOCK_CLIENT) $(SCLIENT)
#	gcc $(SOCK_CFLAG_OUT) $(SERVER_OUT) $(CFLAG_THREAD) $(SOCK_SERVER)

$(TARGETS) : $(SOURCES.x) 
	rpcgen $(RPCGENFLAGS) $(SOURCES.x)

$(OBJECTS_CLNT) : $(SOURCES_CLNT.c) $(SOURCES_CLNT.h) $(TARGETS_CLNT.c) 

$(OBJECTS_SVC) : $(SOURCES_SVC.c) $(SOURCES_SVC.h) $(TARGETS_SVC.c) 

$(CLIENT) : $(OBJECTS_CLNT) 
	$(LINK.c) -o $(CLIENT) $(OBJECTS_CLNT) $(LDLIBS) 

$(SERVER) : $(OBJECTS_SVC) 
	$(LINK.c) -o $(SERVER) $(OBJECTS_SVC) $(LDLIBS)
	
rpc_cl_socket_srv:
	./$(CLIENT) $(PORT) $(RPC_HOST)
	
socket_client:
	./$(SOCK_CLIENT) $(HOST) $(PORT)
	
rpc_server:
	./$(SERVER)

clean:
	$(RM) core $(OBJECTS_CLNT) $(OBJECTS_SVC)
	rm $(SOCK_CLIENT) $(CLIENT) $(SERVER)
