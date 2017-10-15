SUBDIRS := src

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	$(MAKE) -C $@ clean

install:
	cp init.d/softshut /etc/init.d/

.PHONY: all $(SUBDIRS)
