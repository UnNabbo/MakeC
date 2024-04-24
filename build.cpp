#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <system_error>
#include <thread>
#include <string>
#include <array>
#include <vector>
#include <windows.h>
#include <ctype.h>

typedef uint32_t u32;
typedef uint8_t u8;

using namespace std::chrono;

const std::string INITIALS = "/nologo /std:c++20 /W0 /EHsc -Z7";
const std::string INCLUDES = "/external:W0 /cgthreads4 /MD /DYNAMICBASE \"kernel32.lib\" \"user32.lib\" \"gdi32.lib\" \"winspool.lib\" \"winmm.lib\" \"comdlg32.lib\" \"advapi32.lib\" \"shell32.lib\" \"ole32.lib\" \"oleaut32.lib\" \"uuid.lib\" \"odbc32.lib\" \"PowrProf.lib\" \"odbccp32.lib\"";

u8 global_error_code =  0;

std::string external_flags = "";

enum compile_type{
    EXE, DLL
};

void exec(const char* cmd, const char* type, u8* return_code, std::string& return_str) {
    printf("Started building %s\n", type);
    auto start = high_resolution_clock::now();
    char buffer[128];
    
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) {
        printf("Failed to start %s process\n", type);
    }
    
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        return_str += buffer;
    }
    if(return_str.size() > strlen(type) + 1) {
        if(return_str.size() > 62 + strlen(type) * 2) {
            // Hack: since the dll writes "creazione della libreria engine.lib e dell'oggetto engine.exe" in the console
            // we check the length of this string
            *return_code = 1;
        }
    }
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start).count();
    printf("Builded %s in %lldms (%.2fs)\n", type, duration, duration / 1000.0f);
}

void compile(const char * project_name, const char * additional_flags = ""){
	std::string compile_cmd = "pushd out && cl /FC /FS " + external_flags + INITIALS + " " + INCLUDES + " ";
	
    compile_cmd += "../" + std::string(project_name) + "/src/main.c " + std::string(additional_flags) + " /link /INCREMENTAL:NO /OUT:" + project_name + ".exe && popd";
	
    u8 error_code = 0;
    std::string error_msg;
    exec(compile_cmd.c_str(), project_name, &error_code, error_msg);
    if(error_code){
        printf("%s\n", error_msg.c_str());
    }
	
    global_error_code |= error_code;
}

void build() {
    compile("MakeC");
}

int main(int argc, char *argv[]) {
    system("if not exist out mkdir out");
		
    std::string what_to_compile;
    bool is_compiled_with_release = false;
    
    for(int i = 1; i < argc; i++) {
        external_flags += " ";
        
        char lower_string[0xFF];
        for(int k = 0; k < argv[i][k]; k++) {
            lower_string[k] = tolower(argv[i][k]);
        }
        
        if(strcmp(lower_string, "debug")   == 0){ 
			is_compiled_with_release = false; 
		} else if(strcmp(lower_string, "release") == 0){
			is_compiled_with_release = true;  
		}else {
            external_flags += argv[i];
            external_flags += " ";
        }
    }
    
    if(is_compiled_with_release) {
        external_flags += "/O2 ";
    } else {
        external_flags += "/DDEBUG ";
    }
    
    printf("Compiling %s with external parameters '%s'\n", what_to_compile.c_str(), external_flags.c_str());
    

	build();
	
	if(!global_error_code){
        system("pushd out && start .\\makec.exe && popd");
    }
	
    return 0;
}
