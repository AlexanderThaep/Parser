rootdir = $(realpath ..)
inc_dir = $(rootdir)/include

INCLUDE_DIRS = $(rootdir) $(inc_dir)
INCLUDE_FLAGS = $(foreach dir,$(INCLUDE_DIRS),-I$(dir))

FLAGS = -Wall

SRCS = $(shell find ./ -name '*.c')
OUTS = $(SRCS:%.c=%.test)

.PHONY = test build clean

test: clean build

build: $(OUTS)
	$(foreach tst,$^,./$(tst);)

%.test: %.c
	@gcc $(INCLUDE_FLAGS) $(FLAGS) -L"ihct" -rpath "ihct" -lihct -o $@ $<

clean:
	@-rm *.test