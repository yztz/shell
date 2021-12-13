CC 				:= gcc
CFLAGS 			+= -Wall 
#########测试用例##########
# yes or no
TEST			?= no
#########调试信息##########
# yes or no
DEBUG 			?= no
# LOG_LEVEL只在DEBUG=yes时生效: LOG_DEBUG, LOG_INFO, LOG_ERROR, LOG_PANIC
LOG_LEVEL		?= LOG_PANIC
##########################

TARGET 			:= ysh

LEX 			:= flex
BISON 			:= bison

LEX_FILE 		:= lex.l
BISON_FILE 		:= bison.y

LEX_OUT_C 		:= src/lex.c
LEX_OUT_H 		:= include/lex.h
BISON_OUT_H 	:= include/parser.h
BISON_OUT_C 	:= src/parser.c

LEX_OUT			:= $(LEX_OUT_H) $(LEX_OUT_C)
BISON_OUT 		:= $(BISON_OUT_H) $(BISON_OUT_C)

OUT_PATH		:= target

ifeq ($(DEBUG), yes)
CFLAGS  		+= -D$(LOG_LEVEL) -g
endif
ifeq ($(TEST), yes) 
CFLAGS  		+= -DTEST_SAMPLE
SRC 			:= $(wildcard src/*.c) $(wildcard src/builtins/*.c) $(wildcard test/*.c)
else
SRC 			:= $(wildcard src/*.c) $(wildcard src/builtins/*.c)
endif

SRC_NO_DIR		:= $(notdir $(SRC))
OBJ			 	:= $(SRC_NO_DIR:%.c=%.o)
INCLUDE_PATH	:= include

CFLAGS			+= -I $(INCLUDE_PATH)

all: $(LEX_OUT) $(BISON_OUT) $(TARGET)

# 目标生成
$(TARGET) : $(OBJ:%.o=$(OUT_PATH)/%.o)
	$(CC) $(CFLAGS) -o $(TARGET) $^ -lfl -lreadline
 
$(OUT_PATH)/%.o : src/%.c
	$(CC) ${CFLAGS} -c -o $@ $<

$(OUT_PATH)/%.o : src/builtins/%.c
	$(CC) ${CFLAGS} -c -o $@ $<

$(OUT_PATH)/%.o : test/%.c
	$(CC) ${CFLAGS} -c -o $@ $<

# 生成词法分析文件
$(LEX_OUT) : $(LEX_FILE)
	$(LEX) --header-file=${LEX_OUT_H} -o $(LEX_OUT_C) $(LEX_FILE)

# 生成语法分析文件 .h & .c
$(BISON_OUT) : $(BISON_FILE)
	$(BISON) --defines=$(BISON_OUT_H) -o $(BISON_OUT_C) $(BISON_FILE)

.PHONY: clean rebuild
clean:
	rm -rf target/* $(TARGET)
rebuild:
	make clean
	make all