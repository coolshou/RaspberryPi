SUBDIRS := src

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	$(MAKE) -C $(SUBDIRS) clean

install:
	cp init.d/softshut /etc/init.d/
	$(MAKE) -C $(SUBDIRS) install
	
.PHONY: all $(SUBDIRS)
