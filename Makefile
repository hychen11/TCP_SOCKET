TARGETS=ringmaster player

all: $(TARGETS)

#.PHONY 声明了 clean 是一个伪目标，因此在执行 make clean 命令时，即使在目录中不存在名为 clean 的文件，也会执行 clean 目标下定义的清理命令。
.PHONY:
	clean

clean:
	rm -f *.o $(TARGETS)

player: player.cpp function.cpp function.h
	g++ -g -o $@ $^

ringmaster: ringmaster.cpp function.cpp function.h potato.h
	g++ -g -o $@ $^

