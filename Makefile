COMPILER = gcc
DEBUG_COMPILER = -g -DDEBUG=1
OUTPUT_FILE_MON = BikeSharingMON
OUTPUT_FILE_NYC = BikeSharingNYC
FILES_MON = frontMON.c bikeSharingADT.c htmlTable.c
FILES_NYC = frontNYC.c bikeSharingADT.c htmlTable.c

all: $(OUTPUT_FILE_MON) $(OUTPUT_FILE_NYC)

$(OUTPUT_FILE_MON): $(FILES_MON)
	$(COMPILER) -o $(OUTPUT_FILE_MON) $(FILES_MON) -Wall -pedantic -std=c99 -lm -fsanitize=address -g

$(OUTPUT_FILE_NYC): $(FILES_NYC)
	$(COMPILER) -o $(OUTPUT_FILE_NYC) $(FILES_NYC) -Wall -pedantic -std=c99 -lm -fsanitize=address -g

clean:
	rm -f $(OUTPUT_FILE_MON) $(OUTPUT_FILE_NYC) *.o

debug: COMPILER+=$(DEBUG_COMPILER)
debug: all

