TARGET = BigTests

rootdir = $(realpath ..)
inc_dir = $(rootdir)/include

INCLUDE_DIRS = $(rootdir) $(inc_dir)
INCLUDE_FLAGS = $(foreach dir,$(INCLUDE_DIRS),-I$(dir))

.PHONY = test run

clean:
	@-rm ./$(TARGET)

test: clean
	@gcc $(INCLUDE_FLAGS) -L"ihct" -rpath "ihct" -lihct *.c -o $(TARGET)
	-./$(TARGET)