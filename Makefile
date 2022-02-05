CC = gcc
CFLAGS = -Wall -Wextra

ARCHIVE_COBJS = hash-table.o _ht-node.o _ht-table.o
COBJS = $(ARCHIVE_COBJS)
ARCHIVES = libhash-table.a
LL_ARCHIVE_RAW = liblinked-list.a
LL_ARCHIVE = linked-list/$(LL_ARCHIVE_RAW)

default: all

$(COBJS): %.o: %.c
$(COBJS): linked-list-submodule

hash-table.o: hash-table.h _ht-node.h
_ht-node.o: hash-table.h _ht-node.h
_ht-table.o: hash-table.h _ht-node.h _ht-table.h


all: all-objs all-archives
all-objs: $(COBJS)
all-archives: $(ARCHIVES)

libhash-table.a: $(ARCHIVE_COBJS) $(LL_ARCHIVE)
	cp $(LL_ARCHIVE) $@
	$(AR) r $@ $(ARCHIVE_COBJS)


clean-objs:
	$(RM) $(COBJS)

clean-archives:
	$(RM) $(ARCHIVES)


clean: clean-objs clean-archives
	$(MAKE) -C linked-list/ RM="$(RM)" clean

debug: CPPFLAGS += -DDEBUG
debug: CFLAGS += -g
debug: all-objs all-archives

$(LL_ARCHIVE): linked-list-submodule
	$(MAKE) -C linked-list/ CC="$(CC)" CFLAGS="$(CFLAGS)" CPPFLAGS="$(CPPFLAGS)" LDFLAGS="$(LDFLAGS)" $(LL_ARCHIVE_RAW)


linked-list-submodule: | linked-list/linked-list.h

linked-list/linked-list.h:
	git submodule init
	git submodule update

.PHONY: all all-objs all-archives clean-objs clean-archives clean default debug linked-list-submodule
