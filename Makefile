all : client server

client :
	$(MAKE) -C $@

server :
	$(MAKE) -C $@
