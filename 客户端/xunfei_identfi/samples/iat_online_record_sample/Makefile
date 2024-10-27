#common makefile header
shell 脚本-->就是linux终端的命令，装起来 cd  ls
// 库的使用问题。  1.提前帮你写好了makefile / 给你一个configure文件(shell脚本)   2.用cmake编译工程生成makefile
// 动态库和静态库
// 静态库：在编译的时候会直接把库文件的内容复制到可执行文件中，所以编译后的可执行文件会比较大。
  动态库：在编译的时候不会把库文件的内容复制到可执行文件中，而是在运行的时候才会去加载库文件。
linux：静态库：libxxx.a  动态库：libxxx.sodd
// makefile  makefile文件是make工具的配置文件，用于指定编译规则和依赖关系。make工具会根据makefile文件中的规则和依赖关系，自动生成编译命令，并执行这些命令来编译程序。

DIR_INC = ../../include
DIR_BIN = .
DIR_LIB = ../../libs

TARGET	= iat_online_record_sample
BIN_TARGET = $(DIR_BIN)/$(TARGET)

CROSS_COMPILE =
CFLAGS = -g -Wall -I$(DIR_INC)

#可根据实际需要修改，选择所需平台SDK，默认为x64或x86平台。
ifdef LINUX64
LDFLAGS := -L$(DIR_LIB)/x64
else
LDFLAGS := -L$(DIR_LIB)/x86
endif

LDFLAGS += -lmsc -lrt -ldl -lpthread -lasound -lstdc++

OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c))

$(BIN_TARGET) : $(OBJECTS)
	$(CROSS_COMPILE)gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o : %.c
	$(CROSS_COMPILE)gcc -c $(CFLAGS) $< -o $@
clean:
	@rm -f *.o $(BIN_TARGET)

.PHONY:clean

#common makefile foot
