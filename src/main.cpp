#ifdef _LINUX
	#define FSYS_LIB "filesystem_stdio.so"
#else
	#define FSYS_LIB "FileSystem_Stdio.dll"
#endif

#include "GarrysMod/Lua/Interface.h"

#include "interface.h"
#include "filesystem.h"

using namespace GarrysMod;

IFileSystem *g_FileSystem;

int yourmother_Mount(lua_State *state) {
	LUA->CheckType(1, Lua::Type::STRING); // path
	LUA->CheckType(2, Lua::Type::STRING); // pathId

	SearchPathAdd_t addtype = PATH_ADD_TO_HEAD;
	if (LUA->IsType(3, Lua::Type::NUMBER)) {
		addtype = LUA->GetNumber(3); // PATH_ADD enum
	}

	g_FileSystem->AddSearchPath(LUA->GetString(1), "GAME", addtype);
	g_FileSystem->AddSearchPath(LUA->GetString(1), LUA->GetString(2), addtype); // allow for things like file.Find("blah", "cstrike")

	if (LUA->IsType(4, Lua::Type::NUMER)) { // appid
		LUA->PushNumber(g_FileSystem->MountSteamContent(-LUA->GetNumber(4)));

		return 1;
	}

	return 0;
}

int yourmother_Unmount(lua_State *state) {
	LUA->CheckType(1, Lua::Type::STRING); // path
	LUA->CheckType(2, Lua::Type::STRING); // pathId

	g_FileSystem->RemoveSearchPath(LUA->GetString(1), LUA->GetString(2));

	return 0;
}

int yourmother_PrintSearchPaths(lua_State *state) {
	g_FileSystem->PrintSearchPaths();

	return 0;
}

GMOD_MODULE_OPEN() {
	CreateInterfaceFn FileSystem_StdioFactory = Sys_GetFactory(FSYS_LIB);
	g_FileSystem = (IFileSystem*)FileSystem_StdioFactory("VFileSystem022", NULL);

	if (g_FileSystem == NULL) {
		LUA->ThrowError("gm_yourmother: Error getting IFileSystem interface.");

		return 0;
	}

	LUA->PushSpecial(Lua::SPECIAL_GLOB);
		LUA->CreateTable();
			LUA->PushCFunction(yourmother_Mount);
			LUA->SetField(-2, "Mount");

			LUA->PushCFunction(yourmother_Unmount);
			LUA->SetField(-2, "Unmount");

			LUA->PushCFunction(yourmother_PrintSearchPaths);
			LUA->SetField(-2, "PrintSearchPaths");
		LUA->SetField(-2, "yourmother");

		LUA->PushNumber(PATH_ADD_TO_HEAD);
		LUA->SetField(-2, "PATH_ADD_TO_HEAD");
		LUA->PushNumber(PATH_ADD_TO_TAIL);
		LUA->SetField(-2, "PATH_ADD_TO_TAIL");

		LUA->PushNumber(FILESYSTEM_MOUNT_OK);
		LUA->SetField(-2, "FILESYSTEM_MOUNT_OK");
		LUA->PushNumber(FILESYSTEM_MOUNT_FAILED);
		LUA->SetField(-2, "FILESYSTEM_MOUNT_FAILED");
	LUA->Pop();

	return 0;
}

GMOD_MODULE_CLOSE() {
	return 0;
}
