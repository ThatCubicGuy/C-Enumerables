# File organisation

## Parent source folder. Sources may be placed here directly to be compiled solo
## or in a subfolder to be compiled together with every other file in that folder
SRC:=src
## Binaries folder. Compiled and linked executables are created here.
BIN:=.bin
## Object files folder. Compiled but unlinked binaries are created here.
OBJ:=.obj
## Library folder. Universal dependencies may be placed here,
## with the same compilation logic as source files.
LIB:=lib
## Folder for library object files.
LIBOBJ:=${OBJ}/${LIB}
## Header files folder.
HDR:=include
## Header files.
HDRFILES:=$(wildcard ${HDR}/*.h)

FOLDERS:=${SRC} ${BIN} ${OBJ} ${LIBOBJ} ${HDR}
OBJECTS:=$(sort $(patsubst ${SRC}/%.c,${OBJ}/%.o,$(wildcard ${SRC}/*.c) $(wildcard ${SRC}/*/*.c)))
OBJFOLDERS:=$(patsubst ${SRC}/%/,${OBJ}/%,$(wildcard ${SRC}/*/))
LIBOBJECTS:=$(patsubst ${LIB}/%.c,${LIBOBJ}/%.o,$(wildcard ${LIB}/*.c) $(wildcard ${LIB}/*/*.c))
LIBFOLDERS:=$(patsubst ${LIB}/%/,${LIBOBJ}/%,$(wildcard ${LIB}/*/))
EXES:=${OBJFOLDERS:${OBJ}/%=${BIN}/%.exe} $(patsubst ${SRC}/%.c,${BIN}/%.exe,$(wildcard ${SRC}/*.c))
LIBS:=${LIBFOLDERS:${LIBOBJ}/%=${BIN}/%.a} $(patsubst ${LIB}/%.c,${BIN}/%.o,$(wildcard ${LIB}/*.c))

# Using directives
USING:=
USING+=Collections
USING+=Collections/Generic

# Compiler setup
CC:=gcc
CWARNS:=all extra no-microsoft-anon-tag no-dangling-else #required because of one of my macros :moai:
CFLAGS:=${CWARNS:%=-W%} -I${HDR} -I${LIB} ${USING:%=-I${HDR}/%} -std=gnu23 -fms-extensions

# Functions
## Get objects relevant to $1 considering root $2 from list $3.
get_relevant_objs=$(filter $(basename $(patsubst ${BIN}/%,$(2)/%,$(1)))/% $(basename $(patsubst ${BIN}/%,$(2)/%,$(1))).%,$(3))

all: ${EXES}

run: all
	${EXES:%=echo; TARGET=%; $${TARGET} || echo -e "\n$${TARGET}: exit $$?";}

# Basically enable Make to take filenames as arguments
$(basename $(notdir ${EXES})): %: ${BIN}/%.exe
	@$<

pack: ${LIBS}

.SECONDEXPANSION:
${EXES}: $$(call get_relevant_objs,$$@,${OBJ},${OBJECTS}) ${LIBS} | ${BIN}
	${CC} ${CFLAGS} $^ -o $@

.SECONDEXPANSION:
${OBJECTS}: ${OBJ}/%.o: ${SRC}/%.c ${HDRFILES} | ${OBJ} ${OBJFOLDERS}
	${CC} ${CFLAGS} -c $< -o $@

.SECONDEXPANSION:
${LIBS}: $$(call get_relevant_objs,$$@,${LIBOBJ},${LIBOBJECTS}) | ${BIN}
	ar rcs $@ $^

.SECONDEXPANSION:
${LIBOBJECTS}: ${LIBOBJ}/%.o: ${LIB}/%.c ${HDRFILES} $$(wildcard $$(subst ${LIBOBJ},${LIB},$$(dir $$@))/*.h) | ${LIBFOLDERS} ${LIBOBJ}
	${CC} ${CFLAGS} -c $< -o $@

${OBJFOLDERS}: | ${OBJ}
	mkdir $@

${LIBFOLDERS}: | ${OBJ} ${LIBOBJ}
	mkdir $@

${FOLDERS}:
	mkdir $@

clean:
	rm -f ${OBJECTS} ${EXES}

cleanlib:
	rm -f ${LIBOBJECTS} ${LIBS}

reset:
	rm -rf ${BIN} ${OBJ}

debug:
	@echo -e "\033[36mUniversal dependencies:\033[0m"
	@echo -e "$(foreach lib,${LIBS},    \033[35m${lib}\033[0m$(patsubst %,\n      \033[34m└──> \033[31m%\033[0m,$(filter ${lib:${BIN}/%.a=${LIBOBJ}/%}%,${LIBOBJECTS}))\n)"
	@echo -e "\033[36mObject files:\033[33m${OBJECTS:%=\n    %}\033[0m"
	@echo -e "$(foreach ex,${EXES},\n\033[32m${ex}\033[0m$(patsubst %,\033[33m\n  └──> \033[31m%\033[0m,$(filter ${ex:${BIN}/%.exe=${OBJ}/%}%,${OBJECTS})))"

debuger:
	@echo -e "TEST:\n$(call get_relevant_objs,$(firstword ${EXES}),${OBJECTS})"
cc:
	${CC} --version

.PHONY: all run test clean reset debug cc
