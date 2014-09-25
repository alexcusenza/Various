### VCC3 CLI Test Code makefile

### define the search paths
VPATH = $(VCC3_PROJECT) $(VCC3_SOURCE)/cli $(VCC3_SOURCE)/Framework $(VCC3_SOURCE)/Driver $(VCC3_INCLUDE)

### define the executable target
BUILD_EXEC = Hello

### define the source files
SOURCE := Hello.cpp CFwStdioProtocol.cpp

### get configuration and make rules
include $(VCC3_MISC)/vcc3.mak

### make rules

$(BUILD_EXEC) : $(OBJECTS)
	$(CPP) -o $@ $(LDLANGFLAGS) $(OBJECTS) $(VCC3_LIB_LIST) $(VCC3_LIB_LIST)
	# to set up scp to allow password-less copying, use ssh-keygen followed by ssh-copy-id once
	#scp $@ newstack@172.16.17.1:.


