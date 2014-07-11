CL = cl.exe \
	//I"C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\VC\\include" \
	//I"C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v7.0A\\Include" \
	//D "WIN32" //D "_DEBUG" //D "_CONSOLE" //D "_UNICODE" //D "UNICODE" \
	//c //Zi
	
LINK = "C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\VC\\bin\\link.exe" \
	//LIBPATH:"C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v7.0A\\Lib" \
	//LIBPATH:"C:\\Program Files (x86)\\Microsoft Visual Studio 10.0\\VC\\lib" \
	"kernel32.lib" "user32.lib" \
	//SUBSYSTEM:CONSOLE //MACHINE:X86 //DEBUG
	
	

all:
	$(CL) __try.cpp
	$(LINK) //OUT:"__try.exe" __try.obj
	objdump -d -M intel __try.exe > __try.s
	$(CL) __try2.cpp
	$(LINK) //OUT:"__try2.exe" __try2.obj
	objdump -d -M intel __try2.exe > __try2.s
	
clean:
	rm -f *.exe *.obj *.s *.ilk *.pdb
