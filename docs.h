//Special thanks to  Quattro(https://github.com/QuattroMusic) for most of this documentation.

// ----------------
//  Files Handling
// ----------------

static void FolderCreate(const char* Path);
// Creates a folder in the given path if it doesn't already exist


static void FilesCopyAll(const char* Source, const char* Destination);
// Copy all the files and folders from `Source` to `Destination`


static void FilesCopyAllMatching(const char* Source, const char* Destination, const char* Pattern);
// Copy all the files and folders from `Source` to `Destination` with a given pattern (extension)


// -----------------
//  System handling
// -----------------

static char* EnvironmentVariableGet(const char* VarName);
// Get the value of an environment variable


static int ProcessIsRunning(const char* ProcessName);
// Check if a given process is running

// ----------------------------------
//  Command Lines Arguments Handling
// ----------------------------------

static int ArgumentsSearch(const char * Args);
/* Searches for given args in the args given in command line returning:
 * -1 if no arguments was found and the position of the arguments in 
 * the args string if it was found 


// ------------------
//  Project Handling
// ------------------

project ProjectCreate(const char* Name, u32 Compiler, u32 Output);
/*
 * Create a project with a given name, compiler and output type.
 * You may later on add flags, libraries and compilation options
 *
 * Compiler is one of
 * - COMPILER_MSVC
 *
 * Output type is one of
 * - OUTPUT_EXECUTABLE
 * - OUTPUT_DYNAMIC_LIBRARY
 * - OUTPUT_LIBRARY
 */


static inline void ProjectSetCompiler(project* Project, u32 Compiler);
/*
 * Sets the compiler of a project
 *
 * Compiler is one of
 * - COMPILER_MSVC
 */


static inline void ProjectSetOutputType(project* Project, u32 Output);
/*
 * Set the output type of a project
 *
 * Output type is one of
 * - OUTPUT_EXECUTABLE
 * - OUTPUT_DYNAMIC_LIBRARY
 * - OUTPUT_LIBRARY
 */


static inline void ProjectSetFilePath(project* Project, const char* Path);
/*
 * Set the path of the source files given to the compiler
 */


static inline void ProjectSetOutputPath(project* Project, const char* Path);
/*
 * Set the output path of the project
 * Also creates the directory if it doesn't exists
 */

// ------------------
//  Project add data
// ------------------

static inline void ProjectAddFiles(project* Project, const char* Files);
/*
 * Give a list of files to be compiled by the project
 * `Files` is a string with space separated files
 *
 * Example: "file1.cpp file2.cpp file3.cpp"
 */


static inline void ProjectAddCompilerFlags(project* Project, const char* Flags);
/*
 * give a list of definitions to your project
 * `Flags` is a string with space separated definitions
 *
 * Example: "/O2 /Zi /GL"
 */


static inline void ProjectAddLinkerFlags(project* Project, const char* Flags);
/*
 * Give a list of definitions to the linker
 * `Flags` is a string with space separated definitions
 *
 * Example: TODO
 */


static inline void ProjectLinkLibs(project* Project, const char* Libs);
/*
 * Give a list of libraries to be included in your project
 * `Libs` is a string with space separated libraries
 *
 * Example: "math.lib window.lib rendering.lib"
 */

static inline void ProjectAddLibsDirs(project* Project, const char* Dirs);
/*
 * Give a list of directories to search for lib files in your project
 * `Dirs` is a string with space separated directories
 *
 * Example: "game/headers engine/headers"
 */

static inline void ProjectAddIncludeDirs(project* Project, const char* Dirs);
/*
 * Give a list of directories to search for header files in your project
 * `Dirs` is a string with space separated directories
 *
 * Example: "game/headers engine/headers"
 */

static inline void ProjectAddDefines(project* Project, const char* Defines);
/*
 * give a list of definitions to your project
 * `Defines` is a string with space separated definitions
 *
 * Example: "NO_LOG RELEASE NO_CONSOLE"
 */


static inline void ProjectExportSymbols(project* Project, const char* Symbols);
/*
 * give a list of functions / variables names
 * which will be exposed in the library for linking
 * `Symbols` is a string with space separated symbols
 *
 * Example: "init_func update_func"
 */

// --------------------
//  Project reset data
// --------------------


static inline void ProjectResetFiles(project* Project);
/*
 * Clear the source files to be compiled by the project
 * See `ProjectAddSourceFiles`
 */


static inline void ProjectResetCompilerFlags(project* Project);
/*
 * Clear the definitions given to the project
 * See `ProjectAddCompilerFlags`
 */


static inline void ProjectResetLinkerFlags(project* Project);
/*
 * Clear the linker definitions to the project
 * See `ProjectAddLinkerFlags`
 */


static inline void ProjectResetLibs(project* Project);
/*
 * Clear the libraries given to the project
 * See `ProjectLinkLibs`
 */

static inline void ProjectResetLibsDirs(project* Project);
/*
 * Clear the directories given to the project
 * See `ProjectAddLibsDirs`
 */


static inline void ProjectResetIncludeDirs(project* Project);
/*
 * Clear the directories given to the project
 * See `ProjectAddIncludeDirs`
 */


static inline void ProjectResetDefines(project* Project);
/*
 * Clear the defines given to the project
 * See `ProjectAddDefines`
 */


static inline void ProjectResetSymbols(project* Project);
/*
 * Clear the symbols given to the project
 * See `ProjectExportSymbols`
 */

static inline void ProjectLink(project * LinkingProject, project * LinkedProject);
/*
 * Links one project against another, if and only if the linked project output is:
 * -OUTPUT_LIBRARY
 * -OUTPUT_DYNAMIC_LIBRARY
 */
// -------------------------
//  Project start execution
// -------------------------

static inline void ProjectCompileAndWait(project* Project);
// Start the compilation process of your project and stalls the execution until it has terminated

static inline void ProjectLaunch(project * Project);
/* 
 * If the Project output is OUTPUT_EXECUTABLE, sai project will start it's execution
 * If not nothing will happen
*/

static inline void ProjectLaunch(project * Project);
/* 
 * If the Project output is OUTPUT_EXECUTABLE, sai project will start it's execution
 * If not nothing will happen
*/

static inline int ProjectIsRunning(project * Project);
// Checks if a given project executable is running, will return false if project output is not OUTPUT_EXECUTABLE