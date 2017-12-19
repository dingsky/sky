#######################################################################
#功能：静态模式编译库文件                                            #
#作者：丁春                                                          #
#时间：2013-09-05                                                    #
#说明：增加或修改.c文件，本Makefile文件不需要修改，直接编译即可      #
######################################################################

#清除系统自带的MAKEFILES环境变量
ifdef MAKEFILES
	MAKEFILES = 
endif

Objects = $(shell ls *.c | awk -F'.' '{print $$1 ".o"}')
#此处也可以通过这种方法来实现，获得目标文件序列
#Files = $(shell ls *.c)
#Objects = $(Files:%.c=%.o)  #这一句还可以替换成$(Files:.c:.o)
LibName= $(shell basename `pwd` | awk '{print "lib" $$0 ".a"}')
LibDir = $(DCDIR)/lib
IncludeDir = $(DCDIR)/include
BackupDir = $(DCDIR)/user/backup

#定义伪目标
.PHONY : clean install print tar dist TAGS test

all : $(Objects) install clean

$(Objects) : %.o : %.c
	gcc  -c $< -o $@ -I$(IncludeDir)

#清除make创建的文件
clean :
	@rm -f $(Objects) *.bak

#生成库文件，安装编译好的库文件
install : 
	ar -r $(LibName)  ${Objects}
	mv $(LibName) $(LibDir)



