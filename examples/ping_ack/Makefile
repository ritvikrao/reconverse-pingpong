-include ../../common.mk
CHARMC=../../../../../bin/charmc $(OPTS)

OBJS = \
  ping.o \

all: pingack

pingack: $(OBJS)
	$(CHARMC) -o pingack $(OBJS) -language converse++

ping.o: ping.c
	$(CHARMC) ping.c -g
	$(CHARMC) ping.c

clean:
	rm -f core *.cpm.h
	rm -f TAGS *.o
	rm -f pingack
	rm -f conv-host charmrun

test: pingack
	$(call run, ./pingack +p2 )

smptest: pingack
	$(call run, ./pingack +p2 ++ppn 2)
	$(call run, ./pingack +p4 ++ppn 2)

testp: pingack
	$(call run, ./pingack +p$(P) )
