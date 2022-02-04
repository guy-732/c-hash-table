CC = gcc
CFLAGS = -Wall -Wextra

EXECS = add-op search-op remove-op
COBJS = $(EXECS:=.o)

ARCHIVES = ../libhash-table.a


all: all-objs all-progs

all-objs: $(COBJS)
all-progs: $(EXECS)

$(ARCHIVES) &: main-makefile

$(COBJS): %.o: %.c

add-op: add-op.o ../libhash-table.a
search-op: search-op.o ../libhash-table.a
remove-op: remove-op.o ../libhash-table.a


clean: clean-objs clean-progs
	$(MAKE) -C ../ clean

clean-objs:
	$(RM) $(COBJS)

clean-progs:
	$(RM) $(EXECS)

main-makefile:
	$(MAKE) -C ../ CFLAGS="$(CFLAGS)" CPPFLAGS="$(CPPFLAGS)" LDFLAGS="$(LDFLAGS)" all-archives


debug: CFLAGS += -g
debug: CPPFLAGS += -DDEBUG
debug: all

.PHONY: all all-progs all-objs clean clean-objs clean-progs debug main-makefile
