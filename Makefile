CFLAGS=-Wall -Wextra

PROJECTS=picnic

all: $(PROJECTS)
.PHONY: clean
clean:
	$(RM) $(PROJECTS)