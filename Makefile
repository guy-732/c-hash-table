CC = gcc
CFLAGS = -Wall -Wextra

COBJS = hash-table.o _ht-node.o
ARCHIVES = libhash-table.a
LL_ARCHIVE_RAW = liblinked-list.a
LL_ARCHIVE = linked-list/$(LL_ARCHIVE_RAW)
SHARED_OBJS = $(ARCHIVES:.a=.so)

default: all

$(COBJS): %.o: %.c

hash-table.o: hash-table.h _ht-node.h
_ht-node.o: hash-table.h _ht-node.h


all: all-objs all-archives all-shared
all-objs: $(COBJS)
all-archives: $(ARCHIVES)
all-shared: $(SHARED_OBJS)

libhash-table.a: hash-table.o _ht-node.o $(LL_ARCHIVE)
	cp $(LL_ARCHIVE) $@
	$(AR) r $@ hash-table.o _ht-node.o

$(SHARED_OBJS): %.so: %.a
	$(CC) -shared $(LDFLAGS) -o $@ -Wl,-whole-archive $^ -Wl,-no-whole-archive

clean-objs:
	$(RM) $(COBJS)

clean-archives:
	$(RM) $(ARCHIVES)

clean-shared:
	$(RM) $(SHARED_OBJS)

clean: clean-objs clean-archives clean-shared

debug: CPPFLAGS += -DDEBUG
debug: CFLAGS += -g
debug: all-objs all-archives

$(LL_ARCHIVE):
	$(MAKE) -C linked-list/ CC="$(CC)" CFLAGS="$(CFLAGS)" CPPFLAGS="$(CPPFLAGS)" LDFLAGS="$(LDFLAGS)" $(LL_ARCHIVE_RAW)

.PHONY: all all-objs all-archives all-shared clean-objs clean-archives clean-shared clean default debug
